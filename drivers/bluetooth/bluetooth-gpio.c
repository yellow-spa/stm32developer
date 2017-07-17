/**
  ******************************************************************************
  * @file    bluetooth-gpio.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-16-2017
  * @brief   This file contains all the algo of tooth
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "gpio.h"
#include "usart.h"
#include "bluetooth.h"
struct bluetooth_data{
	  int RCCAPB;
	  uint32_t USART_Periph;
	  uint32_t GPIO_Periph;
      uint16_t txGPIOpin;
	  uint16_t rxGPIOpin;
	  uint8_t irqchannel;
	  uint8_t NVIPrePriority;
	  uint8_t NVISubPriority;
	  GPIO_TypeDef * gpio_bus;
	  USART_TypeDef * usartnum;
	  u32 bound;
	  uint16_t mode;
  };
struct bluetooth_data *bluetooth_info;
int bluetooth_uart_probe(void)
{
     struct bluetooth_data *bluetooth;
	 bluetooth = (  struct bluetooth_data * )malloc(sizeof(struct bluetooth_data));
	if(!bluetooth){
		printf("[%s]bluetooth malloc error\n",__func__);
		return -1;
	}
	bluetooth->RCCAPB=USART_RCC_APB1;
	bluetooth->USART_Periph=RCC_APB1Periph_USART2;
	bluetooth->GPIO_Periph=RCC_APB2Periph_GPIOA;
	bluetooth->txGPIOpin=GPIO_Pin_2;
	bluetooth->rxGPIOpin=GPIO_Pin_3;
	bluetooth->irqchannel=USART2_IRQn;
	bluetooth->NVIPrePriority=3;
	bluetooth->NVISubPriority=2;
	bluetooth->gpio_bus=GPIOA;
	bluetooth->usartnum=USART2;
	bluetooth->bound=38400;
	bluetooth->mode=USART_Mode_Rx | USART_Mode_Tx;
	bluetooth_info=bluetooth;
	return 0;
	
}
extern int bluetooth_uart_probe(void);

void bluetooth_uart_removemem(void)
{
	free(bluetooth_info);
}
extern void bluetooth_uart_removemem(void);
int bluetooth_uart_init(void)
{
		int ret=0;
	struct bluetooth_data *bluetooth=bluetooth_info;
	//register gpio tx
	gpio_register_output(bluetooth->GPIO_Periph,bluetooth->gpio_bus,bluetooth->txGPIOpin,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
	//register gpio rx
	gpio_register_input(bluetooth->GPIO_Periph,bluetooth->gpio_bus,bluetooth->rxGPIOpin,GPIO_Mode_IN_FLOATING);
	//set usart
	ret=usart_setting(bluetooth->RCCAPB,bluetooth->USART_Periph,bluetooth->usartnum,bluetooth->bound,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_HardwareFlowControl_None,bluetooth->mode);
	if(ret==-1)
	{
		return -1;
	}
		//initial interrupt
	usart_NVI_setting(bluetooth->irqchannel,bluetooth->NVIPrePriority,bluetooth->NVISubPriority);
	//enable usart
	usart_enable(bluetooth->usartnum);
	return 0;
}

void bluetooth_uart_remove(void)
{
	struct bluetooth_data *bluetooth=bluetooth_info;
	usart_disable(bluetooth->usartnum);
	GPIO_DeInit(bluetooth->gpio_bus);
}
extern void usart_remove(void);