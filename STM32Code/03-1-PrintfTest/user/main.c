#include "stm32f10x.h"
#include "stdio.h"
#include "Delay.h"

void My_USART_SendBytes(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size);
void My_USART1_Init(void);

int main(void)
{	
	Delay_Init();
	My_USART1_Init();
	
//	printf("Hello World\r\n");
	
	while(1)
	{
		uint32_t currentTick = GetTick();
		
		uint32_t millSeconds = currentTick % 1000;
		currentTick /= 1000;
		
		uint32_t seconds = currentTick % 60;
		currentTick /= 60;
		
		uint32_t minutes = currentTick % 60;
		currentTick /= 60;
		
		uint32_t hour = currentTick;
		
		printf("%02u:%02u:%02u:%03u\r\n", hour, minutes, seconds, millSeconds);
		
		Delay(100);
	}
}

//
// @简介：使用串口一次性发送多个字节
//	@参数：USARTx：填写串口的名称
//	@参数：pData：要发送的数据
//	@参数：Szie：要发送数据的数量，单位是字节
//
void My_USART_SendBytes(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size)
{
	for(uint32_t i = 0; i < Size; i++)
	{
		//	#1.	等待发送数据寄存器为空
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		//#2.	写入发送数据寄存器当中
		USART_SendData(USART1, pData[i]);
	}
	//	#3.	等待数据发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

//
//	@简介：对USART1进行初始化
//				 PB6 - Tx PB7 -	Rx
//				 115200，8，1，None，双向
//
void My_USART1_Init(void)
{
	//	初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//	PA9 tx 复用输出推挽模式 10MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,	ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//	//	PA10 rx 上拉输入模式 10MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,	ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
		//	重映射
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
//	
//	//PB6
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,	ENABLE);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//	//PB7
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,	ENABLE);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//	开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,	ENABLE);
	//	初始化USART1
	USART_InitTypeDef USART_InitStruct;
	
	//	USART1 波特率115200，数据位8位，停止位1位，无校验
	USART_InitStruct.USART_BaudRate = 115200;	//	波特率115200
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	// 8位数据位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//	1位停止位
	USART_InitStruct.USART_Parity = USART_Parity_No;	//	无校验
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//	双向
	
	USART_Init(USART1,&USART_InitStruct);	//	初始化
	
	USART_Cmd(USART1, ENABLE);	// 闭合总开关
}

int fputc(int ch, FILE *f)
{
	//#1.等待TDR为空
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	//#2.发送
	USART_SendData(USART1, (uint8_t)ch);
	
	return ch;
}
