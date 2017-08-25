#include "gpio.h"
#include "i2c2.h"
#include "stm32f10x_conf.h"

void i2c2_gpio_init(void)
{
	gpio_register_output(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_10,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
	gpio_register_output(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_11,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
}

//产生i2c2起始信号
void i2c2_Start(void)
{
	I2C2_SDA_OUT();     //sda线输出
	I2C2_SDA=1;	  	  
	I2C2_SCL=1;
	systick_us(4);
 	I2C2_SDA=0;//START:when CLK is high,DATA change form high to low 
	systick_us(4);
	I2C2_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
void i2c2_Stop(void)
{
	I2C2_SDA_OUT();//sda线输出
	I2C2_SCL=0;
	I2C2_SDA=0;//STOP:when CLK is high DATA change form low to high
 	systick_us(4);
	I2C2_SCL=1; 
	I2C2_SDA=1;//发送I2C总线结束信号
	systick_us(4);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 i2c2_Wait_Ack(void)
{
	u8 ucErrTime=0;
	I2C2_SDA_IN();      //SDA设置为输入  
	I2C2_SDA=1;systick_us(1);	   
	I2C2_SCL=1;systick_us(1);	 
	while(READ_I2C2_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			i2c2_Stop();
			return 1;
		}
	}
	I2C2_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void i2c2_Ack(void)
{
	I2C2_SCL=0;
	I2C2_SDA_OUT();
	I2C2_SDA=0;
	systick_us(2);
	I2C2_SCL=1;
	systick_us(2);
	I2C2_SCL=0;
}
//不产生ACK应答		    
void i2c2_NAck(void)
{
	I2C2_SCL=0;
	I2C2_SDA_OUT();
	I2C2_SDA=1;
	systick_us(2);
	I2C2_SCL=1;
	systick_us(2);
	I2C2_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void i2c2_Send_One_Byte(u8 txd)
{                        
    u8 t;   
	I2C2_SDA_OUT(); 	    
    I2C2_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			I2C2_SDA=1;
		else
			I2C2_SDA=0;
		txd<<=1; 	  
		systick_us(2);   //对TEA5767这三个延时都是必须的
		I2C2_SCL=1;
		systick_us(2); 
		I2C2_SCL=0;	
		systick_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 i2c2_Read_One_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	I2C2_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        I2C2_SCL=0; 
        systick_us(2);
		I2C2_SCL=1;
        receive<<=1;
        if(READ_I2C2_SDA)receive++;   
		systick_us(1); 
    }					 
    if (!ack)
        i2c2_NAck();//发送nACK
    else
        i2c2_Ack(); //发送ACK   
    return receive;
}
