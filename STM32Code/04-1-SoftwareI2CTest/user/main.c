#include "stm32f10x.h"

void My_SI2C_Init(void);
void scl_Write(uint8_t Level);
void sda_Write(uint8_t Level);
uint8_t sda_Read(void);
void delay_us(uint32_t us);
void SendStart(void);
void SendStop(void);
uint8_t send_Byte(uint8_t byte);
uint8_t Receive_Byte(uint8_t Ack);

int My_SI2C_SendBytes(uint8_t Addr, uint8_t *pData, uint16_t Size);
int My_SI2C_ReceiveBytes(uint8_t Addr, uint8_t *pBuffer, uint16_t Size);

int main(void)
{
	My_SI2C_Init();
	
	//	命令流	使能电荷泵	打开屏幕开关	让屏幕全亮
	uint8_t commands[] = {0x00, 0x8d, 0x14, 0xaf, 0xa5};
	
	My_SI2C_SendBytes(0x78, commands, 5);
	
	while(1)
	{
	}
}

void My_SI2C_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
}

void scl_Write(uint8_t Level)
{
	//	PA0
	if(Level == 0)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	}
}

void sda_Write(uint8_t Level)
{
	//	PA1
	if(Level == 0)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
	}
}

uint8_t sda_Read(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_SET)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void delay_us(uint32_t us)
{
	// 72MHz 下空循环 1us ≈ 72 次，这里放大 10 倍把 SCL 压到 ~100kHz，
	// 之前的 us*8 会让 I2C 跑到 1MHz+，远超 SSD1306 的 400kHz 上限
	uint32_t n = us * 80;
	for(uint32_t i = 0; i < n; i++);
}

void SendStart(void)
{
	sda_Write(0);
	delay_us(1);
}

void SendStop(void)
{
	scl_Write(0);
	sda_Write(0);
	delay_us(1);
	
	scl_Write(1);
	delay_us(1);
	
	sda_Write(1);
	delay_us(1);
}

uint8_t send_Byte(uint8_t byte)
{
	for(int8_t i = 7; i >= 0; i--)
	{
		scl_Write(0);
		if((byte & (0x01 << i)) != 0)
		{
			sda_Write(1);
		}
		else
		{
			sda_Write(0);
		}
		delay_us(1);
		scl_Write(1);
		delay_us(1);
	}
	
	//	读取ACK或者NAK
	scl_Write(0);
	sda_Write(1);
	delay_us(1);
	scl_Write(1);
	delay_us(1);
	return sda_Read();
}

uint8_t Receive_Byte(uint8_t Ack)
{
	uint8_t byte;
	
	for(int8_t i = 7; i >= 0; i--)
	{
		scl_Write(0);
		sda_Write(1);
			delay_us(1);
		scl_Write(1);
		delay_us(1);
		if(sda_Read() != 0)
		{
			byte |= (0x01 << i);
		}
	}
	
	scl_Write(0);
	sda_Write(!Ack);
	delay_us(1);
	scl_Write(1);
	delay_us(1);
	
	return byte;
}

int My_SI2C_SendBytes(uint8_t Addr, uint8_t *pData, uint16_t Size)
{
	//	发送起始位
	SendStart();
	
	//	寻址阶段
	if(send_Byte(Addr & 0xfe) != 0)
	{
		SendStop();
		return -1;
	}
	
	//	发送数据
	for(uint32_t i = 0; i < Size; i++)
	{
		if(send_Byte(pData[i]) != 0)
		{
			SendStop();
			return -2;
		}
	}
	
	SendStop();
	return 0;
}

int My_SI2C_ReceiveBytes(uint8_t Addr, uint8_t *pBuffer, uint16_t Size)
{
	SendStart();
	
	if(send_Byte(Addr | 0x01) != 0)
	{
		SendStop();
		return -1;
	}
	
	for(uint32_t i = 0; i < Size -1; i++)
	{
		pBuffer[i] = Receive_Byte(1);
	}
	pBuffer[Size - 1] = Receive_Byte(0);
	
	SendStop();
	
	return 0;
}
