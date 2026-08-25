# 实验名称：GPIO 控制 LED 闪烁

## 1. 学习目标
- 理解 GPIO 的输出模式
- 掌握 标准 库中 GPIO 的初始化与操作函数
- 熟悉 Keil的基本配置流程

## 2. 硬件准备
- 开发板：STM32F103C8T6
- LED 连接：PC13（低电平点亮）
- 调试工具：CSMIS-DAP

## 3. 原理简述
GPIO 是通用输入输出端口，每个引脚可配置为输入、输出、复用、通用等模式。
输出模式下可设置推挽/开漏，输入模式下可设置上下拉和悬空。本实验将 PC13 配置为推挽输出，通过周期性翻转电平实现 LED 闪烁。

## 4. 配置步骤
1. 打开 Keil，选择芯片 STM32F103C8T6,D，DeBug设置为CSMIS-DAP。
2. 配置 RCC：HSE 为外部晶振，主频 2MHz。
3. 配置 PC13 为 GPIO_Out_OD，初始电平高（LED 灭）。
4. 生成代码，工具链选择 MDK-ARM。

## 5. 关键代码解析
```
	//	开启GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//	初始化IO引脚，PC13通用输出开漏模式 2MHz
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
```