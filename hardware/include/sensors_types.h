#ifndef __SENSORS_TYPES_H
#define __SENSORS_TYPES_H

#include "stm32f10x_conf.h"

typedef struct 
{
	s16 x;
	s16 y;
	s16 z;	
} Axis3i16;

typedef struct 
{
	int x;
	int y;
	int z;
} Axis3i32;

typedef struct {
	int64_t x;
	int64_t y;
	int64_t z;
} Axis3i64;

typedef struct 
{
	float x;
	float y;
	float z;
} Axis3f;
 
#endif /* __SENSORS_TYPES_H */