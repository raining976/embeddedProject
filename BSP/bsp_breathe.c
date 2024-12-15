#include "bsp_breathe.h"
#include "bsp_time.h"

void Led_Breathe(void){
	u16 light = 0;
	uint16_t time = 1;
	u16 i;
	for(;;){
	if(light == 0){
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			fml_delay_ms(time);
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			fml_delay_ms(401-time);
		time++;
		if(time == 400) light = 1;
	}
	if(light == 1){
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			fml_delay_ms(time);
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			fml_delay_ms(401-time);
		time--;
		if(time == 1) light = 0;
	}
}
}













