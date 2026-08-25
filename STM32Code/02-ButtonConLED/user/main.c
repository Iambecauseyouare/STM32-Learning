#include "stm32f10x.h"

int main(void)
{
	//	开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//	通用输出推挽模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//	初始化PA0
	
//	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	//	输入上拉
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)	//	获取IO引脚值
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
		}
	}
}
