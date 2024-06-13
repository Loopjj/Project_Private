/*
*************************************** (C) COPYRIGHT 2015 LOOP *************************************
*
* File Name          : cdma.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : CDMA header
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef CDMA_H
#define CDMA_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef CDMA_GLOBALS
#define CDMA_EXT
#else
#define CDMA_EXT	extern
#endif 

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#define INDEX_MAX         0x1000  // 4k
#define COUNT_MAX         0x1000  // 4k
#define LENGTH_MAX        0x1000  // 4k
#define MODEM_RX_DELAY    10

typedef enum {
  CAMPED_ON_WCDMA = 101, //use
  PPP_CONNECT = 200,
  PPP_CONNECTED = 600,
  USER_PPP_END = 219,
  PPP_DETACH_SUCCESS = 216,
  PPP_SESSION_FINISH = 217,
  PPP_DISCONNECTED = 601, //use
  TCP_CONNECT = 602,
  TCP_CONNECTED = 603, //use
  TCP_DISCONNECT = 642,
  TCP_DISCONNECTED = 605,
  
  IMSI_DETACH_INDICATION = 350,
  CM_SERVICE_ACCEPT = 363,
  LOCATION_UPDATING_ACCEPT = 359,
  
  BOOT_UP = 900  
}teTellMsg;

typedef enum {
  CM_INFO = ('I'<<24)+('N'<<16)+('F'<<8)+'O',
  CM_FWIF = ('F'<<24)+('W'<<16)+('I'<<8)+'F',
  CM_FWSD = ('F'<<24)+('W'<<16)+('S'<<8)+'D',
  CM_FWAK = ('F'<<24)+('W'<<16)+('A'<<8)+'K',
  CM_DTSC = ('D'<<24)+('T'<<16)+('S'<<8)+'C',
  CM_DTSS = ('D'<<24)+('T'<<16)+('S'<<8)+'S',
	CM_FWER = ('F'<<24)+('W'<<16)+('E'<<8)+'R',
	CM_PRMW = ('P'<<24)+('R'<<16)+('M'<<8)+'W',

}teCommand;

typedef struct {
  unsigned char Index; //0~4
  unsigned char IP[4];
  unsigned short Port;
}tsHostInfo;

typedef struct {
  unsigned int  Process;
  unsigned int  Step;
  unsigned int  Parse;
  unsigned int  Delay;
  unsigned int  Tell;
  unsigned int  Cnum; 
  tsHostInfo    HostInfo;
}tsModem;

typedef enum {
  DV_LOCAL,
  DV_SERVER,
  DV_DPF,
  DV_DTG,
  DV_SCR,
  DV_GPSTR,
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
  ST_PROCESSING
}tePckStep;

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


typedef struct {
    unsigned int   Size;
    unsigned short DivisionSize;
    unsigned short CRC16;
}tsFWIF;


typedef struct {
  unsigned int IndexAddr;
  unsigned int DataAddr;
  unsigned int TripCount;
  unsigned int SentAddr;
}tsSaveCtrl;
  

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

CDMA_EXT void Task_CDM(void *p_arg);
CDMA_EXT void ModemRcvParser(char r);
CDMA_EXT void RxModemDecode(void);
CDMA_EXT int RxModemChar(unsigned char r);

/* Private functions ---------------------------------------------------------*/


#endif

/*
********************* (C) COPYRIGHT 2015 LOOP ***************END OF FILE*****************************
*/