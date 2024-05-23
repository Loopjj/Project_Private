/*
*************************************** (C) COPYRIGHT 2018 LOOP *************************************
*
* File Name          : rtc.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 02/01/2018
*
* Description        : rtc 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define BLE_GLOBALS
#include "includes.h"
      
/* Private define ------------------------------------------------------------*/

typedef enum {
  STEP_START,
  STEP_LENGTH1,
  STEP_LENGTH2,
  STEP_DATA,
  STEP_CHECK,
  STEP_END,
  STEP_WAIT
}st_BLE_Step;

typedef enum {
  BLE_COMM_CHECK,
  BLE_RF_CHECK,
  BLE_ADVER_NAME,
  BLE_DEVICE_NAME,
  BLE_RESET,
  BLE_INIT_OK,
  BLE_INIT_ERROR
}st_BLE_AT_Step;

typedef struct{
	INT32U ulFileSize;
	INT16U uSOnePacketSize;
	INT16U uSTotalPacketCount;
	INT16U uCheckCode;
	INT16U mode;
	INT16U expectedCount;
	INT16U sectorCount;
	char dl_good_finished;
	INT16U uSLastPacketSize;
	INT16U last_CRC16;
}dl_t;

#define STX     0x7E
#define ETX     0x7F

//---------------------------------------------------------------------- command
#define TYPE_HREPORT				0x10
#define TYPE_HSET					  0x20
#define TYPE_LREQUEST				0x01
#define TYPE_LRESPONSE			0x02

//------------------------------------------------------------------ gui command

// download command
#define CMD_DLSTART					0x10
#define CMD_DLPROGRESS			0x11
#define CMD_DLEND					  0x12
#define CMD_DLMODE_NORMAL		0x1F

#define RESPONSE_WAIT       3000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
INT8U m_ucRxStep;
INT16U m_ucRxLength, m_ucRxDataCnt, m_ucCheckSum;
st_BufPara BLEPara;
stBleIsgSetting BleIsgSetting;
dl_t dl;

INT8U BLE_Step, Pro_Step;
int BLEWaitCnt;
INT8U AT_SetFlag = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//#define AT_RETRY_CNT 5

char BLE_ATCmdRxMsg[3][32];
char BLE_AT_Response[100];
int BLE_ModuleWaitCnt;
INT32U  BLE_Current, BLE_Previous;
/*
********************************************************************************
* Description : millis
* Arguments   : none
* Return      : 
* Note        : miliis time 
******************************************************************************** 
*/
uint32_t __millis(void) 
{ 
 	return __millisCounter; 
}

void init__millis(void) 
{ 
 	__millisCounter = 0; 
}

/*
********************************************************************************
* Description : BLE_sendATcommand
* Arguments   : ATcommand, answer1, answer2, timeout
* Return      : 
* Note        : AT command
******************************************************************************** 
*/
void BLE_TxStrToModem(char* str)
{
  u8 len;
  
  len = StringLen(str,100);
  UART_SendDataStr(&huart1, (u8*)str, len);
#ifdef BLE_LOG_ENABLE  
  UART_SendDataStr(DEBUG_UART_PORT,(u8*)str,len);
#endif     
}

/*
********************************************************************************
* Description : BLE_sendATcommand
* Arguments   : ATcommand, answer1, answer2, timeout
* Return      : 
* Note        : AT command
******************************************************************************** 
*/

int8_t BLE_sendATcommand(char* ATcommand, char* expected_answer1,
        char* expected_answer2, unsigned int timeout){
	INT16U tmptail;   
	char data = 0;          
  u8 x=0,  answer=0;
  u32  current, previous;
  
  memset(BLE_AT_Response, ' ', 100);
  
  // Send the AT command    
  BLE_TxStrToModem(ATcommand);

  x = 0;
  init_millis();
  previous = millis();

  // loop waits for the answer
  do{
    if(BLE_RxHead != BLE_RxTail ) { 
      tmptail = (BLE_RxTail + 1) & BLE_RX_BUFFER_MASK; /* Calculate buffer index */
      
      BLE_RxTail = tmptail;                /* Store new index */
      
      data = BLE_RxBuf[tmptail];           /* Return data */ 
#ifdef BLE_LOG_ENABLE
      UART_SendData(DEBUG_UART_PORT,data);
#endif   
      BLE_AT_Response[x] = data;
      if(x < 99)  x++;
      else {
#ifdef BLE_LOG_ENABLE        
        printk("\r\nBLE : BLE_sendATcommand 100 bytes over ERROR\r\n");              
#endif        
        return 0;        
      }
      // check answer 1
      if (strstr(BLE_AT_Response, expected_answer1) != NULL)
      {
          answer = 1;
      }
      // check answer 2
      else if (strstr(BLE_AT_Response, expected_answer2) != NULL)
      {
          answer = 2;
      }
    }
    current = millis();
    vTaskDelay(1);    
  }
  while((answer == 0) && ((current-previous) < timeout));

  return answer;
}

/*
********************************************************************************
* Description : AT_SendWithRecievedData
* Arguments   : cmd, cmdResp, valNum, timeout
* Return      : resp
* Note        : AT command + Recieve data
******************************************************************************** 
*/
int BLE_AT_SendWithRecieveData(char* cmd, char* cmdResp, u8 valNum, unsigned int timeout) 
{
	INT16U tmptail;   
  u32  current, previous;
  int i, idx = 0, val_idx = 0, msg_len = 0;
  u8 str_flag = 0, data, data_flag = 0;
  u8 resp = 0;

  memset(BLE_AT_Response, 0, 100);
  
  for(i = 0; i < 3; i++) {
    memset(BLE_ATCmdRxMsg[i], 0, 32);
  }
 
  BLE_TxStrToModem(cmd);    
  
  init_millis();
  previous = millis();
  do{
    if(BLE_RxHead != BLE_RxTail ) { 
      tmptail = (BLE_RxTail + 1) & BLE_RX_BUFFER_MASK; /* Calculate buffer index */
      
      BLE_RxTail = tmptail;                /* Store new index */
      
      data = BLE_RxBuf[tmptail];           /* Return data */ 
#ifdef BLE_LOG_ENABLE
      UART_SendData(DEBUG_UART_PORT,data);
#endif        
      if(data_flag == 0) {
        if(idx < 99) {
          BLE_AT_Response[idx++] = data;
        }
        else {
#ifdef BLE_LOG_ENABLE          
          printk("\r\nBLE : BLE_AT_SendWithRecieveData 100 bytes over ERROR\r\n");              
#endif          
          resp = 0;        
          data_flag = 2;
        }
        if (strstr(BLE_AT_Response, cmdResp) != NULL)
          data_flag = 1;
      }
      else if(data_flag == 1) {
        if(data == '\r') {
            resp = 1;
            break;
        }
        else if(data == ',') {
          if(val_idx < valNum && val_idx < 9) {
            val_idx++;
            msg_len = 0;
          }
          else {
            resp = 1;
            data_flag = 2;
          }
          str_flag = 0;
        }
        else if(data == '\"') {
          if(str_flag) str_flag = 0;
          else str_flag = 1;
        }
        else {
          if(msg_len < 32) {
            if(!(msg_len == 0 && data == ' '))
              BLE_ATCmdRxMsg[val_idx][msg_len++] = data;
          }
        }
      }
      else {
        break;//idle
      }
    }
    current = millis();
    vTaskDelay(1);
  }
  while(((current-previous) < timeout));

  return resp;
}

/*
********************************************************************************
* Description : BLE_CommCheck
* Arguments   : 
* Return      : 
* Note        : Modem Power ON
******************************************************************************** 
*/
unsigned char BLE_CommCheck(void)
{
  INT8U answer=0, sendcnt=0;

  while(answer != 1 && sendcnt < 5) { 
    answer = BLE_sendATcommand("AT+VER?\r", "+OK", "+ERROR", 1000);
    sendcnt++;
  }
  
  return answer;
}

/*
********************************************************************************
* Description : BLE_Reset
* Arguments   : 
* Return      : 
* Note        : Modem Reset
******************************************************************************** 
*/
unsigned char BLE_Reset(void)
{
  INT8U answer=0, sendcnt=0;

  while(answer != 1 && sendcnt < 5) { 
    answer = BLE_sendATcommand("ATZ\r", "+OK", "+ERROR", 1000);
    sendcnt++;
  }
  
  return answer;
}

/*
********************************************************************************
* Description : BLE_Adver_Name
* Arguments   : resp
* Return      : 
* Note        : BLE local name set
******************************************************************************** 
*/
unsigned char BLE_Adver_Name(void)
{
  INT8U resp, rcnt;
  char tmpATcmd[32];
  
  rcnt=0;  resp=0;

  memset(tmpATcmd, 0, 32);
  sprintf(tmpATcmd, "LDT300BS_%d%d%d%d", Setting.Car_Reg_No[5], Setting.Car_Reg_No[6], Setting.Car_Reg_No[7],  Setting.Car_Reg_No[8]);
        
  resp = BLE_AT_SendWithRecieveData("AT+ADV_NAME?\r", tmpATcmd, 0, 300);
  if(resp == 0) {
    memset(tmpATcmd, 0, 32);    
    sprintf(tmpATcmd, "AT+ADV_NAME=LDT300BS_%d%d%d%d\r", Setting.Car_Reg_No[5], Setting.Car_Reg_No[6], Setting.Car_Reg_No[7],  Setting.Car_Reg_No[8]);
    while(resp != 1 && rcnt < AT_RETRY_CNT) { 
      resp = BLE_sendATcommand(tmpATcmd, "+OK", "+ERROR", 300);
      AT_SetFlag = 1;
      rcnt++;
    }  
    if(rcnt >= AT_RETRY_CNT) {
      return resp;
    }
  }
  return resp;  
}


/*
********************************************************************************
* Description : BLE_Device_Name
* Arguments   : resp
* Return      : 
* Note        : BLE device name set
******************************************************************************** 
*/
unsigned char BLE_Device_Name(void)
{
  unsigned char resp,rcnt;
    
  rcnt=0;  resp=0;
  
  resp = BLE_AT_SendWithRecieveData("AT+DEVICE_NAME?\r", "LDT-300BS", 0,  300);
  if(resp == 0) {
    while(resp != 1 && rcnt < AT_RETRY_CNT) { 
      resp = BLE_sendATcommand("AT+DEVICE_NAME=LDT-300BS\r", "+OK", "+ERROR", 300); 
      AT_SetFlag = 1;
      rcnt++;
    }  
    if(rcnt >= AT_RETRY_CNT) {
      return resp;
    }
  }
  return resp;  
}

/*
********************************************************************************
* Description : BLE_RF_Power
* Arguments   : resp
* Return      : 
* Note        : BLE rf power check
******************************************************************************** 
*/
unsigned char BLE_RF_Power(void)
{
  unsigned char resp,rcnt;
  u8 val1, val2;    
  rcnt=0;  resp=0;
  
  resp = BLE_AT_SendWithRecieveData("AT+TX_PWR?\r", "+OK\r\n", 2,  300);
  val1 = AsciiToVal(BLE_ATCmdRxMsg[0]);
  val2 = AsciiToVal(BLE_ATCmdRxMsg[1]);

  if(val1 != 1 || val2 != 7) {
    resp = 0;    
    while(resp != 1 && rcnt < AT_RETRY_CNT) { 
      resp = BLE_sendATcommand("AT+TX_PWR=1,7\r", "+OK", "+ERROR", 300); 
      AT_SetFlag = 1;
      rcnt++;
    }  
    if(rcnt >= AT_RETRY_CNT) {
      return resp;
    }
  }
  
  return resp;  
}
/*
********************************************************************************
* Description : Module_ATCommand_proc
* Arguments   : none
* Return      : 
* Note        : Modlue AT Command proc
******************************************************************************** 
*/
void BLE_Init(void)
{
  unsigned char resp = 0;
 
  switch(BLE_Step) {
    case BLE_COMM_CHECK:        // Module ver check
      resp = BLE_CommCheck();
      if(resp == 1) BLE_Step = BLE_RF_CHECK;
      else BLE_Step = BLE_INIT_ERROR;
      break;
    case BLE_RF_CHECK:          // Module rf power check
      resp = BLE_RF_Power();
      if(resp == 1) BLE_Step = BLE_ADVER_NAME;
      else BLE_Step = BLE_INIT_ERROR;    
      break;
    case BLE_ADVER_NAME:        // Module advertising name check
      resp = BLE_Adver_Name();
      if(resp == 1) BLE_Step = BLE_DEVICE_NAME;
      else BLE_Step = BLE_INIT_ERROR;
      break;
    case BLE_DEVICE_NAME:       // Module device name check
      resp = BLE_Device_Name();
      if(resp == 1) {
        if(AT_SetFlag) BLE_Step = BLE_RESET;
        else BLE_Step = BLE_INIT_OK;
      }
      else BLE_Step = BLE_INIT_ERROR;
      break;
    case BLE_RESET:             // Module reset
      resp = BLE_Reset();
      if(resp == 1) BLE_Step = BLE_INIT_OK;
      else BLE_Step = BLE_INIT_ERROR;
      break;
    case BLE_INIT_OK:
      BLE_InitFLag = 1;
      break;
    case BLE_INIT_ERROR:
      BLE_InitFLag = 0;
      break;
  }
}

/*
********************************************************************************
* Description : UART_SendDataStr
* Arguments   : handlie , data, len
* Return      : 
* Note        : Send data
******************************************************************************** 
*/
#define BLE_MAX_TXCNT 20
void BLE_SendDataStr(uint8_t *data, int len)
{
  int i, dev_cnt, mod_cnt;
  uint8_t sbuf[BLE_MAX_TXCNT];
  
  dev_cnt = len / BLE_MAX_TXCNT;
  mod_cnt = len % BLE_MAX_TXCNT;

  for(i = 0; i < dev_cnt; i++) {
    memcpy(sbuf, &data[i * BLE_MAX_TXCNT], BLE_MAX_TXCNT);
    UART_SendDataStr(&huart1, (u8*)sbuf, BLE_MAX_TXCNT);
    osDelay(2);
  }
                       
  if(mod_cnt != 0) {
    memcpy(sbuf, &data[dev_cnt * BLE_MAX_TXCNT], mod_cnt);
    UART_SendDataStr(&huart1, (u8*)sbuf, mod_cnt);
    osDelay(2);
  }
}

/*
********************************************************************************
* Description : UART_SendDataStr
* Arguments   : handlie , data, len
* Return      : 
* Note        : Send data
******************************************************************************** 
*/
uint8_t Sbuf[320];
void BLE_Send_Data(uint8_t cmd, uint8_t msgtype, uint8_t systype, uint8_t *data, int length) 
{
  int i, slength, checksum = 0;

  slength = (length + 3);
    
  Sbuf[0] = STX;
  Sbuf[1] = (uint8_t)(slength & 0xFF);
  Sbuf[2] = (uint8_t)(slength>>8 & 0xFF);
  Sbuf[3] = cmd;
  Sbuf[4] = msgtype;
  Sbuf[5] = systype;

  for(i = 0; i < length; i++) {
      Sbuf[i + 6] = data[i];
  }
  for(int i = 0; i < (length + 5); i++) {
      checksum += Sbuf[i + 1];
  }
  Sbuf[length + 6] = (uint8_t)(checksum & 0xFF);
  Sbuf[length + 7] = ETX;

  BLE_SendDataStr(Sbuf, length + 8);
#ifdef BLE_LOG_ENABLE  
  UART_SendDataStr(DEBUG_UART_PORT,Sbuf, length + 8);
#endif     
}

/*
********************************************************************************
* Description : BLE_Data_Decode
* Arguments   : none
* Return      : 
* Note        : BLE Data parsing process
******************************************************************************** 
*/
#define USER_PROGRAM_MAX_SIZE (0x80000-0x8000)
#define USER_PROGRAM_MIN_SIZE 0x100
u32 Nand_Rx_Index, Nand_Addr;

void BLE_Data_Decode(void)
{
  u32 addr;
  u8 dummy = 0, rc = 0;
	INT8U nMsgType=0, errReport=0;
	char tempbuff[4]={0};  
	INT16U packetcount, rcvLastPacketNum;
  u16 sendCount;
  
  Pro_Step = BLE_MSG_RxBuf[0]; 
  switch(Pro_Step) {
	case CMD_DLSTART:
		if(BLE_MSG_RxBuf[1] == (TYPE_HSET | TYPE_LREQUEST)){
		{
				// 파일 사이즈 4Byte copy
				memcpy((char *)&(dl.ulFileSize), (INT8U*)&BLE_MSG_RxBuf[3], sizeof(dl.ulFileSize));
				// 한 패킷 사이즈 2Byte copy 
				memcpy((char *)&dl.uSOnePacketSize, (INT8U*)&BLE_MSG_RxBuf[3+sizeof(dl.ulFileSize)] , sizeof(dl.uSOnePacketSize));
				// 총 패킷 사이즈 2Byte copy
				memcpy((char *)&dl.uSTotalPacketCount, (INT8U*)&BLE_MSG_RxBuf[3+sizeof(dl.ulFileSize) + sizeof(dl.uSOnePacketSize)], sizeof(dl.uSTotalPacketCount));

				memcpy((char *)&dl.uCheckCode, (INT8U*)&BLE_MSG_RxBuf[3+sizeof(dl.ulFileSize) + sizeof(dl.uSOnePacketSize) + sizeof(dl.uSTotalPacketCount)], sizeof(dl.uCheckCode));//4: file size, 2:One's Packet size, 2 : Total Packet Size
#ifdef BLE_LOG_ENABLE
				printk("\n##### DownLoad Command #####\n");
				printk("File size          : %dBytes\n",dl.ulFileSize);
				printk("One Packet size    : %dBytes\n",dl.uSOnePacketSize);
				printk("Total Packet Count : %dPacket\n",dl.uSTotalPacketCount);
				printk("Last Packet Byte   : %dBytes\n",dl.ulFileSize % dl.uSOnePacketSize);
				printk("Download Code Check: %dBytes\n",dl.uCheckCode);
#endif        
				// 전체 패킷 갯수 = (파일 사이즈 / 한 패킷 사이즈) + (파일 사이즈 % 한 패킷 사이즈) 
				dl.uSLastPacketSize = dl.ulFileSize / dl.uSOnePacketSize + (dl.ulFileSize % dl.uSOnePacketSize ? 1 : 0);
				
				if(dl.uSLastPacketSize == dl.uSTotalPacketCount)		// 전체 패킷 갯수와 총 패킷 사이즈가 같으면 	
				{
#ifdef BLE_LOG_ENABLE          
					printk("File size, Packet size, TotalPacket count Ok..\n");
#endif          
					errReport = 1;
					dl.mode = CMD_DLPROGRESS;
					dl.expectedCount = 1;								// 다음 페킷 카운트의 숫자

          if(dl.ulFileSize > USER_PROGRAM_MIN_SIZE &&  dl.ulFileSize < USER_PROGRAM_MAX_SIZE) {
            Nand_Addr = NAND_PROGRAM_ADDR; 
            memset(NandTmpBuffer, 0, NAND_PAGE_SIZE); 
            Nand_Rx_Index = 0;
						dl.last_CRC16 = 0;	
					}
					else
					{
						errReport = 3;
					}
				}
				else
				{
					errReport = 2;										// total packet size error
				}
			}
			if(errReport != 1) dl.mode = CMD_DLMODE_NORMAL;

      BLE_Send_Data(BLE_MSG_RxBuf[0], (TYPE_HSET | TYPE_LRESPONSE), BLE_MSG_RxBuf[2], (uint8_t *)&errReport, sizeof(errReport));
		}
		break;
   
	case CMD_DLPROGRESS:
		if(dl.mode != CMD_DLPROGRESS){
			return ;
		}

		if(BLE_MSG_RxBuf[1] == (TYPE_HSET | TYPE_LREQUEST)) {
			errReport = 0;
			// 패킷 갯수 2Byte copy
			memcpy(&packetcount, (INT8U*)&BLE_MSG_RxBuf[3], sizeof(packetcount));
			// 예상 카운트와 패킷 카운트가 다르면 에러 처리  
			if(dl.expectedCount != packetcount) {
				errReport = 3;
			}
			else {
				if(packetcount == dl.uSTotalPacketCount) { // 데이터를 모두 받을 경우 진입 	
					dl.mode = CMD_DLEND;
					dl.uSLastPacketSize = dl.ulFileSize % dl.uSOnePacketSize;
					if(dl.uSLastPacketSize <= (m_ucRxLength - 5)) {			// 'packet number' 2byte를 뺀다	
						if(packetcount != 0){
							if(dl.uSLastPacketSize == 0)						  // 계산이 0이면 한패킷 풀로 보낸다
								dl.uSLastPacketSize = dl.uSOnePacketSize;
							
              memcpy((INT8U*)&NandTmpBuffer[Nand_Rx_Index], (INT8U*)&BLE_MSG_RxBuf[3 + 2], dl.uSLastPacketSize);
              Nand_Rx_Index += dl.uSLastPacketSize;
							errReport = 1;
							
              Nand_Write_Buffer(NandTmpBuffer, Nand_Addr, Nand_Rx_Index);
              dl.last_CRC16 = getcrc16(dl.last_CRC16, NandTmpBuffer, Nand_Rx_Index);
              Nand_Addr += Nand_Rx_Index;
#ifdef BLE_LOG_ENABLE              
							// Ram에서 계산된 CRC 값 Display
							printk("Check calculated ram fileCRC: [%04X]\n", dl.last_CRC16);
#endif              
						}
						else 
						{
							errReport = 2;
						}
					}
					else{
						errReport = 3;
					}
				}
				else { // 데이터를 받는 중...
					memcpy((INT8U*)&NandTmpBuffer[Nand_Rx_Index], (INT8U*)&BLE_MSG_RxBuf[3 + 2], dl.uSOnePacketSize);  // Packet No 빼고
          Nand_Rx_Index += dl.uSOnePacketSize;
					errReport = 1;

					// OnePacket 단위로 Write를 실행한다.... 
          if(Nand_Rx_Index >= NAND_PAGE_SIZE) {
            Nand_Write_Buffer(NandTmpBuffer, Nand_Addr, NAND_PAGE_SIZE);
            dl.last_CRC16 = getcrc16(dl.last_CRC16, NandTmpBuffer, NAND_PAGE_SIZE);
            Nand_Addr += NAND_PAGE_SIZE;
            Nand_Rx_Index = 0;
            memset(NandTmpBuffer, 0, NAND_PAGE_SIZE);
          }
				}	
/*-----------------------------------------------------------------------------------------------------*/	
				dl.expectedCount++;
			}
			if(errReport != 1) dl.mode = CMD_DLMODE_NORMAL;

			memcpy(tempbuff, (INT8U*)&BLE_MSG_RxBuf[3], 2);  // Packet No
			tempbuff[2] = errReport;                      // Satus
      BLE_Send_Data(BLE_MSG_RxBuf[0], TYPE_HSET | TYPE_LRESPONSE,  BLE_MSG_RxBuf[2], (uint8_t*)tempbuff, 3);
		}
		break;

	case CMD_DLEND:
		if(dl.mode != CMD_DLEND){
			return;
		}

		if(BLE_MSG_RxBuf[1] == (TYPE_HSET | TYPE_LREQUEST)) {
			// 마지막 패킷 갯수 2Byte copy
			memcpy((char *)&rcvLastPacketNum, (INT8U*)&BLE_MSG_RxBuf[3], 2);
			nMsgType = (TYPE_HSET | TYPE_LRESPONSE);

			if((dl.expectedCount-1) == rcvLastPacketNum){ // 정상 
				errReport = 1;
        BLE_Send_Data(BLE_MSG_RxBuf[0], nMsgType, BLE_MSG_RxBuf[2], (uint8_t *)&errReport, 1);
        
				dl.dl_good_finished = 1;
#ifdef BLE_LOG_ENABLE        
				printk("Good\n");
#endif        
			}
			else{
				errReport = 2;
				BLE_Send_Data(BLE_MSG_RxBuf[0], nMsgType, BLE_MSG_RxBuf[2], (uint8_t *)&errReport, 1);
#ifdef BLE_LOG_ENABLE        
				printk("Not matched\n");
#endif        
				dl.dl_good_finished = 0;
			}

			if(errReport != 1) dl.mode = CMD_DLMODE_NORMAL;

			if(dl.dl_good_finished == 1){
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 10); // JUMP
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, dl.ulFileSize);
        NVIC_SystemReset(); 
			}
		}
		break;
    
	case 0x39:        // 가상 속도 
		break;
    
  case 0x58:        // 접속 주기
    if(BLE_MSG_RxBuf[1] == 0x11) {
      sendCount = sizeof(u16);
      memcpy((u8*)&MtTxBuff, (u8*)&ModemInfo.DataServerInterval, sendCount);
      BLE_Send_Data(0x58, 0x12, 0x01,(u8*)&MtTxBuff, sendCount);	           
    }
    else {
      ModemInfo.DataServerInterval = 0;
      memcpy((u8 *)&ModemInfo.DataServerInterval, (u8 *)&BLE_MSG_RxBuf[3], sizeof(u16));
      addr = MODEM_INFO + (u8 *)&ModemInfo.DataServerInterval - (u8 *)&ModemInfo;
      FRAMMultiWrite((u8 *)&ModemInfo.DataServerInterval, addr, sizeof(ModemInfo.DataServerInterval));
      BLE_Send_Data(0x58, 0x22, 0x01, &dummy, 1);      
    }  
    break;
      
  case 0x5C:
    break;
  case 0x5D:
    BLE_SendFlag = BLE_MSG_RxBuf[3];
    break;
  case 0x5E: 
    BLE_Send_Data(0x5E, 0x12, 0x01, (uint8_t *)&CAN_Enable, sizeof(stCanEnable));
    break;
  case 0x5F:
    break;      
  case 0x63:        // 호스트 서버  설정
    if(BLE_MSG_RxBuf[1] == 0x11) {
      sendCount = StringLen(HostSvr.SvrName, 60);;
      memcpy(MtTxBuff, HostSvr.SvrName, sendCount);
      memcpy((u8*)&MtTxBuff[sendCount], (u8*)&HostSvr.SvrPort, 4);
      BLE_Send_Data(0x63, 0x12, 0x01, (u8*)&MtTxBuff, sendCount+4);	           
    }
    else {
      memset(HostSvr.SvrName,0,60);
      memcpy(HostSvr.SvrName, (u8 *)&BLE_MSG_RxBuf[3], m_ucRxLength-7);
      memset((char*)&HostSvr.SvrPort,0, 4);
      memcpy((u8*)&HostSvr.SvrPort, (u8*)&BLE_MSG_RxBuf[m_ucRxLength-4], 4);
      FRAMMultiWrite((u8 *)&HostSvr, DATA_SERVER_INFO, sizeof(tsSvrInfo));
      if(Setting.ExtModemMode == LOOP_PROTOCOL) {
        rc = IpToInt(HostSvr.SvrName);
        if(rc) {
          memcpy(&ModemInfo.LpIP,IP,sizeof(IP)); 
          addr = MODEM_INFO + offsetof(tsModemInfo,LpIP);   
          FRAMMultiWrite((u8 *)&ModemInfo.LpIP, addr, sizeof(IP)); 
          ModemInfo.LpPort =  HostSvr.SvrPort;
          addr = MODEM_INFO + offsetof(tsModemInfo,LpPort);   
          FRAMMultiWrite((u8 *)&ModemInfo.LpPort, addr, sizeof(ModemInfo.LpPort));  
        }        
      }
      SendSettingFlag = 1;    
      BLE_Send_Data(0x63, 0x22, 0x01, &dummy, 1);
    }    
    break;    
    
  case 0x70:    // Info 요청
    if(BLE_MSG_RxBuf[1] == 0x11) {
      if(IsSameString((char*)&Setting.PinCode,(char*)&BLE_MSG_RxBuf[3], 6)) {
        BLE_SendHeaderFlag = 1;
      }
      else {
        BLE_Send_Data(0x70, 0x12, 0x01, &dummy, 0);
      }
      BLE_SmartWaitFlag = 0;
#ifdef BLE_LOG_ENABLE      
      printk("\r\nBLE : cmd=0x%x, msg=0x%x, sys=0x%x\r\n",BLE_MSG_RxBuf[0],BLE_MSG_RxBuf[1],BLE_MSG_RxBuf[2]);
#endif      
    }
    break;
  case 0x71:    // 데이터 요청/중지
    if(BLE_MSG_RxBuf[1] == 0x11) {    
      if(BLE_MSG_RxBuf[3] == 0) {
        BLE_SmartFlag = 0;
      }
      else {
        FRAMMultiRead((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));
        DataAddrToSend = DataSaveAddr;
        BLE_SmartFlag = 1;
        BLE_SmartInfoFlag = 1;   
      }
      BLE_SmartWaitFlag = 0;
#ifdef BLE_LOG_ENABLE      
      printk("\r\nBLE : cmd=0x%x, msg=0x%x, sys=0x%x\r\n",BLE_MSG_RxBuf[0],BLE_MSG_RxBuf[1],BLE_MSG_RxBuf[2]);
#endif      
    }
    break;
  case 0x72:    // 데이터 전송
    if(BLE_MSG_RxBuf[1] == 0x12) {
      if(BLE_MSG_RxBuf[2] == 0 || BLE_MSG_RxBuf[2] == 2) {  // 0 : Ack, 2 : NAck
        BLE_SmartFlag = 1;
      }
      else {                                                // 1 : 종료
        BLE_SmartFlag = 0;
      }
      if(BLE_MSG_RxBuf[2] < 2)
        FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));     
      BLE_SmartWaitFlag = 0;
#ifdef BLE_LOG_ENABLE      
      printk("\r\nBLE : cmd=0x%x, msg=0x%x, sys=0x%x\r\n",BLE_MSG_RxBuf[0],BLE_MSG_RxBuf[1],BLE_MSG_RxBuf[2]);
#endif      
    }
    break;
  case 0x73:    // 이벤트 전송
    if(BLE_MSG_RxBuf[3] == 1)
//      BleServerFlag = 0;
    BLE_SmartWaitFlag = 0;
#ifdef BLE_LOG_ENABLE    
      printk("\r\nBLE : cmd=0x%x, msg=0x%x, sys=0x%x\r\n",BLE_MSG_RxBuf[0],BLE_MSG_RxBuf[1],BLE_MSG_RxBuf[2]);
#endif    
    break;
  }
}

/*
********************************************************************************
* Description : BLE_parse_proc
* Arguments   : none
* Return      : 
* Note        : BLE Data parsing process
******************************************************************************** 
*/
void BLE_parse_proc(void)	
{
	INT16U tmptail;  
	char rdata;
  INT8U errReport=0;
  
	while(BLE_RxHead != BLE_RxTail ) { 
    tmptail = (BLE_RxTail + 1) & BLE_RX_BUFFER_MASK; /* Calculate buffer index */
    
    BLE_RxTail = tmptail;                /* Store new index */
    
    rdata = BLE_RxBuf[tmptail];           /* Return data */ 
#ifdef BLE_LOG_ENABLE
      UART_SendData(DEBUG_UART_PORT,rdata);
#endif  
    switch (m_ucRxStep) {
    case STEP_START:
      if(rdata == STX) {
        m_ucRxStep = STEP_LENGTH1;
        m_ucRxDataCnt = 0;
        m_ucCheckSum = 0;        
        init__millis();
        BLE_Previous = __millis();
      }
      else
        m_ucRxStep = STEP_START;
      break;
    case STEP_LENGTH1:
      m_ucRxLength = rdata;
      m_ucCheckSum = rdata;
      m_ucRxStep = STEP_LENGTH2;
      break;
    case STEP_LENGTH2:
      m_ucRxLength += (rdata << 8);
      m_ucCheckSum += rdata;
      if(m_ucRxLength < 4096)
        m_ucRxStep = STEP_DATA;
      else m_ucRxStep = STEP_WAIT;
      break;
    case STEP_DATA:
      if(m_ucRxLength < m_ucRxDataCnt)
        m_ucRxStep = STEP_START;
      else {
        BLE_MSG_RxBuf[m_ucRxDataCnt++] = rdata;
        m_ucCheckSum += rdata;
        if(m_ucRxLength == m_ucRxDataCnt)
          m_ucRxStep = STEP_CHECK;
      }
      break;
    case STEP_CHECK:
      if(rdata == (char)(m_ucCheckSum & 0xFF))
        m_ucRxStep = STEP_END;
      else m_ucRxStep = STEP_WAIT;
      break;
    case STEP_END:
      if(rdata == ETX) {
        BLE_InitFLag = 1;
        BLE_Data_Decode();
      }
      m_ucRxStep = STEP_START;
      break;
    case STEP_WAIT:
      m_ucRxStep = STEP_START;
      break;
    }
  }
  
  if((m_ucRxStep > STEP_START) && (m_ucRxStep < STEP_END) ) {
    BLE_Current = __millis();
    if((BLE_Current - BLE_Previous) > RESPONSE_WAIT) {
      HAL_UART_Receive_IT(&huart1, &RxUSART1, 1);
      if(Pro_Step == CMD_DLPROGRESS)
        BLE_Send_Data(0x13, (TYPE_HSET | TYPE_LRESPONSE),  0x01, &errReport, 1);
      m_ucRxStep = STEP_START;
    }
  }
}

/*
********************************************************************************
* Description : SecData Send
* Arguments   : INT8U *pbuf
* Return      : 
* Note        : SecData Send
******************************************************************************** 
*/

void BLE_SecData_Send(void)
{
  BLE_Send_Data(0x5D, 0x12, 0x01, (INT8U*)&SecData, sizeof(tsSecData));
}

/*
********************************************************************************
* Description : PinCode_Generation
* Arguments   : None
* Return      : 
* Note        : PinCode for BLE
******************************************************************************** 
*/    
void BLE_PinCode_Generation(void)
{
  int i;

  srand(millis());  
  for(i=0; i<6; i++) {
    BLE_PinCode[i] = (rand() % 10);
  }
}

/*
********************************************************************************
* Description : BLE_Send_SmartData
* Arguments   : INT8U mode
* Return      : 
* Note        : SecData Send
******************************************************************************** 
*/
#define BLE_TX_LIMIT_ENABLE   0
#define BLE_DATA_TX_LIMIT     57600 // 15분 데이터 //38400  // 10분데이터 //1382400   //6시간x 60분 x 60초 x sizeof(tsSecData)
#define BLE_SENDDATA_MAX      256  //Max 4096
st_Setting SendSetting;
void BLE_Send_SmartData(u8 mode)
{
  unsigned long dlen, i, data_cnt, size, data_ok_cnt;  
  
  DataSvrWaitCount = 0;
  
  if(mode == 0) {
    memcpy(&Setting.Lat, &SecData.LT, 10);
    memcpy((INT8U*)&SendSetting,(INT8U*)&Setting, sizeof(st_Setting));
    memcpy((INT8U*)&SendSetting.Car_Reg_No, VRN, 12);
//    memcpy((INT8U*)&SendSetting.CNUM, (u8 *)&ModemInfo.CNUM, 4);
//    SendSetting.Car_Type = Setting.Car_Reg_No[0];
    BLE_Send_Data(0x70, 0x12, 0x01, (INT8U*)&SendSetting, sizeof(st_Setting));
    BLE_SmartWaitFlag = 1;
    BLE_SendHeaderFlag = 0;
    FRAMMultiRead((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));
    DataAddrToSend = DataSaveAddr;
    if(DataAddrToSend >= DataSentAddr)
      dlen = DataAddrToSend-DataSentAddr;
    else
      dlen = NAND_SIZE_DATA+DataAddrToSend-DataSentAddr;
#if BLE_TX_LIMIT_ENABLE    
    if(dlen > BLE_DATA_TX_LIMIT) {
      if(DataAddrToSend >= DataSentAddr) 
        DataSentAddr = DataAddrToSend - BLE_DATA_TX_LIMIT;
      else
        DataSentAddr = (NAND_SIZE_DATA + DataAddrToSend - BLE_DATA_TX_LIMIT)%NAND_SIZE_DATA;
      FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));
    }
#endif    
    if(Setting.ExtModemMode != EXT_MODEM_PROTOCOL_TYPE) {
      Setting.ExtModemMode = EXT_MODEM_PROTOCOL_TYPE;  
      FRAMMultiWrite((u8 *)&Setting.ExtModemMode, (DRV_SETTING_PARM + offsetof(st_Setting,ExtModemMode)), sizeof(Setting.ExtModemMode));   
    }
  }
  else if(mode == 1) {
    if(DataSentAddr == DataAddrToSend) {
      BLE_SmartFlag = 1;
      BLE_SmartWaitFlag = 0;
    }
    else {
      if(DataAddrToSend >= DataSentAddr)
        dlen = DataAddrToSend-DataSentAddr;
      else
        dlen = NAND_SIZE_DATA+DataAddrToSend-DataSentAddr;
      
      if(dlen > BLE_SENDDATA_MAX) {
        dlen = BLE_SENDDATA_MAX;
        BLE_LastFLag = 0;
      }
      else 
        BLE_LastFLag = 1;
        
      if((DataSentAddr % NAND_SIZE_DATA)+dlen > NAND_SIZE_DATA)
        dlen = NAND_SIZE_DATA - (DataSentAddr% NAND_SIZE_DATA); 
      
      if(dlen) { 
        size = sizeof(tsSecData);
        data_cnt = dlen/size;
        data_ok_cnt = 0;
        memset(ModemBuff, 0, dlen);
        for(i = 0; i < data_cnt; i++) {
          Nand_Read_SecData((u8*)&RD_Sec_Data, DataSentAddr, size);
          if(RD_Sec_Data.TM > 0x58000000 && RD_Sec_Data.TM < 0x80000000) {  //2017년 이후... 
            memcpy((u8*)&ModemBuff[data_ok_cnt*size], (u8*)&RD_Sec_Data, size);
            data_ok_cnt++;
          }
          DataSentAddr = (DataSentAddr+size)%NAND_SIZE_DATA;
        }
#ifdef BLE_LOG_ENABLE        
        printk("\r\nBLE : Data Cnt(%u) / SentAddr(%u) / DataAddr(%u) = %u\n",data_ok_cnt, DataSentAddr, DataAddrToSend, DataAddrToSend - DataSentAddr);
#endif        
        if(data_ok_cnt > 0) {      
          dlen = data_ok_cnt*size;
          if(BLE_LastFLag)
            BLE_Send_Data(0x72, 0x12, 0x01, ModemBuff, dlen);   // Last Data
          else 
            BLE_Send_Data(0x72, 0x12, 0x00, ModemBuff, dlen);
          BLE_SmartWaitFlag = 1;
        }
        else {
          FRAMMultiWrite((u8 *)&DataSentAddr, DRV_SENT_ADDR, sizeof(DataSentAddr));  
          BLE_SmartFlag = 1;
          BLE_SmartWaitFlag = 0;
        }
      }
    }
  }
}



/*
********************************************************************************
* Description : Task_BLE function
* Arguments   : none
* Return      : 
* Note        : BLE Task process
******************************************************************************** 
*/
      
void Task_BLE(void const * argument)
{
  /* USER CODE BEGIN Task_BLE */
  BLE_POWER_ON();

  vTaskDelay(5000);

  /* Infinite loop */
  BLE_Step = BLE_COMM_CHECK;
  for(;;)
  {
    BLE_parse_proc();
    if(BLE_InitFLag) {
      if(Setting.FW_Type != EXT_MODEM) {
        if(BLE_SmartWaitFlag) {
          Ble_Wait_Cnt++;
          if(Ble_Wait_Cnt > 9999) {
            BLE_SmartFlag = 0;
            Ble_Wait_Cnt = 0;
            BLE_SmartWaitFlag = 0;
          }
        }
        else {
          if(BLE_SendHeaderFlag){   // Info
            BLE_Send_SmartData(0);
          }
          else if(BLE_SmartFlag) {  //Data
            BLE_Send_SmartData(1);
          }
          Ble_Wait_Cnt = 0;
        }
      }
      if(BLE_IntFlag) {
        if(Setting.FW_Type != EXT_MODEM) {      
            if(BLE_SendFlag) {
            BLE_SecData_Send();
          }
        }
        else {
          if(BLE_SendFlag) {
            BLE_SecData_Send();
          }
        }
        BLE_IntFlag = 0;
      }
    }
    else {
      BLE_Init();
    }
    osDelay(1);
  }
  /* USER CODE END Task_COM */
}