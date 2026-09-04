# 实验名称：EXTI中断按钮控制LED亮灭

## 1.学习目标
- 熟悉并掌握EXTI中断
## 2. 硬件准备
- 开发板：STM32F103C8T6
- LED 连接：PC13（低电平点亮）
- 按钮连接：PA1，PA0
- 调试工具：CSMIS-DAP
## 3.原理简述
EXTI通过中断进行边沿检测按钮是否被按下控制LED灯亮灭。
边沿检测（上升沿/下降沿）

## 4.关键代码
```
//配置引脚，需要外设使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
//初始化EXTI线
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line5 | EXTI_Line6;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_InitStruct);
//配置中断
	//通常设置在main函数开头位置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd =  ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	

```

## 5.问题
赶进度导致不会对代码进行复写，掌握不熟练

## 6.解决方法
后面多做一些对应的项目进行巩固。