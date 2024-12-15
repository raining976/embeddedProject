#ifndef TYPEDEF_H
#define TYPEDEF_H

#include "stm32f10x.h"

typedef struct{
	uint8_t recvBuff[10];	//数据接收缓冲区
	uint8_t recvflag;		//数据接收标志位
	uint8_t recvLen;		//数据接收长度
}usart_data_t;

typedef enum{
	ID_ERR = 1,
	DATA_ERR,
	LED_ON,
	LED_OFF,
	BUZZER_ON,
	BUZZER_OFF,
	MOTOR_FORWARD,
	MOTOR_BACKWORD,
	MOTOR_BRAKE,
	MOTOR_1,
	MOTOR_2,
	MOTOR_3,
	MOTOR_SWITCH, // 开/关
	ADD_LEVEL, // ++
	SUB_LEVEL, // --
}usart_data_result_e;

typedef enum{
	PERIPH_STEP_CFG = 0,		//参数配置阶段
	PERIPH_STEP_TXM,		//数据传输阶段
}periph_comm_step_e;

typedef struct{
	uint8_t recv_data[128];
	uint8_t recv_len;
	uint8_t recv_flag;
	uint8_t comm_step;		//外围器件交互步骤 参数配置、数据透传
	uint8_t send_data[128];
	uint8_t send_len;
}bt_data_cache_t;

typedef struct{
	unsigned short Index;
	unsigned char Msk[32];
}typch;

typedef struct{
	uint8_t remote_flag;		//遥控标志位 优先级最高
	uint8_t motor_state;		//电机状态
	uint8_t motor_speed;		//电机速度
	uint8_t startup_stop;
	uint8_t temprature;			//温度
	uint8_t humidity;			//湿度
	uint8_t beep_state;			//蜂鸣器状态
	uint8_t bt_state;			//蓝牙状态
	uint8_t page;				//主界面显示页数
	uint8_t dataChange;			//显示页数据变化标志位
	uint8_t temprature_limit_high;	//温度高阈值
	uint8_t temprature_limit_low;	//温度低阈值
	uint8_t humidity_limit;		//湿度阈值
	uint8_t flag_buzzer;
	uint8_t cur_ratio;   //当前速度
	uint8_t flag_state_change;
	uint8_t flag_first;
	uint8_t flag_motor_ir;
}sys_info_t;


























#endif
