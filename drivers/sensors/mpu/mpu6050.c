#include "MPU6050.h"
#include "stm32f10x.h"
#include "i2c-core.h"
#include "hardwareconfig.h"
/******************************++++++++++++Initialize+++++++++++++++++++++**********************/

bool MPU6050_Initialize(void)
{
	  bool Status;
	  MPU6050_DeviceReset(ENABLE);
	  systick_ms(50);
	  Status = MPU6050_TestConnection();
	  if(!Status)
		{
			printf("MPU6050 Initialize Fail!");
		}
		
    MPU6050_SetClockSource(MPU6050_CLOCK_PLL_ZGYRO);//时钟源跟Z轴对齐 
	  MPU6050_Int_Enable(DISABLE);	//关闭中断
	  MPU6050_Int_Pin_CFG_Bypass_Enable(ENABLE);	//使能I2C BYPASS
    MPU6050_SetFullScaleGyroRange(MPU6050_GYRO_FS_2000);//gyro的刻度设置 2000
    MPU6050_SetFullScaleAccelRange(MPU6050_ACCEL_FS_8);//acc的刻度设置 ±8G
		MPU6050_SampleRateDivider(0x01);			//采样速率(1000/(1+1)=500Hz)
	  MPU6050_Configuration_DLPF(MPU6050_DLPF_BW_98);	//数字低通滤波器带宽
    MPU6050_SetSleepModeStatus(DISABLE);
	  return Status;
}

/** Set sleep mode status.
 * @param enabled New sleep mode enabled status
 * @see MPU6050_GetSleepModeStatus()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
void MPU6050_SetSleepModeStatus(FunctionalState NewState)
{
    MPU6050_WriteBit(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, NewState);
}
/** Set clock source setting.
 * <pre>
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 * </pre>
 *
 * @param source New clock source setting
 * @see MPU6050_GetClockSource()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_CLKSEL_BIT
 * @see MPU6050_PWR1_CLKSEL_LENGTH
 */
void MPU6050_SetClockSource(uint8_t source)
{
    MPU6050_WriteBits(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see MPU6050_GetFullScaleGyroRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
void MPU6050_SetFullScaleGyroRange(uint8_t range)
{
    MPU6050_WriteBits(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/** Set full-scale accelerometer range.
 * @param range New full-scale accelerometer range setting
 * @see MPU6050_GetFullScaleAccelRange()
 */
void MPU6050_SetFullScaleAccelRange(uint8_t range)
{
    MPU6050_WriteBits(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**
* device reset
**/
void MPU6050_DeviceReset(FunctionalState NewState)
{
    MPU6050_WriteBit(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, NewState);
}
/***********************
* sample rate divider
************************/
void MPU6050_SampleRateDivider(uint8_t range)
{
	 MPU6050_WriteBits(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_SMPLRT_DIV, 7, 8, range);
}

void MPU6050_Configuration_DLPF(uint8_t range)
{
	 MPU6050_WriteBits(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, range);
}

/****
*中断开关
*/
void MPU6050_Int_Enable(FunctionalState NewState)
{
    MPU6050_WriteBit(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_INT_ENABLE, MPU6050_INT_ENABLE_DATA_RAY_EN_BIT, NewState);
}
/**
*启用i2c bypass,让I2C也能够访问HCM5883L
**/
void MPU6050_Int_Pin_CFG_Bypass_Enable(FunctionalState NewState)
{
	  MPU6050_WriteBit(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_INT_PIN_CFG, MPU6050_INT_PIN_CFG_BYPASS, NewState);
}
/******************************---------------Initialize--------------**********************/

/***************************************+++++verify mpu6050+++++************************************************************/
/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, FALSE otherwise
 */
bool MPU6050_TestConnection()
{
    return MPU6050_GetDeviceID() == 0x34 ? TRUE : FALSE; //0b110100; 8-bit representation in hex = 0x34
}

// WHO_AM_I register

/** Get Device ID.
 * This register is used to verify the identity of the device (0b110100).
 * @return Device ID (should be 0x68, 104 dec, 150 oct)
 * @see MPU6050_RA_WHO_AM_I
 * @see MPU6050_WHO_AM_I_BIT
 * @see MPU6050_WHO_AM_I_LENGTH
 */
uint8_t MPU6050_GetDeviceID()
{
    uint8_t tmp;
    MPU6050_ReadBits(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &tmp);
    return tmp;
}

/***************************************-----verify mpu6050-----************************************************************/



/** write a single bit in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 */
void MPU6050_WriteBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
    uint8_t tmp;
	 #if MPU6050APK_SUPPORT
	  MPU6050_I2C_BufferRead(slaveAddr, &tmp, regAddr, 1);
	 #else
    tmp = i2c2DevReadByte(slaveAddr, regAddr);
	 #endif
    tmp = (data != 0) ? (tmp | (1 << bitNum)) : (tmp & ~(1 << bitNum));
	  #if MPU6050APK_SUPPORT
	   MPU6050_I2C_ByteWrite(slaveAddr, &tmp, regAddr);
	  #else
    i2c2DevWriteByte(slaveAddr, regAddr,tmp);
	 #endif
}



/** Write multiple bits in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 */
void MPU6050_WriteBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t tmp;
	  uint8_t mask;
	  #if MPU6050APK_SUPPORT
	   MPU6050_I2C_BufferRead(slaveAddr, &tmp, regAddr, 1);
	  #else
  	tmp = i2c2DevReadByte(slaveAddr,regAddr);
	  #endif
    mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    tmp &= ~(mask); // zero all important bits in existing byte
    tmp |= data; // combine data with existing byte
	  #if MPU6050APK_SUPPORT
	  MPU6050_I2C_ByteWrite(slaveAddr, &tmp, regAddr);
		#else
    i2c2DevWriteByte(slaveAddr, regAddr,tmp);
		#endif
}

/** Read multiple bits from an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
 */
void MPU6050_ReadBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
    uint8_t tmp;
	  uint8_t mask;
	  #if MPU6050APK_SUPPORT
	   MPU6050_I2C_BufferRead(slaveAddr, &tmp, regAddr, 1);
	 #else
	  tmp = i2c2DevReadByte(slaveAddr,regAddr);
	#endif
    mask = ((1 << length) - 1) << (bitStart - length + 1);
    tmp &= mask;
    tmp >>= (bitStart - length + 1);
    *data = tmp;
}

/** Get raw 6-axis motion sensor readings (accel/gyro).
 * Retrieves all currently available motion sensor values.
 * @param AccelGyro 16-bit signed integer array of length 6
 * @see MPU6050_RA_ACCEL_XOUT_H
 */
/*获取ACC 3轴原始数据*/
void MPU6050_GetAccelRawData(s16* ax, s16* ay, s16* az)
{
  s8 buffer[6]={0};
	int i;
	#if MPU6050APK_SUPPORT
	MPU6050_I2C_BufferRead(MPU6050_DEFAULT_ADDRESS, buffer, MPU6050_RA_ACCEL_XOUT_H, 6);
	#else
	i2c2DevRead(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_ACCEL_XOUT_H, 6, buffer);
	#endif
	*ax = (((s16) buffer[0]) << 8) | buffer[1];
	*ay = (((s16) buffer[2]) << 8) | buffer[3];
	*az = (((s16) buffer[4]) << 8) | buffer[5];
}

/** Get raw 6-axis motion sensor readings (accel/gyro).
 * Retrieves all currently available motion sensor values.
 * @param AccelGyro 16-bit signed integer array of length 6
 * @see MPU6050_RA_GYRO_XOUT_H
 */
/*获取gyro 3轴原始数据*/
void MPU6050_GetGyroRawData(s16* gx, s16* gy, s16* gz)
{
	s8 buffer[6]={0};
  #if MPU6050APK_SUPPORT
	MPU6050_I2C_BufferRead(MPU6050_DEFAULT_ADDRESS, buffer, MPU6050_RA_GYRO_XOUT_H, 6);
	#else
	i2c2DevRead(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_GYRO_XOUT_H, 6, buffer);
	#endif
	*gx = (((s16) buffer[0]) << 8) | buffer[1];
	*gy = (((s16) buffer[2]) << 8) | buffer[3];
	*gz = (((s16) buffer[4]) << 8) | buffer[5];
}

