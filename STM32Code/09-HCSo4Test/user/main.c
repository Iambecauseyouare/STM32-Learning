#include "stm32f10x.h"
#include "usart.h"
#include "Delay.h"

void App_USART1_Init(void);
void App_HCSR04_Init(void);

int main(void)
{
	App_USART1_Init();
	App_HCSR04_Init();
	
	My_USART_SendString(USART1, "Hello World\r\n");
	while(1)
	{
		// #1. 向CNT写0
		TIM_SetCounter(TIM1, 0);
		//	#2. 清除CC1和CC2标志位
		TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
		TIM_ClearFlag(TIM1, TIM_FLAG_CC2);
		
		// #3. 开启定时器
		TIM_Cmd(TIM1, ENABLE);
		
		//	#4.向trig引脚发送10us脉冲
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		
		DelayUs(10);
		
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		
		//	#5. 等待测量完成
		while(TIM_GetFlagStatus(TIM1, TIM_FLAG_CC1) == RESET);
		while(TIM_GetFlagStatus(TIM1, TIM_FLAG_CC2) == RESET);
		
		// #6. 关闭定时器
		TIM_Cmd(TIM1, DISABLE);
		
		uint16_t ccr1 = TIM_GetCapture1(TIM1);
		uint16_t ccr2 = TIM_GetCapture1(TIM1);
		
		float distance = (ccr2 - ccr1) * 1.0e-6f * 340.0f / 2;
		
		My_USART_Printf(USART1, "distance = %.4f\r\n", distance);
		
		Delay(100);
	}
}

void App_USART1_Init(void)
{
	//	#1.	初始化IO引脚
	GPIO_InitTypeDef GPIO_InitStruct;
	//	PA9 AF_PP	Tx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//	PA10 IPU Rx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//	#2.	初始化USART1
	//	#2.1 开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//	#2.2 初始化USART1的参数
	USART_InitTypeDef USART1_InitStruct;
	USART1_InitStruct.USART_BaudRate = 115200;
	USART1_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART1_InitStruct.USART_Parity = USART_Parity_No;
	USART1_InitStruct.USART_StopBits = USART_StopBits_1;
	USART1_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART1_InitStruct);
	
	//	#2.3	闭合总开关
	USART_Cmd(USART1, ENABLE);
}

void App_HCSR04_Init(void)
{
	//	#1. 初始化时基单元
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	//	#2.	初始化输入捕获
	//	#2.1	初始化io引脚 PA8 IPD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//	#2.2 初始化输入捕获通道1
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	
	TIM_ICInit(TIM1, &TIM_ICInitStruct);
	
	//	#2.3 初始化输入捕获通道2
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	
	TIM_ICInit(TIM1, &TIM_ICInitStruct);
	
	//	#3.	初始化Trig引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
