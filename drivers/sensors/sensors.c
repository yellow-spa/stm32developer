#include "sensors_types.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "usart.h"
#include "i2c2.h"
#include <stdbool.h>
void sensors_init(void)
{
	bool isMPU6050_Init;
	i2c2_gpio_init();
	isMPU6050_Init = MPU6050_Initialize();
	if(!isMPU6050_Init)
	{
		printf("sensorerror!\n");
	}
	//HMC5883L_Initialize();
}