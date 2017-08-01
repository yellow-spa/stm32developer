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
	TIM_TimeBaseStructure.TIM_Prescaler=prescaler; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_CounterMode=timcountmode;
	TIM_TimeBaseStructure.TIM_Period=period; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_ClockDivision=clkvision;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInit(timernum,&TIM_TimeBaseStructure);////根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
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
