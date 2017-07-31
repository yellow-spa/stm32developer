 # 中文版

## 关于本项目

在学习STM32过程中，想将一些资源全部整合在一起，每天更新一点自己所学的东西也是极好的，因而在此建立一个项目，将自己每天所完成的项目一点一点整合起来。

本项目是基于STM32F103ZE的芯片做成的，主要目的是为了将整个开发板玩烂。

## 相关小实验

### LED
跑马灯设计，在led-core.c中还可以加入不同的算法，所有的接口函数都在led-gpio.c中


### systick
systick设计能够让时间更加的精准。接口函数在systick.c中

### uart
uart gpio在txGPIO 为PA9 rxGPIO 为PA10 作为UART口 UART2优化变成环形数据结构

### bluetooth
bluetooth 为串口透传，现在所使用的为txGPIO 为PA2 rxGPIO 为PA3作为串口输出
