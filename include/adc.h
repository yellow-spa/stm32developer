#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x_conf.h"
void adc_init(void);
u16 get_adc(u8 ch,ADC_TypeDef* adc_bus);
u16 Get_Adc_Average(u8 ch,ADC_TypeDef* adc_bus);
#endif