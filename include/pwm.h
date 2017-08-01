#ifndef PWM_H
#define PWM_H
#include "stm32f10x_conf.h"
void PWM_Init(TIM_TypeDef* timernum,int chanel,uint16_t timOcMode,uint16_t outputmodestate,uint16_t timOcPolar);
#endif