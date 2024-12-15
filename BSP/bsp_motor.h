#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f10x.h"

#include "bsp_gpio.h"
#include "app.h"

#define DUTY_RATIO 50  //占空比

#define MOTOR_PSC    83
#define MOTOR_ARR    100


#define FORWARD()  do {\
	GPIO_SetBits(GPIOA,GPIO_Pin_5);   \
	GPIO_ResetBits(GPIOA,GPIO_Pin_7); \
}while(0)  

#define BACKWARD()  do {\
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);  \
	GPIO_SetBits(GPIOA,GPIO_Pin_7); \
}while(0)   
#define SHORTBRAKE()  do {\
	GPIO_SetBits(GPIOA,GPIO_Pin_5);   \
	GPIO_SetBits(GPIOA,GPIO_Pin_7); \
}while(0)   


void fml_motor_gpio_init(void);
void fml_motor_pwm_init(void);
void fml_motor_init(void);
void fml_motor_forward(void);							
void fml_motor_backward(void);
void fml_motor_stop(void);
void fml_motor_setV(uint8_t level); // 调档位
void fml_motor_isRunning(void); // 是否在转
void fml_motor_resetV(void); // 档位复位
void fml_motor_addV(void);
void fml_motor_subV(void);


#endif
