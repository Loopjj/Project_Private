/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : modem.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 12/21/2017
*
* Description        : modem process
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __MODEM_H
#define __MODEM_H

#define MAX_MODEM_TX_BUFF 0xA000

#ifdef MODEM_GLOBALS

  #pragma location = "EXTCCRAM"
  u8 ModemTxBuff[MAX_MODEM_TX_BUFF];
  #define MODEM_EXT 

#else

  extern u8 ModemTxBuff[MAX_MODEM_TX_BUFF];
  #define MODEM_EXT	extern
  
#endif 


#define MODEM_TMPBUFF_SIZE      4096+2048
#define MODEM_SENDDATA_MAX      4096
#define MODEM_COMM_RX_BUF_SIZE  0x1100             /* Number of characters in Rx ring buffer            */
#define MODEM_COMM_NO_ERR       0                /* 함수 호출이 성공적으로 수행되었음                  */
#define MODEM_COMM_RX_EMPTY     1                /* 수신 버퍼가 비었음. 데이터가 없음                  */

#define AT_RETRY_CNT            5

#define HTTP_RX_BUFF_MAX        256
#define DATA_TX_LIMIT           57600 // 15분 데이터 //38400  // 10분데이터 //1382400   //6시간x 60분 x 60초 x sizeof(tsSecData)
  
typedef enum {
  MODEM_POWER_ON,
  MODEM_POWER_ON_IDLE,
  MODEM_INITIALIZE,
  MODEM_DATA_IDLE,
  MODEM_GET_SERVER,  
  MODEM_VDIS_SERVER,
  MODEM_BLE_SERVER,
  MODEM_CHILD_SERVER,
  MODEM_DATA_SERVER,
  MODEM_ON_NOTI,
  MODEM_QUAL_NOTI,
  MODEM_OFF_NOTI,
  MODEM_FOTA_REQ,
  MODEM_LOOP_FOTA,
  MODEM_ON_ADAS,
  MODEM_SEND_ADAS,
  MODEM_EVENT_ADAS,
  MODEM_OFF_ADAS,
  MODEM_POWER_OFF,
  MODEM_POWER_OFF_IDLE,
  MODEM_RESET_REQ,
  MODEM_IDLE,
  MODEM_FOTA_IDLE
}teModem_Step;

typedef enum {
  URCMODE_IDLE,
  URCMODE_SMS,
  URCMODE_HANURI,
  URCMODE_LOOP,
  URCMODE_DMS,
  URCMODE_GVN,
  URCMODE_VDIS,
  URCMODE_BLE,
  URCMODE_CHILD,
  URCMODE_GET,
  URCMODE_ADAS  
}teURCParserMode;

typedef enum {
  DATA_SVR_DATACHECK,
  DATA_SVR_CONNECT,
  DATA_SVR_SEND_WAIT,
  DATA_SVR_SEND,
  DATA_SVR_RECIEVE_WAIT,
  DATA_SVR_FAIL_WAIT,
  DATA_SVR_CLOSE,
  DATA_SVR_FAIL,
  DATA_SVR_SUCCESS,
  DATA_SVR_IDLE
}teDATA_SVR_STATUS;

typedef enum {
  VDIS_SVR_DATACHECK,
  VDIS_SVR_CONNECT,
  VDIS_SVR_SEND_WAIT,
  VDIS_SVR_SEND,
  VDIS_SVR_RECIEVE_WAIT,
  VDIS_SVR_FAIL_WAIT,
  VDIS_SVR_CLOSE,
  VDIS_SVR_FAIL,
  VDIS_SVR_SUCCESS,
  VDIS_SVR_IDLE
}teVDIS_SVR_STATUS;

typedef enum {
  SOCKET_CONNECT_START,
  SOCKET_CREG_CHECK,
  SOCKET_CSQ_CHECK,
  SOCKET_STATUS_CHECK,
  SOCKET_OPEN,
  SOCKET_OPEN_WAIT,
  SOCKET_CREG_FAIL,
  SOCKET_CSQ_FAIL,
  SOCKET_CONNECT,
  SOCKET_CLOSE,
  SOCKET_FAIL
}teSOCKET_STATUS;

typedef enum {
  ON_NOTI_CONNECT,
  ON_NOTI_SEND,
  ON_NOTI_RECIEVE_WAIT,
  ON_NOTI_CLOSE,
  ON_NOTI_FAIL,
  ON_NOTI_SUCCESS,
  ON_NOTI_IDLE
}teON_NOTI_STATUS;

typedef enum {
  GET_SVR_DATACHECK,
  GET_SVR_CONNECT,
  GET_SVR_SEND_WAIT,
  GET_SVR_SEND,
  GET_SVR_RECIEVE_WAIT,
  GET_SVR_CLOSE,
  GET_SVR_FAIL,
  GET_SVR_SUCCESS,
  GET_SVR_RETRY_WAIT,
  GET_SVR_IDLE
}teGET_SVR_STATUS;

typedef enum {
  HTTP_RX_INIT,
  HTTP_RX_RESP_CODE,
  HTTP_RX_LEN_WAIT,
  HTTP_RX_LEN_VAL,
  HTTP_RX_DATA_WAIT,
  HTTP_RX_DATA_SUCCESS,
  HTTP_RX_DATA_FAIL
}teHTTP_RX_STEP;  

typedef enum {
  LOOP_FOTA_CONNECT,
  LOOP_FOTA_WAIT
}teLOOP_FOTA_STATUS;

MODEM_EXT tsModemInfo ModemInfo;
MODEM_EXT u32 DataAddrToSend, VdisAddrToSend;
MODEM_EXT u8 DTG_Data_Flash_WriteFlag, DTG_Data_Flash_ReadFlag;
MODEM_EXT u8 DataServerFlag, VehStateFlag, VdisServerFlag;
MODEM_EXT int DataServerRetryCount, VdisServerRetryCount;

MODEM_EXT u8 OnNotiFlag;
MODEM_EXT int OnNotiRetryCount;
MODEM_EXT u8 SvrEventFlag, SvrEventCode;
MODEM_EXT u8 SvrSvcError;
MODEM_EXT u8 ModemOffFlag;
MODEM_EXT u8 ModemReadyFlag;
MODEM_EXT u8 URLChangeFlag;
MODEM_EXT char APN_Name[32];

MODEM_EXT u8 Prev_DataFlag, Prev_VdisFlag;
MODEM_EXT u32 Prev_DataAdrrToSend, Prev_VdisAdrrToSend;
MODEM_EXT u8 GetServerFlag;
MODEM_EXT int GetServerRetryCount;

MODEM_EXT u8 ModemBuff[MODEM_TMPBUFF_SIZE];
MODEM_EXT char ModemStrBuff[512];
MODEM_EXT u16 ModemStrLen;
MODEM_EXT char HttpHeaderBuff[512];
MODEM_EXT u16 HttpHeaderLen;
MODEM_EXT char HttpRxStrBuff[HTTP_RX_BUFF_MAX];
MODEM_EXT unsigned char HttpRxStep;
MODEM_EXT unsigned int HttpRxCnt, HttpRxLineCnt, HttpRespCode, HttpRxDataLen, HttpRxDataCnt; 
MODEM_EXT unsigned char HttpRxOrCnt, HttpNetErrCnt;
MODEM_EXT u8 HexEncodeFlag;
MODEM_EXT u8 HexEncodeVal;

MODEM_EXT tsUrc Urc;
MODEM_EXT u8 TCP_Open;
MODEM_EXT u8 TcpState;
MODEM_EXT u8 ModemStep;
MODEM_EXT u32 ModemWaitCnt;
MODEM_EXT u8 FotaRespFlag;

MODEM_EXT u8 SocketConnectStep;
MODEM_EXT u8 SocketRetryCnt;
MODEM_EXT u32 SocketWaitCnt;

MODEM_EXT u8 DataSvrStep, VdisSvrStep;
MODEM_EXT u32 DataSvrWaitCount, VdisSvrWaitCount;

MODEM_EXT u8 GetSvrStep;
MODEM_EXT u32 GetSvrWaitCount;

MODEM_EXT u8 OnNotiStep;
MODEM_EXT int OnNotiWaitCount;
MODEM_EXT u8 OnNotiRespFlag;

MODEM_EXT u8 ModemVer[20];
MODEM_EXT char ModemCTN[11];
MODEM_EXT char ComRxAPN_Name[32];

MODEM_EXT char MdmTmpString[100];
MODEM_EXT char AT_Response[256];
MODEM_EXT char ATCmdRxMsg[10][32];

MODEM_EXT tsSMSData SMSData;
MODEM_EXT u8 ServerIPCheckFlag;

MODEM_EXT tsSvrInfo HostSvr;
MODEM_EXT tsSvrInfo VdisGetSvr;
MODEM_EXT tsSvrInfo VdisDataSvr;
MODEM_EXT tsSvrInfo FWDN_Svr;

MODEM_EXT u8 LoopFotaFlag, LoopFotaStep;
MODEM_EXT u8 RxBuffEmpty;
MODEM_EXT u32 ProgressFotaRxByte;
MODEM_EXT u8 FotaFlag, FotaEnable;

MODEM_EXT u32 SendData_StartTime, SendData_EndTime;

MODEM_EXT u8 SendSettingFlag, SendDailyDataFlag;
MODEM_EXT u8 CMPortEnable;

MODEM_EXT tsSecData RD_Sec_Data, Prev_RD_Sec_Data;
MODEM_EXT char RFID_Str[12];

MODEM_EXT void Task_MOD(void const * argument);
MODEM_EXT void ModemCommBuffCheck(void);
MODEM_EXT void ModemInit(void);
MODEM_EXT void Modem_SecIntProcess(void);
MODEM_EXT void Modem_Parameter_Reset(void);

MODEM_EXT void ModemCommInit(void);
MODEM_EXT void ModemCommPutRxChar(unsigned char c);
MODEM_EXT u8 ModemCommGetChar(unsigned char *err);

MODEM_EXT void TxStrToModem(char* str);
MODEM_EXT void TxDataStrToModem(u8* data, int len);
MODEM_EXT void TxCharToModem(char ch);
MODEM_EXT unsigned char TxHexStrToModem(u8* data, int len);

MODEM_EXT u8 sendATcommand(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout);
MODEM_EXT int AT_SendWithRecieveData(char* cmd, char* cmdResp, u8 valNum, unsigned int timeout);

MODEM_EXT unsigned char Modem_SimCard(void);
MODEM_EXT unsigned char APNProcess(unsigned char mode);
MODEM_EXT unsigned char QoSAdust(unsigned char mode, unsigned short val);
MODEM_EXT void Modem_SMS_ReadAll(void);
MODEM_EXT void Modem_SMSSend(char* snum, char* msg, u8 len);

MODEM_EXT void Modem_SocketConnect(char* svr_name, u16 port, u8 mode);
MODEM_EXT unsigned char Modem_SocketClose(int sw);
MODEM_EXT unsigned char Modem_PPPClose(void);

MODEM_EXT void Modem_URC_Parser(unsigned char r);
MODEM_EXT void Modem_parse_proc(void);

MODEM_EXT void Modem_PowerOn(void);
MODEM_EXT void Modem_PowerOff(void);
MODEM_EXT unsigned char Modem_PowerOnCheck(void);
MODEM_EXT void Modem_ResetProcess(void);
//MODEM_EXT void Modem_URLChangeReset(void);
//MODEM_EXT void  Modem_ChangeURLInfo(void);
MODEM_EXT void  Modem_KeyOffProcess(void);

#endif