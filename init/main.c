#include "stm32f10x.h"
#include "led.h"
#include "bluetooth.h"
#include "usart.h"
#include "pwm-led.h"
#include "tftlcd.h"
int main(void)
{
  	u8 x=0;
	u8 lcd_id[12];			//���LCD ID�ַ���
	systick_init();
	usart_work_init();
	usart_enable(USART1);
	bluetooth_work_init();
	usart_enable(USART2);
    led_init();
  TIM3_PWM_Init(899,0);
	LCD_Init();
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣	
while(1)
	{
	  switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			case 2:LCD_Clear(BLUE);break;
			case 3:LCD_Clear(RED);break;
			case 4:LCD_Clear(MAGENTA);break;
			case 5:LCD_Clear(GREEN);break;
			case 6:LCD_Clear(CYAN);break;

			case 7:LCD_Clear(YELLOW);break;
			case 8:LCD_Clear(BRRED);break;
			case 9:LCD_Clear(GRAY);break;
			case 10:LCD_Clear(LGRAY);break;
			case 11:LCD_Clear(BROWN);break;
		}
		POINT_COLOR=RED;	  
		LCD_ShowString(30,40,210,24,24,"WarShip STM32 ^_^"); 
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID
		LCD_ShowString(30,130,200,12,12,"2014/5/4");
		 x++;
		if(x==12)x=0;
		 systick_ms(1000);
	}
  return 0;
}
