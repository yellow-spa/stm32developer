/**
  ******************************************************************************
  * @file    timer-core.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the algo of timer
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  /*
  #include "stm32f10x_conf.h"
  void Timer_Init(uint32_t timer,)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	if( timer&0x1ff != 0  ){  //timer 2 3 4 5 6 7 12 13 14
	   RCC_APB1PeriphClockCmd(timer,ENABLE);
	}else {
		RCC_APB2PeriphClockCmd(timer,ENABLE)
	}
	TIM_TimeBaseStructure.TIM_Prescaler=
}

int Timer_NVI_setting(uint8_t irqchannel,uint8_t NVIPrePriority,uint8_t NVISubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=irqchannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=NVIPrePriority ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVISubPriority;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);
	return 0;
}
extern int Timer_NVI_setting(uint8_t irqchannel,uint8_t NVIPrePriority,uint8_t NVISubPriority);
*/