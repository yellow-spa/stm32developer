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
	bluetooth_data_set(USART_RCC_APB1,RCC_APB1Periph_USART2,GPIO_Pin_2,GPIO_Pin_3,USART2_IRQn,0,0,USART2,38400);
	ret=bluetooth_uart_init();
	if(ret){
	   goto bluetooth_error;
	}
#ifdef EN_USART2_RX
	Uart2Txbuf.Wd_Indx = 0;
  Uart2Txbuf.Rd_Indx = 0;
  Uart2Txbuf.Mask = TX_BUFFER_SIZE - 1;
  Uart2Txbuf.pbuf = &tx_buf[0];
  
  Uart2Rxbuf.Wd_Indx = 0;
  Uart2Rxbuf.Rd_Indx = 0;
  Uart2Rxbuf.Mask = RX_BUFFER_SIZE - 1;
  Uart2Rxbuf.pbuf = &rx_buf[0];
#endif
	return 0;
	bluetooth_error:
	bluetooth_uart_remove();
	bluetooth_probe_error:
	bluetooth_uart_removemem();
	return -1;
}


const char ATcmdAsk[]    =		 {"AT"};
const char ATcmdAnswer[] =     {"OK"};

const char ATcmdNameAsk[] = 	 {"AT+NAME?"};
const char ATcmdNameAnswer[] =  {"OK+NAME:athena"};	
const char ATcmdNameSet[] = 	 {"AT+NAMEathena"};    

const char ATcmdCodeAsk[] = 	 {"AT+PIN?"};
const char ATcmdCodeAnswer[] = {"OK+PIN:1234"};	
const char ATcmdCodeSet[] =		 {"AT+PIN1234"};         

const char ATcmdRenewAsk[] = 	 {"AT+RENEW"};	//reset config
const char ATcmdRenewAnswer[] = {"OK+RENEW"};	

const char ATcmdBaudAsk[] =		 {"AT+BAUD?"};
const char ATcmdBaudAnswer[] = {"OK+BAUD:115200"};



char Cmdreturn[CmdreturnLength];// cmd return buffer

void UART2SendABTCmd(const char *cmd)
{
	int i;
	for(i=0;i<CmdreturnLength;i++)
	  Cmdreturn[i] = 0;
	systick_ms(100);
	for(i=0;i<strlen(cmd);i++)
	   UART2_Put_Char(*(cmd+i)); 
	systick_ms(100);
	i=0;
	while(Uart2Buf_Cnt(&Uart2Rxbuf) != 0)
		Cmdreturn[i++] = Uart2Buf_RD(&Uart2Rxbuf);
}
char CmdJudgement(const char *p)
{
  char i;
  for(i=0;i<strlen(p);i++) if(Cmdreturn[i] != *(p+i)) break;
  if(i != strlen(p)) return 0;
  return 1;
}