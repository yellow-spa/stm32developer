#include "stm32f10x.h"
#include "led.h"
int main(void)
{
	systick_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	usart_work_init();
	usart_enable(USART2);
    led_init();
while(1){
	  led_work();
}
  return 0;
}
