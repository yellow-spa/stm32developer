#include "stm32f10x_conf.h"
#include "ATKPackage.h"
#include "adc.h"
#include "battery.h"
#include "stm32system.h"

static float    batteryVoltage;
static float    batteryVoltageMin = 6.0;
static float    batteryVoltageMax = 0.0;
static u16 batterytempvalue[10]={0};
static int count=0;
void pmInit(void)
{
	adc_init();
	pmSetBatteryVoltage(3.7f); 
}

float pmGetBatteryVoltage(void)
{
	return batteryVoltage;
}
static void pmSetBatteryVoltage(float voltage)	/*设置电池电压最大最小值*/
{
	batteryVoltage = voltage;
	if (batteryVoltageMax < voltage)
	{
		batteryVoltageMax = voltage;
	}
	if (batteryVoltageMin > voltage)
	{
		batteryVoltageMin = voltage;
	}
}

PMStates pmUpdateState()	/* 更新电源状态 */
{
	PMStates state;
	if(batteryVoltage < PM_BAT_LOW_VOLTAGE )
	{
		 state=lowPower;
	}else{
	   state=battery;
	}
	return state;
}

void BaterryUpdateTask(const u32 tick)	/* 电源管理更新 */
{
	if (RATE_DO_EXECUTE(RATE_500_HZ, tick)) /** 500Hz 2ms update **/
	{
		batterytempvalue[count]=get_adc(ADC_Channel_4,ADC1);
		count++;
	}
	 if(count == 10)
	 {
		  int i;
		  u16 sum;
		  for(i=0;i < 10 ; i++)
		 {
			  sum += batterytempvalue[i];
			  batterytempvalue[i]=0;
		 }
		 sum=sum/10;
		 pmSetBatteryVoltage((float)sum/1000);
		 #if DEBUG_MODE
		 printf("battery = %f\n",batteryVoltage);
		 #endif
		 count = 0;
	 }
	 
}