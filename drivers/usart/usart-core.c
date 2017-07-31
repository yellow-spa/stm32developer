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
	printf("usart OK");
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
 
Uart2Buf Uart2Txbuf;
Uart2Buf Uart2Rxbuf;
 unsigned char rx_buf[RX_BUFFER_SIZE];
 unsigned char tx_buf[TX_BUFFER_SIZE];
//��ȡ���������е�һ���ֽ�
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

//�����������Ŀ����ֽڳ��ȣ���дָ��д��һȦ��׷�϶�ָ��
//��ô����д���ˣ���ʱӦ�����ӻ��������ȣ�����������Χ���ݴ���ʱ��
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

