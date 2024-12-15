#include "app_bluetooth.h"
#include "bluetooth.h"
#include "myutils.h"
void BluetoothInit(void)
{
	fml_bt_uart_init();
	fml_bt_uart_data_clear();
	fml_bt_state_init();//����״̬�ܽų�ʼ��
	bt_usart.comm_step = PERIPH_STEP_CFG;		//�������ò���
}


void sendmsg(char* msg){
	fml_bt_uart_data_send(msg);
}

void BluetoothCtrlTask(void)
{	
	if(BT_STATE_READ() == 1){
		sys_parm.bt_state = 1;
		led1_on();
	}else{
		sys_parm.bt_state = 0;
		led1_off();
	}
	if (bt_usart.comm_step != PERIPH_STEP_TXM || fml_bt_data_st() != 1 )
		return;
	
	handleCmd(getCmd(bt_usart.recv_data));
	printf("recv_data: ");
	for (int i = 0; i < bt_usart.recv_len; i++) {
        // ��ӡÿ���ֽڣ���ʮ�����Ƹ�ʽ
        printf("%02X ", bt_usart.recv_data[i]);
    }
	fml_bt_uart_data_clear();
	
		
}


	
	
/*
 *	�����ײ㴮�ڳ�ʼ����ɺ󣬽��в�������
 *
*/
void BluetoothConfig(void)
{
	static uint8_t step = 0;
	static uint8_t temp = 0;
	
	while(step != 5)
	{
		if (BT_STATE_READ() == 1)
		{
			led1_on();
			bt_usart.comm_step = PERIPH_STEP_TXM;		//�������ò���
			printf("> bluetooth connect success\r\n");
			return;
		}
		switch(step)
		{
			case 0:
				temp = BluetoothCmdSend("AT\r\n","OK\r\n",NULL,500);//����ATָ��
				if (temp)
					step = 1;
			break;
			case 1:
				BluetoothCmdSend("AT+NAME=kukudedadie\r\n","OK\r\n",NULL,500);//�޸�����
				temp = BluetoothCmdSend("AT+NAME?\r\n","+NAME:kukudedadie\r\n","OK\r\n",500);  //��ȡ�޸ĵ�����
				if (temp)
					step = 2;
			break;
			case 2:
				BluetoothCmdSend("AT+PSWD=\"1234\"\r\n","OK\r\n",NULL,500);//�޸������
				temp = BluetoothCmdSend("AT+PSWD?\r\n","+PSWD:1234\r\n","OK\r\n",500);
				if (temp)
					step = 3;
			break;
			case 3:				
				BluetoothCmdSend("AT+ROLE=0\r\n","+ROLE:0\r\n","OK\r\n",500);
				temp =  BluetoothCmdSend("AT+ROLE?\r\n","+ROLE:0\r\n","OK\r\n",500);
				if (temp)
					step = 4;
			case 4:
				BluetoothCmdSend("AT+UART=38400,0,0\r\n","OK\r\n",NULL,500);
				temp = BluetoothCmdSend("AT+UART?\r\n","+UART:38400,0,0\r\n","OK\r\n",500);
				if (temp)
					BluetoothCmdSend("AT+RESET\r\n","OK\r\n",NULL,100);//��λ
				while(BT_STATE_READ() != 1);
				step = 5;
				bt_usart.comm_step = PERIPH_STEP_TXM;		//�������ò���
				printf("> bluetooth connect success\r\n");
			break;

		}
		
	}
	
	step = 0;
}

/**
��* @brief  ����ATָ��ͺ���
��* @param  cmd:ָ��
  * @param  ack1:��ָ�����Ӧ1
  * @param  ack2:��ָ�����Ӧ2
  * @param  timeout:��ָ��ĳ�ʱ����
��* @retval uint8_t 1 NO_ERR 0:ERR
  */
uint8_t BluetoothCmdSend(char *cmd,char *ack1,char *ack2,u16 timeout)
{
	u8 result = 0;
	
	fml_bt_uart_data_send(cmd);
	fml_delay_ms(timeout);

	printf("> %s\r\n",bt_usart.recv_data);
	if ( (ack1 != NULL) && (ack2 != NULL) )                     //��ָ��������Ӧ��
	{
		result = strstr((const char *)bt_usart.recv_data,ack1) ||      //�Ƿ������һ��Ӧ�� 
						 strstr((const char *)bt_usart.recv_data,ack2);        //�Ƿ�����ڶ���Ӧ�� 
	}
	else if (ack1 != NULL)                                      //��ָ��ֻ��ack1Ӧ��
	{
		result = strstr((const char *)bt_usart.recv_data,ack1) != NULL;
	}
	else if (ack2 != NULL)                                                       //��ָ��ֻ��ack2Ӧ�� 
	{
		result = strstr((const char *)bt_usart.recv_data,ack2) != NULL;
	}	
	fml_bt_uart_data_clear();	
	return result;
}















