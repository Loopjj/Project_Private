/*
*************************************** (C) COPYRIGHT 2016 LOOP *************************************
*
* File Name          : temp_ut1.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 08/04/2016
*
* Description        : temperature sensor protocol
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define UT1PROTOCOL_GLOBALS
#include "includes.h"

typedef struct {
    u8 T1Sign;
    u8 T1Val[3];
    u8 T2Sign;
    u8 T2Val[3];
}stTempProto;

typedef enum {
  ST_UT1_START,
  ST_UT1_T1SIGN,
  ST_UT1_T1VAL,
  ST_UT1_T2SIGN,
  ST_UT1_T2VAL,
  ST_UT1_END  
}stTempStep;


#define TEMP_CH1       0
#define TEMP_CH2       1

#define TEMP_STX       0x3E
#define TEMP_ETX       0x3C

#define TEMP_NOTUSED   0x58      // "XXXX"
#define TEMP_SHORT     0x53      // "SSSS"
#define TEMP_OPEN      0x4F      // "OOOO"

stTempProto TempPoto;

u8 UT1Step, Tcnt = 0;
u8 TempEnableCount;
/*
********************************************************************************
* Description : Temp_Process
* Arguments   : none
* Return      : none 
* Note        : Temp process
******************************************************************************** 
*/
void Temp_Process(void) 
{
  // Temp ch1 process
  if(TempPoto.T1Sign == TEMP_NOTUSED) {    // Temp ch1 not used
    TempStatus.Ch1Enable = 0;
    TempStatus.Ch1Open = 0;
    TempStatus.Ch1Short = 0;
    Temp_dVal[TEMP_CH1] = 0;
  }
  else if(TempPoto.T1Sign == TEMP_SHORT) { // Temp ch1 short
    TempStatus.Ch1Enable = 0;
    TempStatus.Ch1Short = 1;
    Temp_dVal[TEMP_CH1] = 0;
  }
  else if(TempPoto.T1Sign == TEMP_OPEN) {  // Temp ch1 open
    TempStatus.Ch1Enable = 0;
    TempStatus.Ch1Open = 1;
    Temp_dVal[TEMP_CH1] = 0;
  }
  else {                              // Temp ch1 value
    if(TempPoto.T1Sign <= 1) {
      Temp_dVal[TEMP_CH1] =  TempPoto.T1Val[0]*100;
      Temp_dVal[TEMP_CH1] += TempPoto.T1Val[1]*10;
      Temp_dVal[TEMP_CH1] += TempPoto.T1Val[2];
      if(TempPoto.T1Sign)
        Temp_dVal[TEMP_CH1] *= -1;
      TempStatus.Ch1Enable = 1;
      TempStatus.Ch1Open = 0;
      TempStatus.Ch1Short = 0;
    }
    else {
      TempStatus.Ch1Enable = 0;
      TempStatus.Ch1Open = 1;
      Temp_dVal[TEMP_CH1] = 0;
    }
  }
  // Temp ch2 process
  if(TempPoto.T2Sign == TEMP_NOTUSED) {    // Temp ch2 not used
    TempStatus.Ch2Enable = 0;
    TempStatus.Ch2Short = 0;
    TempStatus.Ch2Open = 0;
    Temp_dVal[TEMP_CH2] = 0;
  }
  else if(TempPoto.T2Sign == TEMP_SHORT) { // Temp ch2 start
    TempStatus.Ch2Enable = 0;
    TempStatus.Ch2Short = 1;
    Temp_dVal[TEMP_CH2] = 0;    
  }
  else if(TempPoto.T2Sign == TEMP_OPEN) {  // Temp ch2 open
    TempStatus.Ch2Enable = 0;
    TempStatus.Ch2Open = 1;
    Temp_dVal[TEMP_CH2] = 0;    
  }
  else {                              // Temp ch2 value
    if(TempPoto.T2Sign <= 1) {
      Temp_dVal[TEMP_CH2] =  TempPoto.T2Val[0]*100;
      Temp_dVal[TEMP_CH2] += TempPoto.T2Val[1]*10;
      Temp_dVal[TEMP_CH2] += TempPoto.T2Val[2];
      if(TempPoto.T2Sign) 
        Temp_dVal[TEMP_CH2] *= -1;
      TempStatus.Ch2Enable = 1;
      TempStatus.Ch2Short = 0;
      TempStatus.Ch2Open = 0;
    }
    else {
      TempStatus.Ch2Enable = 0;
      TempStatus.Ch2Open = 1;
      Temp_dVal[TEMP_CH2] = 0;    
    }
  }
  TempStatus.TempEnable = 1;
  TempEnableCount = 0;
}

/*
********************************************************************************
* Description : UT1_parse_proc
* Arguments   : rx r
* Return      : 
* Note        : UT1 parsing process
******************************************************************************** 
*/

void UT1_parse_proc(unsigned char r)	
{
  if(UT1Step > ST_UT1_START && UT1Step < ST_UT1_END ) {
    if((r >= '0' && r <= '9') || r <= 0x09 || r == TEMP_NOTUSED || r == TEMP_SHORT  || r == TEMP_OPEN) {
      if(r >= '0' && r <= '9') r = r-'0';
    }
    else 
      UT1Step = ST_UT1_START;
  }
  switch(UT1Step) {
    case ST_UT1_START:
      if(r == TEMP_STX)
        UT1Step = ST_UT1_T1SIGN;
      else UT1Step = ST_UT1_START;
      break;
    case ST_UT1_T1SIGN:
      TempPoto.T1Sign = r;
      UT1Step = ST_UT1_T1VAL;
      break;
    case ST_UT1_T1VAL:
      TempPoto.T1Val[Tcnt++] = r;
      if(Tcnt >= 3) {
        Tcnt = 0;
        UT1Step = ST_UT1_T2SIGN;
      }
      break;
    case ST_UT1_T2SIGN:
      TempPoto.T2Sign = r;
      UT1Step = ST_UT1_T2VAL;
      break;
    case ST_UT1_T2VAL:
      TempPoto.T2Val[Tcnt++] = r;
      if(Tcnt >= 3) {
        Tcnt = 0;
        UT1Step = ST_UT1_END;
      }    
      break;
    case ST_UT1_END:
      if(r == TEMP_ETX) 
        Temp_Process();
      Tcnt = 0;
      UT1Step = ST_UT1_START;
      break;
  }
}

/*
********************************************************************************
* Description : CheckTemperature
* Arguments   : none
* Return      : none 
* Note        : Temp process
******************************************************************************** 
*/
void CheckTemperatureEnable(void) 
{
  if(TempEnableCount > 3) {
    TempStatus.TempEnable = 0; 
    TempStatus.Ch1Enable = 0;
    TempStatus.Ch2Enable = 0;
  }
  TempEnableCount++;  
}

/*
********************* (C) COPYRIGHT 2016 LOOP ***************END OF FILE*****************************
*/
