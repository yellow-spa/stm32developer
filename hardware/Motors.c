#include "motors.h"
#include "timer.h"
#include "gpio.h"
#include "pwm.h"
#include "battery.h"

u32 motor_ratios[] = {0, 0, 0, 0};

static u16 ratioToCCRx(u16 val)
{
	return ((val) >> (16 - MOTORS_PWM_BITS) & ((1 << MOTORS_PWM_BITS) - 1));
}


void motorsInit(void)	/*电机初始化*/
{
	uint16_t PrescalerValue = 0;    //控制电机的频率
	PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);	//使能定时器2\3时钟
	//申请GPIO
	gpio_register_output(RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_0|GPIO_Pin_1,GPIO_Speed_2MHz,GPIO_Mode_AF_PP);
	gpio_register_output(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_0|GPIO_Pin_1,GPIO_Speed_2MHz,GPIO_Mode_AF_PP);
	//设置定时器
	TIM_DeInit(TIM2);
	Timer_Init(RCC_APB1Periph_TIM2,TIM2,PrescalerValue,TIM_CounterMode_Up,999,0);
	TIM_DeInit(TIM3);
	Timer_Init(RCC_APB1Periph_TIM3,TIM3,PrescalerValue,TIM_CounterMode_Up,999,0);
	//设置PWM
	PWM_Init(TIM2,2,TIM_OCMode_PWM1,TIM_OutputState_Enable,TIM_OCPolarity_High);
	PWM_Init(TIM2,3,TIM_OCMode_PWM1,TIM_OutputState_Enable,TIM_OCPolarity_High);
	PWM_Init(TIM3,3,TIM_OCMode_PWM1,TIM_OutputState_Enable,TIM_OCPolarity_High);
	PWM_Init(TIM3,4,TIM_OCMode_PWM1,TIM_OutputState_Enable,TIM_OCPolarity_High);
	 Timer_enable(TIM2,1);  //使能TIM3
	 Timer_enable(TIM3,1);  //使能TIM3
}

/*设置电机PWM占空比*/
void motorsSetRatio(u32 id, u16 ithrust)
{
	
		u16 ratio=ithrust;

	#if ENABLE_THRUST_BAT_COMPENSATED		
		float thrust = ((float)ithrust / 65536.0f) * 60;
		float volts = -0.0006239f * thrust * thrust + 0.088f * thrust;
		float supply_voltage = pmGetBatteryVoltage();
		float percentage = volts / supply_voltage;
		percentage = percentage > 1.0f ? 1.0f : percentage;
		ratio = percentage * UINT16_MAX;
		motor_ratios[id] = ratio;		
	#endif
		
		switch(id)
		{
			case 0:		/*MOTOR_M1*/
				TIM_SetCompare2(TIM2,ratioToCCRx(ratio));
				break;
			case 1:		/*MOTOR_M2*/
				TIM_SetCompare3(TIM2,ratioToCCRx(ratio));
				break;
			case 2:		/*MOTOR_M3*/
				TIM_SetCompare3(TIM3,ratioToCCRx(ratio));
				break;
			case 3:		/*MOTOR_M4*/	
				TIM_SetCompare4(TIM3,ratioToCCRx(ratio));
				break;
			default: break;
		}	

}