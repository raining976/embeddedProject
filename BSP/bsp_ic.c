#include "bsp_ic.h"
#include "stdio.h"
#include "string.h"
#include "bsp_gpio.h"

/*******************************************************************************************************
�������ƣ� TIM2_CH1_Init
�������ܣ� ��ʼ���������ͷ�ܽ�ͨ��
�����βΣ� ��
��������ֵ��
���������� 
	IR -- PB3 -- ��ӳ��TIM2_CH2 -- ��������
	PB3Ĭ�Ϲ���ΪJTAG����ر�JTAG����SW
********************************************************************************************************/
 		void fml_ir_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;                      //GPIO�ڳ�ʼ�������ṹ�����
	TIM_TimeBaseInitTypeDef TIM_BaseInitStruct = {0};
	TIM_ICInitTypeDef TIM_ICInitStruct = {0};
	// ���ö�ʱ�����벶������ص�GPIO�ܽ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //ʹ��GPIOBʱ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;     //��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;                 //ѡ�йܽ�3
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//�ܽ���ӳ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //ʹ��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�JATG��ʹ��SW
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);   //��ʱ��2������ӳ��
	//ʱ����Ԫ����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);			//ѡ���ڲ�ʱ��Դ
	TIM_BaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 	//ʱ�ӷָ�����
	TIM_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_BaseInitStruct.TIM_Prescaler = 71;              	//��Ƶ΢�뼶
	TIM_BaseInitStruct.TIM_Period = 10000;                 	//����PWM����
	TIM_BaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_BaseInitStruct);            	//��ʼ����ʱ��1
	TIM_SetCounter(TIM2,0);
	//IC��ʼ������
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�½��ز���
	TIM_ICInitStruct.TIM_ICFilter = 0x0;						//�½��ش����󣬲ɼ�8�ε͵�ƽ��Ϊһ����Ч����
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);				//��λ������
	//�ж�����
	TIM_GenerateEvent(TIM2,TIM_EventSource_Update);		
	TIM_ITConfig(TIM2,TIM_IT_CC2|TIM_IT_Update,ENABLE);		//ʹ��ͨ��1�����жϺͶ�ʱ�������ж�
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	NVIC_EnableIRQ(TIM2_IRQn);	
	NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(5,0,0));
	
	TIM_Cmd(TIM2, ENABLE);
}
IR_STRUCT ir;
/*******************************************************************************************************
�������ƣ� TIM2_IRQHandler
�������ܣ� ��ʱ��2�жϷ�����
�����βΣ� ��
��������ֵ��
���������� 
	���պ����źŲ�����
********************************************************************************************************/
//ir.remoteStateң��������״̬
//bit7:�յ����������־
//bit6:�õ���һ��������������Ϣ
//bit5:����
//bit4:����������Ƿ��Ѿ�������
//bit[3:0]:�����ʱ��

void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3))//�����ز���
        {
            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);//CC1P=1 ����Ϊ�½��ز���
            TIM_SetCounter(TIM2,0); //��ն�ʱ��ֵ
            ir.remoteState|= 1 << 4;//����������Ѿ�������
        }
        else //�½��ز���
        {
            ir.capturevalue = TIM_GetCapture2(TIM2);//��ȡCCR2Ҳ������CC2IF��־λ
            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0����Ϊ�����ز���
            if(ir.remoteState & 0x10)//���һ�θߵ�ƽ����
            {
                if(ir.remoteState & 0x80)//���յ���������
                {
                    if((ir.capturevalue >= 300) && (ir.capturevalue <= 800))//�߼�0��560Ϊ��׼ֵ,560us
                    {
                        ir.remoteCode <<= 1;//����һλ.
                        ir.remoteCode |= 0;//���յ�0
                    }
                    else if((ir.capturevalue >= 1400) && (ir.capturevalue <= 1800))//�߼�1��1690Ϊ��׼ֵ,1680us
                    {
                        ir.remoteCode <<= 1;//����һλ.
                        ir.remoteCode |= 1;//���յ�1
                    }
                    else if((ir.capturevalue > 2200) && (ir.capturevalue <= 2600))//������2250Ϊ��׼ֵ2.5ms
                    {
						ir.remoteCnt++; //������������1��
						ir.remoteState &= 0xF0;//��ռ�ʱ��
                    }
                }
                else if((ir.capturevalue >= 4200) && (ir.capturevalue <= 4700))//4500Ϊ��׼ֵ4.5ms
                {
                    ir.remoteState |= 1<<7;			//��ǳɹ����յ���������
                    ir.remoteCnt = 0;				//�����������������
                }

            }
            ir.remoteState &= ~(1 << 4);//ȡ���������Ѿ���������
        }
    }	
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
    {
        if(ir.remoteState & 0x80)//�ϴ������ݱ����յ���
        {
            ir.remoteState &= ~(1 << 4);//ȡ���������Ѿ���������
            if((ir.remoteState & 0x0F) == 0x00)
                ir.remoteState|= 1 << 6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�

            if((ir.remoteState & 0x0F) < 10)//100ms
                ir.remoteState++;
            else
            {
                ir.remoteState &= ~(1 << 7);//���������ʶ
                ir.remoteState &= 0xF0;//��ռ�����				
				printf("0x%X,%d\r\n",ir.remoteCode,ir.remoteCnt);
				ir.remoteCnt /= 3;
					
            }
        }
    }   
    TIM_ClearFlag(TIM2,TIM_IT_Update|TIM_IT_CC2);	
}

uint8_t fml_remote_decode(void)
{
	uint8_t key = 0;
	uint8_t t1 = 0,t2 = 0;

	if(ir.remoteState & (1 << 6))//�õ�һ��������������Ϣ��
	{
		if((ir.remoteState & 0x80) == 0)//�õ�һ���������ݺ� ң���Ѿ�û�а�����
		{
			t1 = ir.remoteCode >> 24;//�õ���ַ��
			t2 = (ir.remoteCode >> 16) & 0xff;//�õ���ַ����
			if((t1 == (uint8_t)~t2) && (t1 == 0x00))//����ң��ʶ����(ID)����ַ
			{
				t1 = ir.remoteCode >> 8;
				t2 = ir.remoteCode;
				if(t1 == (uint8_t)  ~t2) 
				{
					key = t1;//��ֵ��ȷ
					printf("0x%X,%d\r\n",ir.remoteCode,ir.remoteCnt);
					
					if(ir.remoteCnt == 0)
					{
						ir.remoteState &= ~(1 << 6);//������յ���Ч������ʶ
					}
					else
						ir.remoteCnt--;
				}
					
			}
			if(key == 0)//�õ�һ���������ݺ� ���ݴ���
			{
				printf("0x%X,%d\r\n",ir.remoteCode,ir.remoteCnt);
				if(ir.remoteCnt == 0)
				{
					ir.remoteState &= ~(1 << 6);//������յ���Ч������ʶ
				}
				else
					ir.remoteCnt--;
			}
		}		
	}
	
	return key;
}



uint8_t getIrCmd(uint8_t key)
{
	uint8_t cmd;
	switch(key){
		case 0xc2: // ����
			cmd = MOTOR_SWITCH;
		  sys_parm.flag_motor_ir = 1;
			break;
		case 0x30: // 1��
			cmd = MOTOR_1;
			break;
		case 0x18: // 2��
			cmd = MOTOR_2;
			break;
		case 0x7a: // 3��
			cmd = MOTOR_3;
			break;
		case 0xa8: // ++
			cmd = ADD_LEVEL;
			break;
		case 0xe0: //--
			cmd = SUB_LEVEL;
			break;
		case 0x42:
			cmd = LED_ON;
			break;
		case 0x4a:
			cmd = LED_OFF;
			break;
		case 0x52:
			cmd = MOTOR_BRAKE;
		  sys_parm.flag_motor_ir = 1;
			break;
		case 0x68:
			cmd = BUZZER_OFF;
			break;
		case 0x02://next
			sys_parm.page += 1;
			if (sys_parm.page > 2)
				sys_parm.page = 1;
			break;
		case 0x22://prev
			sys_parm.page -= 1;
			if (sys_parm.page < 1)
				sys_parm.page = 2;
			break;
		default:
			cmd = ID_ERR;
			break;
	}
	return cmd;
}
int i = 0;
void irCtrlTask(){
	if(ir.remoteState & (1 << 6)){
    uint8_t key = fml_remote_decode(); // ���밴��
		if(key == 0) return;
		handleCmd(getIrCmd(key));
		sys_parm.dataChange = 1;
		i++;
		printf("�յ�����ţ�%d\r\n", i);
		sys_parm.flag_state_change = 1;
		ir.remoteState &= ~(1 << 6); // ������յ���Ч������ʶ
    ir.remoteCnt = 0; // �����������������
	}

}
	
	
	
	
	

