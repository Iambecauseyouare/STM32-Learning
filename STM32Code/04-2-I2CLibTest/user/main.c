#include "stm32f10x.h"
#include "si2c.h"

//void My_I2C1_Init(void);
void My_OnBoardLED(void);

SI2C_TypeDef si2c;

int main(void)
{
//	My_I2C1_Init();
	My_OnBoardLED();
	
	si2c.SCL_GPIOx = GPIOB;
	si2c.SCL_GPIO_Pin = GPIO_Pin_6;
	si2c.SDA_GPIOx = GPIOB;
	si2c.SDA_GPIO_Pin = GPIO_Pin_7;
	My_SI2C_Init(&si2c);
	
	// 有应答：发送命令流 使能电荷泵→开显示→全屏点亮(0xa5)
	uint8_t commands[] = {0x00, 0x8d, 0x14, 0xaf, 0xa5};
//	My_I2C_SendBytes(I2C1, 0x78, commands, 5);
	My_SI2C_SendBytes(&si2c, 0x78, commands, 5);
	
	uint8_t rcvd;
	
//	My_I2C_ReceiveBytes(I2C1, 0x78, &rcvd, 1);
	My_SI2C_ReceiveBytes(&si2c, 0x78, &rcvd, 1);
	
	if((rcvd & (0x01 << 6)) == 0)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	}
	
	while(1)
	{
	}
}

//void My_I2C1_Init(void)
//{
//	//	#1. 对io引脚初始化
//	GPIO_InitTypeDef GPIO_InitStruct;
//	
//	//	PB6和PB7初始化
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	
//	GPIO_Init(GPIOB, &GPIO_InitStruct);
//	
//	//	对I2C1进行初始化
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
//	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
//	
//	I2C_InitTypeDef I2C_InitStruct;
//	I2C_InitStruct.I2C_ClockSpeed = 400000;	// 400kHz降到100kHz（04-1项目经验：该屏在高速下不稳定）
//	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
//	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
//	//I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;	// 必须显式初始化，否则未定义值会写入CR1（可能置位SMBUS位）
//	//I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

//	I2C_Init(I2C1, &I2C_InitStruct);
//	
//	I2C_Cmd(I2C1, ENABLE);	//	闭合总开关
//}

void My_OnBoardLED(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC, &GPIOInitStruct);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
}
