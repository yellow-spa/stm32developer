#include "gpio.h"
#include "i2c2.h"
#include "stm32f10x_conf.h"
#include "hardwareconfig.h"
#include "mpu6050.h"
#if MPU6050APK_SUPPORT
/**
 * @brief  Initializes the I2C peripheral used to drive the MPU6050
 * @param  None
 * @return None
 */
void MPU6050_I2C_Init()
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable I2C and GPIO clocks */
    RCC_APB1PeriphClockCmd(MPU6050_I2C_RCC_Periph, ENABLE);
    RCC_APB2PeriphClockCmd(MPU6050_I2C_RCC_Port, ENABLE);

    /* Configure I2C pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin = MPU6050_I2C_SCL_Pin | MPU6050_I2C_SDA_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(MPU6050_I2C_Port, &GPIO_InitStructure);

    /* I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = MPU6050_DEFAULT_ADDRESS; // MPU6050 7-bit adress = 0x68, 8-bit adress = 0xD0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = MPU6050_I2C_Speed;

    /* Apply I2C configuration after enabling it */
    I2C_Init(MPU6050_I2C, &I2C_InitStructure);
    /* I2C Peripheral Enable */
    I2C_Cmd(MPU6050_I2C, ENABLE);
}


#endif

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
       I2C2_SDA=(txd&0x80)>>7;
	/*	if((txd&0x80)>>7)
			I2C2_SDA=1;
		else
			I2C2_SDA=0;*/
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

