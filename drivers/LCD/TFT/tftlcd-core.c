/*****************************************************
*
* alex_wang
*
*
******************************************************/
#include "gpio.h"

void TFTLCD_GPIO_config(void)
{
	//PB0 推挽输出 背光
	gpio_register_output(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_0,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
		//PORTD复用推挽输出  
	gpio_register_output(RCC_APB2Periph_GPIOD,GPIOD,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
		//PORTE复用推挽输出  
	gpio_register_output(RCC_APB2Periph_GPIOE,GPIOE,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
	//PORTG复用推挽输出	
	gpio_register_output(RCC_APB2Periph_GPIOG,GPIOG,GPIO_Pin_0|GPIO_Pin_12,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
}