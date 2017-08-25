#include "gpio.h"
#include "i2c2.h"
#include "stm32f10x_conf.h"

void i2c2_gpio_init(void)
{
	gpio_register_output(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_10,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
	gpio_register_output(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_11,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
}

//����i2c2��ʼ�ź�
void i2c2_Start(void)
{
	I2C2_SDA_OUT();     //sda�����
	I2C2_SDA=1;	  	  
	I2C2_SCL=1;
	systick_us(4);
 	I2C2_SDA=0;//START:when CLK is high,DATA change form high to low 
	systick_us(4);
	I2C2_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�
void i2c2_Stop(void)
{
	I2C2_SDA_OUT();//sda�����
	I2C2_SCL=0;
	I2C2_SDA=0;//STOP:when CLK is high DATA change form low to high
 	systick_us(4);
	I2C2_SCL=1; 
	I2C2_SDA=1;//����I2C���߽����ź�
	systick_us(4);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 i2c2_Wait_Ack(void)
{
	u8 ucErrTime=0;
	I2C2_SDA_IN();      //SDA����Ϊ����  
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
	I2C2_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void i2c2_Send_One_Byte(u8 txd)
{                        
    u8 t;   
	I2C2_SDA_OUT(); 	    
    I2C2_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			I2C2_SDA=1;
		else
			I2C2_SDA=0;
		txd<<=1; 	  
		systick_us(2);   //��TEA5767��������ʱ���Ǳ����
		I2C2_SCL=1;
		systick_us(2); 
		I2C2_SCL=0;	
		systick_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 i2c2_Read_One_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	I2C2_SDA_IN();//SDA����Ϊ����
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
        i2c2_NAck();//����nACK
    else
        i2c2_Ack(); //����ACK   
    return receive;
}
