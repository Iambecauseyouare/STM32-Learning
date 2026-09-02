# 实验名称：SPI写入读取W25Q64

## 1.学习目标
- 理解掌握SPI的初始化流程
- 熟悉对W25Q64模块进行操作
## 2. 硬件准备
- 开发板：STM32F103C8T6
- LED 连接：PA0（低电平点亮）
- 按钮连接：PA1
- 调试工具：CSMIS-DAP
- W25Q64
- 按钮
## 3.原理简述
SPI通过主机对从机NSS发送高低电平决定从机是否被选择，低电平为被选中，高电平为没被选中。SPI的极性就是看SCK空闲时间是处于高电平还是低电平来判断是高低极性。相位是根据上升沿还是下降沿采样进行判断是第1还是第2，时钟模式是根据硬件配置图决定的，有四种时钟模式，传输顺序是根据数据的读取写入方向决定的。
W25Q64有着固定的输入值：
0x06:写使能指令
0x20:擦除指令
0x05:等待空闲指令
0xff:读一个字节作为状态寄存器1的值
0x02:页编程指令
0x03:读取数据指令

## 4.关键代码
```
	//  对SPI进行初始化
	//	#1.初始化IO引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
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
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);
	
	//	#2. 对SPI本身进行初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStruct);
	// 设置软NSS
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
	
	//  SPI编程流程
	//	#1.	闭合总开关
	SPI_Cmd(SPIx, ENABLE);
	
	//	#2.	发送第一个字节
	SPI_I2S_SendData(SPIx, pDataTx[0]);
	
	//	#3.	
	for(uint16_t i = 0; i < Size - 1; i++)
	{
		//	向TDR写数据
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPIx, pDataTx[i + 1]);
		
		//	从RDR读数据
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		pDataRx[i] = SPI_I2S_ReceiveData(SPIx);
	}
	
	//	#4.	读取最后一个字节的数据
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	pDataRx[Size - 1] = SPI_I2S_ReceiveData(SPI1);
	
	//	#5.	闭合总开关
	SPI_Cmd(SPIx, DISABLE);
```

## 5.问题
在写使能代码的时候忘记对buffer复制。
在调用button.h头文件时，不熟悉调用操作没有对button接口进行初始化赋值。

## 6.解决方法
细心检查代码，对驱动接口文件这么进行自主的一次重写调用。