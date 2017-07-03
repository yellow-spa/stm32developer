/**
  ******************************************************************************
  * @file    gpio-common.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the control gpio 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef GPIO_H
#define GPIO_H
#include "stm32f10x_conf.h"

int gpio_register(const uint32_t GPIO_Periph,GPIO_TypeDef * bus,const uint16_t pin,const GPIOSpeed_TypeDef gpio_speed,const GPIOMode_TypeDef gpio_mode);
int gpio_output(GPIO_TypeDef * bus,const uint16_t pin,int value);
int gpio_register_output(const uint32_t GPIO_Periph,GPIO_TypeDef * bus,const uint16_t pin,const GPIOSpeed_TypeDef gpio_speed,const GPIOMode_TypeDef gpio_mode);
int gpio_register_input(const uint32_t GPIO_Periph,GPIO_TypeDef * bus,const uint16_t pin,const GPIOMode_TypeDef gpio_mode);
#endif