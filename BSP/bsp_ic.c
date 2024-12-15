#include "bsp_ic.h"
#include "stdio.h"
#include "string.h"
#include "bsp_gpio.h"

/*******************************************************************************************************
函数名称： TIM2_CH1_Init
函数功能： 初始化红外接收头管脚通道
函数形参： 无
函数返回值：
函数描述： 
	IR -- PB3 -- 重映射TIM2_CH2 -- 浮空输入
	PB3默认功能为JTAG，需关闭JTAG启用SW
********************************************************************************************************/
 		void fml_ir_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;                      //GPIO口初始化参数结构体变量
	TIM_TimeBaseInitTypeDef TIM_BaseInitStruct = {0};
	TIM_ICInitTypeDef TIM_ICInitStruct = {0};
	// 配置定时器输入捕获功能相关的GPIO管脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //使能GPIOB时钟
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;     //浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;                 //选中管脚3
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//管脚重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //使能AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭JATG，使能SW
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);   //定时器2部分重映射
	//时基单元配置
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);			//选择内部时钟源
	TIM_BaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 	//时钟分割因子
	TIM_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_BaseInitStruct.TIM_Prescaler = 71;              	//分频微秒级
	TIM_BaseInitStruct.TIM_Period = 10000;                 	//定义PWM周期
	TIM_BaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_BaseInitStruct);            	//初始化定时器1
	TIM_SetCounter(TIM2,0);
	//IC初始化设置
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;	//下降沿捕获
	TIM_ICInitStruct.TIM_ICFilter = 0x0;						//下降沿触发后，采集8次低电平则为一个有效触发
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);				//复位计数器
	//中断配置
	TIM_GenerateEvent(TIM2,TIM_EventSource_Update);		
	TIM_ITConfig(TIM2,TIM_IT_CC2|TIM_IT_Update,ENABLE);		//使能通道1捕获中断和定时器更新中断
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	NVIC_EnableIRQ(TIM2_IRQn);	
	NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(5,0,0));
	
	TIM_Cmd(TIM2, ENABLE);
}
IR_STRUCT ir;
/*******************************************************************************************************
函数名称： TIM2_IRQHandler
函数功能： 定时器2中断服务函数
函数形参： 无
函数返回值：
函数描述： 
	接收红外信号并保存
********************************************************************************************************/
//ir.remoteState遥控器接收状态
//bit7:收到了引导码标志
//bit6:得到了一个按键的所有信息
//bit5:保留
//bit4:标记上升沿是否已经被捕获
//bit[3:0]:溢出计时器

void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3))//上升沿捕获
        {
            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);//CC1P=1 设置为下降沿捕获
            TIM_SetCounter(TIM2,0); //清空定时器值
            ir.remoteState|= 1 << 4;//标记上升沿已经被捕获
        }
        else //下降沿捕获
        {
            ir.capturevalue = TIM_GetCapture2(TIM2);//读取CCR2也可以清CC2IF标志位
            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0设置为上升沿捕获
            if(ir.remoteState & 0x10)//完成一次高电平捕获
            {
                if(ir.remoteState & 0x80)//接收到了引导码
                {
                    if((ir.capturevalue >= 300) && (ir.capturevalue <= 800))//逻辑0：560为标准值,560us
                    {
                        ir.remoteCode <<= 1;//左移一位.
                        ir.remoteCode |= 0;//接收到0
                    }
                    else if((ir.capturevalue >= 1400) && (ir.capturevalue <= 1800))//逻辑1：1690为标准值,1680us
                    {
                        ir.remoteCode <<= 1;//左移一位.
                        ir.remoteCode |= 1;//接收到1
                    }
                    else if((ir.capturevalue > 2200) && (ir.capturevalue <= 2600))//连按：2250为标准值2.5ms
                    {
						ir.remoteCnt++; //按键次数增加1次
						ir.remoteState &= 0xF0;//清空计时器
                    }
                }
                else if((ir.capturevalue >= 4200) && (ir.capturevalue <= 4700))//4500为标准值4.5ms
                {
                    ir.remoteState |= 1<<7;			//标记成功接收到了引导码
                    ir.remoteCnt = 0;				//清除按键次数计数器
                }

            }
            ir.remoteState &= ~(1 << 4);//取消上升沿已经被捕获标记
        }
    }	
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
    {
        if(ir.remoteState & 0x80)//上次有数据被接收到了
        {
            ir.remoteState &= ~(1 << 4);//取消上升沿已经被捕获标记
            if((ir.remoteState & 0x0F) == 0x00)
                ir.remoteState|= 1 << 6;//标记已经完成一次按键的键值信息采集

            if((ir.remoteState & 0x0F) < 10)//100ms
                ir.remoteState++;
            else
            {
                ir.remoteState &= ~(1 << 7);//清空引导标识
                ir.remoteState &= 0xF0;//清空计数器				
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

	if(ir.remoteState & (1 << 6))//得到一个按键的所有信息了
	{
		if((ir.remoteState & 0x80) == 0)//得到一个按键数据后 遥控已经没有按下了
		{
			t1 = ir.remoteCode >> 24;//得到地址码
			t2 = (ir.remoteCode >> 16) & 0xff;//得到地址反码
			if((t1 == (uint8_t)~t2) && (t1 == 0x00))//检验遥控识别码(ID)及地址
			{
				t1 = ir.remoteCode >> 8;
				t2 = ir.remoteCode;
				if(t1 == (uint8_t)  ~t2) 
				{
					key = t1;//键值正确
					printf("0x%X,%d\r\n",ir.remoteCode,ir.remoteCnt);
					
					if(ir.remoteCnt == 0)
					{
						ir.remoteState &= ~(1 << 6);//清除接收到有效按键标识
					}
					else
						ir.remoteCnt--;
				}
					
			}
			if(key == 0)//得到一个按键数据后 数据错误
			{
				printf("0x%X,%d\r\n",ir.remoteCode,ir.remoteCnt);
				if(ir.remoteCnt == 0)
				{
					ir.remoteState &= ~(1 << 6);//清除接收到有效按键标识
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
		case 0xc2: // 开关
			cmd = MOTOR_SWITCH;
		  sys_parm.flag_motor_ir = 1;
			break;
		case 0x30: // 1档
			cmd = MOTOR_1;
			break;
		case 0x18: // 2档
			cmd = MOTOR_2;
			break;
		case 0x7a: // 3档
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
    uint8_t key = fml_remote_decode(); // 解码按键
		if(key == 0) return;
		handleCmd(getIrCmd(key));
		sys_parm.dataChange = 1;
		i++;
		printf("收到的序号：%d\r\n", i);
		sys_parm.flag_state_change = 1;
		ir.remoteState &= ~(1 << 6); // 清除接收到有效按键标识
    ir.remoteCnt = 0; // 清除按键次数计数器
	}

}
	
	
	
	
	

