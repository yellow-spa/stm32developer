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
  #include "gpio.h"
 int gpio_register(const uint32_t GPIO_Periph,GPIO_TypeDef * bus,const uint16_t pin,const GPIOSpeed_TypeDef gpio_speed,const GPIOMode_TypeDef gpio_mode)
  {
	 GPIO_InitTypeDef GPIO_InitStrure;
	 RCC_APB2PeriphClockCmd(GPIO_Periph, ENABLE); //enable  RCC
	 GPIO_InitStrure.GPIO_Pin = pin;
	 GPIO_InitStrure.GPIO_Mode = gpio_mode;
	 GPIO_InitStrure.GPIO_Speed = gpio_speed;
	 GPIO_Init(bus,&GPIO_InitStrure);
	 return 0;
  }
int gpio_output(GPIO_TypeDef * bus,const uint16_t pin,int value)
{
	if(value)
	{
	  GPIO_SetBits(bus,pin);
	}else{
	  GPIO_ResetBits(bus,pin);
	}
	return 0;
}

int gpio_register_output(const uint32_t GPIO_Periph,GPIO_TypeDef * bus,const uint16_t pin,const GPIOSpeed_TypeDef gpio_speed,const GPIOMode_TypeDef gpio_mode)
{
	GPIO_InitTypeDef GPIO_InitStrure;
	if( (gpio_mode == GPIO_Mode_AIN) || (gpio_mode == GPIO_Mode_IN_FLOATING) || (gpio_mode == GPIO_Mode_IPD) || (gpio_mode == GPIO_Mode_IPU) )
	{
		printf("[%s]gpio_register_output error\n",__FUNCTION__);
		return -1;
	}
	 RCC_APB2PeriphClockCmd(GPIO_Periph, ENABLE); //enable  RCC
	 GPIO_InitStrure.GPIO_Pin = pin;
	 GPIO_InitStrure.GPIO_Mode = gpio_mode;
	 GPIO_InitStrure.GPIO_Speed = gpio_speed;
	 GPIO_Init(bus,&GPIO_InitStrure);
	return 0;
}
int gpio_register_input(const uint32_t GPIO_Periph,GPIO_TypeDef * bus,const uint16_t pin,const GPIOMode_TypeDef gpio_mode)
{
	GPIO_InitTypeDef GPIO_InitStrure;
	if( (gpio_mode == GPIO_Mode_Out_OD) || (gpio_mode == GPIO_Mode_Out_PP) || (gpio_mode == GPIO_Mode_AF_OD) || (gpio_mode == GPIO_Mode_AF_PP) )
	{
		printf("[%s]gpio_register_input error\n",__FUNCTION__);
		return -1;
	}
	 RCC_APB2PeriphClockCmd(GPIO_Periph, ENABLE); //enable  RCC
	 GPIO_InitStrure.GPIO_Pin = pin;
	 GPIO_InitStrure.GPIO_Mode = gpio_mode;
	 GPIO_Init(bus,&GPIO_InitStrure);
	return 0;
}