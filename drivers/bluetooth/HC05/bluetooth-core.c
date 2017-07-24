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
#include "bluetooth.h"
int bluetooth_work_init(void){
	int ret=0;
	ret = bluetooth_uart_probe();
	if(ret){
	   goto bluetooth_probe_error;
	}
	bluetooth_data_set(USART_RCC_APB1,RCC_APB1Periph_USART2,GPIO_Pin_2,GPIO_Pin_3,USART2_IRQn,3,2,USART2,38400);
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

char Cmdreturn[CmdreturnLength];// cmd return buffer

void BTsendcmd(const char *cmd)
{
	int i;
	for(i=0;i<CmdreturnLength;i++);
}

