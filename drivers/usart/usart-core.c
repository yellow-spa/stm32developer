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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
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

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;       
	return ch;
}

 #if EN_USART1_RX  //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���



 void USART1_IRQHandler(void)                 
 {
  u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
  {
      Res =USART_ReceiveData(USART1);
      if((USART_RX_STA&0x8000)==0)//����δ���
			 {
			     if(USART_RX_STA&0x4000)//���յ���0x0d
				   {
				       if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
			        	else USART_RX_STA|=0x8000;	//��������� 
				   }
			     else //��û�յ�0X0D
		     	{	
				      if(Res==0x0d)USART_RX_STA|=0x4000;
				      else
					    {
					       USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					       USART_RX_STA++;
					        if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					    }		 
			    }
			}   		  
  } 
 }
 
 #endif

 
//uart2 work+++++


#if EN_USART2_RX 
u8 UART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 UART2_RX_STA=0;
void USART2_IRQHandler(void)                 
{
 u8 Res;
 if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
 {
    Res =USART_ReceiveData(USART2);
     if((UART2_RX_STA&0x8000)==0)//����δ���
			 {
			     if(UART2_RX_STA&0x4000)//���յ���0x0d
				   {
				       if(Res!=0x0a)UART2_RX_STA=0;//���մ���,���¿�ʼ
			        	else UART2_RX_STA|=0x8000;	//��������� 
				   }
			     else //��û�յ�0X0D
		     	{	
				      if(Res==0x0d)UART2_RX_STA|=0x4000;
				      else
					    {
					      UART2_RX_BUF[UART2_RX_STA&0X3FFF]=Res ;
					       UART2_RX_STA++;
					        if(UART2_RX_STA>(USART_REC_LEN-1))UART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
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
	
	printf("���͵���Ϣ \n");

	uart2_send_data(UART2_RX_BUF,UART2_RX_STA&0X3FFF);
}
#endif
//uart2 work-----

