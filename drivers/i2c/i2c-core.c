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
