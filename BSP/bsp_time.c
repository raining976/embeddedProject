#include "bsp_time.h"


void fml_delay_ms(uint16_t ms)//10000 -- 1000ms
{
	TIM_TimeBaseInitTypeDef TIM_InitSruct = {0};
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_InitSruct.TIM_ClockDivision = 0;
	TIM_InitSruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitSruct.TIM_Prescaler = 7199;
	TIM_InitSruct.TIM_Period = ms * 10;
	TIM_TimeBaseInit(TIM4,&TIM_InitSruct);
	TIM_GenerateEvent(TIM4,TIM_EventSource_Update);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_Cmd(TIM4,ENABLE);
	while(!TIM_GetFlagStatus(TIM4,TIM_FLAG_Update));
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_Cmd(TIM4,DISABLE);
}

void fml_delay_us(uint16_t us)//10000 -- 1000ms
{
	TIM_TimeBaseInitTypeDef TIM_InitSruct = {0};
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_InitSruct.TIM_ClockDivision = 0;
	TIM_InitSruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitSruct.TIM_Prescaler = 71;
	TIM_InitSruct.TIM_Period = us;
	TIM_TimeBaseInit(TIM4,&TIM_InitSruct);
	TIM_GenerateEvent(TIM4,TIM_EventSource_Update);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_Cmd(TIM4,ENABLE);
	while(!TIM_GetFlagStatus(TIM4,TIM_FLAG_Update));
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_Cmd(TIM4,DISABLE);
}














