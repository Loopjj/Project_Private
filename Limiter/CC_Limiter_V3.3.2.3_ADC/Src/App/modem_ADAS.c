/*
*************************************** (C) COPYRIGHT 2019 Loop *************************************
*
* File Name          : modem_ADAS.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 02/19/2019
*
* Description        : ADAS transmit header
*
*********************************************************************************************************
*/
#define MODEM_ADAS_GLOBALS

#include "includes.h"
#include "modem_ADAS.h"
#include "modem_DMS.h"
#include "crypt.h"

typedef enum {
  ON_ADAS_CONNECT,
  ON_ADAS_SEND,
  ON_ADAS_RECIEVE_WAIT,
  ON_ADAS_CLOSE,
  ON_ADAS_FAIL,
  ON_ADAS_SUCCESS,
  ON_ADAS_IDLE
}teON_ADAS_STATUS;

typedef enum {
  SEND_ADAS_CONNECT,
  SEND_ADAS_SEND,
  SEND_ADAS_RECIEVE_WAIT,
  SEND_ADAS_CLOSE,
  SEND_ADAS_FAIL,
  SEND_ADAS_SUCCESS,
  SEND_ADAS_IDLE
}teSEND_ADAS_STATUS;

typedef enum {
  EVENT_ADAS_CONNECT,
  EVENT_ADAS_SEND,
  EVENT_ADAS_RECIEVE_WAIT,
  EVENT_ADAS_CLOSE,
  EVENT_ADAS_FAIL,
  EVENT_ADAS_SUCCESS,
  EVENT_ADAS_IDLE
}teEVENT_ADAS_STATUS;

typedef enum {
  OFF_ADAS_CONNECT,
  OFF_ADAS_SEND,
  OFF_ADAS_RECIEVE_WAIT,
  OFF_ADAS_CLOSE,
  OFF_ADAS_FAIL,
  OFF_ADAS_SUCCESS,
  OFF_ADAS_IDLE
}teOFF_ADAS_STATUS;

typedef enum {
  ADAS_ON,
  ADAS_EVENT,
  ADAS_SEND,
  ADAS_OFF
}teADAS_OPCODE;

typedef enum {
  ST_ADAS_STX,
  ST_ADAS_OPCODE1,
  ST_ADAS_OPCODE2,
  ST_ADAS_DATA,  
  ST_ADAS_ETX
}teADAS_Step;

u8 AdasResultCode;
u8 AdasSendingTime;

char ADAS_TxMsg[256];
char ADAS_RxMsg[128];

unsigned char AdasRxStep;
unsigned int AdasRxCnt;

/* Key Initialization vector */
uint8_t key_vectors[] = {0x2b, 0x7e, 0x15, 0x16,
                         0x28, 0xae, 0xd2, 0xa6,
                         0xab, 0xf7, 0x15, 0x88,
                         0x09, 0xcf, 0x4f, 0x3c};

/*  Initialization vector 
 * Note: AES Standard FIPS SP800-38A provides detailed explanation 
 * on generation of init_vector for different CFB modes
 */
#if (AES_CBC == true) | (AES_CFB == true) | (AES_OFB == true)

	uint8_t init_vector[] = {0x00, 0x01, 0x02, 0x03,
                           0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0a, 0x0b,
                           0x0c, 0x0d, 0x0e, 0x0f};
#endif
  
/* Input plain text data that are to be encrypted */
uint8_t pText[64] = {"Input_Text_blck1Input_Text_blck2Input_Text_blck3Input_Text_blck4"};
/* array to store the encrypted message */
uint8_t cText[64];
/* array to store the decrypted message */
uint8_t pText1[64]; 

/*
********************************************************************************
* Description : ADAS_Trip_Count
* Arguments   : none
* Return      : none
* Note        : Trip Count
******************************************************************************** 
*/
void ADAS_Trip_Count(void)
{
  ADASPckNo.AdasTripNo++;
  if(ADASPckNo.AdasTripNo > 999) 
    ADASPckNo.AdasTripNo = 0;
  FRAMMultiWrite((u8 *)&ADASPckNo, ADAS_HEADER, sizeof(tsADASPckNo));   
}

/*
********************************************************************************
* Description : Modem_AdasSend
* Arguments   : len, sw
* Return      : checksum
* Note        : Modem TCP socket open
******************************************************************************** 
*/
u8 ADAS_Checksum(char *data, char length)
{
  int i;
  int checksum = 0;
  u8 rc;
  
  for (i = 0; i < length; i++) {
    checksum += data[i];
  }
  checksum = checksum * (-1);
  rc = (u8)(checksum & 0xFF);
  
  return rc;
}

/*
********************************************************************************
* Description : ADAS_Rx_Process
* Arguments   : r
* Return      : none
* Note        : Adas Server rx process
******************************************************************************** 
*/
u8 checks;
void ADAS_Rx_Process(u8 r)
{
  u32 addr;
  char sendtime[3], checksum[3];
  
  switch(AdasRxStep) 
  {
  case ST_ADAS_STX:
    if(r == '[') {
      AdasRxCnt = 0;
      AdasRxStep = ST_ADAS_OPCODE1;
    }
    else AdasRxStep = ST_ADAS_STX;
    break;
  case ST_ADAS_OPCODE1:
    if(r == '1' || r == '2'){
      ADAS_RxMsg[AdasRxCnt++] = r;
      AdasRxStep = ST_ADAS_OPCODE2;
    }
    else
      AdasRxStep = ST_ADAS_STX;
    break;
  case ST_ADAS_OPCODE2:
    if(r == '1' || r == '2' || r == '3' || r == '5') {
      ADAS_RxMsg[AdasRxCnt++] = r;
      AdasRxStep = ST_ADAS_DATA;
    }
    else
      AdasRxStep = ST_ADAS_STX;
    break;
  case ST_ADAS_DATA:
    if(r != ']') {
      if(AdasRxCnt < 128)
        ADAS_RxMsg[AdasRxCnt++] = r;
      else 
        AdasRxStep = ST_ADAS_STX;
    }
    else {
      checks = ADAS_Checksum((char*)&ADAS_RxMsg, 28);
      sprintf(checksum, "%02X", checks); 
      if(checksum[0] == ADAS_RxMsg[28] && checksum[1] == ADAS_RxMsg[29]) {
        if(ADAS_RxMsg[0] == '1' && ADAS_RxMsg[1] == '1') {            // 주기 데이터
          SendAdasStep = SEND_ADAS_CLOSE;
          SendAdasWaitCount = 0;
        } 
        else if(ADAS_RxMsg[0] == '1' && ADAS_RxMsg[1] == '2') {       // 시동 On
          OnAdasStep = ON_ADAS_CLOSE;
          OnAdasWaitCount = 0;
        }
        else if(ADAS_RxMsg[0] == '1' && ADAS_RxMsg[1] == '3') {       // 시동 Off
          OffAdasStep = OFF_ADAS_CLOSE;
          OffAdasWaitCount = 0;
        }     
        else if(ADAS_RxMsg[0] == '2' && ADAS_RxMsg[1] == '5') {       // 이벤트
          EventAdasStep = EVENT_ADAS_CLOSE;
          EventAdasWaitCount = 0;
        }      
        if(ADAS_RxMsg[25] == 'S')     // Success
          AdasResultCode = 1;          
        else AdasResultCode = 0;      // Fail
        
        sendtime[0] = ADAS_RxMsg[26];
        sendtime[1] = ADAS_RxMsg[27];
        sendtime[2] = 0;
        AdasSendingTime = (char)(atoi(sendtime));
        if(AdasSendingTime != ModemInfo.AdasInterval) {
          if(AdasSendingTime > 0 && AdasSendingTime <= 60) {
            ModemInfo.AdasInterval = AdasSendingTime;
            addr = MODEM_INFO + (u8 *)&ModemInfo.AdasInterval - (u8 *)&ModemInfo;
            FRAMMultiWrite((u8 *)&ModemInfo.AdasInterval, addr, sizeof(ModemInfo.AdasInterval));
          }
        }        
      }
      AdasRxStep = ST_ADAS_STX;
    }
    break;
  }
}

/*
********************************************************************************
* Description : GPS_Encrypt
* Arguments   : data
* Return      : none
* Note        : GPS Encrytion for Lat, Lon
******************************************************************************** 
*/
u32 GPS_Encrypt_Lon(u32 lon)
{
  char tmp[20];
  u32 rc;
  
  memset(tmp, 0, 20);                   // GPS 위도
  sprintf(tmp,"%010u", lon);
  rc =  (tmp[0] - 0x30) * 1000000000;
  rc += (tmp[2] - 0x30) * 100000000;
  rc += (tmp[4] - 0x30) * 10000000;
  rc += (tmp[6] - 0x30) * 1000000;
  rc += (tmp[8] - 0x30) * 100000;
  rc += (tmp[1] - 0x30) * 10000;
  rc += (tmp[3] - 0x30) * 1000;
  rc += (tmp[5] - 0x30) * 100;
  rc += (tmp[7] - 0x30) * 10;
  rc += (tmp[9] - 0x30) * 1;

  return rc;
}

/*
********************************************************************************
* Description : Modem_AdasSend
* Arguments   : len, sw
* Return      : resp
* Note        : Modem TCP socket open
******************************************************************************** 
*/
u32 GPS_Encrypt_Lat(u32 lat)
{
  char tmp[20];
  u32 rc;
  
  memset(tmp, 0, 20);                   // GPS 경도
  sprintf(tmp,"%09u", lat);
  rc =  (tmp[0] - 0x30) * 100000000;
  rc += (tmp[2] - 0x30) * 10000000;
  rc += (tmp[4] - 0x30) * 1000000;
  rc += (tmp[6] - 0x30) * 100000;
  rc += (tmp[8] - 0x30) * 10000;
  rc += (tmp[1] - 0x30) * 1000;
  rc += (tmp[3] - 0x30) * 100;
  rc += (tmp[5] - 0x30) * 10;
  rc += (tmp[7] - 0x30) * 1;

  return rc;
}

/*
********************************************************************************
* Description : Modem_AdasSend
* Arguments   : len, sw
* Return      : resp
* Note        : Modem TCP socket open
******************************************************************************** 
*/
unsigned char Modem_ADASSend(int op)
{
  char tmp[20];
  u16 val, i;
  rtc_time time;
  u8 checksum;
  unsigned char resp = 0;  
  unsigned int mlen = 0;
  u32 trip, tmplat, tmplon;
  
  mlen = 0;
  memset(ADAS_TxMsg, 0, 512);
  
  ADAS_TxMsg[mlen] = '[';               //STX
  mlen += 1;  
  memset(tmp, 0, 20);
  if(op == ADAS_ON)                     // 메세지코드
    sprintf(tmp,"%02u",12);             // 시동 On
  else if(op == ADAS_EVENT)
    sprintf(tmp,"%02u",25);             // ADAS 이벤트
  else if(op == ADAS_SEND)
    sprintf(tmp,"%02u",11);             // 주기전송
  else
    sprintf(tmp,"%02u",13);             // 시동 Off
  memcpy(&ADAS_TxMsg[mlen], tmp, 2);   
  mlen += 2;

  memset(tmp, 0, 20);                   // 일련번호
  sprintf(tmp,"%04u",ADASPckNo.AdasSerialNo);
  memcpy(&ADAS_TxMsg[mlen], tmp, 4);   
  mlen += 4;
  
  memset(tmp, 0, 20);                   // 트립 일련번호
  sprintf(tmp,"%03u",ADASPckNo.AdasTripNo);
  memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
  mlen += 3;
                                        // 단말기 ID
  memcpy(&ADAS_TxMsg[mlen], ModemCTN, 11);   
  mlen += 11;
  
  memset(tmp, 0, 20);                   // 오류코드
  sprintf(tmp,"%02u", 0);
  memcpy(&ADAS_TxMsg[mlen], tmp, 2);   
  mlen += 2;  

  memset(tmp, 0, 20);  
  if(op == ADAS_ON)                     // 데이터 길이
    sprintf(tmp,"%03u", 53);
  else if(op == ADAS_EVENT)
    sprintf(tmp,"%03u", 1);    
  else if(op == ADAS_SEND)
    sprintf(tmp,"%03u", 59);    
  else
    sprintf(tmp,"%03u", 142);
  memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
  mlen += 3; 
  
  memset(tmp, 0, 20);
  time = gmtime_conv(SecData.Time);
  sprintf(tmp,"%04u%02u%02u%02u%02u%02u", (2000 + time.tm_year), time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
  memcpy(&ADAS_TxMsg[mlen], tmp, 14);
  mlen += 14;
  
  if(!SecData.Status.GPS_Fault)
    ADAS_TxMsg[mlen] = rmc.warn;          // GPS 상태
  else  ADAS_TxMsg[mlen] = 'V'; 
  mlen += 1;  
  
  memset(tmp, 0, 20);                     // GPS 위도
  if(ADASPckData.AdasGPSLat > 1000000 && ADASPckData.AdasGPSLat < 9000000) {
    if(time.tm_min)
      tmplat = ADASPckData.AdasGPSLat * time.tm_min;
    else 
      tmplat = ADASPckData.AdasGPSLat * 60;
//    tmplat = GPS_Encrypt_Lat(ADASPckData.AdasGPSLat);
    sprintf(&tmp[10],"%09u", tmplat);
    for(i = 0; i < 5; i++) {
      tmp[i] = tmp[10+2*i];
      tmp[i+5] = tmp[11+2*i];
    }
  }
  else 
    sprintf(tmp,"000000000");
  memcpy(&ADAS_TxMsg[mlen], tmp, 9);
  mlen += 9;

  memset(tmp, 0, 20);                   // GPS 경도
  if(ADASPckData.AdasGPSLon > 10000000 && ADASPckData.AdasGPSLon < 18000000) {
    if(time.tm_min)
      tmplon = ADASPckData.AdasGPSLon * time.tm_min;
    else 
      tmplon = ADASPckData.AdasGPSLon * 60;
//    tmplon = GPS_Encrypt_Lon(ADASPckData.AdasGPSLon);
    sprintf(&tmp[10],"%010u", tmplon);
    for(i = 0; i < 5; i++) {
      tmp[i] = tmp[10+2*i];
      tmp[i+5] = tmp[11+2*i];
    }
  }
  else
    sprintf(tmp,"0000000000");
  memcpy(&ADAS_TxMsg[mlen], tmp, 10);
  mlen += 10;  
    
  memset(tmp, 0, 20);                   // GPS 속도
  if(!SecData.Status.GPS_Fault) {
    val = SecData.GPS_Speed;
    if(val > 255) val = 255;
  }
  else val = 0;
  sprintf(tmp,"%03u", val);
  memcpy(&ADAS_TxMsg[mlen], tmp, 3);
  mlen += 3;
  
  if(!SecData.Status.GPS_Fault) {       // GPS 방위각
    val = SecData.Azi;
    if(val == 0) val = 360;
  }
  else val = 0;
  sprintf(tmp,"%03u", val);  
  memcpy((u8*)&ADAS_TxMsg[mlen], tmp, 3);
  mlen += 3;
  
  if(op == ADAS_ON) {
    memset(tmp,0x20,20);                  // 단말기 시리얼 번호
    memcpy(tmp, (char *)Setting.Serial_No, sizeof(Setting.Serial_No));
    memcpy(&ADAS_TxMsg[mlen], tmp, 20);
    mlen += 20;
    memset(tmp,0x20,20);                  // 단말기  버젼
    sprintf(tmp,"V%1u.%1u%1u%1u", MAJOR, MINOR, RELEASE, BUILD);
    memcpy(&ADAS_TxMsg[mlen], tmp, 6);    
    mlen += 6;
    memset(tmp,0x20,20);                  // OBD 버젼
    memcpy(&ADAS_TxMsg[mlen], tmp, 6);    
    mlen += 6;
    memset(tmp, 0, 20);
    sprintf(tmp,"%08u%04u", 0, 0);      
    memcpy(&ADAS_TxMsg[mlen], tmp, 12);   // OBD 버젼, OBD 부착일시
    mlen += 12;
    memset(tmp, 0, 20);                   // 누적 주행거리
    sprintf(tmp,"%07u", 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 7);   
    mlen += 7;
    memset(tmp, 0, 20);                   // 오류코드 갯수
    sprintf(tmp,"%02u", 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 2);   
    mlen += 2;    
    checksum = ADAS_Checksum(&ADAS_TxMsg[1], 118);
  }
  else if(op == ADAS_EVENT) {
    ADAS_TxMsg[mlen] = ADASPckData.EventCode;
    mlen += 1;
    checksum = ADAS_Checksum(&ADAS_TxMsg[1], 66);    
  }
  else if(op == ADAS_SEND) {
    memset(tmp, 0, 20);                   // 현재 RPM / 최고 RPM
    sprintf(tmp,"%04u%04u",0, 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 8);   
    mlen += 8;
    memset(tmp, 0, 20);                   // 현재속도
    sprintf(tmp,"%03u", SecData.Speed[0]);
    memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
    mlen += 3;     
    memset(tmp, 0, 20);                   // 평균속도
    sprintf(tmp,"%03u", ADASPckData.TripAvgSpeed);
    memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
    mlen += 3;     
    memset(tmp, 0, 20);                   // 최고속도
    sprintf(tmp,"%03u", ADASPckData.TripMaxSpeed);
    memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
    mlen += 3;     
    memset(tmp, 0, 20);                   // 현재냉각수온도, 최고냉각수온도, 115도 이상시간
    sprintf(tmp,"%03u%03u%04u", 0, 0, 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 10);   
    mlen += 10;
    memset(tmp, 0, 20);                   // 현재엔진오일온도, 최고엔진오일온도, 130도 이상시간
    sprintf(tmp,"%03u%03u%04u", 0, 0, 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 10);   
    mlen += 10;
    memset(tmp, 0, 20);                   // 현재전압, 11V 이하시간, 15V 이상시간, 최고전압, 최저전압,
    sprintf(tmp,"%03u%04u%04u%03u%03u,", 0, 0, 0, 0, 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 17);   
    mlen += 17;
    memset(tmp, 0, 20);                   // TPS
    sprintf(tmp,"%03u", 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
    mlen += 3;
    memset(tmp, 0, 20);                   // 경고상태
    sprintf(tmp,"%01u", 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 1);   
    mlen += 1;
    memset(tmp, 0, 20);                   // 시동상태
    sprintf(tmp,"%01u", SecData.Signal.KeyOn);
    memcpy(&ADAS_TxMsg[mlen], tmp, 1);   
    mlen += 1;
    checksum = ADAS_Checksum(&ADAS_TxMsg[1], 124);    
  }
  else {
    trip = (u32)(SecData.Trip*1000.0);
    memset(tmp, 0, 20);                   // 누적운행거리(단위 m)
    sprintf(tmp,"%07u", trip);
    memcpy(&ADAS_TxMsg[mlen], tmp, 7);   
    mlen += 7;    
    memset(tmp, 0, 20);                   // 속도구간별 시간 1
    sprintf(tmp,"%05u", ADASPckData.Speed40TIme);
    memcpy(&ADAS_TxMsg[mlen], tmp, 5);   
    mlen += 5;    
    memset(tmp, 0, 20);                   // 속도구간별 시간 2
    sprintf(tmp,"%05u", ADASPckData.Speed4060TIme);
    memcpy(&ADAS_TxMsg[mlen], tmp, 5);   
    mlen += 5;    
    memset(tmp, 0, 20);                   // 속도구간별 시간 3
    sprintf(tmp,"%05u", ADASPckData.Speed6080TIme);
    memcpy(&ADAS_TxMsg[mlen], tmp, 5);   
    mlen += 5;    
    memset(tmp, 0, 20);                   // 속도구간별 시간 4
    sprintf(tmp,"%05u", ADASPckData.Speed80100TIme);
    memcpy(&ADAS_TxMsg[mlen], tmp, 5);   
    mlen += 5;    
    memset(tmp, 0, 20);                   // 속도구간별 시간 5
    sprintf(tmp,"%05u", ADASPckData.Speed100TIme);
    memcpy(&ADAS_TxMsg[mlen], tmp, 5);   
    mlen += 5;    
    memset(tmp, 0, 20);                   // 급가속횟수, 급감속횟수
    sprintf(tmp,"%03u%03u", ADASPckData.OverSpeedCnt, ADASPckData.UnderSpeedCnt);
    memcpy(&ADAS_TxMsg[mlen], tmp, 6);   
    mlen += 6;
    memset(tmp, 0, 20);                   // 공회전 시간
    sprintf(tmp,"%03u", 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
    mlen += 3;    
    memset(tmp, 0, 20);                   // 과속시간
    sprintf(tmp,"%05u", ADASPckData.OverSpeedTIme);
    memcpy(&ADAS_TxMsg[mlen], tmp, 5);   
    mlen += 5;    
    memset(tmp, 0, 20);                   // Green,Yellow, Red Zone 디젤
    sprintf(tmp,"%05u%05u%05u", 0, 0, 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 15);   
    mlen += 15;    
    memset(tmp, 0, 20);                   // Green, Yellow, Red Zone 가솔린
    sprintf(tmp,"%05u%05u%05u", 0, 0, 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 15);   
    mlen += 15;    
    memset(tmp, 0, 20);                   // 연료분사량 1, 연류분사량 2, 연료분사량 3
    sprintf(tmp,"%08u%08u%08u", 0, 0, 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 24);   
    mlen += 24;       
    memset(tmp, 0, 20);                   // ISG 시간
    sprintf(tmp,"%04u", 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 4);   
    mlen += 4;
    memset(tmp, 0, 20);                   // 평균속도
    sprintf(tmp,"%03u", ADASPckData.TripAvgSpeed);
    memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
    mlen += 3; 
    memset(tmp, 0, 20);                   // 평균 RPM
    sprintf(tmp,"%04u", 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 4);
    mlen += 4; 
    memset(tmp, 0, 20);                   // MAF, MAP, ATS 센서
    sprintf(tmp,"%01u%01u%01u", 0, 0, 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
    mlen += 3; 
    memset(tmp, 0, 20);                   // 최고 속도
    sprintf(tmp,"%03u", ADASPckData.TripMaxSpeed);
    memcpy(&ADAS_TxMsg[mlen], tmp, 3);   
    mlen += 3; 
    memset(tmp, 0, 20);                   // 최고 RPM
    sprintf(tmp,"%04u", 0);
    memcpy(&ADAS_TxMsg[mlen], tmp, 4);   
    mlen += 4; 
    memset(tmp, 0, 20);                   // FCW On 회수, Right LDW On 횟수, Left LDW On 횟수
    sprintf(tmp,"%03u%03u%03u", ADASEventCount.FCWOn, ADASEventCount.RightLDWOn, ADASEventCount.LeftLDWOn);
    memcpy(&ADAS_TxMsg[mlen], tmp, 9);   
    mlen += 9; 
    memset(tmp, 0, 20);                   // 보행자 전방추돌, 진행중 차간거리, 도심지 전방추돌, 제한속도 초과
    sprintf(tmp,"%03u%03u%03u%03u", ADASEventCount.PedesFDWOn, ADASEventCount.HWL, ADASEventCount.U_FCW, ADASEventCount.SLI);
    memcpy(&ADAS_TxMsg[mlen], tmp, 12);   
    mlen += 12; 
    checksum = ADAS_Checksum(&ADAS_TxMsg[1], 207);
  }
  memset(tmp, 0, 20);  
  sprintf(tmp,"%02X", checksum);          // Checksum
  memcpy(&ADAS_TxMsg[mlen], tmp, 2);
  mlen += 2;
  ADAS_TxMsg[mlen] = ']';                 // ETX
  mlen += 1;
  
  resp = 0;
  if(mlen > 0) {
    if(op == ADAS_ON || op == ADAS_SEND || op == ADAS_EVENT || op == ADAS_OFF)  {
      ModemCommInit();
      Urc.Mode = URCMODE_ADAS;
      Urc.RespFlag = 1;
      Urc.Cnt = 0;
      memset(Urc.Buff,0, URC_BUFF_MAX);
    }
    else {
      Urc.Mode = URCMODE_IDLE;
      Urc.RespFlag = 0;
      Urc.Cnt = 0;
    }
    
    TxDataStrToModem((u8*)ADAS_TxMsg, mlen);
    resp = 1;

    ADASPckNo.AdasSerialNo++;
    if(ADASPckNo.AdasSerialNo > 9999) 
      ADASPckNo.AdasSerialNo = 0;
    FRAMMultiWrite((u8 *)&ADASPckNo, ADAS_HEADER, sizeof(tsADASPckNo));    
  }
  
  return resp;
}

/*
********************************************************************************
* Description : Modem_OnAdasProcess
* Arguments   : none
* Return      : 
* Note        : Key On Adas Process
******************************************************************************** 
*/
void Modem_OnAdasProcess(void)
{
  if(!OnAdasFlag || Setting.ExtModemADAS != ADAS_DTG_MODE) {
    OnAdasStep = ON_ADAS_IDLE;
    return;
  }
  switch(OnAdasStep) {
    case ON_ADAS_CONNECT:
      Modem_SocketConnect(AdasSvr.SvrName, AdasSvr.SvrPort, 7);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          OnAdasStep = ON_ADAS_SEND;
        }
        else
          OnAdasStep = ON_ADAS_FAIL;
      }
      break;
      
    case ON_ADAS_SEND:
      if(Modem_ADASSend(ADAS_ON) == 1) {
        OnAdasStep = ON_ADAS_RECIEVE_WAIT;
        OnAdasWaitCount = 0;
        OnAdasRespFlag = 1;
      }
      else {
        if(Modem_SocketClose(0) == 1)
          OnAdasStep = ON_ADAS_CONNECT;
        else {
          OnAdasStep = ON_ADAS_FAIL;
        }
      }
      break;
      
    case ON_ADAS_RECIEVE_WAIT:
      OnAdasWaitCount++;
      if(OnAdasWaitCount > 10000 ) {  // 10초
        OnAdasWaitCount = 0;
        Modem_SocketClose(0);
        OnAdasStep = ON_ADAS_FAIL;
      }
      break;
      
    case ON_ADAS_CLOSE:
      if(Modem_SocketClose(0) == 1) {
        if(AdasResultCode)
          OnAdasStep = ON_ADAS_SUCCESS;
        else 
          OnAdasStep = ON_ADAS_FAIL;
      }
      else
        OnAdasStep = ON_ADAS_FAIL;
      break;

    case ON_ADAS_FAIL:
      printk("\r\nMODEM : On-Adas send error\r\n");
      ModemWaitCnt = 0;
      //OnAdasRetryCount++;
      //if(OnAdasRetryCount >= 2) {
      //  OnAdasRetryCount = 0;
        Modem_PPPClose();
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON;
      //}
      //else {
      //  ModemWaitCnt = 0;
      //  ModemStep = MODEM_IDLE;//MODEM_POWER_ON_IDLE;
      //}
      break;

    case ON_ADAS_SUCCESS:
      printk("\r\nMODEM : On-Adas send Success\r\n");
      ModemWaitCnt = 0;
      OnAdasFlag = 0;
      TripAdasFlag = 0;
      ModemStep = MODEM_DATA_IDLE;;
      Modem_PPPClose();
      break;

    case ON_ADAS_IDLE:
      ModemWaitCnt = 0;
      ModemStep = MODEM_DATA_IDLE;
      break;
      
    default : 
      OnAdasStep = ON_ADAS_FAIL;
      break;  
  }  
}

/*
********************************************************************************
* Description : Modem_SendAdasProcess
* Arguments   : none
* Return      : 
* Note        : Send Adas Process
******************************************************************************** 
*/
void Modem_SendAdasProcess(void)
{
  if(!SendAdasFlag || Setting.ExtModemADAS != ADAS_DTG_MODE) {
    SendAdasStep = SEND_ADAS_IDLE;
    return;
  }
  switch(SendAdasStep) {
    case SEND_ADAS_CONNECT:
      Modem_SocketConnect(AdasSvr.SvrName, AdasSvr.SvrPort, 7);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          SendAdasStep = SEND_ADAS_SEND;
        }
        else
          SendAdasStep = SEND_ADAS_FAIL;
      }
      break;
      
    case SEND_ADAS_SEND:
      if(Modem_ADASSend(ADAS_SEND) == 1) {
        SendAdasStep = SEND_ADAS_RECIEVE_WAIT;
        SendAdasWaitCount = 0;
        SendAdasRespFlag = 1;
      }
      else {
        if(Modem_SocketClose(0) == 1)
          SendAdasStep = SEND_ADAS_CONNECT;
        else {
          SendAdasStep = SEND_ADAS_FAIL;
        }
      }
      break;
      
    case SEND_ADAS_RECIEVE_WAIT:
      SendAdasWaitCount++;
      if(SendAdasWaitCount > 10000 ) {  // 10초
        SendAdasWaitCount = 0;
        Modem_SocketClose(0);
        SendAdasStep = SEND_ADAS_FAIL;
      }
      break;
      
    case SEND_ADAS_CLOSE:
      if(Modem_SocketClose(0) == 1) {
        if(AdasResultCode)
          SendAdasStep = SEND_ADAS_SUCCESS;
        else 
          SendAdasStep = SEND_ADAS_FAIL;
      }
      else
        SendAdasStep = SEND_ADAS_FAIL;
      break;

    case SEND_ADAS_FAIL:
      printk("\r\nMODEM : Send-Adas send error\r\n");
      ModemWaitCnt = 0;
      SendAdasRetryCount++;
      if(SendAdasRetryCount >= 2) {
        SendAdasRetryCount = 0;
        Modem_PPPClose();
        ModemStep = MODEM_IDLE;//MODEM_POWER_OFF;
        if(SendAdasFlag == 2) {
          OffAdasFlag = 1;
          OffAdasRetryCount = 0;   
        }
        SendAdasFlag = 0;
      }
      else {
        ModemWaitCnt = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON_IDLE;
      }
      break;

    case SEND_ADAS_SUCCESS:
      printk("\r\nMODEM : Send-Adas send Success\r\n");
      ModemWaitCnt = 0;
      if(SendAdasFlag == 2) {
        OffAdasFlag = 1;
        OffAdasRetryCount = 0;   
      }
      SendAdasFlag = 0;
      Modem_PPPClose();
      ModemStep = MODEM_DATA_IDLE;
      break;

    case SEND_ADAS_IDLE:
      ModemWaitCnt = 0;
      SendAdasFlag = 0;
      ModemStep = MODEM_DATA_IDLE;
      break;
      
    default : 
      SendAdasStep = SEND_ADAS_FAIL;
      break;  
  }   
}

/*
********************************************************************************
* Description : Modem_EventAdasProcess
* Arguments   : none
* Return      : 
* Note        : Event Adas Process
******************************************************************************** 
*/
void Modem_EventAdasProcess(void)
{
  if(!EventAdasFlag || Setting.ExtModemADAS != ADAS_DTG_MODE) {
    EventAdasStep = EVENT_ADAS_IDLE;
    return;
  }
  switch(EventAdasStep) {
    case EVENT_ADAS_CONNECT:
      Modem_SocketConnect(AdasSvr.SvrName, AdasSvr.SvrPort, 7);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          EventAdasStep = EVENT_ADAS_SEND;
        }
        else
          EventAdasStep = EVENT_ADAS_FAIL;
      }
      break;
      
    case EVENT_ADAS_SEND:
      if(Modem_ADASSend(ADAS_EVENT) == 1) {
        EventAdasStep = EVENT_ADAS_RECIEVE_WAIT;
        EventAdasWaitCount = 0;
        EventAdasRespFlag = 1;
      }
      else {
        if(Modem_SocketClose(0) == 1)
          EventAdasStep = EVENT_ADAS_CONNECT;
        else {
          EventAdasStep = EVENT_ADAS_FAIL;
        }
      }
      break;
      
    case EVENT_ADAS_RECIEVE_WAIT:
      EventAdasWaitCount++;
      if(EventAdasWaitCount > 10000 ) {  // 10초
        EventAdasWaitCount = 0;
        Modem_SocketClose(0);
        EventAdasStep = EVENT_ADAS_FAIL;
      }
      break;
      
    case EVENT_ADAS_CLOSE:
      if(Modem_SocketClose(0) == 1) {
        EventAdasStep = EVENT_ADAS_SUCCESS;
        if(AdasResultCode)
          EventAdasStep = EVENT_ADAS_SUCCESS;
        else 
          EventAdasStep = EVENT_ADAS_FAIL;        
      }
      else
        EventAdasStep = EVENT_ADAS_FAIL;
      break;

    case EVENT_ADAS_FAIL:
      printk("\r\nMODEM : Event-Adas send error\r\n");
      ModemWaitCnt = 0;
      EventAdasRetryCount++;
      if(EventAdasRetryCount >= 2) {
        EventAdasRetryCount = 0;
        EventAdasFlag = 0;
        EventTxComplete = 1;
        Modem_PPPClose();
        ModemStep = MODEM_IDLE;//MODEM_POWER_OFF;
      }
      else {
        ModemWaitCnt = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON_IDLE;
      }
      break;

    case EVENT_ADAS_SUCCESS:
      printk("\r\nMODEM : Event-Adas send Success\r\n");
      ModemWaitCnt = 0;
      EventAdasFlag = 0;
      EventTxComplete = 1;
      Modem_PPPClose();
      ModemStep = MODEM_DATA_IDLE;
      break;

    case EVENT_ADAS_IDLE:
      ModemWaitCnt = 0;
      EventAdasFlag = 0;
      EventTxComplete = 1;
      ModemStep = MODEM_DATA_IDLE;
      break;
      
    default : 
      EventAdasStep = EVENT_ADAS_FAIL;
      break;  
  }    
}

/*
********************************************************************************
* Description : Modem_OffAdasProcess
* Arguments   : none
* Return      : 
* Note        : Key Off Adas Process
******************************************************************************** 
*/
void Modem_OffAdasProcess(void)
{
  if(!OffAdasFlag || Setting.ExtModemADAS != ADAS_DTG_MODE) {
    OffAdasStep = OFF_ADAS_IDLE;
    return;
  }
  switch(OffAdasStep) {
    case OFF_ADAS_CONNECT:
      Modem_SocketConnect(AdasSvr.SvrName, AdasSvr.SvrPort, 7);
      if(SocketConnectStep > SOCKET_OPEN_WAIT) {
        if(SocketConnectStep == SOCKET_CONNECT) {
          OffAdasStep = OFF_ADAS_SEND;
        }
        else
          OffAdasStep = OFF_ADAS_FAIL;
      }
      break;
      
    case OFF_ADAS_SEND:
      if(Modem_ADASSend(ADAS_OFF) == 1) {
        OffAdasStep = OFF_ADAS_RECIEVE_WAIT;
        OffAdasWaitCount = 0;
        OffAdasRespFlag = 1;
      }
      else {
        if(Modem_SocketClose(0) == 1)
          OffAdasStep = OFF_ADAS_CONNECT;
        else {
          OffAdasStep = OFF_ADAS_FAIL;
        }
      }
      break;
      
    case OFF_ADAS_RECIEVE_WAIT:
      OffAdasWaitCount++;
      if(OffAdasWaitCount > 10000 ) {  // 10초
        OffAdasWaitCount = 0;
        Modem_SocketClose(0);
        OffAdasStep = OFF_ADAS_FAIL;
      }
      break;
      
    case OFF_ADAS_CLOSE:
      if(Modem_SocketClose(0) == 1) {
        if(AdasResultCode)
          OffAdasStep = OFF_ADAS_SUCCESS;
        else 
          OffAdasStep = OFF_ADAS_FAIL;           
      }
      else
        OffAdasStep = OFF_ADAS_FAIL;
      break;

    case OFF_ADAS_FAIL:
      printk("\r\nMODEM : Off-Adas send error\r\n");
      ModemWaitCnt = 0;
      OffAdasRetryCount++;
      if(OffAdasRetryCount >= 2) {
        OffAdasRetryCount = 0;
        OffAdasFlag = 0;
        Modem_PPPClose();
        ModemStep = MODEM_DATA_IDLE;
        if(TripAdasFlag)
          OnAdasFlag = 1;
        else {        
          if((DataServerFlag == 0 && VdisServerFlag == 0 && SendAdasFlag == 0 && OffAdasFlag == 0) || PushResetStart) {  
            OffNotiFlag = 1;  
            OffNotiRetryCount = 0;
          }
        }
      }
      else {
        ModemWaitCnt = 0;
        ModemStep = MODEM_IDLE;//MODEM_POWER_ON_IDLE;
      }
      break;

    case OFF_ADAS_SUCCESS:
      printk("\r\nMODEM : Off-Adas send Success\r\n");
      ModemWaitCnt = 0;
      OffAdasFlag = 0;
      Modem_PPPClose();
      ModemStep = MODEM_DATA_IDLE;
      if(TripAdasFlag)
        OnAdasFlag = 1;
      else {
        if((DataServerFlag == 0 && VdisServerFlag == 0 && SendAdasFlag == 0 && OffAdasFlag == 0) || PushResetStart) {  
          OffNotiFlag = 1;  
          OffNotiRetryCount = 0;
        }
      }
      break;

    case OFF_ADAS_IDLE:
      ModemWaitCnt = 0;
      OffAdasFlag = 0;
      ModemStep = MODEM_DATA_IDLE;
      if(TripAdasFlag)
        OnAdasFlag = 1;
      else {      
        if((DataServerFlag == 0 && VdisServerFlag == 0 && SendAdasFlag == 0 && OffAdasFlag == 0) || PushResetStart) {  
          OffNotiFlag = 1;  
          OffNotiRetryCount = 0;
        }
      }
      break;
      
    default : 
      OffAdasStep = OFF_ADAS_FAIL;
      break;  
  }  
}

/*
********************************************************************************
* Description : Sec Interrupt for ADAS
* Arguments   : none
* Return      : 
* Note        : Sec Interrupt Adas Process
******************************************************************************** 
*/
void Modem_SecInt_ADAS(void)
{
  if(!Power_Off_Flag) {
    SendPolTimeCnt++;
    if(ModemInfo.AdasInterval >= 1 && ModemInfo.AdasInterval <= 60) {
      if(SendPolTimeCnt >= ModemInfo.AdasInterval * 60) {
        SendPolTimeCnt = 0;
        SendAdasFlag = 1;  
        SendAdasRetryCount = 0;
      }
    }
  }
}

/*
********************************************************************************
* Description : Modem_Parameter_Reset_ADAS
* Arguments   : none
* Return      : 
* Note        : ADAS Parameter Reset
******************************************************************************** 
*/
void Modem_Parameter_Reset_ADAS(void)
{
  ModemInfo.AdasInterval = 0;
  FRAMMultiWrite((u8 *)&ModemInfo.AdasInterval, MODEM_INFO + (u8 *)&ModemInfo.AdasInterval - (u8 *)&ModemInfo, sizeof(ModemInfo.AdasInterval));
  memset(AdasSvr.SvrName, 0, sizeof(AdasSvr.SvrName));
#ifdef __USE_ADAS_TEST_SERVER__    
  sprintf(AdasSvr.SvrName,"14.63.248.9");         // TB ADAS 서버
#else 
  sprintf(AdasSvr.SvrName,"14.63.151.156");       // 상용 ADAS 서버    
#endif   
  AdasSvr.SvrPort = 44005;
  FRAMMultiWrite((u8 *)&AdasSvr, ADAS_SVR_INFO, sizeof(tsSvrInfo));  
}

/*
********************************************************************************
* Description : Modem_Init_ADAS
* Arguments   : none
* Return      : 
* Note        : ADAS Parameter Initialization
******************************************************************************** 
*/
void Modem_Init_ADAS(void)
{
  u32 addr;
  
  FRAMMultiRead((u8 *)&ModemInfo, MODEM_INFO, sizeof(tsModemInfo));
  
  if(ModemInfo.AdasInterval < 1 || ModemInfo.AdasInterval > 60) {
    ModemInfo.AdasInterval = 1;
    addr = MODEM_INFO + (u8 *)&ModemInfo.AdasInterval - (u8 *)&ModemInfo;
    FRAMMultiWrite((u8 *)&ModemInfo.AdasInterval, addr, sizeof(ModemInfo.AdasInterval));          
  }

  FRAMMultiRead((u8 *)&AdasSvr, ADAS_SVR_INFO, sizeof(tsSvrInfo));
  if(AdasSvr.SvrName[0] < ' ' || AdasSvr.SvrName[0] > 'z' || AdasSvr.SvrPort == 0 || AdasSvr.SvrPort > 99999) {
    memset(AdasSvr.SvrName, 0, sizeof(AdasSvr.SvrName));
#ifdef __USE_ADAS_TEST_SERVER__    
    sprintf(AdasSvr.SvrName,"14.63.248.9");         // TB ADAS 서버
#else 
    sprintf(AdasSvr.SvrName,"14.63.151.156");       // 상용 ADAS 서버    
#endif    
    AdasSvr.SvrPort = 44005;
    FRAMMultiWrite((u8 *)&AdasSvr, ADAS_SVR_INFO, sizeof(tsSvrInfo));
  }
  
  FRAMMultiRead((u8 *)&ADASPckNo, ADAS_HEADER, sizeof(tsADASPckNo));
  
  OnAdasFlag = 1;
  TripAdasFlag = 1;
  OnAdasRetryCount = 0;
  SendAdasRetryCount = 0;
  EventAdasRetryCount = 0;
  OffAdasRetryCount = 0;
  SendPolTimeCnt = 0;
  
	/* Generate key schedule for AES-128 from the Cipher Key */
	aes_init(key_vectors);
	ecb_encrypt( pText, cText, sizeof(pText) );

  memset(&ADASEventCount, 0, sizeof(tsADASEventCount));
  memset(&ADASPckData, 0, sizeof(tsADASPckData));
}

/*
********************************************************************************
* Description : Modem_DataIdleProcess_ADAS
* Arguments   : none
* Return      : 
* Note        : Modem Adas Data Process
******************************************************************************** 
*/
void Modem_DataIdleProcess_ADAS(void)
{
  if(OnAdasFlag) {
    OnAdasStep = ON_ADAS_CONNECT;
    OnAdasWaitCount = 0;
    ModemStep = MODEM_ON_ADAS;    
  }
  else if(!OnAdasFlag && EventAdasFlag) {
    EventAdasStep = EVENT_ADAS_CONNECT;
    EventAdasWaitCount = 0;
    ModemStep = MODEM_EVENT_ADAS;    
  }
  else if(!OnAdasFlag && SendAdasFlag) {
    SendAdasStep = SEND_ADAS_CONNECT;
    SendAdasWaitCount = 0;
    ModemStep = MODEM_SEND_ADAS;    
  }
  else if(!OnAdasFlag && OffAdasFlag) {
    OffAdasStep = OFF_ADAS_CONNECT;
    OffAdasWaitCount = 0;
    ModemStep = MODEM_OFF_ADAS;    
  }
}