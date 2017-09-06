
#include "ATKPackage.h"
#include "usart.h"
#include "stabilizer_types.h"
#include "sensors_types.h"
#include "sensors.h"
#include "battery.h"
/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

//数据返回周期时间（单位ms)
#define  PERIOD_STATUS		30
#define  PERIOD_SENSOR 		10
#define  PERIOD_RCDATA 		40
#define  PERIOD_POWER 		100
#define  PERIOD_MOTOR		  40
#define  PERIOD_SENSOR2 	40
#define  PERIOD_SPEED   	50

static ATKPack_t rxPacket; //接收的数据

//接受预先判断是否符合协议帧头

void ATKPackage_Receive_Prepare(u8 data)
{
	static u8 rxState = waitForStartByte1;
	static u8 dataIndex = 0;
	switch(rxState)
	 {
		case waitForStartByte1 :
			     rxState = (data == DOWN_BYTE1) ? waitForStartByte2 : waitForStartByte1;
		       break;
		case waitForStartByte2:
				 	 rxState = (data == DOWN_BYTE2) ? waitForMsgID : waitForStartByte1;
					 break;
		case waitForMsgID:
					 rxPacket.funcID = data;
					 rxState = waitForDataLength;
					break;
		case waitForDataLength:
					if (data <= ATKP_MAX_DATA_SIZE)
					{
						rxPacket.dataLen = data;
						dataIndex = 0;
						rxState = (data > 0) ? waitForData : waitForChksum1;	/*c=0,数据长度为0，校验1*/
					}
					else 
					{
						rxState = waitForStartByte1;
					}
					break;
		case waitForData:
					rxPacket.data[dataIndex] = data;
					dataIndex++;
					if (dataIndex == rxPacket.dataLen)
					{
						rxState = waitForChksum1;
					}
					break;
		case waitForChksum1:
					if (ATKPackage_Receive_checksum(&rxPacket) == data)	/*所有校验正确*/
					{
				     ATKPackage_Receive_Anl(&rxPacket);
					} 
					else	/*校验错误*/
					{
						rxState = waitForStartByte1;	
					}
					rxState = waitForStartByte1;
					break;
				default:
					break;
	 }
	
}
//正式判断并作出相应的操作
void ATKPackage_Receive_Anl(ATKPack_t *rxPacket)
{
	if(rxPacket->funcID	== DOWN_COMMAND)
	{
		switch(rxPacket->data[0])
		{
			case COMMAND_ACC_CALIB:
				break;
			case COMMAND_GYRO_CALIB:
				break;
			case COMMAND_MAG_CALIB:
				break;
			case COMMAND_BRAO_CALIB:
				break;
			case COMMAND_FLIGHT_LOCK:
				break;
			case COMMAND_FLIGHT_ULOCK:
				break;
			default:
				break;
		}
	}
	else if(rxPacket->funcID	== DOWN_COMMAND2){
		 switch(rxPacket->data[0])
		{
			 case COMMAND2_READ_PID: /* read PID*/
				 break;
			 case COMMAND2_READ_DUTSETTING:
				 break;
			 case COMMAND2_GET_VERSION:
				   ATKPackage_Send_Version(4,300,100,400,0);
				 break;
		}
	}else if(rxPacket->funcID	== DOWN_RCDATA){
		  // fly setting data
	}else if(rxPacket->funcID	== DOWN_POWER){
	}
}

//receive data时的check sum
u8 ATKPackage_Receive_checksum(ATKPack_t *rxPacket)
{
	u8 cksum = 0;
	u8 i;
	cksum = DOWN_BYTE1+DOWN_BYTE2;
	cksum = cksum+rxPacket->funcID+rxPacket->dataLen;
	for(i=0;i<rxPacket->dataLen;i++)
	{
		cksum = cksum + rxPacket->data[i];
	}
	return cksum;
}

//send data 时需要计算的sum
u8 ATKPackage_send_sum(const ATKPack_t txPacket)
{
	u8 cksum = 0;
	u8 i;
	cksum +=UP_BYTE1;
	cksum +=UP_BYTE2;
	cksum = cksum+txPacket.funcID+txPacket.dataLen;
	for(i=0;i<txPacket.dataLen;i++)
	{
		cksum = cksum + txPacket.data[i];
	}
	return cksum;
}

//发送数据包
void ATKPackage_SendBuffer(const ATKPack_t txPacket)
{
	u8 sendBuffer[64];
	u8 cksum;
	u8 dataLen;
	sendBuffer[0] = UP_BYTE1;
	sendBuffer[1] = UP_BYTE2;
	sendBuffer[2] = txPacket.funcID;
	sendBuffer[3] = txPacket.dataLen;
	memcpy(&sendBuffer[4], txPacket.data, txPacket.dataLen);
	dataLen = txPacket.dataLen + 5;
	cksum = ATKPackage_send_sum(txPacket);
	sendBuffer[dataLen - 1] = cksum;
	ATKPackage_SendData(sendBuffer, dataLen);
	
}

/////////////////////////////////////////////////////////////////////////////////////
//Send_Data函数是协议中所有发送数据功能使用到的发送函数
//移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
void ATKPackage_SendData(u8 *dataToSend , u8 length)
{
#ifdef EN_USART2_RX
	 Uart2SendBuffer(dataToSend, length);
#endif
}

/********************************
* 发送版本信息
* 
*******************************/
void ATKPackage_Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver)
{
	ATKPack_t txPacket;
	u8 cnt=0,sum=0;
  txPacket.funcID=UP_VERSION;
	txPacket.dataLen=0;
	txPacket.data[cnt++]=hardware_type;
	txPacket.data[cnt++]=BYTE1(hardware_ver);
	txPacket.data[cnt++]=BYTE0(hardware_ver);
	txPacket.data[cnt++]=BYTE1(software_ver);
	txPacket.data[cnt++]=BYTE0(software_ver);
	txPacket.data[cnt++]=BYTE1(protocol_ver);
	txPacket.data[cnt++]=BYTE0(protocol_ver);
	txPacket.data[cnt++]=BYTE1(bootloader_ver);
	txPacket.data[cnt++]=BYTE0(bootloader_ver);
	txPacket.dataLen = cnt-1;
  ATKPackage_SendBuffer(txPacket);
}

static void ATKPackage_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
	u8 _cnt=0;
	ATKPack_t p;
	vs16 _temp;
	
	p.funcID = UP_SENSER;

	_temp = a_x;
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);
	_temp = 0;	
	p.data[_cnt++]=BYTE1(_temp);
	p.data[_cnt++]=BYTE0(_temp);	
	
	p.dataLen = _cnt;
	ATKPackage_SendBuffer(p);
}

static void sendPower(u16 votage, u16 current)
{
	u8 _cnt=0;
	ATKPack_t p;
	
	p.funcID = UP_POWER;
	
	p.data[_cnt++]=BYTE1(votage);
	p.data[_cnt++]=BYTE0(votage);
	p.data[_cnt++]=BYTE1(current);
	p.data[_cnt++]=BYTE0(current);
	
	p.dataLen = _cnt;
	ATKPackage_SendBuffer(p);
}

//数据周期上报给上位机，每1ms报一次
void ATKPackage_SendPeriod(void)
{
		static u16 count_ms = 1;

	if(!(count_ms % PERIOD_STATUS))
	{
		attitude_t attitude;
		//getAttitudeData(&attitude);
	//	sendStatus(attitude.roll, attitude.pitch, -attitude.yaw,0,0,flyable);				
	}
	if(!(count_ms % PERIOD_SENSOR))
	{
		Axis3i16 acc;
		Axis3i16 gyro;
		Axis3i16 mag;
		getSensorRawData(&acc, &gyro, &mag);
		ATKPackage_Send_Senser(acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z,mag.x,mag.y,mag.z);
	}
	if(!(count_ms % PERIOD_RCDATA))
	{
	//	sendRCData(rcdata.thrust, rcdata.yaw, rcdata.roll,
	//				rcdata.pitch, 0, 0, 0, 0, 0, 0);
	}
	if(!(count_ms % PERIOD_POWER))
	{
	 	float bat = pmGetBatteryVoltage();
		sendPower(bat*100,500);
	}
	if(!(count_ms % PERIOD_MOTOR))
	{
		u16 m1,m2,m3,m4;
	//	motorPWM_t motorPWM;
	//	getMotorPWM(&motorPWM);
	//	m1 = (float)motorPWM.m1/65535*1000;
	//	m2 = (float)motorPWM.m2/65535*1000;
	//	m3 = (float)motorPWM.m3/65535*1000;
	//	m4 = (float)motorPWM.m4/65535*1000;
	//	sendMotorPWM(m1,m2,m3,m4,0,0,0,0);
	}
	if(!(count_ms % PERIOD_SENSOR2))
	{
	//	int baro = getBaroData();
	//	sendSenser2(baro,0);
	}
	if(++count_ms>=65535) 
		count_ms = 1;	
}
//heihei
