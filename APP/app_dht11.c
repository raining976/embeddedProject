#include "app_dht11.h"
#include "stdio.h"
/*****************************************************************************************
函数名称  ： DHT11_Init
函数功能  ： DHT11管脚初始化、发送起始信号并检测应答
函数形参  ： 无
函数返回值： 初始化成功：0；初始化失败：1
函数描述  ：
******************************************************************************************/
uint8_t DHT11Init (void)
{
	DHT11_IO_Init();
	
	if(!DHT11_Strat_Ack())
	{
		return 0;
	}
	return 0;
}
