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
u8 UART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 UART2_RX_STA=0;
void USART2_IRQHandler(void)                 
{
 u8 Res;
 if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
 {
    Res =USART_ReceiveData(USART2);
     if((UART2_RX_STA&0x8000)==0)//接收未完成
			 {
			     if(UART2_RX_STA&0x4000)//接收到了0x0d
				   {
				       if(Res!=0x0a)UART2_RX_STA=0;//接收错误,重新开始
			        	else UART2_RX_STA|=0x8000;	//接收完成了 
				   }
			     else //还没收到0X0D
		     	{	
				      if(Res==0x0d)UART2_RX_STA|=0x4000;
				      else
					    {
					      UART2_RX_BUF[UART2_RX_STA&0X3FFF]=Res ;
					       UART2_RX_STA++;
					        if(UART2_RX_STA>(USART_REC_LEN-1))UART2_RX_STA=0;//接收数据错误,重新开始接收	  
					    }		 
			    }
			}
 } 
}

 void uart2_send_data(u8* buf,u16 len)
{
	u16 i;
	for(i=0;i<len;i++){
		 USART_SendData(USART2,*(buf++));
		 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}
	UART2_RX_STA=0;
}
 void uart1_send_data(u8* buf,u16 len)
{
	u16 i;
	for(i=0;i<len;i++){
		 USART_SendData(USART1,*(buf++));
		 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
	UART2_RX_STA=0;
}
void usart2_receive_test(void)
{
	
	printf("发送的信息 \n");

	uart2_send_data(UART2_RX_BUF,UART2_RX_STA&0X3FFF);
}
#endif
//uart2 work-----

