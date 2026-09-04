#include "stm32f10x.h"
#include "delay.h"

uint32_t blinkInterval = 1000;	//	…¡µ∆º‰∏Ù

void App_OnBoardLED_Init(void);
void App_Usart1_Init(void);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	App_OnBoardLED_Init();
	Delay_Init();
	App_Usart1_Init();
	
	while(1)
	{
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);	//	¡¡
			Delay(blinkInterval);	//	—”≥Ÿ
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);	//	√
			Delay(blinkInterval);	//	—”≥Ÿ
	}
}

void App_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void App_Usart1_Init(void)
{
	//	#1. ≥ı ºªØPA9∫ÕPA10
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//	#2. ≥ı ºªØUSART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
	
	//	#3. ≈‰÷√÷–∂œ 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//	#4.	≈‰÷√NVICƒ£øÈ
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;	//	0-3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}

//	÷–∂œ∫Ø ˝
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		uint8_t byte = USART_ReceiveData(USART1);
	
		if(byte == '0')
		{
			blinkInterval = 1000;
		}
		else if(byte == '1')
		{
			blinkInterval = 200;
		}
		else if(byte == '2')
		{
			blinkInterval = 50;
		}
	}
}
