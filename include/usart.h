
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
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		
typedef enum
{ 
  USART_RCC_APB1 = 1,
  USART_RCC_APB2
}USARTRCC_TypeDef;
int usart_work_init(void);
#if EN_USART2_RX 
void usart2_receive_test(void);
#endif
#endif