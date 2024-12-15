#include "bsp_motor.h"

/*
	* 函数名称   ： MOTOR_GPIOInit
	* 函数功能   ： 电机管脚初始化
	* 函数形参   ： 无
	* 函数返回值 ： 无
	* 函数描述   ： 无
 */
 /**
  * @brief  初始化TB6612一路电机输入管脚和使能管脚
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
	* 函数名称   ： MOTOR_PwmInit
	* 函数功能   ： 电机PWM初始化
	* 函数形参   ： 无
	* 函数返回值 ： 无
	* 函数描述   ： 无
*/
void fml_motor_pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	TIM_TimeBaseInitTypeDef TIM_BaseInitStruct = {0};
	TIM_OCInitTypeDef TIM_OCInitStruct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//GPIO配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;                  //复用功能
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;  			 		  //选择管脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;       		  //输出速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct);                   			 //初始化B组
	
	//时基单元配置
	TIM_BaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 			 //时钟分割因子
	TIM_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数模式
	TIM_BaseInitStruct.TIM_Prescaler = MOTOR_PSC;              //分频微秒级
	TIM_BaseInitStruct.TIM_Period = MOTOR_ARR;                 //定义PWM周期为10KHz
	TIM_TimeBaseInit(TIM3,&TIM_BaseInitStruct);                //初始化定时器3
	//OC初始化设置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //PWM模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //输出状态使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //初始状态高电平
	TIM_OCInitStruct.TIM_Pulse = DUTY_RATIO;                   //占空比设置
	//初始化对应通道
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);                       //初始化定时器3通道1
	TIM_OC1PreloadConfig(TIM3,ENABLE);                         //使能TIM3通道3预装载寄存器
	TIM_ARRPreloadConfig(TIM3,ENABLE);                         //使能TIM3预分频寄存器
	//使能定时器
	TIM_Cmd(TIM3,ENABLE);                                      //使能定时器3
	
	
	
}

void fml_motor_init(void)
{
	fml_motor_gpio_init();
	fml_motor_pwm_init();
}
/*
	* 函数名称   ： MOTOR_Forward
	* 函数功能   ： 电机前进状态
	* 函数形参   ： 无
	* 函数返回值 ： 无
	* 函数描述   ： 无
*/
void fml_motor_forward(void)
{
	//motor
	FORWARD();
	sys_parm.cur_ratio = 50;
}
/*
	* 函数名称   ： MOTOR_Backward
	* 函数功能   ： 电机后退状态
	* 函数形参   ： 无
	* 函数返回值 ： 无
	* 函数描述   ： 无
*/
void fml_motor_backward(void)
{
	BACKWARD();
	sys_parm.cur_ratio = 50;
}

/*
	* 函数名称   ： MOTOR_Stop
	* 函数功能   ： 电机刹车状态
	* 函数形参   ： 无
	* 函数返回值 ： 无
	* 函数描述   ： 无
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


	








