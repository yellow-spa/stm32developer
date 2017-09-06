#include "sensors_types.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "usart.h"
#include "i2c2.h"
#include <stdbool.h>
#include "sensors.h"
#include "hardwareconfig.h"
#include "HMC5883L.h"

#define SENSORS_BIAS_SAMPLES		512		/*计算方差的采样样本个数*/
#define GYRO_VARIANCE_BASE			5000	/* 陀螺仪零偏方差阈值 */
#define GYRO_MIN_BIAS_TIMEOUT_MS   (1000)
static float sensorsAccLpfAttFactor;

static bool isMPUPresent=false;
static bool isMAGPresent=false;


typedef struct
{
	Axis3f     bias;
	bool       isBiasValueFound;
	bool       isBufferFilled;
	Axis3i16*  bufHead;
	Axis3i16   buffer[SENSORS_BIAS_SAMPLES];
}BiasObj;

BiasObj	gyroBias;

static Axis3i16	gyroRaw;
static Axis3i16	accRaw;
static Axis3i16 magRaw;

static Axis3i16	accLPF;

static void sensorsBiasInit(BiasObj* bias)
{
	bias->isBufferFilled = false;
	bias->bufHead = bias->buffer;
}


void sensors_init(void)
{
	bool Status;
	#if MPU6050APK_SUPPORT
	MPU6050_I2C_Init();
	#else
	i2c2_gpio_init();
	#endif
	Status = MPU6050_Initialize();
	if(Status)
	{
		isMPUPresent = true;
		printf("mpu sensor work!\n");
	}
	
	Status = HMC5883L_Initialize();
	if(Status)
	{
		isMAGPresent = true;
		printf("akm sensor work!\n");
	}
	
	sensorsBiasInit(&gyroBias);
	sensorsAccLpfAttFactor = SENSORS_ACC_IIR_FACTOR;
	
}

void getSensorRawData(Axis3i16* acc, Axis3i16* gyro, Axis3i16* mag)
{
	*acc = accRaw;
	*gyro = gyroRaw;
	*mag = magRaw;
}

static void sensorsAddBiasValue(BiasObj* bias, Axis3i16* dval)
{
	bias->bufHead->x = dval->x;
	bias->bufHead->y = dval->y;
	bias->bufHead->z = dval->z;
	bias->bufHead++;

	if (bias->bufHead >= &bias->buffer[SENSORS_BIAS_SAMPLES])
	{
		bias->bufHead = bias->buffer;
		bias->isBufferFilled = true;
	}
}
/* 加速计IIR低通滤波 */
static void sensorsAccIIRLPFilter(Axis3i16 *in,Axis3i16 *out)
{
	out->x = out->x + sensorsAccLpfAttFactor*(in->x - out->x); 
	out->y = out->y + sensorsAccLpfAttFactor*(in->y - out->y); 
	out->z = out->z + sensorsAccLpfAttFactor*(in->z - out->z); 
}
/*计算方差和平均值*/
static void sensorsCalculateVarianceAndMean(BiasObj* bias, Axis3f* varOut, Axis3f* meanOut)
{
	u32 i;
	int64_t sum[3] = {0};
	int64_t sumsq[3] = {0};

	for (i = 0; i < SENSORS_BIAS_SAMPLES; i++)
	{
		sum[0] += bias->buffer[i].x;
		sum[1] += bias->buffer[i].y;
		sum[2] += bias->buffer[i].z;
		sumsq[0] += bias->buffer[i].x * bias->buffer[i].x;
		sumsq[1] += bias->buffer[i].y * bias->buffer[i].y;
		sumsq[2] += bias->buffer[i].z * bias->buffer[i].z;
	}

	varOut->x = (sumsq[0] - ((int64_t)sum[0] * sum[0]) / SENSORS_BIAS_SAMPLES);
	varOut->y = (sumsq[1] - ((int64_t)sum[1] * sum[1]) / SENSORS_BIAS_SAMPLES);
	varOut->z = (sumsq[2] - ((int64_t)sum[2] * sum[2]) / SENSORS_BIAS_SAMPLES);

	meanOut->x = (float)sum[0] / SENSORS_BIAS_SAMPLES;
	meanOut->y = (float)sum[1] / SENSORS_BIAS_SAMPLES;
	meanOut->z = (float)sum[2] / SENSORS_BIAS_SAMPLES;
}

static bool sensorsFindBiasValue(BiasObj* bias)
{
	bool foundbias = false;

	if (bias->isBufferFilled)
	{
		Axis3f variance;
		Axis3f mean;

		sensorsCalculateVarianceAndMean(bias, &variance, &mean);

		if (variance.x < GYRO_VARIANCE_BASE && variance.y < GYRO_VARIANCE_BASE && variance.z < GYRO_VARIANCE_BASE)
		{
			bias->bias.x = mean.x;
			bias->bias.y = mean.y;
			bias->bias.z = mean.z;
			foundbias = true;
			bias->isBiasValueFound= true;
		}else
			bias->isBufferFilled=false;
	}
	return foundbias;
}

//read acc gyro  raw data
void sensor6Read(Axis3f* gyroOut, Axis3f* accOut)
{
	MPU6050_GetAccelRawData(&accRaw.x, &accRaw.y, &accRaw.z);
	MPU6050_GetGyroRawData(&gyroRaw.x, &gyroRaw.y, &gyroRaw.z);

	if (!gyroBias.isBiasValueFound)
	{
		sensorsAddBiasValue(&gyroBias, &gyroRaw);
		sensorsFindBiasValue(&gyroBias);
	}

	sensorsAccIIRLPFilter(&accRaw,&accLPF);

	gyroOut->x =-(gyroRaw.x - gyroBias.bias.x) * MPU6500_DEG_PER_LSB_2000;
	gyroOut->y = (gyroRaw.y - gyroBias.bias.y) * MPU6500_DEG_PER_LSB_2000;
	gyroOut->z = (gyroRaw.z - gyroBias.bias.z) * MPU6500_DEG_PER_LSB_2000;

	accOut->x =-(accLPF.x) * MPU6500_G_PER_LSB_8;
	accOut->y = (accLPF.y) * MPU6500_G_PER_LSB_8;
	accOut->z = (accLPF.z) * MPU6500_G_PER_LSB_8;	
	
}

static bool sensorsIsCalibrated(void)
{
	bool status;

	status = gyroBias.isBiasValueFound;
	
	return status;
}

bool sensorsAreCalibrated()	/*传感器数据校准*/
{
	Axis3f dummyData;
	sensor6Read(&dummyData, &dummyData);
	return sensorsIsCalibrated();
}
void sensor9Read(Axis3f* gyroOut, Axis3f* accOut, Axis3f* magOut)
{
	  if(isMPUPresent)
		{
	    sensor6Read(gyroOut, accOut);
		}else{
			 gyroOut->x = 0.0;
       gyroOut->y = 0.0;
       gyroOut->z = 0.0;
			 accOut->x = 0.0;
       accOut->y = 0.0;
       accOut->z = 0.0;
		}
	if(isMAGPresent)
	{
		s16 buf[3];
		HMC5883L_GetHeading(buf);
  // 	HMC5883L_GetLockStatus();
		magOut->x = -(float)buf[0] / MAG_GAUSS_PER_LSB;
		magOut->y = (float)buf[1] / MAG_GAUSS_PER_LSB;
		magOut->z = (float)buf[2] / MAG_GAUSS_PER_LSB;
		magRaw.x = buf[0];
		magRaw.y = buf[1];
		magRaw.z = buf[2];
		#if DEBUG_MODE
		printf("%f,%f,%f",magOut->x,magOut->y,magOut->z);
			#endif
	}else{
	magOut->x = 0.0;
  magOut->y = 0.0;
	magOut->z = 0.0;
  }
	
}

void sensorsAcquire(sensorData_t *sensors, const u32 tick)	/*获取传感器数据*/
{	
	if (RATE_DO_EXECUTE(SENSOR9_UPDATE_RATE, tick)) /** 500Hz 2ms update **/
	{
		sensor9Read(&sensors->gyro, &sensors->acc, &sensors->mag);
	}
	#if DEBUG_MODE
	printf("sensors:%f,%f,%f,%f,%f,%f,%f,%f,%f\n",sensors->acc.x,sensors->acc.y,sensors->acc.z,sensors->gyro.x,sensors->gyro.y,sensors->gyro.z,sensors->mag.x,sensors->mag.y,sensors->mag.z);
	#endif
//	if (RATE_DO_EXECUTE(BARO_UPDATE_RATE, tick) && isBaroPresent==true) /** 50Hz 40ms update **/
//	{	
//	#ifdef SENSORS_ENABLE_PRESSURE_BMP280			
//		bmp280GetData(&sensors->baro.pressure, &sensors->baro.temperature, &sensors->baro.asl);
//	#endif
//	}

}
