/**
  ******************************************************************************
  * @file    usart-gpio.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the control gpio of the usart
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  #include "gpio.h"
  #include "usart.h"
  struct usart_data{
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
struct usart_data *usart_info;
  
int usart_setting( int RCCAPB,uint32_t USART_Periph, USART_TypeDef * usart,uint32_t bound,uint16_t wordlength,uint16_t stopbitnum,uint16_t parity,uint16_t flowcontrol,uint16_t mode)
{
	  USART_InitTypeDef USART_InitStructure;
      if(RCCAPB == 1){
		  RCC_APB1PeriphClockCmd(USART_Periph, ENABLE);
	  }else if(RCCAPB == 2){
		  RCC_APB2PeriphClockCmd(USART_Periph, ENABLE);
	  }else{
		  return -1;
	  }
	  USART_InitStructure.USART_BaudRate = bound;
	  USART_InitStructure.USART_WordLength = wordlength;
	  USART_InitStructure.USART_StopBits = stopbitnum;
	  USART_InitStructure.USART_Parity = parity;
	  USART_InitStructure.USART_HardwareFlowControl = flowcontrol;
	  USART_InitStructure.USART_Mode = mode;	
      USART_Init(usart, &USART_InitStructure);
      return 0;	  
}
extern int usart_setting( int RCCAPB,uint32_t USART_Periph, USART_TypeDef * usart,uint32_t bound,uint16_t wordlength,uint16_t stopbitnum,uint16_t parity,uint16_t flowcontrol,uint16_t mode);

int usart_NVI_setting(uint8_t irqchannel,uint8_t NVIPrePriority,uint8_t NVISubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=irqchannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=NVIPrePriority ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVISubPriority;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	return 0;
}
extern int usart_NVI_setting(uint8_t irqchannel,uint8_t NVIPrePriority,uint8_t NVISubPriority);


int usart_enable(USART_TypeDef * usart)
{
	  USART_ITConfig(usart, USART_IT_RXNE, ENABLE);//开启串口接受中断
      USART_Cmd(usart, ENABLE);                    //使能串口
}
int usart_disable(USART_TypeDef * usart)
{
	  USART_ITConfig(usart, USART_IT_RXNE, DISABLE);//close串口接受中断
      USART_Cmd(usart, DISABLE);                    //close串口
}
int usart_probe(void)
{
     struct usart_data *usart;
	 usart = (  struct usart_data * )malloc(sizeof(struct usart_data));
	if(!usart){
		printf("[%s]usart malloc error\n");
		return -1;
	}
	 usart->RCCAPB=USART_RCC_APB1;
	  usart->USART_Periph=RCC_APB1Periph_USART2;
	usart->GPIO_Periph=RCC_APB2Periph_GPIOA;
	usart->txGPIOpin=GPIO_Pin_2;
	usart->rxGPIOpin=GPIO_Pin_3;
	usart->irqchannel=USART2_IRQn;
	usart->NVIPrePriority=3;
	usart->NVISubPriority=3;
	usart->gpio_bus=GPIOA;
	usart->usartnum=USART2;
	usart->bound=115200;
	usart->mode=USART_Mode_Rx | USART_Mode_Tx;
	usart_info=usart;
	return 0;
	
}
extern int usart_probe(void);

void usart_removemem(void)
{
	free(usart_info);
}
extern void usart_removemem(void);

int usart_init(void)
{
	int ret=0;
	struct usart_data *usart=usart_info;
	 GPIO_InitTypeDef GPIO_InitStructure;
	/*
		//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART2_RX	  GPIOA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/
	
	//register gpio tx
	gpio_register_output(usart->GPIO_Periph,usart->gpio_bus,usart->txGPIOpin,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
	//register gpio rx
	gpio_register_input(usart->GPIO_Periph,usart->gpio_bus,usart->rxGPIOpin,GPIO_Mode_IN_FLOATING);
	//set usart
	ret=usart_setting(usart->RCCAPB,usart->USART_Periph,usart->usartnum,usart->bound,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_HardwareFlowControl_None,usart->mode);
	if(ret==-1)
	{
		return -1;
	}
	//initial interrupt
	usart_NVI_setting(usart->irqchannel,usart->NVIPrePriority,usart->NVISubPriority);
	//enable usart
	usart_enable(usart->usartnum);
	return 0;
}
extern int usart_init(void);

void usart_remove(void)
{
	struct usart_data *usart=usart_info;
	usart_disable(usart->usartnum);
	GPIO_DeInit(usart->gpio_bus);
}
extern void usart_remove(void);



