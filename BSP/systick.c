#include "systick.h"

/****************************************************************************************
函数名称  ： Systick_Init
函数功能  ： 初始化滴答定时器 
函数形参  ： 无
函数返回值： 无
函数描述： 
	作为时间片的时间节拍时 定义 TASK
	否则用作普通延时
	时钟源选择：
	systick->ctrl[2] :0 -- 外部9M时钟，1 -- 内部72M时钟
*****************************************************************************************/
void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock/1000))         //1ms定时器
	{
		while(1);
	}
}

uint8_t TimebaseFlag;

uint8_t SystickGetTimebaseFlag(void)
{
	return TimebaseFlag;
}

void SystickSetTimebaseFlag(void)
{
	TimebaseFlag = SET;
}

void SystickClearTimebaseFlag(void)
{
	TimebaseFlag = RESET;
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void SysTick_Handler(void)
{
	SystickSetTimebaseFlag();
}









