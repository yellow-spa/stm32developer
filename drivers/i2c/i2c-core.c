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
	//��ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 i2c2DevReadByte(u8 devaddr,u8 addr)
{				  
	u8 temp=0;		  	    																 
	i2c2_Start();  
	i2c2_Send_One_Byte(devaddr);//��������д���� 	   
	i2c2_Wait_Ack(); 
	i2c2_Send_One_Byte(addr);   //���͵͵�ַ
	i2c2_Wait_Ack();	

	i2c2_Start();  	 	   
	i2c2_Send_One_Byte(devaddr|1);//��������������			   
	i2c2_Wait_Ack();	 
	temp=i2c2_Read_One_Byte(0);			   
	i2c2_Stop();//����һ��ֹͣ����	    
	return temp;
}

//����������ֽ�
//addr:��ʼ��ַ
//rbuf:�����ݻ���
//len:���ݳ���
void i2c2DevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf)
{
	int i=0;
	i2c2_Start();  
	i2c2_Send_One_Byte(devaddr);  
	i2c2_Wait_Ack();	
	i2c2_Send_One_Byte(addr);//��ַ����  
	i2c2_Wait_Ack();	

	i2c2_Start();  	
	i2c2_Send_One_Byte(devaddr|1);  	
	i2c2_Wait_Ack();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i] = i2c2_Read_One_Byte(0);//���һ���ֽڲ�Ӧ��
		}
		else
			rbuf[i] = i2c2_Read_One_Byte(1);
	}
	i2c2_Stop();	
}

//��ָ����ַд��һ������
//WriteAddr :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void i2c2DevWriteByte(u8 devaddr,u8 addr,u8 data)
{				   	  	    																 
	i2c2_Start();  
	i2c2_Send_One_Byte(devaddr); //��������д���� 	 
	i2c2_Wait_Ack();	   
	i2c2_Send_One_Byte(addr);   //���͵͵�ַ
	i2c2_Wait_Ack(); 	 										  		   
	i2c2_Send_One_Byte(data); //�����ֽ�							   
	i2c2_Wait_Ack();  		    	   
	i2c2_Stop();		//����һ��ֹͣ���� 	 
}

//����д����ֽ�
//addr:��ʼ��ַ
//wbuf:д���ݻ���
//len:���ݵĳ���
void i2c2DevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf)
{
	int i=0;
	i2c2_Start();  
	i2c2_Send_One_Byte(devaddr);  	
	i2c2_Wait_Ack();	
	i2c2_Send_One_Byte(addr);  //��ַ����
	i2c2_Wait_Ack();	
	for(i=0; i<len; i++)
	{
		i2c2_Send_One_Byte(wbuf[i]);  
		i2c2_Wait_Ack();		
	}
	i2c2_Stop();	
}
