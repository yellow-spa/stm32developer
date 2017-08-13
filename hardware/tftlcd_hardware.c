#include "tftlcd_hardware.h"
#include "tftlcd.h"
u8 lcd_id[12];	//存放LCD ID字符串
u8 x=0;
void tftlcd_print_id(void)
{
	  POINT_COLOR=RED;
	  sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。	
}

void tftlcd_work(void)
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
		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID
		LCD_ShowString(30,130,200,12,12,"2014/5/4");
		 x++;
		if(x==12)x=0;
		 systick_ms(1000);
}