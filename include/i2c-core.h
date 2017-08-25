#ifndef __I2CCORE_H
#define __I2CCORE_H
#include "stm32f10x_conf.h"
u8 i2c2DevReadByte(u8 devaddr,u8 addr); 	//��ָ����ַ����һ������
void i2c2DevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf); //����������ֽ�
void i2c2DevWriteByte(u8 devaddr,u8 addr,u8 data); //��ָ����ַд��һ������
void i2c2DevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf); //����д����ֽ�
#endif
