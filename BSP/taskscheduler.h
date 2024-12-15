#ifndef __TIMEFUN_H__
#define __TIMEFUN_H__

#include "stm32f10x.h"

#define TASK_ENABLE     1    //����ʹ��״̬
#define TASK_DISABLE    0    //����ʧ��״̬ 
#define TASK_COUNT      5    //�������

/* ����ṹ���������� */
typedef struct
{
	uint16_t TaskJitter;     //����ʱ����
	uint16_t TaskPeriod;     //����ִ������
	void(*PProcess)(void);   //������
}TaskTimeParament_t;
/*
	char * fun(void);  //ָ�뺯��
	���ʣ����Ǹ�����  ����ֵ�����Ǹ�ָ��
	char(*fun)(void);  //����ָ��
	���ʣ�ָ�����
*/

void TaskSchedulerInit(void);
void TaskSchedulerStart(void);

#endif



