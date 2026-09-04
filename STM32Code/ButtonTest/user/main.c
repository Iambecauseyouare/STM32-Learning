#include "stm32f10x.h"
#include "delay.h"

void App_Button_Init(void);
void App_OnBoardLED_Init(void);

int main(void)
{
	App_Button_Init();	//	按钮初始化
	App_OnBoardLED_Init();	//	LED初始化
	
	uint8_t current = Bit_SET;
	uint8_t previous = Bit_SET;
	
	while(1)
	{
		previous = current;
		
		current = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		
		if(previous != current)
		{
			if(current == Bit_SET)
			{
				//	改变LED亮灭状态
				if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET)
				{
					GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
				}
				else
				{
					GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
				}
//				GPIO_WriteBit(GPIOC, GPIO_Pin_13, (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET) ? Bit_RESET : Bit_SET);
				Delay(10);
			}
		}
	}
}

void App_Button_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(GPIOA, &GPIOInitStruct);
}

void App_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC, &GPIOInitStruct);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
}
