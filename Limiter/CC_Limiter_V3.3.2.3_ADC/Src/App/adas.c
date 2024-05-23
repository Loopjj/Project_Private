/*
*************************************** (C) COPYRIGHT 2016 LOOP *************************************
*
* File Name          : adas.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 30/01/2019
*
* Description        : adas protocol
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define ADAS_GLOBALS
#include "includes.h"

u8 RxADAS_Step, RxADAS_RxCnt;
u16 RxADAS_CalcCRC16, RxADAS_GetCRC16;
u8 RxADAS_DataLen, RxADAS_Data[3];
/*
********************************************************************************
* Description : RxDataADAS
* Arguments   : none
* Return      : none 
* Note        : adas process
******************************************************************************** 
*/
void RxDataADAS(unsigned char r)
{  
  switch(RxADAS_Step) {
    case ST_STX:
      if(r == '$')
        RxADAS_Step = ST_DUMMY;
      else 
        RxADAS_Step = ST_STX;
      break;
    case ST_DUMMY:
      if(r == '0')
        RxADAS_Step = ST_DIRECTION;
      else
        RxADAS_Step = ST_STX;
      break;
    case ST_DIRECTION:
      if(r == '0' || r == '1' || r =='2' || r == '3') {
        RxADAS_CalcCRC16 = 0;
        RxADAS_GetCRC16 = 0;
        RxADAS_Data[0] = r;
        RxADAS_CalcCRC16 = getcrc16c(RxADAS_CalcCRC16,r);
        RxADAS_RxCnt = 0;
        RxADAS_Step = ST_SPEED; 
      }
      else
        RxADAS_Step = ST_STX;
      break;
    case ST_SPEED:
      RxADAS_CalcCRC16 = getcrc16c(RxADAS_CalcCRC16,r);
      RxADAS_RxCnt++;
      if(RxADAS_RxCnt >= 3) {
        RxADAS_RxCnt = 0;
        RxADAS_Step = ST_LDW; 
      }
      break;
    case ST_LDW:
      if(r == '0' || r == '1' || r =='2') {
        RxADAS_Data[1] = r;
        RxADAS_CalcCRC16 = getcrc16c(RxADAS_CalcCRC16,r);
        RxADAS_Step = ST_FCW;
      }
      else
        RxADAS_Step = ST_STX;
      break;    
    case ST_FCW:
      if(r == '0' || r == '1' || r =='2') {
        RxADAS_Data[2] = r;
        RxADAS_CalcCRC16 = getcrc16c(RxADAS_CalcCRC16,r);
        RxADAS_Step = ST_RESERVED;
      }
      else
        RxADAS_Step = ST_STX;
      break;    
    case ST_RESERVED:
      RxADAS_CalcCRC16 = getcrc16c(RxADAS_CalcCRC16,r);
      RxADAS_RxCnt++;
      if(RxADAS_RxCnt >= 8) {
        RxADAS_RxCnt = 0;
        RxADAS_Step = ST_CRC; 
      }
      break;
    case ST_CRC:
      if(RxADAS_RxCnt == 0) {
        RxADAS_GetCRC16 = r<<8;
        RxADAS_RxCnt++;
      }
      else {
        RxADAS_GetCRC16 += r;
        RxADAS_RxCnt = 0;
        if(RxADAS_CalcCRC16 == RxADAS_GetCRC16)
          RxADAS_Step = ST_CR;
        else
          RxADAS_Step = ST_STX;
      }
      break;
    case ST_CR:
      if(r == 0x0D)
        RxADAS_Step = ST_LF;
      else
        RxADAS_Step = ST_STX;
      break;
    case ST_LF:
      if(r == 0x0A) {
        UartADAS_Enable = 1;
        UartADAS_EnableCnt = 0;
        UartADAS_LDWS = RxADAS_Data[1];
        UartADAS_FCWS = RxADAS_Data[2];
        RxADAS_Step = ST_STX;
      }
      else 
        RxADAS_Step = ST_STX;
      break;
  }
}

/*
********************************************************************************
* Description : CheckUartADAS
* Arguments   : none
* Return      : none 
* Note        : adas process
******************************************************************************** 
*/
void CheckUartADASEnable(void) 
{
  if(UartADAS_EnableCnt > 3) {
    UartADAS_Enable = 0;
    UartADAS_LDWS = 0;
    UartADAS_FCWS = 0;
  }
  UartADAS_EnableCnt++;
}


