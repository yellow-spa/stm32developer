/**
  ******************************************************************************
  * @file    bluetooth-core.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-16-2017
  * @brief   This file contains all the algo of bluetooth
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "usart.h"
#include "stm32f10x_conf.h"
int bluetooth_work_init(void){
	int ret=0;
	ret = bluetooth_uart_probe();
	if(ret){
	   goto bluetooth_probe_error;
	}
	ret=bluetooth_uart_init();
	if(ret){
	   goto bluetooth_error;
	}
	return 0;
	bluetooth_error:
	bluetooth_uart_remove();
	bluetooth_probe_error:
	bluetooth_uart_removemem();
	return -1;
}
