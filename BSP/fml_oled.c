#include "fml_oled.h"
#include "OLED_Font.h"
#include "string.h"
#include "bsp_time.h"
/*引脚配置*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))
#define OLED_R_SDA			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
/*引脚初始化*/
void OLED_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	//fml_delay_us(4);
	OLED_W_SDA(0);
	//fml_delay_us(4);
	OLED_W_SCL(0);
}


/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(0);
	//fml_delay_us(4);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
	//fml_delay_us(4);
}
/**  
  *  功能：等待应答信号ACK
  *  入口参数：无
  *  返回值：0，接受应答成功；1，接受应答失败
  */
uint8_t OLED_I2C_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;

	OLED_W_SDA(1);
	//fml_delay_us(1);
	OLED_W_SCL(1);
	//fml_delay_us(1);
	while(OLED_R_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			OLED_I2C_Stop();
			return 1;
		}
	}
	OLED_W_SCL(0);
	return 0;
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		//fml_delay_us(2);
		OLED_W_SCL(0);
		//fml_delay_us(2);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	//fml_delay_us(2);
	OLED_W_SCL(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
//	OLED_WriteCommand(0x20);						//设置GDDRAM寻址方式
//	OLED_WriteData(0x01);							//页寻址方式
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}
/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_ShowFrame(const uint8_t data[][128])
{  
	uint8_t i, j;
	
	OLED_WriteCommand(0x20);						//设置GDDRAM寻址方式
	OLED_WriteCommand(0x00);							//页寻址方式
	OLED_WriteCommand(0x21);					//设置Y位置
	OLED_WriteCommand(0x00);		//设置X位置高4位
	OLED_WriteCommand(0x7f);
	OLED_WriteCommand(0x22);
	OLED_WriteCommand(0x00);
	OLED_WriteCommand(0x07);
	for (j = 0; j < 8; j++)
	{
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(data[j][i]);
		}
	}
}
/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED显示一个汉字
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~8
  * @param  str 要显示的一个汉字 hzk中包含的汉字
  * @retval 无
  */
void OLED_ShowChineseWord(uint8_t Line, uint8_t Column, char *str)
{
	while(*str != '\0')
	{
		if (Column > 8)
		{
			Line += 1;
			Column = 1;
		}
		OLED_ShowChinese(Line, Column, str);
		str += 2;
		Column += 1;
	}
}
/**
  * @brief  OLED显示一个汉字
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~8
  * @param  str 要显示的一个汉字 hzk中包含的汉字
  * @retval 无
  */
void OLED_ShowChinese(uint8_t Line, uint8_t Column, char *str)
{      	
	uint8_t i = 0,flag = 0;
	char buff[32] = {0};
	
	for(i = 0;i < HZK_NUM;i++)
	{
		if ((*str == hzk[i * 2]) && (*(str + 1) == hzk[i * 2 + 1]))
		{
			memcpy(buff,&OLED_F16x16[i * 32],32);
			flag = 1;
			break;
		}
	}
	if(flag == 0)
		return;
	
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 16);		//设置光标位置在上半部分
	for (i = 0; i < 16; i++)
	{
		OLED_WriteData(buff[i]);							//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 16);	//设置光标位置在下半部分
	for (i = 16; i < 32; i++)
	{
		OLED_WriteData(buff[i]);							//显示下半部分内容
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
//	OLED_WriteCommand(0XAE);		// 关OLED显示
//	// 基础设置
//	OLED_WriteCommand(0XA4);		// 输出GDDRAM内容	
//	OLED_WriteCommand(0XA6);		// 正常显示(1亮0灭)	
//	OLED_WriteCommand(0X81);		// 设置对比度		
//	OLED_WriteCommand(0X7F);		// 第127级对比度	
//	// COM和SEG输出设置
//	OLED_WriteCommand(0XD3);		// 设置垂直显示偏移(向上)
//	OLED_WriteCommand(0X00);		// 偏移0行					
//	OLED_WriteCommand(0X40);		// 设置GDDRAM起始行 0		
//	OLED_WriteCommand(0XA8);		// 设置MUX数 (显示行数)		
//	OLED_WriteCommand(0X3F);		//  MUX=63	 (显示63行)			
//	OLED_WriteCommand(0XA1);		// 左右反置关(段重映射)		
//	OLED_WriteCommand(0XC8);		// 上下反置关(行重映射)		
//	OLED_WriteCommand(0XDA);		// 设置COM引脚配置		
//	OLED_WriteCommand(0X02);		// 序列COM配置,禁用左右反置
//	// 时钟设置
//	OLED_WriteCommand(0XD5);		// 设置DCLK分频和OSC频率
//	OLED_WriteCommand(0X80);		// 无分频,第8级OSC频率	
//	// 开OLED
//	OLED_WriteCommand(0X8D);		// 启用电荷泵
//	OLED_WriteCommand(0X14);		// 启用电荷泵
//	OLED_WriteCommand(0XAF);		// 开OLED显示
	OLED_Clear();				//OLED清屏
	
	


}
