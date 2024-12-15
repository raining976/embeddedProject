#ifndef __TIMEFUN_H__
#define __TIMEFUN_H__

#include "stm32f10x.h"

#define TASK_ENABLE     1    //任务使能状态
#define TASK_DISABLE    0    //任务失能状态 
#define TASK_COUNT      5    //任务个数

/* 任务结构体类型声明 */
typedef struct
{
	uint16_t TaskJitter;     //任务时间间隔
	uint16_t TaskPeriod;     //任务执行周期
	void(*PProcess)(void);   //任务函数
}TaskTimeParament_t;
/*
	char * fun(void);  //指针函数
	本质：就是个函数  返回值类型是个指针
	char(*fun)(void);  //函数指针
	本质：指针变量
*/

void TaskSchedulerInit(void);
void TaskSchedulerStart(void);

#endif



