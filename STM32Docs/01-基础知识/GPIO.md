[GPIO_Function](##Function)
# GPIO
> 37个GPIO,分别为PA0-PA15、PB0-PB15、PC13-PC15、PD0-PD1

# GPIO的8种工作模式
## 输出
![输出](../../Picture/输出.png)
### 推挽
PN结交替导通，不可以同时导通，0低1高
![推挽](../../Picture/推挽.png)
### 开漏
P管默认断开，0低1悬空高阻抗
![开漏](../../Picture/开漏.png)
## 输入
![输入](../../Picture/输入.png)
> 上拉和下拉电阻起到一个稳定的作用

## Mode
![Mode](../../Picture/GPIO_Mode.png)
## 什么限制了IO的最大输出速度
上升时间和下降时间

## GPIO模块编程接口
![GPIO](../../Picture/GPIO.png)

## Function

### GPIO_Init
![Init](../../Picture/Init.png)
### GPIO_WriteBit
![WriteBit](../../Picture/WriteBit.png)

### GPIO_ReadInputDataBit
![ReadInputDataBit](../../Picture/ReadInputDataBit.png)
