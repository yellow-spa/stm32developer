#include "stm32f10x.h"
#include "led.h"
int main(void)
{
	systick_init();
	usart_work_init();
	usart_enable(USART2);
    led_init();
while(1){
	  led_work();
}
  return 0;
}
