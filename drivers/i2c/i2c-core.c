/**
  ******************************************************************************
  * @file    timer-core.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the algo of timer
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "i2c2.h"
#include "i2c-core.h"
#include "hardwareconfig.h"
#if MPU6050APK_SUPPORT
/**
 * @brief  Writes one byte to the  MPU6050.
 * @param  slaveAddr : slave address MPU6050_DEFAULT_ADDRESS
 * @param  pBuffer : pointer to the buffer  containing the data to be written to the MPU6050.
 * @param  writeAddr : address of the register in which the data will be written
 * @return None
 */
void MPU6050_I2C_ByteWrite(u8 slaveAddr, u8* pBuffer, u8 writeAddr)
{
    // ENTR_CRT_SECTION();

    /* Send START condition */
    I2C_GenerateSTART(MPU6050_I2C, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send MPU6050 address for write */
    I2C_Send7bitAddress(MPU6050_I2C, slaveAddr, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Send the MPU6050's internal address to write to */
    I2C_SendData(MPU6050_I2C, writeAddr);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send the byte to be written */
    I2C_SendData(MPU6050_I2C, *pBuffer);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STOP condition */
    I2C_GenerateSTOP(MPU6050_I2C, ENABLE);

    // EXT_CRT_SECTION();
}

/**
 * @brief  Reads a block of data from the MPU6050.
 * @param  slaveAddr  : slave address MPU6050_DEFAULT_ADDRESS
 * @param  pBuffer : pointer to the buffer that receives the data read from the MPU6050.
 * @param  readAddr : MPU6050's internal address to read from.
 * @param  NumByteToRead : number of bytes to read from the MPU6050 ( NumByteToRead >1  only for the Mgnetometer readinf).
 * @return None
 */
void MPU6050_I2C_BufferRead(u8 slaveAddr, u8* pBuffer, u8 readAddr, u16 NumByteToRead)
{
    // ENTR_CRT_SECTION();

    /* While the bus is busy */
    while (I2C_GetFlagStatus(MPU6050_I2C, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(MPU6050_I2C, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send MPU6050 address for write */
    I2C_Send7bitAddress(MPU6050_I2C, slaveAddr, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(MPU6050_I2C, ENABLE);

    /* Send the MPU6050's internal address to write to */
    I2C_SendData(MPU6050_I2C, readAddr);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(MPU6050_I2C, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send MPU6050 address for read */
    I2C_Send7bitAddress(MPU6050_I2C, slaveAddr, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    while (NumByteToRead)
    {
        if (NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(MPU6050_I2C, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(MPU6050_I2C, ENABLE);
        }

        /* Test on EV7 and clear it */
        if (I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the MPU6050 */
            *pBuffer = I2C_ReceiveData(MPU6050_I2C);

            /* Point to the next location where the byte read will be saved */
            pBuffer++;

            /* Decrement the read bytes counter */
            NumByteToRead--;
        }
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(MPU6050_I2C, ENABLE);
    // EXT_CRT_SECTION();
}
#endif

	//从指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 i2c2DevReadByte(u8 devaddr,u8 addr)
{				  
	u8 temp=0;		  	    																 
	i2c2_Start();  
	i2c2_Send_One_Byte(devaddr);//发送器件写命令 	   
	i2c2_Wait_Ack(); 
	i2c2_Send_One_Byte(addr);   //发送低地址
	i2c2_Wait_Ack();	

	i2c2_Start();  	 	   
	i2c2_Send_One_Byte(devaddr|1);//发送器件读命令			   
	i2c2_Wait_Ack();	 
	temp=i2c2_Read_One_Byte(0);			   
	i2c2_Stop();//产生一个停止条件	 
  printf("read %d\n",temp);	
	return temp;
}

//连续读多个字节
//addr:起始地址
//rbuf:读数据缓存
//len:数据长度
void i2c2DevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf)
{
	int i=0;
	i2c2_Start();  
	i2c2_Send_One_Byte(devaddr);  
	i2c2_Wait_Ack();	
	i2c2_Send_One_Byte(addr);//地址自增  
	i2c2_Wait_Ack();	

	i2c2_Start();  	
	i2c2_Send_One_Byte(devaddr|1);  	
	i2c2_Wait_Ack();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i] = i2c2_Read_One_Byte(0);//最后一个字节不应答
		}
		else
			rbuf[i] = i2c2_Read_One_Byte(1);
		printf("%d\n",rbuf[i]);
	}
	i2c2_Stop();	
}

//从指定地址写入一个数据
//WriteAddr :写入数据的目的地址    
//DataToWrite:要写入的数据
void i2c2DevWriteByte(u8 devaddr,u8 addr,u8 data)
{				   	  	    																 
	i2c2_Start();  
	i2c2_Send_One_Byte(devaddr); //发送器件写命令 	 
	i2c2_Wait_Ack();	   
	i2c2_Send_One_Byte(addr);   //发送低地址
	i2c2_Wait_Ack(); 	 										  		   
	i2c2_Send_One_Byte(data); //发送字节							   
	i2c2_Wait_Ack();  		    	   
	i2c2_Stop();		//产生一个停止条件 	 
}

//连续写多个字节
//addr:起始地址
//wbuf:写数据缓存
//len:数据的长度
void i2c2DevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf)
{
	int i=0;
	i2c2_Start();  
	i2c2_Send_One_Byte(devaddr);  	
	i2c2_Wait_Ack();	
	i2c2_Send_One_Byte(addr);  //地址自增
	i2c2_Wait_Ack();	
	for(i=0; i<len; i++)
	{
		i2c2_Send_One_Byte(wbuf[i]);  
		i2c2_Wait_Ack();		
	}
	i2c2_Stop();	
}
