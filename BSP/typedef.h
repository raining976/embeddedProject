#ifndef TYPEDEF_H
#define TYPEDEF_H

#include "stm32f10x.h"

typedef struct{
	uint8_t recvBuff[10];	//���ݽ��ջ�����
	uint8_t recvflag;		//���ݽ��ձ�־λ
	uint8_t recvLen;		//���ݽ��ճ���
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
	MOTOR_SWITCH, // ��/��
	ADD_LEVEL, // ++
	SUB_LEVEL, // --
}usart_data_result_e;

typedef enum{
	PERIPH_STEP_CFG = 0,		//�������ý׶�
	PERIPH_STEP_TXM,		//���ݴ���׶�
}periph_comm_step_e;

typedef struct{
	uint8_t recv_data[128];
	uint8_t recv_len;
	uint8_t recv_flag;
	uint8_t comm_step;		//��Χ������������ �������á�����͸��
	uint8_t send_data[128];
	uint8_t send_len;
}bt_data_cache_t;

typedef struct{
	unsigned short Index;
	unsigned char Msk[32];
}typch;

typedef struct{
	uint8_t remote_flag;		//ң�ر�־λ ���ȼ����
	uint8_t motor_state;		//���״̬
	uint8_t motor_speed;		//����ٶ�
	uint8_t startup_stop;
	uint8_t temprature;			//�¶�
	uint8_t humidity;			//ʪ��
	uint8_t beep_state;			//������״̬
	uint8_t bt_state;			//����״̬
	uint8_t page;				//��������ʾҳ��
	uint8_t dataChange;			//��ʾҳ���ݱ仯��־λ
	uint8_t temprature_limit_high;	//�¶ȸ���ֵ
	uint8_t temprature_limit_low;	//�¶ȵ���ֵ
	uint8_t humidity_limit;		//ʪ����ֵ
	uint8_t flag_buzzer;
	uint8_t cur_ratio;   //��ǰ�ٶ�
	uint8_t flag_state_change;
	uint8_t flag_first;
	uint8_t flag_motor_ir;
}sys_info_t;


























#endif
