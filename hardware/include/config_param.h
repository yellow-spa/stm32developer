#ifndef __CONFIG_PARAM_H
#define __CONFIG_PARAM_H
#include "stm32f10x_conf.h"
#include <stdbool.h>
												
typedef struct 
{
	float kp;
	float ki;
	float kd;
} pidInit_t;

typedef struct
{
	pidInit_t roll;
	pidInit_t pitch;	
	pidInit_t yaw;	
} pidParam_t;

typedef struct
{
	pidInit_t vx;
	pidInit_t vy;
	pidInit_t vz;	
} pidParamPos_t;

typedef struct	
{
	u8 version;				/*����汾��*/
	pidParam_t pidAngle;	/*�Ƕ�PID*/	
	pidParam_t pidRate;		/*���ٶ�PID*/	
	pidParamPos_t pidPos;	/*λ��PID*/
	u16 thrustBase;			/*���Ż���ֵ*/
	u8 cksum;				/*У��*/
} configParam_t;


extern configParam_t configParam;

void configParamInit(void);	/*�������ó�ʼ��*/
void configParamTask(void* param);	/*������������*/
bool configParamTest(void);

void configParamGiveSemaphore(void);
void resetConfigParamPID(void);

#endif /*__CONFIG_PARAM_H */

