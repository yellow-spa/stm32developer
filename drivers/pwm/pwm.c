/**
  ******************************************************************************
  * @file   pwm.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-16-2017
  * @brief   This file contains all the algo of pwm
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "pwm.h"
	void PWM_Init(TIM_TypeDef* timernum,int chanel,uint16_t timOcMode,uint16_t outputmodestate,uint16_t timOcPolar)
	{
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		TIM_OCStructInit(&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_OCMode = timOcMode; //选择定时器模式:TIM脉冲宽度调制模式2
 	  TIM_OCInitStructure.TIM_OutputState = outputmodestate; //比较输出使能
	  TIM_OCInitStructure.TIM_OCPolarity = timOcPolar; //输出极性:TIM输出比较极性高
		
		switch(chanel){
			case 1:
			{
				TIM_OC1Init(timernum, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	      TIM_OC1PreloadConfig(timernum, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
				break;
			}
			case 2:
			{
				TIM_OC2Init(timernum, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	      TIM_OC2PreloadConfig(timernum, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
				break;
			}
			case 3:
			{
				TIM_OC3Init(timernum, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	      TIM_OC3PreloadConfig(timernum, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
				break;
			}
			case 4:
			{
				TIM_OC4Init(timernum, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	      TIM_OC4PreloadConfig(timernum, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
				break;
			}
			default:
				break;
		}
 
	}