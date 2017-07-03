#include "stm32f10x.h"
#include "led.h"
int main(void)
{
	systick_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	usart_work_init();
	usart_enable(USART2);
    led_init();
while(1){
	  led_work();
}
  return 0;
}
