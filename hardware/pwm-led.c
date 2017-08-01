#include "timer.h"
#include "gpio.h"
#include "pwm.h"
#include "pwm-led.h"
//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
  gpio_register_output(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_5,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
	Timer_Init(RCC_APB1Periph_TIM3,TIM3,psc,TIM_CounterMode_Up,arr,0);
  PWM_Init(TIM3,2,TIM_OCMode_PWM2,TIM_OutputState_Enable,TIM_OCPolarity_High);	
  Timer_enable(TIM3,1);  //ʹ��TIM3
}
