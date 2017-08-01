#include "timer.h"
#include "gpio.h"
#include "pwm.h"
#include "pwm-led.h"
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
  gpio_register_output(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_5,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
	Timer_Init(RCC_APB1Periph_TIM3,TIM3,psc,TIM_CounterMode_Up,arr,0);
  PWM_Init(TIM3,2,TIM_OCMode_PWM2,TIM_OutputState_Enable,TIM_OCPolarity_High);	
  Timer_enable(TIM3,1);  //使能TIM3
}
