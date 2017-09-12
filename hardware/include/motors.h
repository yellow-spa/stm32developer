#ifndef __MOTORS_H
#define __MOTORS_H
#include "stm32f10x_conf.h"
#define ENABLE_THRUST_BAT_COMPENSATED 1


//#define TIM_CLOCK_HZ 				96000000
#define MOTORS_PWM_BITS           	8
//#define MOTORS_PWM_PERIOD         	((1<<MOTORS_PWM_BITS) - 1)
//#define MOTORS_PWM_PRESCALE       	0

#define NBR_OF_MOTORS 	4
#define MOTOR_M1  		0
#define MOTOR_M2  		1
#define MOTOR_M3  		2
#define MOTOR_M4  		3


void motorsInit(void);
void motorsSetRatio(u32 id, u16 ithrust);
#endif /* __MOTORS_H */