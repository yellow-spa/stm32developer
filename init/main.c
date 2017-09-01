#include "stm32f10x.h"
#include "led.h"
#include "bluetooth.h"
#include "usart.h"
#include "pwm-led.h"
#include "tftlcd.h"
#include "tftlcd_hardware.h"
#include "ATKPackage.h"
#include "sensors.h"
#include "i2c2.h"


static sensorData_t sensorData;	/*传感器数据*/

int main(void)
{	
	systick_init();
	usart_work_init();
	usart_enable(USART1);
	bluetooth_work_init();
	usart_enable(USART2);
	sensors_init();
// led_init();
// TIM3_PWM_Init(899,0);
//	LCD_Init();
//	tftlcd_print_id();
while(1)
	{
			//systick_ms(500);	
		  sensorsAcquire(&sensorData, 500);
		  ATKPackage_SendPeriod();
		//	printf("test");
	//	  sensors_test();
	}
  return 0;
}
