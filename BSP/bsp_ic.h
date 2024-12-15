#ifndef BSP_IC_H
#define BSP_IC_H

#include "stm32f10x.h"
#include "stdio.h"
#include "typedef.h"
#include "myutils.h"


typedef struct
{
	uint8_t remoteState; // ����ң��������״̬ 0000 0000
	uint16_t capturevalue; // �½���ʱ��������ֵ
	uint32_t remoteCode; // ������յ�������
	uint8_t remoteCnt; // �������µĴ���
	uint8_t repeat_count; // �ظ���������
	uint8_t end_flag; // ��ֵ������ɱ�־λ

}IR_STRUCT;

void fml_ir_init(void);
uint8_t fml_ir_data_check(uint16_t time1, uint16_t time2, uint16_t err);
uint8_t fml_remote_decode(void);
void irCtrlTask(void);
uint8_t getIrCmd(uint8_t key);
#endif












