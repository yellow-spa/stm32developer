
/**
  ******************************************************************************
  * @file    led.h
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the functions prototypes for the LED 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef USART_H
#define USART_H

#include "stm32f10x_conf.h"
//UART receive define++++
#define EN_USART1_RX 			0
#define EN_USART2_RX 			1		
//UART receive define----
// USART Receiver buffer
#define RX_BUFFER_SIZE   128
#define TX_BUFFER_SIZE   128
extern unsigned char rx_buf[RX_BUFFER_SIZE];
extern unsigned char tx_buf[TX_BUFFER_SIZE];





#define USART_REC_LEN  			200  	//定义最大接收字节数 200

typedef enum
{ 
  USART_RCC_APB1 = 1,
  USART_RCC_APB2
}USARTRCC_TypeDef;

int usart_work_init(void);

#if EN_USART2_RX 

typedef struct 
{
	uint16_t volatile Wd_Indx;
  uint16_t volatile Rd_Indx;
  uint16_t Mask;
  uint8_t *pbuf;
}Uart2Buf;

extern Uart2Buf Uart2Txbuf;
extern Uart2Buf Uart2Rxbuf;


uint8_t Uart2Buf_RD(Uart2Buf *buf);
void Uart2Buf_WD(Uart2Buf *Ringbuf,uint8_t DataIn);
uint16_t Uart2Buf_Cnt(Uart2Buf *Ringbuf);
void UART2_Put_Char(unsigned char DataToSend);
void Uart2SendBuffer(uint8_t *dat, uint8_t len);
#endif

#endif
