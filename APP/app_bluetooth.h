#ifndef BLL_BLUETOOTH_H
#define BLL_BLUETOOTH_H

#include "bluetooth.h"
#include "bsp_time.h"
#include "bsp_gpio.h"

#include "stm32f10x.h"


void BluetoothInit(void);
void BluetoothCtrlTask(void);
uint8_t BluetoothCmdSend(char *cmd,char *ack1,char *ack2,u16 timeout);
void BluetoothConfig(void);
void sendmsg(char* msg);
#endif















