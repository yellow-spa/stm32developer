#include "stm32f10x.h"
#include "led.h"
#include "bluetooth.h"
#include "usart.h"
#include "pwm-led.h"
#include "tftlcd.h"
#include "tftlcd_hardware.h"
#include "ATKPackage.h"
int main(void)
{	
	systick_init();
	usart_work_init();
	usart_enable(USART1);
	bluetooth_work_init();
	usart_enable(USART2);
  led_init();
  TIM3_PWM_Init(899,0);
	LCD_Init();
	tftlcd_print_id();
while(1)
	{
	    tftlcd_work();
			systick_ms(5);	 
		  ANO_DT_Data_Exchange();
	}
  return 0;
}
