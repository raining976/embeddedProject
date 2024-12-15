#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "stm32f10x.h"
#include "bsp_time.h"

#define LED_CONTROL_PIN0		GPIO_Pin_0
#define LED_CONTROL_PIN1		GPIO_Pin_1
#define BUZZER_CONTROL_PIN		GPIO_Pin_12


#define LED1_PutVal(val)		(GPIO_WriteBit(GPIOA,LED_CONTROL_PIN0,val))
#define LED2_PutVal(val)		(GPIO_WriteBit(GPIOA,LED_CONTROL_PIN1,val))
#define BUZZER_PutVal(val)		(GPIO_WriteBit(GPIOB,BUZZER_CONTROL_PIN,val))

#define led1_on() (LED1_PutVal(Bit_RESET))
#define led1_off() (LED1_PutVal(Bit_SET))
#define led2_on() (LED2_PutVal(Bit_RESET))
#define led2_off() (LED2_PutVal(Bit_SET))


#define buzzer_on() (BUZZER_PutVal(Bit_RESET))
#define buzzer_off() (BUZZER_PutVal(Bit_SET))

#define buzzer_flash() do{\
GPIO_WriteBit(GPIOB,BUZZER_CONTROL_PIN,Bit_RESET);\
fml_delay_ms(100);\
GPIO_WriteBit(GPIOB,BUZZER_CONTROL_PIN,Bit_SET);\
}while(0);

void BSP_LedInit(void);
void fml_beep_init(void);

#endif
