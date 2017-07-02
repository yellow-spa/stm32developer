#include "led.h"

int led_init(void){
	int ret=0;
	ret = gpio_led_probe();
	if(ret){
	   goto led_error;
	}
	ret=gpio_led_init();
	if(ret){
	   goto led_error;
	}
	return 0;
	led_error:
	return -1;
}
extern int led_init(void);

 static void Delay(u32 count)
 {
   u32 i=0;
   for(;i<count;i++);
 }
 
void led_work(void)
{
	gpio_red_enable();
	gpio_green_disable();
	Delay(10000000);
	gpio_red_disable();
	gpio_green_enable();
	Delay(10000000);
}
extern void led_work(void);
