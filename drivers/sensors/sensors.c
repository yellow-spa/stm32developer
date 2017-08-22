#include "sensors_types.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "usart.h"
#include <stdbool.h>
void sensors_init(void)
{
	MPU6050_I2C_Init();
	MPU6050_Initialize();
	//HMC5883L_Initialize();
}
void sensors_test(void)
{
	bool status;
	status = MPU6050_TestConnection();
	//printf("MPU%d",status);
//	status = HMC5883L_TestConnection();
//	printf("HMC%d",status);
}