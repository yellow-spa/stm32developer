#ifndef __I2C2_H
#define __I2C2_H
#include "stm32system.h"
#include "hardwareconfig.h"


#if MPU6050APK_SUPPORT
#define MPU6050_I2C                  I2C2
#define MPU6050_I2C_RCC_Periph       RCC_APB1Periph_I2C2
#define MPU6050_I2C_Port             GPIOB
#define MPU6050_I2C_SCL_Pin          GPIO_Pin_10
#define MPU6050_I2C_SDA_Pin          GPIO_Pin_11
#define MPU6050_I2C_RCC_Port         RCC_APB2Periph_GPIOB
#define MPU6050_I2C_Speed            100000 // 100kHz standard mode
void MPU6050_I2C_Init();
#endif


//IO方向设置
#define I2C2_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)8<<11;}
#define I2C2_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)3<<11;}

//IO操作函数	 
#define I2C2_SCL    PBout(10) //SCL
#define I2C2_SDA    PBout(11) //SDA	 
#define READ_I2C2_SDA   PBin(11)  //输入SDA 

/*
//IO方向设置
#define I2C2_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}	//PB9输入模式
#define I2C2_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;} //PB9输出模式
//IO操作函数/	 
#define I2C2_SCL    PBout(6) 	//SCL
#define I2C2_SDA    PBout(7) 	//SDA	 
#define READ_I2C2_SDA 	PBin(7)  	//输入SDA 
*/
//IIC所有操作函数
void i2c2_gpio_init(void);               //初始化IIC的IO口				 
void i2c2_Start(void);				//发送IIC开始信号
void i2c2_Stop(void);	  			//发送IIC停止信号
void i2c2_Send_One_Byte(u8 txd);			//IIC发送一个字节
u8 i2c2_Read_One_Byte(unsigned char ack);//IIC读取一个字节
u8 i2c2_Wait_Ack(void); 				//IIC等待ACK信号
void i2c2_Ack(void);					//IIC发送ACK信号
void i2c2_NAck(void);				//IIC不发送ACK信号






#endif
