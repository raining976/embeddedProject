#include "bsp_motor.h"

/*
	* ��������   �� MOTOR_GPIOInit
	* ��������   �� ����ܽų�ʼ��
	* �����β�   �� ��
	* ��������ֵ �� ��
	* ��������   �� ��
 */
 /**
  * @brief  ��ʼ��TB6612һ·�������ܽź�ʹ�ܹܽ�
  * @brief  AIN1-PA5 AIN2-PA7 PWMA-PA6 STBY-VCC
  * @param	None
  * @retval None
  */
void fml_motor_gpio_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//ch A input 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruct); 
	fml_motor_stop();	
}
/*
	* ��������   �� MOTOR_PwmInit
	* ��������   �� ���PWM��ʼ��
	* �����β�   �� ��
	* ��������ֵ �� ��
	* ��������   �� ��
*/
void fml_motor_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	TIM_TimeBaseInitTypeDef TIM_BaseInitStruct = {0};
	TIM_OCInitTypeDef TIM_OCInitStruct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//GPIO����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;                  //���ù���
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;  			 		  //ѡ��ܽ�
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;       		  //����ٶ�50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct);                   			 //��ʼ��B��
	
	//ʱ����Ԫ����
	TIM_BaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 			 //ʱ�ӷָ�����
	TIM_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���ģʽ
	TIM_BaseInitStruct.TIM_Prescaler = MOTOR_PSC;              //��Ƶ΢�뼶
	TIM_BaseInitStruct.TIM_Period = MOTOR_ARR;                 //����PWM����Ϊ10KHz
	TIM_TimeBaseInit(TIM3,&TIM_BaseInitStruct);                //��ʼ����ʱ��3
	//OC��ʼ������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //PWMģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //���״̬ʹ��
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //��ʼ״̬�ߵ�ƽ
	TIM_OCInitStruct.TIM_Pulse = DUTY_RATIO;                   //ռ�ձ�����
	//��ʼ����Ӧͨ��
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);                       //��ʼ����ʱ��3ͨ��1
	TIM_OC1PreloadConfig(TIM3,ENABLE);                         //ʹ��TIM3ͨ��3Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM3,ENABLE);                         //ʹ��TIM3Ԥ��Ƶ�Ĵ���
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM3,ENABLE);                                      //ʹ�ܶ�ʱ��3
	
	
	
}

void fml_motor_init(void)
{
	fml_motor_gpio_init();
	fml_motor_pwm_init();
}
/*
	* ��������   �� MOTOR_Forward
	* ��������   �� ���ǰ��״̬
	* �����β�   �� ��
	* ��������ֵ �� ��
	* ��������   �� ��
*/
void fml_motor_forward(void)
{
	//motor
	FORWARD();
	sys_parm.cur_ratio = 50;
}
/*
	* ��������   �� MOTOR_Backward
	* ��������   �� �������״̬
	* �����β�   �� ��
	* ��������ֵ �� ��
	* ��������   �� ��
*/
void fml_motor_backward(void)
{
	BACKWARD();
	sys_parm.cur_ratio = 50;
}

/*
	* ��������   �� MOTOR_Stop
	* ��������   �� ���ɲ��״̬
	* �����β�   �� ��
	* ��������ֵ �� ��
	* ��������   �� ��
*/
void fml_motor_stop(void)
{
	SHORTBRAKE();
	sys_parm.cur_ratio = 0;
}


void fml_motor_resetV(void)
{
	TIM_SetCompare1(TIM3,DUTY_RATIO);
}

void fml_motor_setV(uint8_t level)
{
	if(sys_parm.motor_state == 1){
		
		switch(level){
		case 1:
			sys_parm.cur_ratio = DUTY_RATIO*0.6; // 30
			TIM_SetCompare1(TIM3,DUTY_RATIO*0.6);
			break;
		case 2:
			sys_parm.cur_ratio = DUTY_RATIO*0.8; // 40
			TIM_SetCompare1(TIM3,DUTY_RATIO*0.8);
			break;
		case 3:
			sys_parm.cur_ratio = DUTY_RATIO; // 50
			TIM_SetCompare1(TIM3,DUTY_RATIO);
			break;
	}
	buzzer_flash();
		
		
	}
	
}

void fml_motor_addV(void)
{
	if(sys_parm.motor_state == 1){
		if(sys_parm.cur_ratio >= DUTY_RATIO) return;
	buzzer_flash();
	sys_parm.cur_ratio += 5;
	if(sys_parm.motor_state == 1) TIM_SetCompare1(TIM3,sys_parm.cur_ratio);	
	}

}
void fml_motor_subV(void)
{
	if(sys_parm.motor_state == 1){
			if(sys_parm.cur_ratio <= 30) return;
	buzzer_flash();
	sys_parm.cur_ratio -= 5;
	 TIM_SetCompare1(TIM3,sys_parm.cur_ratio);
		
	}
}


	








