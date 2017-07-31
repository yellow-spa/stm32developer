/**
  ******************************************************************************
  * @file    usart-core.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the algo of usart
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "usart.h"
#include "stm32f10x_conf.h"
#include "stdio.h"

int usart_work_init(void){
	int ret=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	ret = usart_probe();
	if(ret){
	   goto usart_probe_error;
	}
	ret=usart_init();
	if(ret){
	   goto usart_error;
	}
	printf("usart OK");
	return 0;
	usart_error:
	usart_remove();
	usart_probe_error:
	usart_removemem();
	return -1;
}

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;       
	return ch;
}

 #if EN_USART1_RX  //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记



 void USART1_IRQHandler(void)                 
 {
  u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
  {
      Res =USART_ReceiveData(USART1);
      if((USART_RX_STA&0x8000)==0)//接收未完成
			 {
			     if(USART_RX_STA&0x4000)//接收到了0x0d
				   {
				       if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
			        	else USART_RX_STA|=0x8000;	//接收完成了 
				   }
			     else //还没收到0X0D
		     	{	
				      if(Res==0x0d)USART_RX_STA|=0x4000;
				      else
					    {
					       USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					       USART_RX_STA++;
					        if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					    }		 
			    }
			}   		  
  } 
 }
 
 #endif

 
//uart2 work+++++


#if EN_USART2_RX 
 
Uart2Buf Uart2Txbuf;
Uart2Buf Uart2Rxbuf;
 unsigned char rx_buf[RX_BUFFER_SIZE];
 unsigned char tx_buf[TX_BUFFER_SIZE];
//读取环形数据中的一个字节
uint8_t Uart2Buf_RD(Uart2Buf *buf)
{
  uint8_t temp;
  temp = buf->pbuf[buf->Rd_Indx & buf->Mask];
  buf->Rd_Indx++;
  return temp;
}

void Uart2Buf_WD(Uart2Buf *Ringbuf,uint8_t DataIn)
{
  
  Ringbuf->pbuf[Ringbuf->Wd_Indx & Ringbuf->Mask] = DataIn;
  Ringbuf->Wd_Indx++;

}

//环形数据区的可用字节长度，当写指针写完一圈，追上读指针
//那么数据写满了，此时应该增加缓冲区长度，或者缩短外围数据处理时间
uint16_t Uart2Buf_Cnt(Uart2Buf *Ringbuf)
{
  return (Ringbuf->Wd_Indx - Ringbuf->Rd_Indx) & Ringbuf->Mask;
}


volatile uint8_t Udatatmp;

void USART2_IRQHandler(void)                 
{
 if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET) 
 {
	// printf("send\n");
	 USART_SendData(USART2, Uart2Buf_RD(&Uart2Txbuf)); 
	 if(Uart2Buf_Cnt(&Uart2Txbuf) == 0)
		  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
 }
 else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
 {
	   USART_ClearITPendingBit(USART2, USART_IT_RXNE);//reset flag
	   Udatatmp = (uint8_t)USART_ReceiveData(USART2); 
	    Uart2Buf_WD(&Uart2Rxbuf,Udatatmp);
 }
}
//uart2 function
void UART2_Put_Char(unsigned char DataToSend)
{
  
  Uart2Buf_WD(&Uart2Txbuf,DataToSend);
  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}

#endif
//uart2 work-----

