/**
  ******************************************************************************
  * @file    cdma.c 
  * @author  LOOP
  * @version V1.0
  * @date    09/03/2012
  * @brief   
  ******************************************************************************
  * @copy
  *
  * <h2><center>&copy; COPYRIGHT 2012 LOOP</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#define MDT_GLOBALS
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE     NAND_PAGE_SIZE //nand page size == 4096

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//- S1 -//
u8  MDT_RxBuf[MDT_RX_MAX_INDEX][MDT_RX_BUFF_SIZE];
u8  MDT_RxStep;
u16 MDT_RxLength[MDT_RX_MAX_INDEX], MDT_RxDataCnt[MDT_RX_MAX_INDEX], MDT_RxCommand[MDT_RX_MAX_INDEX];
u8  MDT_RxIndex, MDT_DecodeIndex, MDT_RxBufFlag[MDT_RX_MAX_INDEX]; 
u8 MDT_RxComplete[MDT_RX_MAX_INDEX];


u8  FOTA_Page_Index, FOTA_RxErr;
u8  FOTA_RxBuf[2][BUFFER_SIZE];
u32 FOTA_Nand_Addr;
u16 FOTA_CRC16;

u8 ComTmpBuff[TX_BUFF_MAX];

u32 FOTA_FileLength;
u8  FOTA_Flag;

u8 MDT_Send_Flag;

u8  WifiRcvFlag;
u32 WifiCommand;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define CAR_NO_REGION_MAX 18
#define CAR_NO_CLASS_MAX 51
const u8 CarNoRegionLG[CAR_NO_REGION_MAX] = 
{
'Z', //없음
'J', //서울
'M', //부산
'N', //대구
'K', //인천
'O', //광주
'L', //대전
'P', //울산
'A', //경기
'B', //강원
'C', //충북
'D', //충남
'E', //전북
'F', //전남
'G', //경북
'H', //경남
'I', //제주
'Q'  //세종
};

const char CarNoClassLG[CAR_NO_CLASS_MAX][6] =
{
"GA", //가
"NA", //나
"DA", //다
"RA", //라
"MA", //마
"GEO", //거
"NEO", //너
"DEO", //더
"REO", //러
"MEO", //머
"BEO", //버
"SEO", //서
"EO", //어
"JEO", //저
"GO", //고
"NO", //노
"DO", //도
"RO", //로
"MO", //모
"BO", //보
"SO", //소
"OH", //오
"JO", //조
"GU", //구
"NU", //누
"DU", //두
"RU", //루
"MU", //무
"BU", //부
"SU", //수
"WO", //우
"JU", //주
"BA", //바
"SA", //사
"AH", //아
"JA", //자
"HEO", //허
"DIPL", //외교
"CNSL", //영사
"S-DIP", //준외
"SCNSL", //준영
"INTL", //국기
"AGREE", //협정
"D-PIO", //대표
"ARMY", //육
"AIR-F", //공
"NAVY", //해
"EO", //어
"BAE", //배
"HA", //하
"HO"//호    
}; //const char CarNoClassLG[][6] =


//----------------------------------- S1 -------------------------------------//

void TxStrToMDT(char* str)
{
  u8 len;
  
  len = StringLen(str,100);
  UART_SendDataStr(&huart3,(u8*)str,len);
}

void TxDataStrToMDT(u8* data, int len)
{
  UART_SendDataStr(&huart3,data,len);
}

void TxCharToMDT(char ch)
{
  UART_SendData(&huart3,ch);
}

/**
  * @brief  MDT_RxData_S1
  * @param  u8 rdata
  * @retval None
  */
void MDT_RxData_S1(unsigned char rdata)
{
  static u8 checksum;
  
  
  if(!MDT_RxComplete[MDT_RxIndex]) {
    switch(MDT_RxStep) {
      case 0:         //packet header
        if(rdata == 0x02) {
          checksum = rdata;
          MDT_RxStep  = 10;
        }
        break;
      case 10:         //device id
        if(rdata == 0x50) {
          checksum += rdata;
          MDT_RxStep = 20;
        }
        else 
          MDT_RxStep = 0;
        break;
      case 20:         //length low
        MDT_RxLength[MDT_RxIndex] = rdata;
        checksum += rdata;
        MDT_RxStep   = 30;
        break;
      case 30:         //length high
        MDT_RxLength[MDT_RxIndex] += rdata*0x100;    
        if(MDT_RxLength[MDT_RxIndex]  < 5) 
          MDT_RxStep = 0;
        else {
          MDT_RxLength[MDT_RxIndex] -= 4;
          checksum += rdata;
          MDT_RxStep   = 40;
        }
        break;
      case 40:         //command 1
        MDT_RxCommand[MDT_RxIndex] = rdata<<8;
        MDT_RxStep  = 50;
        checksum += rdata;
        break;
      case 50:         //command 2
        MDT_RxCommand[MDT_RxIndex] += rdata;
        checksum += rdata;
        MDT_RxDataCnt[MDT_RxIndex] = 0;
        if(MDT_RxCommand[MDT_RxIndex] == (('F'<<8)+'L')) FOTA_Flag = 1;
        
        if(MDT_RxCommand[MDT_RxIndex] == (('F'<<8)+'D')) {
          MDT_RxStep = 61;
        }
        else 
          MDT_RxStep = 60;
        break;
      case 60:         //data[len]
        if(MDT_RxLength[MDT_RxIndex]  < MDT_RX_BUFF_SIZE) {
          MDT_RxBuf[MDT_RxIndex][MDT_RxDataCnt[MDT_RxIndex]] = rdata;
          checksum += rdata;
          MDT_RxDataCnt[MDT_RxIndex]++;
          if(MDT_RxDataCnt[MDT_RxIndex] >= MDT_RxLength[MDT_RxIndex])
            MDT_RxStep = 70;
        }
        else {
          MDT_RxStep = 0;
        }
        break;
      case 61:         //data[len]
        if(MDT_RxDataCnt[MDT_RxIndex] == 0) {
          if(FOTA_Page_Index != rdata) {
            FOTA_RxErr = 1;
            MDT_RxStep = 0;
          }
        }
        else if(MDT_RxDataCnt[MDT_RxIndex] > 0 && MDT_RxDataCnt[MDT_RxIndex] < BUFFER_SIZE+1)
          FOTA_RxBuf[FOTA_Page_Index%2][MDT_RxDataCnt[MDT_RxIndex]-1] = rdata;
        else {
          FOTA_RxErr = 1;
          MDT_RxStep = 0;
        }
        checksum += rdata;
        MDT_RxDataCnt[MDT_RxIndex]++;
        if(MDT_RxDataCnt[MDT_RxIndex] >= MDT_RxLength[MDT_RxIndex])
          MDT_RxStep = 70;
        break;
      case 70:         //chksum
        if(checksum == rdata)
            MDT_RxStep = 80;
        else {
            MDT_RxStep = 0;
        }
        break;
      case 80:         //etx
        MDT_RxStep = 0;
        if(rdata == 0x03) {
          MDT_RxComplete[MDT_RxIndex] = 1;    
          MDT_RxBufFlag[MDT_RxIndex] = 1;
          MDT_RxIndex = (MDT_RxIndex+1)%MDT_RX_MAX_INDEX;
        }
        break;
    }
  }
//  UART_PutChar4(rdata);
}

int IsDigitBuff(u8 *buff, int size)
{
  int cnt = 0;
  for(int i = 0; i < size; i++) 
//    if(isdigit(buff[i])) cnt++;   
    if(buff[i] >= '0' && buff[i] <= '9') cnt++;   
  if(size == cnt) return cnt;
  else            return 0;
}

int IsAlNumBuff(u8 *buff, int size)
{
  int cnt = 0;
  for(int i = 0; i < size; i++) 
//    if(isalnum(buff[i])) cnt++;   
    if((buff[i] >= '0' && buff[i] <= '9') || (buff[i] >= 'A' && buff[i] <= 'Z')) cnt++;   
  if(size == cnt) return cnt;
  else            return 0;
}


const u16 ev_data_cnt[6] = {349,350,350,350,350,256}, ev_data_size[6] = {1+3836,1+3840,1+3840,1+3840,1+3840,1+2811};
u8 Make_EventDataLG(u8 step, u8* ev_data, u16 ccrc)
{
  
  u32 addr;
  st_DATA stemp;
  u16 idx = 0, i, crc;
  u8 size;  
  u8 eidx = 0;
#ifdef EVENT_DATA_SAVE_ENABLE
  if(Evt.Idx == 0) eidx =9;
  else             eidx = Evt.Idx-1;
#endif    
  rtc_time stime;
  crc = ccrc;
  
  addr = eidx*NAND_BLOCK_SIZE + NAND_EVENT_DATA_ADDR + 256; 
  size = sizeof(st_DATA);
  if(step > 0)
    addr += (348+(step-1)*349)*size;
  
  ev_data[idx] = step; idx++;
  for(i = 0; i < ev_data_cnt[step]; i++) {
    Nand_Read_Buffer((u8*)&stemp, addr+i*size, size);
    if(step == 0 && i == 0) {
      stime = gmtime_conv(stemp.Time);
      memcpy(&ev_data[idx], (u8*)&stime, 6);
      idx += 6;
      ev_data[idx] = stemp.SubSec;
      idx++;
    }
    ev_data[idx] = stemp.Speed; idx++;
    memcpy(&ev_data[idx], &stemp.RPM, 2); idx += 2;
    ev_data[idx] = *((u8*)&stemp.Signal)<<1; idx++;
    memcpy(&ev_data[idx], (u8*)&stemp.Status, 2); idx += 2;
    ev_data[idx] = stemp.AccX; idx++;
    ev_data[idx] = stemp.AccY; idx++;
    ev_data[idx] = 1; idx++;
    ev_data[idx] = 0; idx++;
    ev_data[idx] = 0; idx++;
  }
  for(i = 1; i < idx; i++) { 
    if(step > 0) 
      ev_data[i] = ev_data[i+step]; 
    if(i < ev_data_size[step]) {
      if(step==0 && i < 8) continue; 
      crc = getcrc16c(crc, ev_data[i]);
    }
  }
  return crc;    
}


/**
  * @brief  MDT_Check_S1
  *   example

//- Request Trip Information per Minute -//
    buff[13]
    02 50 09 00 52 4D 07 01 01 0E 0E 1F 03 
    02             : Packet Header
    50             : Device ID
    09 00          : Length ex)buff[4:12] 52~03
    52 4D          : Command "RM"
    07 01 01 0E 0E : Data "yy mm dd hh nn"
    1F             : CheckSum ex)buff[0:11] sum 02~1F
    03             : Packet End

  * @param  None
  * @retval None
  */


void MDT_Check_S1(void)
{
  u8 *ptr;
  u16 size, len;
  u16 i;
//  u8 sdata[BUFFER_SIZE]; 
  u8 didx;
  u8 tmp;
  u16 cmd;
  u16 fota_crc, file_crc;
  
  
  for(didx = 0; didx < MDT_RX_MAX_INDEX; didx++) {
    if(MDT_RxBufFlag[(MDT_DecodeIndex+didx)%MDT_RX_MAX_INDEX]) break;
  }
  
//  if(MDT_RxComplete) {  //  0 (FALSE) if the receive buffer is empty 
  if(didx < MDT_RX_MAX_INDEX) {
    //UART_PutChar5(0x4C);
    MDT_DecodeIndex = (MDT_DecodeIndex+didx)%MDT_RX_MAX_INDEX;
    //for(i = 0; i < 256; i++) ComTmpBuff[i] = 0;
    memset(ComTmpBuff,0,sizeof(ComTmpBuff));
    switch(MDT_RxCommand[MDT_DecodeIndex]) {
      case (('R'<<8)+'I'):   
        
        len = 0;
        ptr = CarNo_BinToAscii_S1((u8 *)&Setting.Car_Reg_No);
        size = sizeof(Setting.Car_Reg_No);
        Mem_Copy(&ComTmpBuff[len], ptr, size);
//        ComTmpBuff[0] = 0x5A;
//        ComTmpBuff[1] = 0x30;
//        ComTmpBuff[2] = 0x34;
//        ComTmpBuff[3] = 0x48;
//        ComTmpBuff[4] = 0x45;
//        ComTmpBuff[5] = 0x4f;
//        ComTmpBuff[6] = 0x20;
//        ComTmpBuff[7] = 0x20;
//        ComTmpBuff[8] = 0x36;
//        ComTmpBuff[9] = 0x31;
//        ComTmpBuff[10] = 0x37;
//        ComTmpBuff[11] = 0x34;
        len += size;
        
        size = sizeof(Setting.Drv_Name);
        Mem_Copy2(&ComTmpBuff[len], (u8 *)&Setting.Drv_Name, size);
        len += size;

        size = sizeof(Setting.Drv_Code);
        Mem_Copy2(&ComTmpBuff[len], (u8 *)&Setting.Drv_Code, size);
        len += size;

        size = sizeof(Setting.Car_No);
        Mem_Copy2(&ComTmpBuff[len], (u8 *)&Setting.Car_No, size);
        len += size;

        size = sizeof(Setting.Company_Name);
        Mem_Copy2(&ComTmpBuff[len], (u8 *)&Setting.Company_Name, size);
        len += size;

        size = sizeof(Setting.Company_Reg_No);
        Mem_Copy2(&ComTmpBuff[len], (u8 *)&Setting.Company_Reg_No, size);
        len += size;

        size = sizeof(Setting.Model_No);
        Mem_Copy2(&ComTmpBuff[len], (u8 *)&Setting.Model_No, size);
        
        ComTmpBuff[len+9]  = ' ';
        ComTmpBuff[len+10] = ' ';
        
        ComTmpBuff[len+11] = 'F';
        ComTmpBuff[len+12] = Setting.FW_Ver[0]+0x30;
        ComTmpBuff[len+13] = Setting.FW_Ver[1]+0x30;
        ComTmpBuff[len+14] = Setting.FW_Ver[2]+0x30;
        ComTmpBuff[len+15] = Setting.FW_Ver[3]+0x30;
        ComTmpBuff[len+16] = 'B';
        ComTmpBuff[len+17] = Setting.FW_Ver[4]+0x30;
        ComTmpBuff[len+18] = Setting.FW_Ver[5]+0x30;
        if(Setting.DTG_Type == DTG_NORMAL)
          ComTmpBuff[len+19] = 'N';
        else if(Setting.DTG_Type == DTG_GREEN)
          ComTmpBuff[len+19] = 'G';
        
        len += size;

        size = sizeof(Setting.Form_Approval_No);
        Mem_Copy2(&ComTmpBuff[len], (u8 *)&Setting.Form_Approval_No, size);
        len += size;
        
        size = sizeof(Setting.Serial_No);
        Mem_Copy2(&ComTmpBuff[len], (u8 *)&Setting.Serial_No, size);
        len += size;

        len += 3;
        
        ComTmpBuff[len]   = Car_Type_Code[Setting.Car_Reg_No[0]]/10 + 0x30;
        ComTmpBuff[len+1] = Car_Type_Code[Setting.Car_Reg_No[0]]%10 + 0x30;
        len += 2;
        
        
        len += 25; //reserved
        
        
        
        size = sizeof(Setting.Time)-1; //msec remove
        Mem_Copy(&ComTmpBuff[len], &Setting.Time[0], size);
        len += size;
        
        len += 3;
        
        len += 4; //msisdn
        
        
        memcpy(&ComTmpBuff[len], &SecData, sizeof(SecData));
        
        len += 80;

        //len += 87; //reserved
        
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, len);

        break; //case (('R'<<8)+'I'):  
        
      case (('R'<<8)+'S'):    
        Mem_Copy(&ComTmpBuff[0], (u8 *)&RTC_Val, 6);
        ComTmpBuff[6] = SecData.Speed[0];
        Val_Copy(&ComTmpBuff[7], SecData.RPM, 2);
        Val_Copy(&ComTmpBuff[9], (u32)(SecData.Trip*10), 4);
        Val_Copy(&ComTmpBuff[13], (u32)(SecData.TTrip), 4);
        Val_Copy(&ComTmpBuff[17], SecData.Lat, 4);
        Val_Copy(&ComTmpBuff[21], SecData.Lon, 4);
        Val_Copy(&ComTmpBuff[25], SecData.Azi, 2);
        Val_Copy(&ComTmpBuff[27], (u16)gga.altitude, 2);
//        Val_Copy(&ComTmpBuff[29], SecData.Azi, 2); //vehicle head
        
        tmp = *((u8*)&SecData.Signal);
        ComTmpBuff[39] = (u8)(tmp<<1);     
        if(Setting.FW_Type != MDT_LG)
          MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 40);
        else {
          //ComTmpBuff[39] = *((u8*)&SecData.Signal);   
          Val_Copy(&ComTmpBuff[40], (u32)(SecData.DTrip*10), 4);
          Val_Copy(&ComTmpBuff[44], (u32)(SecData.DFuel), 4);
          Val_Copy(&ComTmpBuff[48], (u32)(SecData.TFuel), 4);
          if(SecData.Fuel > 0.0)
            Val_Copy(&ComTmpBuff[52], (u16)((SecData.Trip/(SecData.Fuel/1000.0))*1000.0), 2);
          ComTmpBuff[54] = CAN_Val.AEBS;
          ComTmpBuff[55] = CAN_Val.LDWS1;
          ComTmpBuff[56] = CAN_Val.LDWS2;
          MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 40+17);
        }
        break; 
        
      case (('R'<<8)+'G'):          
        Mem_Copy(&ComTmpBuff[0], (u8 *)&RTC_Val, 6);
        ComTmpBuff[6] = SecData.Speed[0];
        Val_Copy(&ComTmpBuff[7], SecData.RPM, 2);
        Val_Copy(&ComTmpBuff[9], SecData.Lat, 4);
        Val_Copy(&ComTmpBuff[13], SecData.Lon, 4);
        Val_Copy(&ComTmpBuff[17], SecData.Azi, 2);
        Val_Copy(&ComTmpBuff[19], (u16)gga.altitude, 2);
        if(SecData.Status.GPS_Fault)          
          ComTmpBuff[21] = 'V';
        else
          ComTmpBuff[21] = 'A';
        ComTmpBuff[22] = gga.satellites; //위성 개수
          
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 23);   
        break; 

      case (('R'<<8)+'M'):   
        { 
          Mem_Copy(&ComTmpBuff[0], (u8 *)&MDT_RxBuf[MDT_DecodeIndex][0], 5);
          if(ComTmpBuff[0] < 10 || ComTmpBuff[1] > 12 || ComTmpBuff[1] == 0 
             || ComTmpBuff[2] > 31 || ComTmpBuff[2] == 0 || ComTmpBuff[3] > 23 || ComTmpBuff[4] > 59) break;
           
          tmp = ComTmpBuff[4]%4;
        
          if(Setting.FW_Type != MDT_LG) {
            Val_Copy(&ComTmpBuff[5], MDT_DTrip[tmp], 4);
            Val_Copy(&ComTmpBuff[9], MDT_ODO[tmp], 4);

            if(Setting.DTG_Type > DTG_NORMAL) {//1.4.0.0
              size = sizeof(st_MDT_DATA)*60; //24*60 = 1440 //1.4.0.0
              memcpy(&ComTmpBuff[13], (u8 *)&MDT_Min_Buff[tmp][0], size);  //2.1.1
            }
            else {//1.4.0.0 //2.1.1
              size = sizeof(st_MDT_DATA)-8;     //2.1.1
              for(i = 0; i < 60; i++) //2.1.1
                memcpy(&ComTmpBuff[13+(i*size)], (u8 *)&MDT_Min_Buff[tmp][i], size);  //2.1.1
              size = (sizeof(st_MDT_DATA)-8)*60; //16*60 = 960      //1.4.0.0    //2.1.1        
            }
    //        Mem_Copy(&ComTmpBuff[13], (u8 *)&MDT_Min_Buff[tmp][0], size); //2.1.1
            
            MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, size+5+8);  
          }
          else {
            size = sizeof(st_MDT_DATA_LG);    
            for(i = 0; i < 60; i++) 
              memcpy(&ComTmpBuff[5+(i*size)], (u8 *)&MDT_Min_Buff[tmp][i], size);  //2.1.1
            
            u32 uval;
            uval = (u32)MDT_MTrip[tmp];
            memcpy(&ComTmpBuff[5+(i*size)], (u8 *)&uval, 4);
            memcpy(&ComTmpBuff[5+(i*size+4)], (u8 *)&MDT_DTrip[tmp], 4);
            memcpy(&ComTmpBuff[5+(i*size+8)], (u8 *)&MDT_ODO[tmp], 4);
            uval = (u32)MDT_MFuel[tmp];
            memcpy(&ComTmpBuff[5+(i*size)+12], (u8 *)&uval, 4);
            memcpy(&ComTmpBuff[5+(i*size+16)], (u8 *)&MDT_DFuel[tmp], 4);
            memcpy(&ComTmpBuff[5+(i*size+20)], (u8 *)&MDT_TFuel[tmp], 4);
            ComTmpBuff[5+(i*size+24)] = MDT_AEBS[tmp];
            ComTmpBuff[5+(i*size+25)] = MDT_LDWS1[tmp];
            ComTmpBuff[5+(i*size+26)] = MDT_LDWS2[tmp];
            size = sizeof(st_MDT_DATA_LG)*60; //16*60 = 960      //1.4.0.0    //2.1.1     
            MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, size+5+27);          
          }
        }
        break; 

      case (('F'<<8)+'L'):    
        Mem_Copy((u8 *)&FOTA_FileLength, (u8 *)&MDT_RxBuf[MDT_DecodeIndex][0], 4);
        ComTmpBuff[0] = 0x00;
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 1); 
        FOTA_Page_Index = 0;
        FOTA_RxErr = 0;
        FOTA_Nand_Addr = NAND_PROGRAM_ADDR;
        break;         
        
      case (('F'<<8)+'D'):    
        if(FOTA_FileLength == sizeof(st_Setting)+2) {
          st_Setting *hd;
          hd = (st_Setting*)&FOTA_RxBuf[0];
          if(hd->Identity == 0xdddddddd){
            fota_crc = 0;
            fota_crc = getcrc16(fota_crc, FOTA_RxBuf[0], (FOTA_FileLength-2)%BUFFER_SIZE);
            file_crc = ((u16)FOTA_RxBuf[0][(FOTA_FileLength%BUFFER_SIZE)-2]<<8) + FOTA_RxBuf[0][(FOTA_FileLength%BUFFER_SIZE)-1];
            if(fota_crc == file_crc || fota_crc == SwapEndian16(file_crc)) {
              if(Setting.FW_Type != MDT_LG)
                cmd = ('F'<<8)+'S';
              else 
                cmd = ('F'<<8)+'O';                  
              ComTmpBuff[0] = 0x00;
              MDT_Tx_Data_S1(cmd, ComTmpBuff, 1); 
              
              if(hd->Drv_Name[0] != 0xff) memcpy(Setting.Drv_Name,hd->Drv_Name,sizeof(Setting.Drv_Name));
              if(hd->Drv_Code[0] != 0xff) memcpy(Setting.Drv_Code,hd->Drv_Code,sizeof(Setting.Drv_Code));
              if(hd->Car_Reg_No[0] < MAX_CAR_TYPE) Setting.Car_Reg_No[0] = hd->Car_Reg_No[0];
              if(hd->Car_Reg_No[1] < MAX_CAR_REGION || (hd->Car_Reg_No[1] >= '0' && hd->Car_Reg_No[1] <= '9')) {
                if(hd->Car_Reg_No[4] < MAX_CAR_CLASS) {
                     memcpy(&Setting.Car_Reg_No[1], &hd->Car_Reg_No[1], 8);
                     CarRegNoToVrn();
                }
              } 
              if(hd->Car_No[0] != 0xff) memcpy(Setting.Car_No,hd->Car_No,sizeof(Setting.Car_No));
              if(hd->Company_Name[0] != 0xff) memcpy(Setting.Company_Name,hd->Company_Name,sizeof(Setting.Company_Name));
              if(hd->Company_Reg_No[0] != 0xff) memcpy(Setting.Company_Reg_No,hd->Company_Reg_No,sizeof(Setting.Company_Reg_No));
              if(hd->Form_Approval_No[0] != 0xff) memcpy(Setting.Form_Approval_No,hd->Form_Approval_No,sizeof(Setting.Form_Approval_No));
              if(hd->K_Factor != 0xffff && hd->K_Factor > 0) Setting.K_Factor = hd->K_Factor;
              if(hd->RPM_Factor != 0xffff && hd->RPM_Factor > 0) Setting.RPM_Factor = hd->RPM_Factor;
              if(hd->FW_Type != 0xff && hd->FW_Type > 0) Setting.FW_Type = hd->FW_Type;
              if(hd->Set_Direction != 0xff) Setting.Set_Direction = hd->Set_Direction;
              if(hd->Can_Enable != 0xff) Setting.Can_Enable = hd->Can_Enable;
              if(hd->Can_Type != 0xff) Setting.Can_Type = hd->Can_Type;
              if(hd->Car_Idx != 0xff) Setting.Car_Idx = hd->Car_Idx;
              if(hd->Car_SetFactor != 0xff) Setting.Car_SetFactor = hd->Car_SetFactor;
              if(hd->Brake_Type != 0xff) Setting.Brake_Type = hd->Brake_Type;
              if(hd->DAC_Speed != 0xff) Setting.DAC_Speed = hd->DAC_Speed;
              if(hd->DAC_RPM != 0xff) Setting.DAC_RPM = hd->DAC_RPM;
              if(hd->DTG_Type != 0xff) Setting.DTG_Type = hd->DTG_Type;
              if(hd->ODO_Offeset != 0xffffffff) Setting.ODO_Offeset = hd->ODO_Offeset;
              if(hd->Fuel_Factor > 0 && hd->Fuel_Factor < 1000) {
                Calib_F_Factor = hd->Fuel_Factor;
                Setting.Fuel_Factor = Calib_F_Factor;
                FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);
              }              
              if(hd->K_Factor_Percent > 0 && hd->K_Factor_Percent < 0xff) 
                Setting.K_Factor = Setting.K_Factor*100/hd->K_Factor_Percent;                
              if(hd->RPM_Factor_Percent > 0 && hd->RPM_Factor_Percent < 0xff) 
                Setting.RPM_Factor = Setting.RPM_Factor*hd->RPM_Factor_Percent/100;           
              if(hd->Fuel_Factor_Percent > 0 && hd->Fuel_Factor_Percent < 0xff) {
                Calib_F_Factor = Calib_F_Factor*100/hd->Fuel_Factor_Percent;
                Setting.Fuel_Factor = Calib_F_Factor;
                FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);                
              }
              
              FRAMMultiWrite((u8 *)&Setting, DRV_SETTING_PARM, sizeof(Setting));

              if(hd->ODO_Offeset != 0xffffffff) {
                DailyData.TTrip = (double)hd->ODO_Offeset;
                FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
                Val.TTrip = DailyData.TTrip;
                Val.TTripOld = Val.TTrip;
              }
                            
              lcd_clear();
              lcd_printf(4,1,0,"Found New Param");
              lcd_printf(4,2,0,"System Reset");   
              NVIC_SystemReset();  
              
            }//if(fota_crc == file_crc) {
          }//if(hd->Identity == 0xdddddddd){
          lcd_printf(4,1,0,"Wrong Data");
          lcd_printf(4,2,0,"System Reset");   
          NVIC_SystemReset();            
        }//if(FOTA_FileLength == sizeof(st_Setting)+2) {
        
        
        if(FOTA_Nand_Addr >= NAND_PROGRAM_ADDR && FOTA_Nand_Addr < NAND_TRIP_BASE-4096) {
          Nand_Write_Buffer(FOTA_RxBuf[FOTA_Page_Index%2], FOTA_Nand_Addr, MDT_RxDataCnt[MDT_DecodeIndex]-1);
          FOTA_Nand_Addr += MDT_RxDataCnt[MDT_DecodeIndex]-1;
        }

        if(FOTA_Nand_Addr-NAND_PROGRAM_ADDR == FOTA_FileLength) {
          fota_crc = 0;
          for(i = 0; i < (FOTA_FileLength-2)/BUFFER_SIZE; i++) { //-2 = crc
            Nand_Read_Buffer(FOTA_RxBuf[0], NAND_PROGRAM_ADDR + i*BUFFER_SIZE, BUFFER_SIZE);
            fota_crc = getcrc16(fota_crc, FOTA_RxBuf[0], BUFFER_SIZE);
          }
          Nand_Read_Buffer(FOTA_RxBuf[0], NAND_PROGRAM_ADDR + i*BUFFER_SIZE, FOTA_FileLength%BUFFER_SIZE);
          fota_crc = getcrc16(fota_crc, FOTA_RxBuf[0], (FOTA_FileLength-2)%BUFFER_SIZE);
          file_crc = ((u16)FOTA_RxBuf[0][(FOTA_FileLength%BUFFER_SIZE)-2]<<8) + FOTA_RxBuf[0][(FOTA_FileLength%BUFFER_SIZE)-1];

          if(fota_crc == file_crc || fota_crc == SwapEndian16(file_crc)) {
            if(Setting.FW_Type != MDT_LG)
              cmd = ('F'<<8)+'S';
            else
              cmd = ('F'<<8)+'O';              
            ComTmpBuff[0] = 0x00;
            MDT_Tx_Data_S1(cmd, ComTmpBuff, 1); 

//            IT_Configuration(DISABLE);
//            Nand_Write_SaveData(SEC_DATA);
//            Nand_Write_SaveData(DATA_FLUSH);
            Parameter_Save();
#if 1
            RTC_WriteBackupRegister(RTC_BKP_DR0, 10);  //0x50524F47);   //PROG    0x4A554D50); // JUMP
            RTC_WriteBackupRegister(RTC_BKP_DR1, FOTA_FileLength-2);  //0x50524F47);   //PROG    0x4A554D50); // JUMP
#else //debug usb downloads
            if(USB_Detect_Flag) {
              char fname[20];  
              int ns = (FOTA_FileLength-2)/BUFFER_SIZE;
              u32 bytesWritten;
              FIL file;  
              sprintf(fname, "%02d%02d%02d%s",RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, ".BIN");
              
              f_open(&file, fname, FA_CREATE_ALWAYS | FA_WRITE);
              for(i = 0; i < ns; i++) {
                Nand_Read_Buffer(ComTmpBuff, NAND_PROGRAM_ADDR + i*BUFFER_SIZE, BUFFER_SIZE);
                f_write (&file, ComTmpBuff, BUFFER_SIZE, &bytesWritten);
                if(i%100 == 0) {
                  draw_fill_box(0, (i*80/ns), 6, 4);
                  disp_lcd_16val(88,5,0,3,(i*100/ns));
                }
                IWDG_ReloadCounter();  
                delay_us(1);
              } 
              Nand_Read_Buffer(ComTmpBuff, NAND_PROGRAM_ADDR + i*BUFFER_SIZE, (FOTA_FileLength-2)%BUFFER_SIZE);
              f_write (&file, ComTmpBuff, (FOTA_FileLength-2)%BUFFER_SIZE, &bytesWritten);
              f_close (&file);
            }  
#endif            
            lcd_clear();
            lcd_printf(4,1,0,"Found New Ver");
            lcd_printf(4,2,0,"System Reset");   
            vTaskDelay(2000);
            NVIC_SystemReset();
          }
          else {
            if(Setting.FW_Type != MDT_LG)
              cmd = ('F'<<8)+'E';
            else
              cmd = ('F'<<8)+'F';                        
            ComTmpBuff[0] = 0x00;
            MDT_Tx_Data_S1(cmd, ComTmpBuff, 1); 
          }
        }
        else if(FOTA_Nand_Addr < NAND_PROGRAM_ADDR+FOTA_FileLength) {
          ComTmpBuff[0] = FOTA_Page_Index; //for mds //3.0.2
          MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 1); 
          FOTA_Page_Index++;
        }
        else {
          if(Setting.FW_Type != MDT_LG)
              cmd = ('F'<<8)+'E';
          else
              cmd = ('F'<<8)+'F';                        
          ComTmpBuff[0] = 0x00;
          MDT_Tx_Data_S1(cmd, ComTmpBuff, 1); 
        }
        break;      
        
      case (('S'<<8)+'V'):    //SMS Parameter Setting
        {
          stSMS *sms = (stSMS *)&MDT_RxBuf[MDT_DecodeIndex][0];
          u16 val;
//          memcpy(&sms.CarType,MDT_RxBuf[MDT_DecodeIndex],MDT_RxLength[MDT_DecodeIndex]);
          if(sms->CarType < 0x0C) {
            Setting.Car_Reg_No[0] = sms->CarType;
            FRAMMultiWrite((u8 *)&Setting.Car_Reg_No
                         , DRV_SETTING_PARM + offsetof(st_Setting,Car_Reg_No) 
                         , 1);       
          }
          
          if(IsDigitBuff(sms->CarRegNo,sizeof(sms->CarRegNo))) {      
            u8 r[2];
            r[0] = (sms->CarRegNo[0]-'0')*10 + sms->CarRegNo[1]-'0';
            r[1] = (sms->CarRegNo[4]-'0')*10 + sms->CarRegNo[5]-'0';
            if((r[0] < MAX_CAR_REGION || (r[0] >= '0' && r[0] <= '9' )) && r[1] < MAX_CAR_CLASS) {//r[0]=18 세종 , r[1]=50 호
              Setting.Car_Reg_No[1] = r[0];
              Setting.Car_Reg_No[2] = sms->CarRegNo[2]-'0'; 
              Setting.Car_Reg_No[3] = sms->CarRegNo[3]-'0';               
              Setting.Car_Reg_No[4] = r[1]; 
              Setting.Car_Reg_No[5] = sms->CarRegNo[6]-'0'; 
              Setting.Car_Reg_No[6] = sms->CarRegNo[7]-'0'; 
              Setting.Car_Reg_No[7] = sms->CarRegNo[8]-'0'; 
              Setting.Car_Reg_No[8] = sms->CarRegNo[9]-'0';   
              CarRegNoToVrn();
              FRAMMultiWrite((u8 *)&Setting.Car_Reg_No
                           , DRV_SETTING_PARM + offsetof(st_Setting,Car_Reg_No) 
                           , sizeof(Setting.Car_Reg_No));                    
            }
          }
          
          if(IsAlNumBuff(sms->CarNo,sizeof(sms->CarNo))) {
            memcpy(Setting.Car_No,sms->CarNo,sizeof(sms->CarNo));
            FRAMMultiWrite((u8 *)&Setting.Car_No
                         , DRV_SETTING_PARM + offsetof(st_Setting,Car_No) 
                         , sizeof(Setting.Car_No));      
          }
          
          if(IsDigitBuff(sms->CompanyNo,sizeof(sms->CompanyNo))) {
            memcpy(Setting.Company_Reg_No,sms->CompanyNo,sizeof(sms->CompanyNo)); 
            FRAMMultiWrite((u8 *)&Setting.Company_Reg_No
                         , DRV_SETTING_PARM + offsetof(st_Setting,Company_Reg_No) 
                         , sizeof(Setting.Company_Reg_No));      
          }
          
          if(sms->SpeedFactor > 99 && sms->SpeedFactor < 60000) {
            Setting.K_Factor = sms->SpeedFactor;  
            FRAMMultiWrite((u8 *)&Setting.K_Factor
                         , DRV_SETTING_PARM + offsetof(st_Setting,K_Factor) 
                         , sizeof(Setting.K_Factor));      
          }
          
          if(sms->RPM_Factor > 0 && sms->RPM_Factor < 60000) {
            Setting.RPM_Factor = sms->RPM_Factor;
            FRAMMultiWrite((u8 *)&Setting.RPM_Factor
                         , DRV_SETTING_PARM + offsetof(st_Setting,RPM_Factor) 
                         , sizeof(Setting.RPM_Factor));  
          }
          
          if(sms->CarIdx < 8) {//ext3 : 7
            Setting.Car_Idx = sms->CarIdx;         
            FRAMMultiWrite((u8 *)&Setting.Car_Idx
                         , DRV_SETTING_PARM + offsetof(st_Setting,Car_Idx) 
                         , sizeof(Setting.Car_Idx));   
          }
          
          if(sms->SpeedAutoTune < 2) {
            Setting.SpeedAutoTune = sms->SpeedAutoTune;
            FRAMMultiWrite((u8 *)&Setting.SpeedAutoTune
                         , DRV_SETTING_PARM + offsetof(st_Setting,SpeedAutoTune) 
                         , sizeof(Setting.SpeedAutoTune));   
          }
                    
          if(sms->DistTune > 0 && sms->DistTune < 1000) {
            val = Setting.K_Factor*100/sms->DistTune;
            if(val > 99 && val < 60000) {
              Setting.K_Factor = val;
              FRAMMultiWrite((u8 *)&Setting.K_Factor
                           , DRV_SETTING_PARM + offsetof(st_Setting,K_Factor) 
                           , sizeof(Setting.K_Factor));                  
            }
          }
                    
          if(sms->RPM_Tune > 0 && sms->RPM_Tune < 1000) {
            val = Setting.RPM_Factor*sms->RPM_Tune/100;
            if(val > 0 && val < 60000) {
              Setting.RPM_Factor = val;
              FRAMMultiWrite((u8 *)&Setting.RPM_Factor
                           , DRV_SETTING_PARM + offsetof(st_Setting,RPM_Factor) 
                           , sizeof(Setting.RPM_Factor));   
            }
          }
                    
          if(sms->FuelTune > 9 && sms->FuelTune < 1000) {
            Setting.Fuel_Factor = sms->FuelTune;
            Calib_F_Factor = Setting.Fuel_Factor;
            FRAMMultiWrite((u8 *)&Calib_F_Factor, CALIB_F_FACTOR, 2);
            FRAMMultiWrite((u8 *)&Setting.Fuel_Factor
                         , DRV_SETTING_PARM + offsetof(st_Setting,Fuel_Factor) 
                         , sizeof(Setting.Fuel_Factor));  
          }
          
//          FRAMMultiWrite((u8 *)&Setting, DRV_SETTING_PARM, sizeof(Setting));
//          lcd_clear();
//          lcd_printf(4,1,0,"Found New Param");
//          lcd_printf(4,2,0,"System Reset");   
//          NVIC_SystemReset();  
        }
        break; //case (('S'<<8)+'V'):  
        
      case (('S'<<8)+'E'):    //SMS Extend Parameter Setting
        {
          stSMS_Ex *sms = (stSMS_Ex *)&MDT_RxBuf[MDT_DecodeIndex][0];   
          u16 addr, size;
          if(IsDigitBuff(sms->DrvCode,sizeof(sms->DrvCode))) {
            memcpy(&Driver_Info[Selected_Driver].Code,sms->DrvCode,sizeof(sms->DrvCode));            
            FRAMMultiWrite((u8 *)&Driver_Info[Selected_Driver].Code
                             , DRIVER_INFO+(Selected_Driver*sizeof(st_DRIVER_INFO)) + offsetof(st_DRIVER_INFO,Code)                               
                             , sizeof(sms->DrvCode));                      
              
            memcpy(&Setting.Drv_Code[0],sms->DrvCode,sizeof(sms->DrvCode)); 
            FRAMMultiWrite((u8 *)&Setting.Drv_Code[0]
                         , DRV_SETTING_PARM + offsetof(st_Setting,Drv_Code) 
                         , sizeof(sms->DrvCode));      
          }

          if(IsDigitBuff(sms->DrvNum,sizeof(sms->DrvNum))) {
            memcpy(&Driver_Info[Selected_Driver].Num,sms->DrvNum,sizeof(sms->DrvNum));            
            FRAMMultiWrite((u8 *)&Driver_Info[Selected_Driver].Num
                             , DRIVER_INFO+(Selected_Driver*sizeof(st_DRIVER_INFO)) + offsetof(st_DRIVER_INFO,Num)
                             , sizeof(sms->DrvNum));                      
              
            memcpy(&Setting.Drv_Code[11],sms->DrvNum,sizeof(sms->DrvNum)); 
            FRAMMultiWrite((u8 *)&Setting.Drv_Code[11]
                         , DRV_SETTING_PARM + offsetof(st_Setting,Drv_Code) +11
                         , sizeof(sms->DrvNum));      
          }
          if(sms->ODO_Init == 0x31) {                   

            Setting.ODO_Offeset = 0;
            
            SecData.Trip = 0;
            SecData.DTrip = 0;
            SecData.TTrip = 0;
            DailyData.Trip = 0;
            DailyData.DTrip = 0;
            DailyData.TTrip = 0;
            DailyData.LapTime = 0;
            DailyData.DLapTime = 0;
            DailyData.Drv_Time = 0;
                            
            addr = DRV_SETTING_PARM + offsetof(st_Setting, ODO_Offeset);
            size = sizeof(Setting.ODO_Offeset);
            FRAMMultiWrite((u8*)&Setting.ODO_Offeset, addr, size); 
            
            FRAMMultiWrite((u8*)&DailyData, DRV_DAILY_DATA, sizeof(stDailyData)); 
          }
          if(sms->FuelInit == 0x31) {   

            SecData.Fuel = 0;
            SecData.DFuel = 0;
            SecData.TFuel= 0;
            DailyData.Fuel = 0;
            DailyData.DFuel = 0;
            DailyData.TFuel = 0;

            CAN_Val.FuelRate = 0;
            CAN_Val.FuelRateSum = 0;
            CAN_Val.FuelRateCnt = 0;
            CAN_Val.InstKPL = 0;
            CAN_Val.InstKPLSum = 0;
            CAN_Val.InstKPLCnt = 0;
            CAN_Val.AvgKPL = 0;
            CAN_Val.TFuel = 0;

            size = sizeof(DailyData.Fuel) + sizeof(DailyData.DFuel) + sizeof(DailyData.TFuel);
            
            addr = DRV_DAILY_DATA + offsetof(stDailyData, Fuel);
            FRAMMultiWrite((u8*)&DailyData.Fuel, addr, size);                  
          }      
          if(Setting.Can_Enable) {
            if(sms->ODO_Type == 0x30) {
              Setting.ODO_Type = 0;              
              addr = DRV_SETTING_PARM + offsetof(st_Setting, ODO_Type);
              FRAMMultiWrite((u8*)&Setting.ODO_Type, addr, sizeof(Setting.ODO_Type));  
            }
            else if(sms->ODO_Type == 0x31) {
              Setting.ODO_Type = 1;
              addr = DRV_SETTING_PARM + offsetof(st_Setting, ODO_Type);
              FRAMMultiWrite((u8*)&Setting.ODO_Type, addr, sizeof(Setting.ODO_Type));  
            }
          }
        }
        break; //case (('S'<<8)+'E'): 
        
      case (('R'<<8)+'T'):    //lg
        if(Setting.FW_Type != MDT_LG) break;
        Val_Copy(&ComTmpBuff[0], (u32)SecData.TTrip, 4);
        Val_Copy(&ComTmpBuff[4], DailyData.Drv_Time, 4);
        Val_Copy(&ComTmpBuff[8], (u32)SecData.TFuel, 4);
        Val_Copy(&ComTmpBuff[12], (u32)SecData.Trip, 4);
        Val_Copy(&ComTmpBuff[16], DailyData.LapTime, 4);
        Val_Copy(&ComTmpBuff[20], (u32)SecData.Fuel, 4);
        ComTmpBuff[24] = DailyData.Evt_Count;
        Val_Copy(&ComTmpBuff[25], (u32)SecData.DFuel, 4);
        Mem_Copy(&ComTmpBuff[49], &Setting.Time[0], 6);
        Mem_Copy(&ComTmpBuff[55], (u8 *)&MDT_SecDataTimeBuffer[0], 6);
          
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 61);   
      
        break; 
        
      case (('R'<<8)+'D'):    {
        if(Setting.FW_Type != MDT_LG) break;
          //memset(ComTmpBuff,0xff,sizeof(ComTmpBuff));
          Mem_Copy(&ComTmpBuff[0], (u8 *)&MDT_RxBuf[MDT_DecodeIndex][0], 3);
          if(ComTmpBuff[0] < 10 || ComTmpBuff[1] > 12 || ComTmpBuff[1] == 0 || ComTmpBuff[2] > 31)
            Mem_Copy(&ComTmpBuff[0], (u8 *)&RTC_Val, 3);
          if(Nand_Read_DailyData_LG(ComTmpBuff)) {
            Val_Copy(&ComTmpBuff[3], (u32)(TmpDailyData.DTrip*10), 4);
            Val_Copy(&ComTmpBuff[7], (u32)TmpDailyData.TTrip, 4);        
            Val_Copy(&ComTmpBuff[11], (u32)TmpDailyData.Fuel, 4);
          }
          MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 15);      
        }
        break; 
        
      case (('R'<<8)+'A'): 
        if(Setting.FW_Type != MDT_LG) break;
#ifdef EVENT_DATA_SAVE_ENABLE
        if(Evt.Save_Flag) break;
#endif        
        
        file_crc = 0;
        for(i = 0; i < 6; i++) {
          file_crc = Make_EventDataLG(i, ComTmpBuff, file_crc);
          if(i < 5)
            MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, ev_data_size[i]);    
          else {
            memset(&ComTmpBuff[ev_data_size[i]], 0, 3);
            memcpy(&ComTmpBuff[ev_data_size[i]+3], &file_crc, 2);
            MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, ev_data_size[i]+5);    
          }
          vTaskDelay(1000);
        }
//        const u16 ev_data_cnt[6] = {349,350,350,350,350,256}, ev_data_size[6] = {3836,3840,3840,3840,3840,2811};
//u8 Make_EventDataLG(u8 step, u8* ev_data, u8 ccrc)

#if 0
        if(Evt.Idx > 0)
          s_addr = (Evt.Idx-1)*NAND_BLOCK_SIZE + NAND_SIZE_DATA; 
       else
          s_addr = 9*NAND_BLOCK_SIZE + NAND_SIZE_DATA; 

        pno = 0;
        ComTmpBuff[0] = pno;
        Nand_Read_Buffer(buff, s_addr, 32);
        Mem_Copy(&ComTmpBuff[1], &buff[4], 7);
        if(buff[0] != 0xee && buff[1] != 0xee) break;

        s_addr += sizeof(st_Setting);
        size = sizeof(st_DATA);
        idx = 0;
        base = 7;
        for(i = 0; i < 2000; i++) { //j =32에서 멈춤..
          addr = i*size+s_addr;
          Nand_Read_Buffer(buff, addr, size);

          Mem_Copy(&ComTmpBuff[11*idx+base+1], &buff[7], 3); //speed:1 / rpm:2
           
          ComTmpBuff[11*idx+base+4] = buff[24];
            
          Mem_Copy(&ComTmpBuff[11*idx+base+5], &buff[26], 2); //status
           
          ComTmpBuff[11*idx+base+7] = 0;   //GX
          ComTmpBuff[11*idx+base+8] = 0;   //GY
          ComTmpBuff[11*idx+base+9] = 0;   //GZ

          ComTmpBuff[11*idx+base+10] = 0;   //resewrved
          ComTmpBuff[11*idx+base+11] = 0;

          idx++;
          if(idx == 350) {
            if(pno == 0) {
              MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, idx*11+8);    //3850+8+4 = 3862 
            }
            else {
              ComTmpBuff[0] = pno;
              MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, idx*11+1);  //3850+1+4 = 3855  
            }
            idx = 0;
            base = 0;
            pno++;
          }
        }      
        ComTmpBuff[0] = pno;
        
        ComTmpBuff[11*idx+1] = 0;  // blank
        ComTmpBuff[11*idx+2] = 0;
        ComTmpBuff[11*idx+3] = 0;
        ComTmpBuff[11*idx+4] = 0;  // CRC
        ComTmpBuff[11*idx+5] = 0;
        
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, idx*11+6);  // 250*11+6+4 = 2760 
#endif
        break; 
        
      case (('R'<<8)+'C'):    //lg
        if(Setting.FW_Type != MDT_LG) break;
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 10);  
        break; 
        
      case (('D'<<8)+'C'):    //lg
        if(Setting.FW_Type != MDT_LG) break;
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 1);  
        break; 
        
      case (('S'<<8)+'R'):   //lg 
        if(Setting.FW_Type != MDT_LG) break;
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 1); 
        break; 
        
      case (('P'<<8)+'F'):  //lg  
        if(Setting.FW_Type != MDT_LG) break;
        MDT_Key_Off_Time = 610; //강제로 10분후로 조정하여 꺼지게 함
        MDT_Tx_Data_S1(MDT_RxCommand[MDT_DecodeIndex], ComTmpBuff, 1);  
        CAN_Key_Off_Time = 610;
        break; 
      case (('R'<<8)+'U'): //lg
        if(Setting.FW_Type != MDT_LG) break;
        SendRU_Data();
        break;
    }     
    if(MDT_RxCommand[MDT_DecodeIndex] != ('F'<<8)+'D') {
      for(i = 0; i < MDT_RxLength[MDT_DecodeIndex]; i++) {
        MDT_RxBuf[MDT_DecodeIndex][i] = 0;
      }
    }
     //- debug
//    UART_printf5("[%02u:%02u:%02u] %c%c\n", 
//                 RTC_Val.tm_hour,RTC_Val.tm_min,RTC_Val.tm_sec, 
//                 MDT_RxCommand[MDT_DecodeIndex]>>8,MDT_RxCommand[MDT_DecodeIndex]);

    //- debug
    MDT_RxLength[MDT_DecodeIndex] = 0;
    MDT_RxDataCnt[MDT_DecodeIndex] = 0;
    COM_Command_Char = MDT_RxCommand[MDT_DecodeIndex];
    MDT_RxCommand[MDT_DecodeIndex] = 0;
    MDT_RxBufFlag[MDT_DecodeIndex] = 0;
    MDT_RxComplete[MDT_DecodeIndex] = 0;
    MDT_DecodeIndex = (MDT_DecodeIndex+1)%MDT_RX_MAX_INDEX;
    MDT_Check_Cnt = 0;
  }
}


void SendRU_Data(void)
{
  st_U_Plus_Data ud;  
  memset(&ud,0,sizeof(ud));
  u32 tmp;
  
  memcpy(ud.DateTime, MDT_SecDataTimeBuffer, sizeof(ud.DateTime));
  tmp = (u32)(SecData.Trip*10);
  ud.Trip = tmp;
  tmp = (u32)(SecData.TTrip*10);
  ud.ODO = tmp;
  tmp = (u32)(SecData.DTrip*10);
  ud.DTrip = tmp;
  ud.LabTime = DailyData.LapTime;
  tmp = (u32)(SecFuelRate*0.05);
  ud.Fuel = tmp;
  ud.Speed = SecData.Speed[0];
  ud.RPM = SecData.RPM;  
  if(COM_Command_Char == 'P') ud.GPS_Status = 1;
  else                         ud.GPS_Status = 0;
  memcpy(ud.GPS_DateTime, MDT_SecDataTimeBuffer, sizeof(ud.GPS_DateTime));
  ud.Lat = SecData.Lat;
  ud.Lon = SecData.Lon;
  ud.Azi = SecData.Azi;
  ud.GPS_Speed = GPS_Speed_Val;
  ud.AccX = SecData.AccX;
  ud.AccY = SecData.AccY;
  ud.Aux = SecData.Signal.Brake;
  
  if(SecData.Status.GPS_Fault)         ud.Status = 11;
  else if(SecData.Status.Speed_Fault)  ud.Status = 12;
  else if(SecData.Status.RPM_Fault)    ud.Status = 13;
  else if(SecData.Status.Brake_Fault)  ud.Status = 14;
  else if(SecData.Status.Input_Fault)  ud.Status = 21;
  else if(SecData.Status.Output_Fault) ud.Status = 22;
  else if(SecData.Status.Save_Fault)   ud.Status = 31;
  else if(SecData.Status.Comm_Fault)   ud.Status = 32;
  else if(SecData.Status.Dist_Fault)   ud.Status = 41;
  else if(SecData.Status.Power_Fault)  ud.Status = 99;
  
  u32 df, tf;
  df = (u32)SecData.DFuel; 
  tf = (u32)SecData.TFuel;   
  memcpy(&ud.DFuel, &df, 4);  //ud.DFuel = (u32)SecData.DFuel_Consump; //이게 안됨...; 임시방편
  memcpy(&ud.TFuel, &tf, 4);
  
  MDT_Tx_Data_S1((('R'<<8)+'U'), (u8*)&ud, sizeof(ud));  
  
}


/**
  * @brief  CarNo_BinToAscii_S1
  * @param  u8 data
  * @retval None
  */
u8* CarNo_BinToAscii_S1(u8 *data)
{
  static u8 ret[12];
  u8 regn, clss, i;
  
  regn = data[1];
  clss = data[4];

  if(Setting.FW_Type != MDT_LG) {
    for(i = 0; i < 12; i++) {
      if(VRN[i] != 0) ret[i] = VRN[i];
      else ret[i] = ' ';
    }
  }
  else {
    memset(ret,' ',12);
    int c=0;
    if(regn < CAR_NO_REGION_MAX)
      ret[c++] = CarNoRegionLG[regn];
    else
      ret[c++] = ' ';
    ret[c++] = data[2] + '0';
    ret[c++] = data[3] + '0';
    
    if(clss < CAR_NO_CLASS_MAX) {
      for(int i = 0; i < 6; i++) {
        if(CarNoClassLG[clss][i] != '\0') 
          ret[c++] = CarNoClassLG[clss][i];      
        else break;
      }
    }
    else {
      ret[c++] = ' '; 
    }
    c=8;
    for(int i = 0; i < 4; i++)
      ret[c++] = data[5+i] + '0';
  }
  
  return ret;
    
}



/**
  * @brief  MDT_Tx_Data_S1
  * @param  u16 command, u8 *data, u16 len
  * @retval None
  */
void MDT_Tx_Data_S1(u16 command, u8 *data, u16 len) //965
{
  u8 tdata[6], check_sum = 0;
  u16 i, tlen = 0;
  tlen += len+4; //command*2, chksum, etx
  
  tdata[0] = 0x02; //stx
  tdata[1] = 0x51; //device ID
  tdata[2] = (u8)tlen;
  tdata[3] = (u8)(tlen>>8);
 
  tdata[4] = (u8)(command>>8);
  tdata[5] = (u8)command;
  for(i = 0; i < 6; i++)  {
    check_sum += tdata[i];
    TxCharToMDT(tdata[i]);
  }

  for(i = 0; i < len; i++) {
    TxCharToMDT(*data);
    check_sum += *data;
    data++;
  }
  TxCharToMDT(check_sum);
  TxCharToMDT(0x03);
}  


#define WIFI_TX_BUFF_MAX 1024
#define WIFI_TX_WAIT_DELAY 10 //10*10ms = 100ms
tsWifiTx WifiTx;

void WifiTxData(void)
{ 
  char str[100];
    
  if(WifiTx.Step && WifiTx.WaitCnt > 500) {    
    WifiTx.Step = 0;
    WifiTx.WaitCnt = 0;
    TxStrToMDT("AT+CIPCLOSE\r");
  }
  
  switch(WifiTx.Step) {
    case 0:  //idle
      break;
    case 1:  //calcurate idx,cnt
      
      WifiTx.SendLen = WifiTx.Cnt - WifiTx.SendIdx*WIFI_TX_BUFF_MAX;
      if(WifiTx.SendLen > WIFI_TX_BUFF_MAX) WifiTx.SendLen = WIFI_TX_BUFF_MAX;
      
      
      if(WifiTx.SendIdx < WifiTx.SendCnt) {
        WifiTx.SendLen = WifiTx.Cnt - WifiTx.SendIdx*WIFI_TX_BUFF_MAX;
        if(WifiTx.SendLen > WIFI_TX_BUFF_MAX) WifiTx.SendLen = WIFI_TX_BUFF_MAX;
      }      

      WifiTx.Step++;
      memset(str,0,100);
      sprintf(str,"AT+CIPSEND=%u\r", WifiTx.SendLen);
      TxStrToMDT(str);
      WifiTx.WaitCnt=0;        
      
      break;
    case 2: //wait > prompt

      break;
      
    case 3: //send data      
      for(int i = 0; i < WifiTx.SendLen; i++) 
        TxCharToMDT(ComTmpBuff[(WifiTx.SendIdx*WIFI_TX_BUFF_MAX)+i]);    
      WifiTx.WaitCnt=0;       
      WifiTx.Step++;
      break;
    case 4:
      if(WifiTx.WaitCnt > WIFI_TX_WAIT_DELAY) {        
        WifiTx.WaitCnt = 0;
        WifiTx.SendIdx++;
        if(WifiTx.SendIdx < WifiTx.SendCnt) 
          WifiTx.Step=1;
        else
          WifiTx.Step=0;
        WifiTx.WaitCnt=0;
      }
      break;
  }  
}

void WifiTxDataToBuff(u8 com, u32 len, u8* data)
{
  u32 cnt=0;
  u16 crc=0; 
  
  ComTmpBuff[cnt++] = 'L'; ComTmpBuff[cnt++] = 'O'; ComTmpBuff[cnt++] = 'O'; ComTmpBuff[cnt++] = 'P'; 
  memcpy(&ComTmpBuff[cnt],&com,4); cnt+=4;
  memcpy(&ComTmpBuff[cnt],&len,4); cnt+=4;
  for(int i = 0; i < len; i++) {
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ data[i])&0x00FF];
    ComTmpBuff[cnt++] = data[i];
  }
  memcpy(&ComTmpBuff[cnt],&crc,2); cnt+=2;
  ComTmpBuff[cnt++] = 0x7E; ComTmpBuff[cnt++] = 0x77; ComTmpBuff[cnt++] = '\r'; ComTmpBuff[cnt++] = '\n'; 
  
  WifiTx.SendIdx = 0;
  if(cnt <= WIFI_TX_BUFF_MAX) {
    WifiTx.SendCnt = 1;
  }
  else {
    WifiTx.SendCnt = cnt/WIFI_TX_BUFF_MAX;
    if(cnt%WIFI_TX_BUFF_MAX) WifiTx.SendCnt++;
  }
  
  WifiTx.Cnt = cnt;
  WifiTx.Step = 1;
  WifiTx.WaitCnt=0;
}

void WifiCheck(void)
{
  if(WifiRcvFlag) {  
    u32 paddr =DataSaveAddr - (DataSaveAddr%NAND_PAGE_SIZE);  
    
//    UART_printf5("rcv ok\r\n");
    switch(WifiCommand) {
      case 0:
        COM_Command_Char = 'H';
        memcpy(&ComTmpBuff[0],&WifiNandAddr,4);
        memcpy(&ComTmpBuff[4],&paddr,4);
        memcpy(&ComTmpBuff[8],&Setting,sizeof(Setting));
        WifiTxDataToBuff(0, 8 + sizeof(Setting),ComTmpBuff);            
        break;
      case 1:
        COM_Command_Char = 'W';
        if(WifiNandAddr >= paddr) break;
        FRAMMultiWrite((u8*)&WifiNandAddr, WIFI_NAND_ADDR,4); 
        memcpy(&ComTmpBuff[0],&WifiNandAddr,4);
        memcpy(&ComTmpBuff[4],&paddr,4);
        Nand_Read_Buffer(&ComTmpBuff[8], WifiNandAddr, NAND_PAGE_SIZE);      
        WifiTxDataToBuff(1,8+NAND_PAGE_SIZE,ComTmpBuff);  
        WifiNandAddr = (WifiNandAddr+NAND_PAGE_SIZE)%NAND_SIZE_DATA;
        break;
      case 2:
        COM_Command_Char = 'Z';
        WifiNandAddr = 0;
        FRAMMultiWrite((u8*)&WifiNandAddr, WIFI_NAND_ADDR,4); 
        memcpy(&ComTmpBuff[0],&WifiNandAddr,4);
        memcpy(&ComTmpBuff[4],&paddr,4);
        WifiTxDataToBuff(2, 8,ComTmpBuff);      
        break;
    }
    MDT_Check_Cnt = 0;
    WifiRcvFlag = 0;
  }
}
  
void WifiScheduler(void)
{
  static int cnt;
  char str[100];
  u8 ip[4];
  u16 port;
  
  if(WifiRcvFlag) cnt = 0;
  if(WifiTx.Step) cnt = 0;
  if(!MDT_Send_Flag || !Nand_Header_Flag) return;
  
  cnt++;
//  UART_printf5("connect wait %u\r\n", cnt);
  if(cnt >= 30) {
    TxStrToMDT("ATE0\r\n");
//    UART_printf5("echo 0 try\r\n");
    memcpy(ip, (u8*)&ModemInfo.LpIP, sizeof(ip));
    port = ModemInfo.LpPort;
    memset(str,0,100);
    sprintf(str,"AT+CIPSTART=\"TCP\",\"%d.%d.%d.%d\",%d\r\n",ip[0],ip[1],ip[2],ip[3],port);
    TxStrToMDT(str);
//    UART_printf5("connect try\r\n");
    cnt=0;
  }
  
  MDT_Send_Flag = 0;
}


/**
  * @brief  WifiRxData
  * @param  None
  * @retval None
  */
void WifiRxData(u8 rdata) 
{
  static int step,com,len,cnt;
  static u8 buf[10];
  static u16 rcrc;
  u16 ccrc;
  
  switch(step) {
    case 0: 
      if(rdata == 'L') step++;  
      break;
    case 1: 
      if(rdata == 'O') step++;  
      else             step = 0;
      break;
    case 2: 
      if(rdata == 'O') step++;  
      else             step = 0;
      break;
    case 3: 
      if(rdata == 'P') {
        com = 0; cnt = 0; step++;
      } 
      else step = 0;
      break;
    case 4: 
      com += rdata<<(cnt*8); cnt++; 
      if(cnt == 4) {
        len = 0; cnt = 0; step++;
      }
      break;
    case 5: 
      len += rdata<<(cnt*8); cnt++; 
      if(cnt == 4) {        
        cnt = 0;
        if(len == 0) {rcrc = 0; step += 2;}
        else         step++;
      }
      break;
    case 6: 
      buf[cnt++] = rdata; 
      if(cnt == len) {
        rcrc = 0; cnt = 0; step++;
      }
      break;
    case 7: 
      rcrc += rdata<<(cnt*8); cnt++; 
      if(cnt == 2) {
        ccrc = getcrc16b(buf,len);
        if(rcrc == ccrc) step++;
        else             step = 0;
      }
      break;
    case 8:
      if(rdata == 0x7E) step++;
      else              step = 0;
      break;
    case 9:
      if(rdata == 0x77) step++;
      else              step = 0;
      break;
    case 10:
      if(rdata == 0x0D) step++;
      else              step = 0;
      break;
    case 11:
      if(rdata == 0x0A) {
//        if(com == 0)      WifiStep = 1;
//        else if(com == 1) {
//          u32 addr;
//          memcpy(&addr, buf, 4);   
//          if(addr < NAND_SIZE_DATA-sizeof(tsSecData)) {
//            WifiNandAddr = addr+NAND_PAGE_SIZE;   
//          }
//          else { WifiStep = 0; break;}
//        }
        WifiCommand = com;        
        WifiRcvFlag = 1;  
      }
      step = 0;        
      break;
  }
}

/**
  * @brief  WifiRxData
  * @param  None
  * @retval None
  */
void WifiRxHeaderParse(u8 rdata) 
{
  static int step,len,cnt;
  
  switch(step) {
   case 0:
      if(rdata == '+') step++;
      break;
    case 1:
      if(rdata == 'I') step++;
      else             step=0;
      break;
    case 2:
      if(rdata == 'P') step++;
      else             step=0;
      break;
    case 3:
      if(rdata == 'D') step++;
      else             step=0;
      break;
    case 4:
      if(rdata == ',') {
        len=0; cnt=1;
        step++;
      }
      else step=0;
      break;
    case 5:
      if(rdata >= '0' && rdata <= '9') {
        len *= cnt;
        len += (rdata-'0');        
        cnt *= 10;        
        if(cnt > 10000) step=0;
      }
      if(rdata == ':') {
        if(len > 0 && len < 5000) {
          cnt=0; step++;
        }
        else    step=0;
      }
      break;
    case 6:      
      WifiRxData(rdata);
      cnt++;
      if(cnt >= len) step=0;
      break;    
  }  
  if(WifiTx.Step == 2) {
    if(rdata == '>') {
      WifiTx.Step++;
      WifiTx.WaitCnt=0;
    }
//    else             WifiTx.Step=0;
  }
//  if(WifiTx.Step == 3) {
//    WifiTx.EchoRxCnt++;
//    if(WifiTx.EchoRxCnt >= WifiTx.SendLen) {
//      WifiTx.Step++;
//    }
////    else             WifiTx.Step=0;
//  }
  
}

void MDT_SecIntProcess(void)
{

  if(Start_Flag && !NotRegist_Flag) {
    if(!FOTA_Flag) {
      MDT_Send_Flag = 1;
    }
  }
  
  if(!Setting.Can_Enable && Setting.FW_Type == MDT_LG) 
    MDT_Key_Off_Time++;
  
  MDT_Check_Cnt++;
  if(MDT_Check_Cnt > 70) {      
    COM_Command_Char = ' ';
  }
}


/******************* (C) COPYRIGHT 2012 LOOP  ******************END OF FILE****/