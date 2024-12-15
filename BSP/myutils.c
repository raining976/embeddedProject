#include "myutils.h"
#include "app.h"


uint8_t getnum(uint8_t hex_value){
	uint8_t high_digit = (hex_value >> 4) & 0x0F;  // 提取高4位
  uint8_t low_digit = hex_value & 0x0F;      // 提取低4位
    
    // 将它们按十进制方式连接成一个新数字
  int result = high_digit * 10 + low_digit; 
	return result;
}

uint8_t getCmd(uint8_t *data)
{
	uint8_t cmd = 0;
	
	if (data[1] != 0x01)
		return ID_ERR;
	
	switch(data[2]) // 第三个帧说明要控制哪个外设
	{
		case 0x01://led 点灯
			if (data[3] == 0x01) // 第四个帧控制开关
				cmd = LED_ON; 
			else if (data[3] == 0x00)
				cmd = LED_OFF;
			else
				cmd = DATA_ERR;
			break;
		case 0x02://蜂鸣器
			if (data[3] == 0x01)
				cmd = BUZZER_ON;
			else if (data[3] == 0x00)
				cmd = BUZZER_OFF;
			else
				cmd = DATA_ERR;
			break;
		case 0x03://motor
			if(data[3] == 0x01)
				cmd = MOTOR_FORWARD;
			else if(data[3] == 0x02)
				cmd = MOTOR_BACKWORD;
			else if(data[3] == 0x00)
				cmd = MOTOR_BRAKE;
			else if(data[3] == 0x11)
				cmd = MOTOR_1;
			else if(data[3] == 0x12)
				cmd = MOTOR_2;
			else if(data[3] == 0x13)
				cmd = MOTOR_3;
			else 
				cmd = DATA_ERR;
			break;
		case 0x04://温度高阈值
			{sys_parm.temprature_limit_high = getnum(data[3]);printf("当前的温度高阈值为：0x%d\r\n",sys_parm.temprature_limit_high);}
			break;
		case 0x05://温度低阈值
			{sys_parm.temprature_limit_low = getnum(data[3]);printf("当前的温度低阈值为：0x%d\r\n",sys_parm.temprature_limit_low);}
			break;
		case 0x06://湿度高阈值
			{sys_parm.humidity_limit = getnum(data[3]);printf("当前的湿度高阈值为：0x%d\r\n",sys_parm.humidity_limit);}
			break;
		case 0x07:
			sys_parm.flag_state_change = 1;
			if(data[3] == 0x01)
				sys_parm.page += 1;
				if (sys_parm.page > 2)
				sys_parm.page = 1;
			if(data[3] == 0x02)
				sys_parm.page -= 1;
				if (sys_parm.page < 1)
				sys_parm.page = 2;
			break;
			
		default:
			cmd = DATA_ERR;
			break;
	}
	return cmd;	
}

uint8_t isRunning = 0;

void motor_start(){
	fml_motor_forward();
	led2_on();
	isRunning = 1;
}

void motor_stop(){
	fml_motor_resetV();
	fml_motor_stop();
	led2_off();
	isRunning = 0;
}


void handleCmd(uint8_t cmd)
{
		switch(cmd)
		{
			case LED_ON:
				led1_on();
				led2_on();
				break;
			case LED_OFF:
				led1_off();
				led2_off();
				break;
			case BUZZER_ON:
				buzzer_on();
				break;
			case BUZZER_OFF:
				buzzer_off();
				sys_parm.flag_buzzer = 1;
				break;
			case MOTOR_FORWARD:
				motor_start();
				sys_parm.motor_state = 1;
				sys_parm.motor_speed = sys_parm.cur_ratio;
				break;
			case MOTOR_BACKWORD:
				fml_motor_backward();
				led2_on();
				isRunning = 1;
				sys_parm.motor_state = 1;
				sys_parm.motor_speed = sys_parm.cur_ratio;
				break;
			case MOTOR_BRAKE:
				motor_stop();
				sys_parm.motor_speed = 0;
				sys_parm.motor_state = 0;
				break;
			case MOTOR_1:
				fml_motor_setV(1);
				sys_parm.motor_speed = sys_parm.cur_ratio;
				break;
			case MOTOR_2:
				fml_motor_setV(2);
				sys_parm.motor_speed = sys_parm.cur_ratio;
				break;
			case MOTOR_3:
				fml_motor_setV(3);
				sys_parm.motor_speed = sys_parm.cur_ratio;
				break;
			case MOTOR_SWITCH:
				if(isRunning == 1){
					motor_stop();
					sys_parm.motor_state = 0;
					sys_parm.motor_speed = 0;
				}else{
					motor_start();
					sys_parm.motor_state = 1;
					sys_parm.motor_speed = sys_parm.cur_ratio;
				}
				break;
			case ADD_LEVEL:
				fml_motor_addV();
				sys_parm.motor_speed = sys_parm.cur_ratio;
				break;
			case SUB_LEVEL:
				fml_motor_subV();
				sys_parm.motor_speed = sys_parm.cur_ratio;
				break;
			case ID_ERR:
				printf("ERROR!!\r\n");
				break;
			default:
				break;
		}
}




void handledht11(uint8_t temperature, uint8_t humidity){
	
	DHT11_Read_Data(&temperature,&humidity);
	sys_parm.dataChange = 1;
	sys_parm.humidity = humidity;
	sys_parm.temprature = temperature;
	if(temperature > sys_parm.temprature_limit_high && sys_parm.flag_motor_ir == 0) motor_start();
	if(temperature <= sys_parm.temprature_limit_high) sys_parm.flag_motor_ir = 0;
	if(temperature < sys_parm.temprature_limit_low) {motor_stop() ; sys_parm.flag_motor_ir = 0;}
	if(humidity < sys_parm.humidity_limit) sys_parm.flag_buzzer = 0;
	if((humidity > sys_parm.humidity_limit) && sys_parm.flag_buzzer == 0) buzzer_on();
	return;
	
}




	
	
	
	
	