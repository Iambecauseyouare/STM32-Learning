#include "stm32f10x.h"

void App_SPI1_Init(void);

int main(void)
{
	App_SPI1_Init();
	while(1)
	{
	}
}

void App_SPI1_Init(void)
{
	//	#1.初始化IO引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
	
	// PB3 SCK AF_PP 2MHz -	PB5 MOSI AFPP 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GOIO_InitStruct;
	GOIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GOIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GOIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB, &GOIO_InitStruct);
	
	//	PB4 MISO IPU
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GOIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GOIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	
	GPIO_Init(GPIOB, &GOIO_InitStruct);
	
	// PA15	普通IO Out_PP 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GOIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GOIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GOIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GOIO_InitStruct);
	
	//	#2. 对SPI本身进行初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
	
}
