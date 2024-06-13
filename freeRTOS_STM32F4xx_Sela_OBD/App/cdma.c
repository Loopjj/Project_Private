/*
*************************************** (C) COPYRIGHT 2015 LOOP ****************
*
* File Name          : modem.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 07/29/2015
*
* Description        : modem source 
*
********************************************************************************
*/
#define CDM_GLOBALS
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

tsModem Md;
tsRpck Rpck;
tsFWIF FW_Info;
tsSaveCtrl SaveCtrl;

u8 DiffDayFlag;

#define SEND_PAGE_SIZE    64*2  // 4k

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
********************************************************************************
* Description : ModemRcvParser
* Arguments   : r
* Return      : 
* Note        : modem receive 
******************************************************************************** 
*/
void ModemRcvParser(char r)
{
  Md.Delay = 0;
  
  switch(Md.Step) {
    //tell
    case 0:
      if(r == '$')      Md.Step++;
      else if(r == '+' && Md.Cnum == 0) Md.Step = 11;
      break;
    case 1:
      if(r == '$') Md.Step++;
      else         Md.Step=0;
      break;
    case 2:
      if(r == 'T') Md.Step++;
      else         Md.Step=0;
      break;
    case 3:
      if(r == 'E') Md.Step++;
      else         Md.Step=0;
      break;
    case 4:
      if(r == 'L') Md.Step++;
      else         Md.Step=0;
      break;
    case 5:
      if(r == 'L') Md.Step++;
      else         Md.Step=0;
      break;
    case 6:
      if(r == ':') Md.Step++;
      else         Md.Step=0;
      break;
    case 7:
      if(r == ' ') {
        Md.Step++;
        Md.Parse=0;
      }
      else  Md.Step=0;
      break;
    case 8:
      if(r >= '0' && r <= '9') {
        Md.Parse *= 10;
        Md.Parse += (r-'0');
      }
      else {
        Md.Tell = Md.Parse;
        Md.Step=0;
      }
      break;
      
    //cnum
    case 11:
      if(r == 'C') Md.Step++;
      else         Md.Step=0;
      break;
    case 12:
      if(r == 'N') Md.Step++;
      else         Md.Step=0;
      break;
    case 13:
      if(r == 'U') Md.Step++;
      else         Md.Step=0;
      break;
    case 14:
      if(r == 'M') Md.Step++;
      else         Md.Step=0;
      break;
    case 15:
      if(r == ':') Md.Step++;
      else         Md.Step=0;
      break;
    case 16:
      if(r == ' ') Md.Step++;
      else  	   Md.Step=0;
      break;
    case 17:
      if(r == ',') Md.Step++;
      else  	   Md.Step=0;
      break;
    case 18:
      if(r == '"') {
        Md.Parse=0;
        Md.Step++;
      }
      else         Md.Step=0;
      break;
    case 19:
      if(r >= '0' && r <= '9') {
        Md.Parse *= 10;
        Md.Parse += (r-'0');
      }
      else {
        Md.Cnum = Md.Parse;
        Md.Step=0;
      }
      break;
      
      
    default :
      Md.Step = 0;
      break;
  }
}

/*
********************************************************************************
* Description : CommandStrToInt
* Arguments   : string
* Return      : 
* Note        : string to inteager
******************************************************************************** 
*/
unsigned int CommandStrToInt(char* str) //not used
{
  unsigned int com = 0;
//  while(*str != '\0') com += 
  for(int i = 3; i >= 0; i--)
    com += str[i]<<(i*8);
  return com;
}
#if 0
unsigned short TxModemDataMake(tsPckHeader hd, char* data, INT8U* sd) //not used
{
  tsPckFooter    ft;
  hd.SPK = 0xCFCC;
  hd.DeviceID = DV_DTG;
  hd.IdentityNo = Md.Cnum;
  ft.CRC16 = 0;
  ft.CRC16 = GenerateCRC((char*)&hd, sizeof(hd), ft.CRC16); //  getcrc16c(ft.CRC16, (unsigned char*)&hd,sizeof(hd));
  ft.CRC16 = GenerateCRC(data, hd.Length, ft.CRC16); //getcrc16c(ft.CRC16, data, hd.Length);
  ft.EPK = 0xD0CF;

  unsigned short scnt=0;
  memcpy(&sd[scnt], &hd, sizeof(hd));
  scnt += sizeof(hd);
  memcpy(&sd[scnt], data, hd.Length);
  scnt += hd.Length;
  memcpy(&sd[scnt], &ft, sizeof(ft));
  scnt += sizeof(ft);

  return scnt;
}
#endif

/*
********************************************************************************
* Description : TxModemData
* Arguments   : ...
* Return      : 
* Note        : tx modem data
******************************************************************************** 
*/
void TxModemData(teCommand com, unsigned short idx, unsigned short cnt, unsigned int len, INT8U* data)
{  
  tsPckHeader hd;
  tsPckFooter ft;
  hd.SPK = 0xCFCC;
  hd.DeviceID = DV_SCR;
  hd.IdentityNo = Md.Cnum;
  hd.Command = com;//CommandStrToInt(cstr);
  hd.Index = idx;
  hd.Count = cnt;
  hd.Length = len;
  UART4_SendDataStr((INT8U*)&hd, sizeof(hd));  
  UART4_SendDataStr(data, len);
  ft.CRC16 = 0;
  ft.CRC16 = GenerateCRC((INT8U*)&hd, sizeof(hd), ft.CRC16);
  ft.CRC16 = GenerateCRC(data, hd.Length, ft.CRC16); 
  ft.EPK = 0xD0CF;
  UART4_SendDataStr((INT8U*)&ft, sizeof(ft));   
}

/*
********************************************************************************
* Description : ModemControl
* Arguments   : delay time
* Return      : 
* Note        : modem control
******************************************************************************** 
*/
u8 skopcnt;
void ModemControl(unsigned int delay)
{ 
  switch(Md.Process) {
    case 0:
      if(Md.Tell == CAMPED_ON_WCDMA) {
        if(delay%100 == 0) { 
#if 0
          UART4_print("AT$$TCP_ADDR=0,218,36,105,4,7777\r"); //todo: ack rcv 받아야됨 귀찮아서 나중...
#else
          UART4_print("AT$$TCP_ADDR=0,14,63,219,97,7777\r"); //todo: ack rcv 받아야됨 귀찮아서 나중...
#endif
          Md.Process++;      
        }
      }
      break;
    case 1: 
      if(Md.Cnum == 0) {
        if(delay%100 == 0) { 
          UART4_print("AT+CNUM\r");  
        }
      }
      else {
        if(ExtFram.DataAddr > (ExtFram.SentAddr + SEND_PAGE_SIZE * 1)) 
          Md.Process++;
      }
      break;
    case 2:
      if(Md.Tell != TCP_CONNECTED) {
        if(delay%5000 == 0) {            
          skopcnt++;
          if(skopcnt > 5) {
            skopcnt = 0;
            UART4_print("AT$$TCP_PPPCL\r");   
            Md.Process--;
          }         
          else UART4_print("AT$$TCP_SCOP\r");   
        }
      }
      else Md.Process++;        
      break;
    case 3:
      TxModemData(CM_INFO,0,1,sizeof(tsModemInfo),(INT8U*)&Fram.ModemInfo);
      Md.Process++;        
      break;
    case 4: //decoding....
      break;
    case 5:
      if(Md.Tell == TCP_CONNECTED) {
        if(delay%1000 == 0)
          UART4_print("AT$$TCP_PPPCL\r");             
      }
      else Md.Process = 4;        
      break;
           
  }
}

/*
********************************************************************************
* Description : RxModemChar
* Arguments   : r
* Return      : 
* Note        : rx modem char
******************************************************************************** 
*/
int RxModemChar(unsigned char r)
{
  //Pck is gloval variable
  //Pck is make a new instance per connection c++

  unsigned int pstep = 0;
	Rpck.Delay = 0;
  
  if(Rpck.Step < ST_CRC16)
    Rpck.CalcCRC16 = GetCRC16(Rpck.CalcCRC16, r); // getcrc16b(Rpck.CalcCRC16,r);

  switch(Rpck.Step) {
    case ST_SPK_ST:
      if(r == 0xCC) {
        Rpck.CalcCRC16 = 0;
        Rpck.CalcCRC16 = GetCRC16(Rpck.CalcCRC16, r); //getcrc16b(Rpck.CalcCRC16,r);
        Rpck.Step++;
      }
      break;
    case ST_SPK_CF:
      if(r == 0xCF) {
        Rpck.Header.DeviceID = 0;
        Rpck.Cnt = 0;
        Rpck.Step++;
      }
      else {
        pstep = Rpck.Step;
        Rpck.Step = ST_SPK_ST;
      }
      break;
    case ST_DEVICE_ID:
      Rpck.Header.DeviceID += r<<((Rpck.Cnt++)*8);
      if(Rpck.Cnt >= sizeof(Rpck.Header.DeviceID)) {
        Rpck.Cnt = 0;
        Rpck.Header.IdentityNo = 0;
        if(Rpck.Header.DeviceID < DV_MAX) {
          Rpck.Step++;
        }
        else {
          pstep = Rpck.Step;
          Rpck.Step = ST_SPK_ST;
        }
      }
      break;
    case ST_IDENTITY_NO:
      Rpck.Header.IdentityNo += r<<((Rpck.Cnt++)*8);
      if(Rpck.Cnt >= sizeof(Rpck.Header.IdentityNo)) {
        Rpck.Header.Command = 0;
        Rpck.Cnt = 0;
        if(Rpck.Header.IdentityNo < 1300000000) {
          Rpck.Step++;
        }
        else {
          pstep = Rpck.Step;
          Rpck.Step = ST_SPK_ST;
        }
      }
      break;
    case ST_COMMAND:
      Rpck.Header.Command += r<<((Rpck.Cnt++)*8);
      if(Rpck.Cnt >= sizeof(Rpck.Header.Command)) {
        Rpck.Header.Index = 0;
        Rpck.Cnt = 0;
        Rpck.Step++;
      }
      break;
    case ST_INDEX:
      Rpck.Header.Index += r<<((Rpck.Cnt++)*8);
      if(Rpck.Cnt >= sizeof(Rpck.Header.Index)) {
        Rpck.Header.Count = 0;
        Rpck.Cnt = 0;
        Rpck.Step++;
//        if(Rpck.Header.Index <= INDEX_MAX) {
//          Rpck.Step++;
//        }
//        else {
//          pstep = Rpck.Step;
//          Rpck.Step = ST_SPK_ST;
//        }
      }
      break;
    case ST_COUNT:
      Rpck.Header.Count += r<<((Rpck.Cnt++)*8);
      if(Rpck.Cnt >= sizeof(Rpck.Header.Count)) {
        Rpck.Header.Length = 0;
        Rpck.Cnt = 0;
        Rpck.Step++;
//        if(Rpck.Header.Count <= COUNT_MAX) {
//          Rpck.Step++;
//        }
//        else {
//          pstep = Rpck.Step;
//          Rpck.Step = ST_SPK_ST;
//        }
      }
      break;
    case ST_LENGTH:
      Rpck.Header.Length += r<<((Rpck.Cnt++)*8);
      if(Rpck.Cnt >= sizeof(Rpck.Header.Length)) {
        Rpck.Cnt = 0;
        memset(Rpck.Data,0,sizeof(Rpck.Data));
        if(Rpck.Header.Length <= LENGTH_MAX) {
          Rpck.Step++;
          if(Rpck.Header.Length == 0) {
            Rpck.Cnt = 0;
            Rpck.Footer.CRC16 = 0;
            Rpck.Step++;
          }          
        }
        else {
          pstep = Rpck.Step;
          Rpck.Step = ST_SPK_ST;
        }
      }
      break;
    case ST_DATA:
      Rpck.Data[Rpck.Cnt++] = r;
      if(Rpck.Cnt >= Rpck.Header.Length) {
        Rpck.Cnt = 0;
        Rpck.Footer.CRC16 = 0;
        Rpck.Step++;
      }
      break;
    case ST_CRC16:
      Rpck.Footer.CRC16 += r<<((Rpck.Cnt++)*8);
      if(Rpck.Cnt >= sizeof(Rpck.Footer.CRC16)) {
        if(Rpck.CalcCRC16 == Rpck.Footer.CRC16) {
          Rpck.Step++;
        }
        else {
          pstep = Rpck.Step;
          Rpck.Step = ST_SPK_ST;
        }
      }
      break;
    case ST_EPK_ST:
      if(r == 0xCF) {
        Rpck.Step++;
      }
      else {
        pstep = Rpck.Step;
        Rpck.Step = ST_SPK_ST;
      }
      break;
    case ST_EPK_CF:
      if(r == 0xD0) {
        Rpck.Step++;
        //while

      }
      else {
        pstep = Rpck.Step;
        Rpck.Step = ST_SPK_ST;
      }
      break;
    case ST_PROCESSING:
      //decode after Rpck.Step is initialized
      break;
  }
  return pstep;
}

INT8U ssd[4096];

/*
********************************************************************************
* Description : RxModemDecode
* Arguments   : none
* Return      : 
* Note        : rx modem data decode
******************************************************************************** 
*/
void RxModemDecode(void)
{
	int sectCnt;
	int i, sectOffset;
	INT16U ramCRC=0;
	
  if(Rpck.Step < ST_PROCESSING) return;
  static unsigned char crc_same_flag;
  
  switch(Rpck.Header.Command) {
    case CM_INFO: 
      break;
    case CM_FWIF: 
      memcpy(&FW_Info, Rpck.Data, sizeof(tsFWIF));
			
      printk("\n##### DownLoad Command #####\n");
			printk("File size          : %dBytes\n",FW_Info.Size);
			printk("One Packet size    : %dBytes\n",FW_Info.DivisionSize);
			printk("Download Code Check: 0x%04x\n",FW_Info.CRC16);
      
			memcpy((char *)tPrjcode.boot_seq,"down_gui",8);
			SPI_FLASH_PageErase(PROJECT_BASE);
      
			if(SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t))) 
			{
				/*-------------------------------------------------------------------------------*/	
				// 다운로드 파일 사이즈 만큼의 SPI Flash의 영역을 지운다.
				sectCnt = (FW_Info.Size / SPI_FLASH_BLOCK)+1;
				if(sectCnt > FLASH_DOWN_BLOCKCOUNT) sectCnt = FLASH_DOWN_BLOCKCOUNT;
				sectOffset = DOWNLOAD_SECTOR;

				// Block 크기 삭제 
				for(i=0;i<sectCnt;i++)
				{
					SPI_FLASH_BlockErase(sectOffset);
					sectOffset += SPI_FLASH_BLOCK;
					printk("Erase %X at 0x%05X\n",SPI_FLASH_BLOCK,sectOffset);
				}
				/*-------------------------------------------------------------------------------*/	
				tPrjcode.file_size = FW_Info.Size;
			}
      ssd[0] = 1;
      TxModemData(CM_FWIF, Rpck.Header.Index, Rpck.Header.Count, 1, ssd);
      break;
    case CM_FWSD: 
      {
        u32 naddr = DOWNLOAD_SECTOR + (Rpck.Header.Index * FW_Info.DivisionSize);        
        u16 nsize = FW_Info.DivisionSize;
        if(Rpck.Header.Index < Rpck.Header.Count) {          
          SPI_FLASH_BufferWrite(Rpck.Data, naddr, nsize);
        }
        else {
          crc_same_flag = 0;
          u16 ccrc=0;
          nsize = FW_Info.Size % FW_Info.DivisionSize;
          SPI_FLASH_BufferWrite(Rpck.Data, naddr, nsize);
          for(int i = 0; i < Rpck.Header.Count; i++) {
            naddr = DOWNLOAD_SECTOR + (i * FW_Info.DivisionSize);
            if(i < (Rpck.Header.Count - 1)) { 
              nsize = FW_Info.DivisionSize; 
              SPI_FLASH_BufferRead(ssd, naddr, nsize);
            }
            else {
              nsize = FW_Info.Size % FW_Info.DivisionSize; 
              SPI_FLASH_BufferRead(ssd, naddr, nsize);              
            } 
            ccrc =  GenerateCRC(ssd, nsize, ccrc);
          }
          if(ccrc == FW_Info.CRC16) {
						// 마지막 계산된 CRC 값 저장 -> PROJECT_BASE 번지 
						tPrjcode.file_crc = ccrc;

						// Flash에 저장된 데이터에 대한 CRC 값 Display
						ramCRC = GetFlashAreaCRC(DOWNLOAD_SECTOR, FW_Info.Size);
						printk("Check wrote flash fileCRC: [%04X]\n", (INT16U)ramCRC);

						SPI_FLASH_PageErase(PROJECT_BASE);
						SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));					
            crc_same_flag = 1;
          }
        }  
        
        ssd[0] = 1;
        TxModemData(CM_FWSD, Rpck.Header.Index, Rpck.Header.Count, 1, ssd);      
      }
      break;
    case CM_FWAK:       
      TxModemData(CM_FWAK, Rpck.Header.Index, Rpck.Header.Count, 1, (u8*)&crc_same_flag);  
      
      if(crc_same_flag) {
				memcpy((char*)tPrjcode.boot_seq,"backup",6);
				SPI_FLASH_PageErase(PROJECT_BASE);
				SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));

				dbg_system_reset(); //ResetCtrl=1; 
      }
      break;
    //SaveCtrl.DataAddr = Nand index SaveCtrl.SentAddr = tx address;(fram)  
    case CM_DTSC:  
    case CM_DTSS:  
      {        
        if(Rpck.Data[0]) {
          FRAM_Addr_ReadWrite(SENT_ADDR_SAVE, SAVE);
        }
        FRAM_Addr_ReadWrite(SENT_ADDR_SAVE, NO_SAVE);
        if(ExtFram.DataAddr > (ExtFram.SentAddr + SEND_PAGE_SIZE)) {
          SPI_FLASH_BufferRead(ssd, (ExtFram.SentAddr % FLASH_DATA_MAX), SEND_PAGE_SIZE);
          ExtFram.SentAddr += SEND_PAGE_SIZE;
//          TxModemData(CM_DTSS, ((ExtFram.SentAddr+0) % 10000), (ExtFram.DataAddr % 10000), SEND_PAGE_SIZE, ssd);            
          TxModemData(CM_DTSS, 0, 0, SEND_PAGE_SIZE, ssd);            
        }
        else {
          UART4_print("AT$$TCP_PPPCL\r");   
          Md.Process = 1;        
        }      
      }
      break;   
  }  
  Rpck.Step = 0;
}

/*
********************************************************************************
* Description : CDMA_parse_proc
* Arguments   : none
* Return      : 
* Note        : CDMA Data parsing process
******************************************************************************** 
*/
void CDMA_parse_proc(void)	
{
	INT8U err;
	char data = 0;
     
	while( 1 ) {
		data = CommGetChar(EXT_PORT,10, &err);
		if ( err == COMM_NO_ERR ) {
      ModemRcvParser(data);
      RxModemChar(data);
      if(Md.Tell != TCP_CONNECTED)
        put_ch_console(data);
    }
		else break;
	}
}

/*
********************************************************************************
* Description : Task_CDM
* Arguments   : none
* Return      : 
* Note        : task for cdma
******************************************************************************** 
*/
void Task_CDM(void *p_arg)
{
  INT32U timecnt;
	
  sysinfo_t *s = (sysinfo_t *)p_arg;
  
//	BSP_UART4RX_IntEnable(ENABLE);

//  GSM_ONOFF(OFF);        
//// // vTaskDelay(1000);
//  vTaskDelay(100);
//  GSM_ONOFF(ON);     // GSM OFF --> MODEM ON  
//  SecData.Status.ModemReset = 1;
//  vTaskDelay(1000);
//  
//  UART4_print("AT$$MODE=3\r");   
//  printd("CDMA Start\n");

  
  for(;;)
	{
		s->twatchCdmFlag = 1; 
    
//    CDMA_parse_proc();
    
    if(Flags.StartFlag) {
      timecnt++;
//      RxModemDecode();
//      ModemControl(timecnt);
    }

		s->twatchCdmFlag = 0;        
    vTaskDelay(2);
	}
}
