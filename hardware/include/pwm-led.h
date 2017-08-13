#ifndef PWM_LED_H
#define PWM_LED_H
#include "stm32f10x_conf.h"
void TIM3_PWM_Init(u16 arr,u16 psc);
void pwm_led_work(void);
#endif