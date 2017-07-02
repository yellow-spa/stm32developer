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
#ifndef LED_H
#define LED_H
#include "stm32f10x_conf.h"
typedef enum
{ 
  LED_RCC_APB1 = 1,
  LED_RCC_APB2
}LEDRCC_TypeDef;

#endif