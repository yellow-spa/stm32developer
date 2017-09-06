#include "adc.h"
#include "gpio.h"
															   
void  adc_setting(const uint32_t Periph,ADC_TypeDef* adc_bus,uint32_t adc_mode,FunctionalState adcscanmode,FunctionalState adcConvMode,uint32_t adc_extern,uint32_t adc_dataaign,int adc_channel)
{
    ADC_InitTypeDef ADC_InitStructure; 
	  RCC_APB2PeriphClockCmd(Periph, ENABLE );	  //使能ADC通道时钟
	  ADC_DeInit(adc_bus);  //复位ADC1 
	 
    ADC_InitStructure.ADC_Mode = adc_mode;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = adcscanmode;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = adcConvMode;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = adc_extern;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = adc_dataaign;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = adc_channel;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(adc_bus, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(adc_bus, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(adc_bus);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(adc_bus));	//等待复位校准结束
	
	ADC_StartCalibration(adc_bus);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(adc_bus));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}

void adc_init(void)
{
	 gpio_register_input(RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_4,GPIO_Mode_AIN);
	 adc_setting(RCC_APB2Periph_ADC1,ADC1,ADC_Mode_Independent,DISABLE,DISABLE,ADC_ExternalTrigConv_None,ADC_DataAlign_Right,1);
}

//获得ADC值
//ch:通道值 0~3
u16 get_adc(u8 ch,ADC_TypeDef* adc_bus)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(adc_bus, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(adc_bus, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(adc_bus, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(adc_bus);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,ADC_TypeDef* adc_bus)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<10;t++)
	{
		temp_val+=get_adc(ch,adc_bus);
		systick_ms(5);
	}
	return temp_val/10;
} 	 
