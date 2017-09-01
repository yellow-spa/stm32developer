#ifndef __I2CCORE_H
#define __I2CCORE_H
#include "stm32f10x_conf.h"
#include "hardwareconfig.h"
#if MPU6050APK_SUPPORT
void MPU6050_I2C_ByteWrite(u8 slaveAddr, u8* pBuffer, u8 writeAddr);
void MPU6050_I2C_BufferRead(u8 slaveAddr, u8* pBuffer, u8 readAddr, u16 NumByteToRead);
#endif
u8 i2c2DevReadByte(u8 devaddr,u8 addr); 	//��ָ����ַ����һ������
void i2c2DevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf); //����������ֽ�
void i2c2DevWriteByte(u8 devaddr,u8 addr,u8 data); //��ָ����ַд��һ������
void i2c2DevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf); //����д����ֽ�
#endif
