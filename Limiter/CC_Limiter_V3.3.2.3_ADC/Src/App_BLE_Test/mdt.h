/**
  ******************************************************************************
  * @file    cdma.h 
  * @author  LOOP
  * @version V1.0
  * @date    09/03/2012
  * @brief   Gloval Variable Header File.
  ******************************************************************************
  * @copy
  *
  * <h2><center>&copy; COPYRIGHT 2012 LOOP</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDT_H
#define __MDT_H

#ifdef MDT_GLOBALS
#define MDT_EXT
#else
#define MDT_EXT	extern
#endif 

/* Includes ------------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/
#define MDT_RX_BUFF_SIZE 100
#define MDT_RX_MAX_INDEX 5
#define MDT_TX_BUFF_SIZE 1024

#define MDT_LG_KEY_OFF_WAIT_LIMIT  600    //10*60 sec LG MDT 모드에서 10분지난후 Key Off
#define MDT_S1_KEY_OFF_WAIT_LIMIT  59    //60-1 sec S1 MDT 모드에서 1분 이내 Key Off

/* Type declarations ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
MDT_EXT u8 MDT_SecDataTimeBuffer[7];
MDT_EXT st_MDT_DATA MDT_Min_Buff[4][60];
MDT_EXT u32 MDT_DTrip[4], MDT_ODO[4], MDT_DFuel[4], MDT_TFuel[4];
MDT_EXT float MDT_MTrip[4], MDT_MFuel[4];
MDT_EXT u8 MDT_AEBS[4], MDT_LDWS1[4], MDT_LDWS2[4]; 
MDT_EXT u8 MDT_Check_Cnt;
MDT_EXT u16 MDT_Key_Off_Time;
MDT_EXT u32 WifiNandAddr;
MDT_EXT u32 MDT_LastDataTime;



/* Private variables ---------------------------------------------------------*/
   
/* Function prototypes -------------------------------------------------------*/
//----------------------------------- S1 -------------------------------------//
MDT_EXT	void MDT_RxData_S1(unsigned char rdata);
MDT_EXT	void MDT_Check_S1(void);
MDT_EXT	u8* CarNo_BinToAscii_S1(u8 *data);
MDT_EXT	void MDT_Tx_Data_S1(u16 command, u8 *data, u16 len) ;

MDT_EXT	void SendDVR_Data(void);
MDT_EXT	void SendRU_Data(void);
MDT_EXT	void Decode_Data_JY(void);

MDT_EXT	void WifiCheck(void);
MDT_EXT	void WifiScheduler(void);
MDT_EXT	void WifiTxData(void);
MDT_EXT	void WifiRxHeaderParse(u8 rdata);
MDT_EXT	void MDT_SecIntProcess(void);
#endif
/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/