/**
  ******************************************************************************
  * @file    led-gpio.c
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the control gpio of the LED 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "stm32f10x_conf.h"

struct gpio_led_data{
    uint16_t gpio_red;
	uint32_t gpio_red_Periph;
	GPIO_TypeDef * gpio_red_bus;
	uint16_t gpio_green;
	uint32_t gpio_green_Periph;
	GPIO_TypeDef * gpio_green_bus;
	GPIOSpeed_TypeDef gpio_speed;
	GPIOMode_TypeDef gpio_mode;
};

struct gpio_led_data *gpio_led_info;

int gpio_led_probe(void){
   struct gpio_led_data *gpio_led;
   gpio_led=(struct gpio_led_data*)malloc(sizeof(struct gpio_led_data));
   if(!gpio_led){
	   printf("[%s]leds gpio malloc error");
      goto gpio_led_error;
   }
   gpio_led->gpio_green_bus=GPIOE;
   gpio_led->gpio_red_bus=GPIOB;
   gpio_led->gpio_green_Periph=RCC_APB2Periph_GPIOE;
   gpio_led->gpio_red_Periph=RCC_APB2Periph_GPIOB;
   gpio_led->gpio_green=GPIO_Pin_5;
   gpio_led->gpio_red=GPIO_Pin_5;
   gpio_led->gpio_speed=GPIO_Speed_50MHz;
   gpio_led->gpio_mode=GPIO_Mode_Out_PP;
   gpio_led_info=gpio_led;
   return 0;
   gpio_led_error:
   return -1;
}
extern int gpio_led_probe(void);

void gpio_led_removemem(void){
   free(gpio_led_info);
}
extern void gpio_led_removemem(void);

 int gpio_led_init(void){
	 struct gpio_led_data *gpio_led=gpio_led_info;
	 GPIO_InitTypeDef GPIO_LED;
	 RCC_APB2PeriphClockCmd(gpio_led->gpio_red_Periph, ENABLE); //enable  RCC
	 GPIO_LED.GPIO_Pin = gpio_led->gpio_red;
	 GPIO_LED.GPIO_Mode = gpio_led->gpio_mode;
	 GPIO_LED.GPIO_Speed = gpio_led->gpio_speed;
	 GPIO_Init(gpio_led->gpio_red_bus,&GPIO_LED);
	 GPIO_SetBits(gpio_led->gpio_red_bus,gpio_led->gpio_red);
	 RCC_APB2PeriphClockCmd(gpio_led->gpio_green_Periph, ENABLE); //enable  RCC
	 GPIO_LED.GPIO_Pin = gpio_led->gpio_green;
	 GPIO_LED.GPIO_Mode = gpio_led->gpio_mode;
	 GPIO_LED.GPIO_Speed = gpio_led->gpio_speed;
	 GPIO_Init(gpio_led->gpio_green_bus,&GPIO_LED);
	 GPIO_SetBits(gpio_led->gpio_green_bus,gpio_led->gpio_red);
	 return 0;
	 init_error:
	 return -1;
}
extern int gpio_led_init(void);

void gpio_led_remove(void){
	struct gpio_led_data *gpio_led=gpio_led_info;
	GPIO_DeInit(gpio_led->gpio_green_bus);
	GPIO_DeInit(gpio_led->gpio_red_bus);
	
}

extern void gpio_led_remove(void);

 int gpio_led_red_pin_set(const uint32_t gpio_red_Periph, GPIO_TypeDef * bus,const uint16_t pin){
	struct gpio_led_data *gpio_led=gpio_led_info;
   gpio_led->gpio_red_bus=bus;
   gpio_led->gpio_red_Periph=gpio_red_Periph;
   gpio_led->gpio_red=pin;
   return 0;
}
extern int gpio_led_red_pin_set(const uint32_t gpio_red_Periph, GPIO_TypeDef * bus,const uint16_t pin);

 int gpio_led_green_pin_set(const uint32_t gpio_green_Periph, GPIO_TypeDef * bus,const uint16_t pin){
	struct gpio_led_data *gpio_led=gpio_led_info;
   gpio_led->gpio_green_bus=bus;
   gpio_led->gpio_green_Periph=gpio_green_Periph;
   gpio_led->gpio_green=pin;
   return 0;
}
extern int gpio_led_green_pin_set(const uint32_t gpio_green_Periph, GPIO_TypeDef * bus,const uint16_t pin);

 int gpio_led_speed_and_mode_set(const GPIOSpeed_TypeDef gpio_speed,const GPIOMode_TypeDef gpio_mode)
{
	struct gpio_led_data *gpio_led=gpio_led_info;
	gpio_led->gpio_speed=gpio_speed;
    gpio_led->gpio_mode=gpio_mode;
	return 0;
}
extern  int gpio_led_speed_and_mode_set(const GPIOSpeed_TypeDef gpio_speed,const GPIOMode_TypeDef gpio_mode);

 void gpio_red_enable(void)
{
	struct gpio_led_data *gpio_led=gpio_led_info;
	GPIO_SetBits(gpio_led->gpio_red_bus,gpio_led->gpio_red);
}
extern void gpio_red_enable(void);

 void gpio_red_disable(void)
{
	struct gpio_led_data *gpio_led=gpio_led_info;
	GPIO_ResetBits(gpio_led->gpio_red_bus,gpio_led->gpio_red);
}
extern void gpio_red_disable(void);

 void gpio_green_enable(void)
{
	struct gpio_led_data *gpio_led=gpio_led_info;
	GPIO_SetBits(gpio_led->gpio_green_bus,gpio_led->gpio_green);
}
extern void gpio_green_enable(void);

 void gpio_green_disable(void)
{
	struct gpio_led_data *gpio_led=gpio_led_info;
	GPIO_ResetBits(gpio_led->gpio_green_bus,gpio_led->gpio_green);
}
extern void gpio_green_disable(void);
