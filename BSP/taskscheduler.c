#include "taskscheduler.h"
#include "systick.h"
#include "bsp_gpio.h"
#include "bsp_usart.h"
#include "bsp_ic.h"
#include "stdlib.h"
#include "app_bluetooth.h"
#include "dht11.h"
#include "myutils.h"
#include "app_bluetooth.h"
#include "app.h"

#define SCALER              		(1)
#define TS_1MS_JITTER  				(0U)
#define TS_5MS_JITTER  				(0U)
#define TS_10MS_JITTER  			(2U * SCALER)
#define TS_20MS_JITTER  			(4U * SCALER)
#define TS_50MS_JITTER  			(6U * SCALER)
#define TS_100MS_JITTER  			(8U * SCALER)
#define TS_200MS_JITTER  			(8U * SCALER)
#define TS_500MS_JITTER  			(10U * SCALER)

#define TS_1MS_COUNT  				(1U * SCALER)
#define TS_5MS_COUNT  				(5U * SCALER)
#define TS_10MS_COUNT  				(10U * SCALER)
#define TS_20MS_COUNT  				(20U * SCALER)
#define TS_50MS_COUNT  				(50U * SCALER)
#define TS_100MS_COUNT  			(100U * SCALER)
#define TS_200MS_COUNT  			(200U * SCALER)
#define TS_500MS_COUNT  			(500U * SCALER)

#define TS_NUM						(8U)

static void Task1MS(void);
static void Task5MS(void);
static void Task10MS(void);
static void Task20MS(void);
static void Task50MS(void);
static void Task100MS(void);
static void Task200MS(void);
static void Task500MS(void);
/* 时间片任务定义 */
static const TaskTimeParament_t g_TSTimeParament[TS_NUM] = 
{
	{TS_1MS_JITTER,TS_1MS_COUNT,Task1MS},
	{TS_5MS_JITTER,TS_5MS_COUNT,Task5MS},
	{TS_10MS_JITTER,TS_10MS_COUNT,Task10MS},
	{TS_20MS_JITTER,TS_20MS_COUNT,Task20MS},
	{TS_50MS_JITTER,TS_50MS_COUNT,Task50MS},
	{TS_100MS_JITTER,TS_100MS_COUNT,Task100MS},
	{TS_100MS_JITTER,TS_200MS_COUNT,Task200MS},
	{TS_500MS_JITTER,TS_500MS_COUNT,Task500MS},
};

static uint16_t g_S_TSRuntimeEnv[TS_NUM];

void TaskSchedulerInit(void)
{
	uint8_t TsInit_Index = 0;
	SysTick_Init();
	for (TsInit_Index = 0;TsInit_Index < TS_NUM;TsInit_Index++)
	{
		g_S_TSRuntimeEnv[TsInit_Index] = 
			g_TSTimeParament[TsInit_Index].TaskJitter + 
			g_TSTimeParament[TsInit_Index].TaskPeriod;
	}
}
void TaskSchedulerStart(void)
{
	uint8_t TsStart_Index = 0;
	
	if (SystickGetTimebaseFlag() == SET)
	{
		for(TsStart_Index = 0;TsStart_Index < TS_NUM;TsStart_Index++)
		{
			if (g_S_TSRuntimeEnv[TsStart_Index] > 0)
			{
				g_S_TSRuntimeEnv[TsStart_Index]--;
			}
			if (g_S_TSRuntimeEnv[TsStart_Index] == 0)
			{
				g_TSTimeParament[TsStart_Index].PProcess();
				g_S_TSRuntimeEnv[TsStart_Index] = g_TSTimeParament[TsStart_Index].TaskPeriod;
			}
			
		}
		SystickClearTimebaseFlag();
	}
}


static void Task1MS(void)
{
	
}
static void Task5MS(void)
{
	
}
static void Task10MS(void)
{
	

}
static void Task20MS(void)
{
	UsartCtrlTask();
	BluetoothCtrlTask();
}
static void Task50MS(void)
{
	
}

static void Task100MS(void)
{		
	irCtrlTask();
}
static void Task200MS(void)
{	

	oledDispalyTask();
	
}


static void Task500MS(void)
{	
	static uint8_t cnt = 0,temperature = 0,humidity = 0;
	cnt++;
	if (cnt == 2)
	{
		DHT11_Read_Data(&temperature,&humidity);
		handledht11(temperature, humidity);
		printf("当前的温度是：%d℃\r\n",temperature);
		printf("当前的湿度是：%d%%\r\n",humidity);
		char msg[50];
		sprintf(msg, "The temperature is：%d°C, the humidity is：%d%%", temperature, humidity);
		sendmsg(msg);
		cnt = 0;
	}
}



















