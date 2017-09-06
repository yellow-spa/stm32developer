#ifndef __STABILIZER_TYPES_H
#define __STABILIZER_TYPES_H
#include "stm32f10x_conf.h"
#include <stdbool.h>
#include "sensors_types.h"
#include "stm32system.h"
typedef struct  
{
	u32 timestamp;	/*时间戳*/

	float roll;
	float pitch;
	float yaw;
} attitude_t;

struct  vec3_s 
{
	u32 timestamp;	/*时间戳*/

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
	Axis3f acc;
	Axis3f gyro;
	Axis3f mag;
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
	enum dir_e flipDir;		/*翻滚方向*/
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



#endif