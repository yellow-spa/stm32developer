#include "stm32f10x.h"
#include "led.h"
#include "bluetooth.h"
#include "usart.h"
int main(void)
{

	systick_init();
	usart_work_init();
	usart_enable(USART1);
	bluetooth_work_init();
	usart_enable(USART2);
    led_init();
while(1){
	  led_work();
	systick_ms(300);
}
  return 0;
}
