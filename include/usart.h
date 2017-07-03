
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
typedef enum
{ 
  USART_RCC_APB1 = 1,
  USART_RCC_APB2
}USARTRCC_TypeDef;
int usart_work_init(void);
#endif