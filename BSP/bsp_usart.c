#include "bsp_usart.h"
#include "myutils.h"
#define USART1_IT_ENABLE	(1)
#define USART_BAUDRATE		(115200)

usart_data_t bsp_uartCache;

void bsp_usart_init(void)
{
	//�ṹ���������
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	USART_InitTypeDef USART_InitStruct = {0}; 
	
	//���ڳ�ʼ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitStruct.USART_BaudRate = USART_BAUDRATE;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	
	//�ж�����
	#if USART1_IT_ENABLE
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(5,0,0));
	#endif
	USART_Cmd(USART1,ENABLE);                                //��������
	
	//GPIO�ܽ�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	
}

// ��ȡ״̬�ֶ�
uint8_t bsp_usart_data_st(void)
{
	return bsp_uartCache.recvflag;
}

void bsp_usart_data_clear(void)
{
	memset(&bsp_uartCache,0,sizeof(bsp_uartCache));
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

void USART1_IRQHandler(void)
{
	uint8_t temp = 0;
	
	if (USART_GetITStatus(USART1,USART_IT_RXNE)) // ���ռĴ������գ��ӵ�������
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		temp = USART_ReceiveData(USART1);
		
		if ((bsp_uartCache.recvLen == 0) && (temp != 0x55)) // ��һ֡����55��ֱ�Ӷ���
		{
			memset(&bsp_uartCache,0,sizeof(bsp_uartCache));
			return;
		}
		else if (bsp_uartCache.recvLen == 4) // ���һ֡ff������
		{
			USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
			bsp_uartCache.recvflag = 1;
		}
		bsp_uartCache.recvBuff[bsp_uartCache.recvLen++] = temp;
	}
}

int fputc(int ch, FILE* file)
{	
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == 0)
	{
		/* �ȴ���һ�����ݷ������ */
	}
	return ch;
}

int fgetc(FILE*file)
{
	 while(!(USART1->SR & 1<<5));
	 return USART1->DR;
}

void UsartCtrlTask(void)
{	
	if (bsp_usart_data_st() != 1 || bsp_uartCache.recvBuff[1] != 0x01)
		return;
	
	// ��myutils.c�ļ���
	uint8_t cmd = getCmd(bsp_uartCache.recvBuff);
	handleCmd(cmd);

	bsp_usart_data_clear();
	
	
}


