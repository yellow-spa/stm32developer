
/**
  ******************************************************************************
  * @file     bluetooth.h
  * @author  alex_wang
  * @version V1.0.0
  * @date    02-07-2017
  * @brief   This file contains all the functions prototypes for the bluetooth
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#define CmdreturnLength 20
	
const char ATcmdAsk[]    =		 {"AT"};
const char ATcmdAnswer[] =     {"OK"};

const char ATcmdNameAsk[] = 	 {"AT+NAME?"};
const char ATcmdNameAnswer[] =  {"OK+NAME:athena"};	
const char ATcmdNameSet[] = 	 {"AT+NAMEathena"};    

const char ATcmdCodeAsk[] = 	 {"AT+PIN?"};
const char ATcmdCodeAnswer[] = {"OK+PIN:1234"};	
const char ATcmdCodeSet[] =		 {"AT+PIN1234"};         

const char ATcmdRenewAsk[] = 	 {"AT+RENEW"};	//reset config
const char ATcmdRenewAnswer[] = {"OK+RENEW"};	

const char ATcmdBaudAsk[] =		 {"AT+BAUD?"};
const char ATcmdBaudAnswer[] = {"OK+BAUD:115200"};

	
int bluetooth_work_init(void);