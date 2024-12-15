#ifndef BSP_USART_H
#define BSP_USART_H

#include "bsp_gpio.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "typedef.h"
#include "bsp_motor.h"

extern usart_data_t fml_uartRecv;

void bsp_usart_init(void);
uint8_t bsp_usart_data_st(void);
void bsp_usart_data_clear(void);
uint8_t bsp_usart_data_handle(void);
void UsartCtrlTask(void);


#endif





