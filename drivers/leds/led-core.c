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
#include "led.h"

int led_init(void){
	int ret=0;
	ret = gpio_led_probe();
	if(ret){
	   goto led_error;
	}
	ret=gpio_led_init();
	if(ret){
	   goto led_error;
	}
	return 0;
	led_error:
	return -1;
}
extern int led_init(void);


void led_work(void)
{
	gpio_red_enable();
	gpio_green_disable();
	systick_ms(300);
	gpio_red_disable();
	gpio_green_enable();
	systick_ms(300);
}
extern void led_work(void);
