/**
  ******************************************************************************
  * @file    led-core.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the algo of LED 
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
	   goto led_probe_error;
	}
	ret=gpio_led_init();
	if(ret){
	   goto led_error;
	}
	printf("led probe OK\n");
	return 0;
	led_error:
	gpio_led_remove();
	led_probe_error:
	gpio_led_removemem();
	return -1;
}

void led_work(void)
{
	gpio_red_enable();
	gpio_green_disable();
	systick_ms(300);
	gpio_red_disable();
	gpio_green_enable();
	systick_ms(300);
}

