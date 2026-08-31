# 实验名称：串口接收和发送数据
## 1.学习目标
- 理解串口通信的概念
- 熟悉串口的初始化，函数的调用和封装
- 熟悉重映射机制

## 2.硬件准备
-  开发板：STM32F103C8T6
- LED 连接：PA0（低电平点亮）
- 按钮连接：PA1
- 调试工具：CSMIS-DAP
- 串口调试助手
- 串口调试工具

## 3.原理简述
USART通过移位寄存器和串并转换接收和发送数据，依靠波特率和数据帧格式实现双向或者单向的通信，通过特定的标志位来判断寄存器是否为空。

## 4.关键代码
```
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
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
//	
//	//重映射PB6
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,	ENABLE);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//	//重映射PB7
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,	ENABLE);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//	开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,	ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	
	//	USART1 波特率115200，数据位8位，停止位1位，无校验
	USART_InitStruct.USART_BaudRate = 115200;	//	波特率115200
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	// 8位数据位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//	1位停止位
	USART_InitStruct.USART_Parity = USART_Parity_No;	//	无校验
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//	双向
	
	USART_Init(USART1,&USART_InitStruct);	//	初始化
	
	USART_Cmd(USART1, ENABLE);	// 闭合总开关
```

## 5.问题
对GPIO引脚和USART初始化还不熟练，对函数和函数封装还不够熟悉和理解

## 6.解决方法
对初始化操作进行更多练习，多看别人写的函数，多对函数进行仿写练习。