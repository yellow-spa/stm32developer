#include "adc.h"
#include "gpio.h"
															   
void  adc_setting(const uint32_t Periph,ADC_TypeDef* adc_bus,uint32_t adc_mode,FunctionalState adcscanmode,FunctionalState adcConvMode,uint32_t adc_extern,uint32_t adc_dataaign,int adc_channel)
{
    ADC_InitTypeDef ADC_InitStructure; 
	  RCC_APB2PeriphClockCmd(Periph, ENABLE );	  //ʹ��ADCͨ��ʱ��
	  ADC_DeInit(adc_bus);  //��λADC1 
	 
    ADC_InitStructure.ADC_Mode = adc_mode;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = adcscanmode;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = adcConvMode;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = adc_extern;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = adc_dataaign;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = adc_channel;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(adc_bus, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(adc_bus, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(adc_bus);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(adc_bus));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(adc_bus);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(adc_bus));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}

void adc_init(void)
{
	 gpio_register_input(RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_4,GPIO_Mode_AIN);
	 adc_setting(RCC_APB2Periph_ADC1,ADC1,ADC_Mode_Independent,DISABLE,DISABLE,ADC_ExternalTrigConv_None,ADC_DataAlign_Right,1);
}

//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 get_adc(u8 ch,ADC_TypeDef* adc_bus)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(adc_bus, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(adc_bus, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(adc_bus, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(adc_bus);	//�������һ��ADC1�������ת�����
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
