#include "stm32f10x.h"
int main(void)
{
  led_init();
while(1){
	  led_work();
}
  return 0;
}
