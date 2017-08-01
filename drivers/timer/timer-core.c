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
#include "timer.h"
void Timer_Init(uint32_t timer,TIM_TypeDef* timernum,uint16_t prescaler,uint16_t timcountmode,uint16_t period ,uint16_t clkvision)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	if( timer&0x1ff != 0  ){  //timer 2 3 4 5 6 7 12 13 14
	   RCC_APB1PeriphClockCmd(timer,ENABLE);
	}else {
		 RCC_APB2PeriphClockCmd(timer,ENABLE);
	}
	TIM_TimeBaseStructure.TIM_Prescaler=prescaler; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_CounterMode=timcountmode;
	TIM_TimeBaseStructure.TIM_Period=period; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=clkvision;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInit(timernum,&TIM_TimeBaseStructure);////����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
}

void Timer_enable(TIM_TypeDef* timernum,int en)
{
	if(en)
	{
	  TIM_Cmd(timernum,ENABLE);
	}
	else
	{
		TIM_Cmd(timernum,DISABLE);
	}
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
