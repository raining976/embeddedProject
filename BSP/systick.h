#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f10x.h"



void SysTick_Init(void);
uint8_t SystickGetTimebaseFlag(void);
void SystickSetTimebaseFlag(void);
void SystickClearTimebaseFlag(void);


#endif
