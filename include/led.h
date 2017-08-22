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
int led_init(void);
void led_work(void);

#endif
