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
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
    USART_SendData(USART2,(uint8_t)ch);     
	return ch;
}



void uart2_send_data(u8* buf,int len)
 {
 	int i;
 	for(i=0;i<len;i++){
 		 USART_SendData(USART2,*(buf++));
 		 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
 	}
 }
 
 u8 cmd_get_in[10]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};
 void uart2_test(void){
 	uart2_send_data(cmd_get_in,10);
 }
 #if EN_USART2_RX 
 void USART2_IRQHandler(void)                 
 {
  u8 Res;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
  {
      Res =USART_ReceiveData(USART2);
        
  } 
 }
 #endif

