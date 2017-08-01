#ifndef TIMER_H
#define TIMER_H
#include "stm32f10x_conf.h"

void Timer_enable(TIM_TypeDef* timernum,int en);
void Timer_Init(uint32_t timer,TIM_TypeDef* timernum,uint16_t prescaler,uint16_t timcountmode,uint16_t period ,uint16_t clkvision);
int Timer_NVI_setting(uint8_t irqchannel,uint8_t NVIPrePriority,uint8_t NVISubPriority);


#endif