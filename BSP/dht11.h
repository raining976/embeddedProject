#ifndef DHT11_H
#define DHT11_H

#include "stm32f10x.h"
#include "bsp_time.h"
//宏定义
#define DHT11_BUS_H GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define DHT11_BUS_L GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define DHT11_BUS_IN  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)


#define DHT11_DO_H GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define DHT11_DO_L GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define DHT11_DI GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
//函数声明
void DHT11_Read_Data(u8 *Temperature,u8 *Humidity);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Strat_Ack(void);
void DHT11_IO_Init(void);

#endif
