#include "led.h"
#include "stm32f10x_conf.h"

struct gpio_led_data{
    uint16_t gpio_red;
	uint32_t gpio_red_Periph;
	GPIO_TypeDef * gpio_red_bus;
	int gpio_red_rcc;
	uint16_t gpio_green;
	uint32_t gpio_green_Periph;
	GPIO_TypeDef * gpio_green_bus;
	int gpio_green_rcc;
	int gpio_speed;
	int gpio_mode;
};

struct gpio_led_data *gpio_led_info;

 int gpio_led_probe(void){
   struct gpio_led_data *gpio_led;
   gpio_led=(struct gpio_led_data*)malloc(sizeof(struct gpio_led_data));
   if(!gpio_led){
      goto gpio_led_error;
   }
   gpio_led->gpio_green_bus=GPIOE;
   gpio_led->gpio_red_bus=GPIOB;
   gpio_led->gpio_red_rcc=LED_RCC_APB2;
   gpio_led->gpio_green_rcc=LED_RCC_APB2;
   gpio_led->gpio_green_Periph=RCC_APB2Periph_GPIOE;
   gpio_led->gpio_red_Periph=RCC_APB2Periph_GPIOB;
   gpio_led->gpio_green=GPIO_Pin_5;
   gpio_led->gpio_red=GPIO_Pin_5;
   gpio_led->gpio_speed=GPIO_Speed_50MHz;
   gpio_led->gpio_mode=GPIO_Mode_Out_PP;
   gpio_led_info=gpio_led;
   return 0;
   gpio_led_error:
   free(gpio_led);
   return -1;
}
extern int gpio_led_probe(void);


 int gpio_led_init(void){
	 struct gpio_led_data *gpio_led=gpio_led_info;
	 GPIO_InitTypeDef GPIO_LED;
	 if(gpio_led->gpio_red_rcc==LED_RCC_APB1){
		 RCC_APB1PeriphClockCmd(gpio_led->gpio_red_Periph, ENABLE); 
	 }else if(gpio_led->gpio_red_rcc==LED_RCC_APB2){
	     RCC_APB2PeriphClockCmd(gpio_led->gpio_red_Periph, ENABLE); //enable  RCC
	 }else{
		 goto init_error;
	 }
	 GPIO_LED.GPIO_Pin = gpio_led->gpio_red;
	 GPIO_LED.GPIO_Mode = gpio_led->gpio_mode;
	 GPIO_LED.GPIO_Speed = gpio_led->gpio_speed;
	 GPIO_Init(gpio_led->gpio_red_bus,&GPIO_LED);
	 GPIO_SetBits(gpio_led->gpio_red_bus,gpio_led->gpio_red);
	 if(gpio_led->gpio_green_rcc==LED_RCC_APB1){
		 RCC_APB1PeriphClockCmd(gpio_led->gpio_green_Periph, ENABLE); 
	 }else if(gpio_led->gpio_green_rcc==LED_RCC_APB2){
	     RCC_APB2PeriphClockCmd(gpio_led->gpio_green_Periph, ENABLE); //enable  RCC
	 }else{
		 goto init_error;
	 }
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

 int gpio_led_red_pin_set(const int rcc,const uint32_t gpio_red_Periph, GPIO_TypeDef * bus,const uint16_t pin){
	struct gpio_led_data *gpio_led=gpio_led_info;
   gpio_led->gpio_red_bus=bus;
   gpio_led->gpio_red_rcc=rcc;
   gpio_led->gpio_red_Periph=gpio_red_Periph;
   gpio_led->gpio_red=pin;
   return 0;
}
extern int gpio_led_red_pin_set(const int rcc,const uint32_t gpio_red_Periph, GPIO_TypeDef * bus,const uint16_t pin);

 int gpio_led_green_pin_set(const int rcc,const uint32_t gpio_green_Periph, GPIO_TypeDef * bus,const uint16_t pin){
	struct gpio_led_data *gpio_led=gpio_led_info;
   gpio_led->gpio_green_bus=bus;
   gpio_led->gpio_green_rcc=rcc;
   gpio_led->gpio_green_Periph=gpio_green_Periph;
   gpio_led->gpio_green=pin;
   return 0;
}
extern int gpio_led_green_pin_set(const int rcc,const uint32_t gpio_green_Periph, GPIO_TypeDef * bus,const uint16_t pin);

 int gpio_led_speed_and_mode_set(int gpio_speed,int gpio_mode)
{
	struct gpio_led_data *gpio_led=gpio_led_info;
	gpio_led->gpio_speed=gpio_speed;
    gpio_led->gpio_mode=gpio_mode;
	return 0;
}
extern  int gpio_led_speed_and_mode_set(int gpio_speed,int gpio_mode);

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
