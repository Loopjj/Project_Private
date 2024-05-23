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

#define DATA_TX_LIMIT 38400  // 10분데이터 //1382400   //6시간x 60분 x 60초 x sizeof(tsSecData)

#define NOT_USE_MODEM           0
#define USE_MODEM               1
  
#define LOOP_PROTOCOL           0
#define MAX_EXT_MODEM_PROTOCOL  0
#define EXT_MODEM_PROTOCOL_TYPE LOOP_PROTOCOL

#define LOOP_SERVER             0
#define EXT_MODEM_SERVER        LOOP_SERVER

#define LOOP_OTA_SERVER_ENABLE
  
#define LENGTH_MAX      4096
  
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

typedef enum {
    CM_INFO = ('I'<<24)+('N'<<16)+('F'<<8)+'O',
    CM_FWIF = ('F'<<24)+('W'<<16)+('I'<<8)+'F',
    CM_FWSD = ('F'<<24)+('W'<<16)+('S'<<8)+'D',
    CM_FWAK = ('F'<<24)+('W'<<16)+('A'<<8)+'K',
    CM_DTSC = ('D'<<24)+('T'<<16)+('S'<<8)+'C',
    CM_DTSS = ('D'<<24)+('T'<<16)+('S'<<8)+'S',
    CM_FWER = ('F'<<24)+('W'<<16)+('E'<<8)+'R',
    CM_PRMW = ('P'<<24)+('R'<<16)+('M'<<8)+'W',
    CM_CLSE = ('C'<<24)+('L'<<16)+('S'<<8)+'E',
    CM_DYSS = ('D'<<24)+('Y'<<16)+('S'<<8)+'S',
    CM_MDIR = ('M'<<24)+('D'<<16)+('I'<<8)+'R',
    CM_MDIW = ('M'<<24)+('D'<<16)+('I'<<8)+'W',
    CM_F2IF = ('F'<<24)+('2'<<16)+('I'<<8)+'F',
    CM_F2SD = ('F'<<24)+('2'<<16)+('S'<<8)+'D',
    CM_F2AK = ('F'<<24)+('2'<<16)+('A'<<8)+'K',
    CM_P1DR = ('P'<<24)+('1'<<16)+('D'<<8)+'R',
    CM_P1SD = ('P'<<24)+('1'<<16)+('S'<<8)+'D',
    CM_P1DD = ('P'<<24)+('1'<<16)+('D'<<8)+'D',
    CM_P1SC = ('P'<<24)+('1'<<16)+('S'<<8)+'C',
    CM_P2DR = ('P'<<24)+('2'<<16)+('D'<<8)+'R',
    CM_P2SD = ('P'<<24)+('2'<<16)+('S'<<8)+'D',
    CM_P2DD = ('P'<<24)+('2'<<16)+('D'<<8)+'D',
    CM_P2SC = ('P'<<24)+('2'<<16)+('S'<<8)+'C'
}teCommand;

typedef enum {
  DV_SERVER,          //Server ack
  DV_LOOPBACK,        //Test Only
  DV_SCR2,            //SCR AEA
  DV_DTG,             //DTG
  DV_SCR,             //Not Use, Old Data structure
  DV_GPSTR,           //GPS Tracker for Philippines
  DV_MOBILE,          //For Mobile Phone
  DV_MDT_EL,          //MDT Elex
  DV_DPF,             //DPF
  DV_COLOMBIA,        //Not Use For Gasan Testing
  DV_TRK_BC,          //GPS Tracker with Beacon
  DV_TRK_RF,          //GPS Tracker with RFID (For Malaysia)
  DV_ISG,             //Idling Stop
  DV_DATALOGGOER,     //Data Logger
  DV_MAX
}teDevice;

typedef enum {
  ST_SPK_ST,
  ST_SPK_CF,
  ST_DEVICE_ID,
  ST_IDENTITY_NO,
  ST_COMMAND,
  ST_INDEX,
  ST_COUNT,
  ST_LENGTH,
  ST_DATA,
  ST_CRC16,
  ST_EPK_ST,
  ST_EPK_CF,
  ST_RETRY,
  ST_PROCESSING
}tePckStep;

__packed typedef struct {  
  u32  SrvIP;
  u16  SrvPort;
  u32  CNUM;
  char MSISDN[16];
  u16  DataServerInterval;
  u16  VehStateInterval;
  u16  VehStateInterval_Idle;
  u32  LpIP;
  u16  LpPort;
  u16  FotaPollingIntrval;
  u16  QualPollingIntrval;
  u8   FactoryInit;
  u8   ServerChange;
  u8   ServerConnected;
  u16  DataInterval;
  u8   res2[15];
  u16  CRC16;
}tsModemInfo; //64

#define URC_BUFF_MAX 100
typedef struct {
  int  Step;
  char Buff[URC_BUFF_MAX];  
  int  Cnt;
  int  Len;
  int  RespFlag;
  int  Mode;  
}tsUrc;

typedef struct {
  int  Step;
  u8   RCnt;
  u8   MCnt;
  u8   DCnt;
  char RcvNum [20];
  char Date[30];
  char Msg[80];
}tsSMSData;

__packed typedef struct {  
  char SvrName[60];
  u32  SvrPort;
}tsSvrInfo; //64

__packed typedef struct {
  unsigned short SPK;
  unsigned short DeviceID;
  unsigned int   IdentityNo;
  unsigned int   Command;
  unsigned short Index;
  unsigned short Count;
  unsigned int   Length;
}tsPckHeader;

__packed typedef struct {
  unsigned short CRC16;
  unsigned short EPK;
}tsPckFooter;

__packed typedef struct {
  unsigned int   Step;
  unsigned int   Cnt;
  unsigned short CalcCRC16;
  unsigned int   Delay;
  tsPckHeader    Header;
  unsigned char  Data[LENGTH_MAX];   //if c++ : TMemoryStream
  tsPckFooter    Footer;
}tsRpck;

__packed typedef struct {
  unsigned int   Size;
  unsigned short DivisionSize;
  unsigned short CRC16;
  unsigned char  Type;    
}tsFWIF;

MODEM_EXT u32 Loop_SendTimeCnt;

MODEM_EXT const unsigned short crc16tab[256];
MODEM_EXT uint32_t _millisCounter;

MODEM_EXT u8 MtTxAPNFlag;
MODEM_EXT u8 MtReadAPNFlag,MtChangeAPNFlag;
MODEM_EXT u8 COM_Command_Char;

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
MODEM_EXT u8 ModemPowerOnFlag;
MODEM_EXT char APN_Name[32];

MODEM_EXT u8 Prev_DataFlag, Prev_VdisFlag;
MODEM_EXT u32 Prev_DataAdrrToSend, Prev_VdisAdrrToSend;
MODEM_EXT u8 GetServerFlag;
MODEM_EXT int GetServerRetryCount;

MODEM_EXT u8 ModemBuff[MODEM_TMPBUFF_SIZE];

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
MODEM_EXT tsSvrInfo FWDN_Svr;

MODEM_EXT u8 LoopFotaFlag, LoopFotaStep;
MODEM_EXT u8 RxBuffEmpty;
MODEM_EXT u32 ProgressFotaRxByte;
MODEM_EXT u8 FotaFlag, FotaEnable;

MODEM_EXT u32 SendData_StartTime, SendData_EndTime;

MODEM_EXT u8 SendSettingFlag, SendDailyDataFlag;

MODEM_EXT int StringLen(char* str, int len);
MODEM_EXT uint32_t millis(void);
MODEM_EXT void init_millis(void);
MODEM_EXT long AsciiToVal(char* str);
MODEM_EXT u8 IsSameString(char* str1, char* str2, u8 len);
MODEM_EXT u8 IsSameString2(char* str1, char* str2, u8 mlen, u8 len, u8* start);
MODEM_EXT unsigned short int  getcrc16(u16 crc, unsigned char *buf, unsigned int len);
MODEM_EXT unsigned short getcrc16c(unsigned short crc, unsigned char data);

MODEM_EXT void Task_MOD(void *p_arg);
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
MODEM_EXT void Modem_URLChangeReset(void);
MODEM_EXT void  Modem_ChangeURLInfo(void);
MODEM_EXT void  Modem_KeyOffProcess(void);

#endif