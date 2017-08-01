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
		TIM_OCInitStructure.TIM_OCMode = timOcMode; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	  TIM_OCInitStructure.TIM_OutputState = outputmodestate; //�Ƚ����ʹ��
	  TIM_OCInitStructure.TIM_OCPolarity = timOcPolar; //�������:TIM����Ƚϼ��Ը�
		
		switch(chanel){
			case 1:
			{
				TIM_OC1Init(timernum, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	      TIM_OC1PreloadConfig(timernum, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
				break;
			}
			case 2:
			{
				TIM_OC2Init(timernum, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	      TIM_OC2PreloadConfig(timernum, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
				break;
			}
			case 3:
			{
				TIM_OC3Init(timernum, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	      TIM_OC3PreloadConfig(timernum, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
				break;
			}
			case 4:
			{
				TIM_OC4Init(timernum, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	      TIM_OC4PreloadConfig(timernum, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
				break;
			}
			default:
				break;
		}
 
	}