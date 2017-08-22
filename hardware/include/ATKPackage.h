#ifndef _ATKPACKAGE_H
#define	_ATKPACKAGE_H

#include "stm32f10x_conf.h"

/*上行帧头*/
#define UP_BYTE1 0xAA
#define UP_BYTE2 0xAA

/*下行帧头*/
#define DOWN_BYTE1 0xAA
#define DOWN_BYTE2 0xAF

#define ATKP_MAX_DATA_SIZE 30

/*通讯数据结构*/
typedef struct
{
	uint8_t funcID;
	uint8_t dataLen;
	uint8_t data[ATKP_MAX_DATA_SIZE];
}ATKPack_t;

/*接收状态变化表*/
static enum
{
	waitForStartByte1,
	waitForStartByte2,
	waitForMsgID,
	waitForDataLength,
	waitForData,
	waitForChksum1,
}ReceiveState;

/*下行指令ID*/
typedef enum 
{
	DOWN_COMMAND	= 0x01,
	DOWN_COMMAND2	= 0x02,
	DOWN_RCDATA = 0x03,
	DOWN_GPSDATA = 0x04,
	DOWN_POWER		= 0x05,
	DOWN_FLYMODE	= 0x0A,
	DOWN_PID1		= 0x10,
	DOWN_PID2		= 0x11,
	DOWN_PID3		= 0x12,
	DOWN_PID4		= 0x13,
	DOWN_PID5		= 0x14,
	DOWN_PID6		= 0x15,
	DOWN_RADIO		= 0x40,
	
	DOWN_REMOTER	= 0x50,
}downfuncID;
/*DOWN_COMMAND work id*/
typedef enum 
{
	COMMAND_ACC_CALIB = 0x01,
	COMMAND_GYRO_CALIB = 0x02,
	COMMAND_MAG_CALIB = 0x04,
	COMMAND_BRAO_CALIB = 0x05,
	COMMAND_FLIGHT_LOCK = 0xA0,
	COMMAND_FLIGHT_ULOCK = 0xA1,
}command_work;

/*DOWN_COMMAND2 work id*/
typedef enum 
{
	COMMAND2_READ_PID=0x01,
	COMMAND2_READ_DUTSETTING=0x02,
	COMMAND2_GET_VERSION = 0xA0,
}command2_work;

/*上行指令ID*/
typedef enum 
{
	UP_VERSION		= 0x00,
	UP_STATUS		= 0x01,
	UP_SENSER		= 0x02,
	UP_RCDATA		= 0x03,
	UP_GPSDATA		= 0x04,
	UP_POWER		= 0x05,
	UP_MOTOR		= 0x06,
	UP_SENSER2		= 0x07,
	UP_FLYMODE		= 0x0A,
	UP_SPEED 		= 0x0B,
	UP_PID1			= 0x10,
	UP_PID2			= 0x11,
	UP_PID3			= 0x12,
	UP_PID4			= 0x13,
	UP_PID5			= 0x14,
	UP_PID6			= 0x15,
	UP_RADIO		= 0x40,
	UP_MSG			= 0xEE,
	UP_CHECK		= 0xEF,
	
	UP_REMOTER		= 0x50,
	UP_PRINTF		= 0x51,
}upfuncID;


void ATKPackage_Receive_Prepare(u8 data);
void ATKPackage_Receive_Anl(ATKPack_t *rxPacket);
u8 ATKPackage_Receive_checksum(ATKPack_t *rxPacket);
u8 ATKPackage_send_sum(const ATKPack_t txPacket);
void ATKPackage_SendData(u8 *dataToSend , u8 length);
void ATKPackage_SendBuffer(const ATKPack_t txPacket);
void ATKPackage_Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver);
void ATKPackage_SendPeriod(void);	

#endif
