#ifndef __BATTERY_H
#define	__BATTERY_H
#include "stm32f10x_conf.h"
#define PM_BAT_LOW_VOLTAGE   			3.3f

typedef enum
{
	battery,
	charging,
	charged,
	lowPower,
	shutDown,
	
} PMStates;

void pmInit(void);
void BaterryUpdateTask(const u32 tick);
PMStates pmUpdateState();
float pmGetBatteryVoltage(void);
static void pmSetBatteryVoltage(float voltage);
#endif