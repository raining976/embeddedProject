#ifndef BSP_IC_H
#define BSP_IC_H

#include "stm32f10x.h"
#include "stdio.h"
#include "typedef.h"
#include "myutils.h"


typedef struct
{
	uint8_t remoteState; // 接收遥控器数据状态 0000 0000
	uint16_t capturevalue; // 下降沿时计数器的值
	uint32_t remoteCode; // 红外接收到的数据
	uint8_t remoteCnt; // 按键按下的次数
	uint8_t repeat_count; // 重复按键次数
	uint8_t end_flag; // 键值解码完成标志位

}IR_STRUCT;

void fml_ir_init(void);
uint8_t fml_ir_data_check(uint16_t time1, uint16_t time2, uint16_t err);
uint8_t fml_remote_decode(void);
void irCtrlTask(void);
uint8_t getIrCmd(uint8_t key);
#endif












