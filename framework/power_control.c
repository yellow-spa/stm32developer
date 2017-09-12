#include "power_control.h"
#include "motors.h"

static motorPWM_t motorPWM;
static motorPWM_t motorPWMSet={0, 0, 0, 0};


void powerControlInit(void)
{
	motorsInit();
}

void getMotorPWM(motorPWM_t* get)
{
	*get = motorPWM;
}