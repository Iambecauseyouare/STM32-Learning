#include "stm32f10x.h"

int main(void)
{
	//	开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,	ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	
	//	USART1 波特率115200，数据位8位，停止位1位，无校验
	USART_InitStruct.USART_BaudRate = 115200;	//	波特率115200
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	// 8位数据位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//	1位停止位
	USART_InitStruct.USART_Parity = USART_Parity_No;	//	无校验
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//	双向
	
	USART_Init(USART1,&USART_InitStruct);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
//	//	PA9 tx 复用输出推挽模式 10MHz
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,	ENABLE);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//	//	PA10 rx 复用输出推挽模式 10MHz
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,	ENABLE);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	
	//PB6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,	ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PB7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,	ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	while(1)
	{
	}
}
