#ifndef __STABILIZER_TYPES_H
#define __STABILIZER_TYPES_H
#include "stm32f10x_conf.h"
#include <stdbool.h>
#include "sensors_types.h"
typedef struct  
{
	u32 timestamp;	/*ʱ���*/

	float roll;
	float pitch;
	float yaw;
} attitude_t;

struct  vec3_s 
{
	u32 timestamp;	/*ʱ���*/

	float x;
	float y;
	float z;
};

typedef struct vec3_s point_t;
typedef struct vec3_s velocity_t;
typedef struct vec3_s acc_t;

typedef struct
{
	float pressure;
	float temperature;
	float asl;
} baro_t;

typedef struct
{
	sensordata_f acc;
	sensordata_f gyro;
	sensordata_f mag;
	baro_t baro;
	point_t position;
} sensorData_t;

typedef struct
{
	attitude_t attitude;
	point_t position;
	velocity_t velocity;
	acc_t acc;
} state_t;

enum dir_e
{
	CENTER=0,
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
};

typedef struct
{
	s16 roll;
	s16 pitch;
	s16 yaw;
	float thrust;
	enum dir_e flipDir;		/*��������*/
} control_t;

typedef enum
{
	modeDisable = 0,
	modeAbs,
	modeVelocity
} mode_t;

typedef struct
{
	attitude_t attitude;
	velocity_t velocity;
	float thrust;	
	bool isAltHold;
} setpoint_t;


#define RATE_5_HZ		5
#define RATE_10_HZ		10
#define RATE_25_HZ		25
#define RATE_50_HZ		50
#define RATE_100_HZ		100
#define RATE_200_HZ 	200
#define RATE_250_HZ 	250
#define RATE_500_HZ 	500
#define RATE_1000_HZ 	1000

#define MAIN_LOOP_RATE 	RATE_1000_HZ
#define MAIN_LOOP_DT	(u32)(1000/MAIN_LOOP_RATE)	/*��λms*/

#define RATE_DO_EXECUTE(RATE_HZ, TICK) ((TICK % (MAIN_LOOP_RATE / RATE_HZ)) == 0)


#endif