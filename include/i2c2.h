#ifndef __I2C2_H
#define __I2C2_H
#include "stm32system.h"

/*IO��������*/
#define I2C2_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRL|=(u32)8<<11;}
#define I2C2_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRL|=(u32)3<<11;}

//IO��������	 
#define I2C2_SCL    PBout(10) //SCL
#define I2C2_SDA    PBout(11) //SDA	 
#define READ_I2C2_SDA   PBin(11)  //����SDA 

//IIC���в�������
void i2c2_gpio_init(void);               //��ʼ��IIC��IO��				 
void i2c2_Start(void);				//����IIC��ʼ�ź�
void i2c2_Stop(void);	  			//����IICֹͣ�ź�
void i2c2_Send_One_Byte(u8 txd);			//IIC����һ���ֽ�
u8 i2c2_Read_One_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 i2c2_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void i2c2_Ack(void);					//IIC����ACK�ź�
void i2c2_NAck(void);				//IIC������ACK�ź�

#endif
