#include "systick.h"

/****************************************************************************************
��������  �� Systick_Init
��������  �� ��ʼ���δ�ʱ�� 
�����β�  �� ��
��������ֵ�� ��
���������� 
	��Ϊʱ��Ƭ��ʱ�����ʱ ���� TASK
	����������ͨ��ʱ
	ʱ��Դѡ��
	systick->ctrl[2] :0 -- �ⲿ9Mʱ�ӣ�1 -- �ڲ�72Mʱ��
*****************************************************************************************/
void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock/1000))         //1ms��ʱ��
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









