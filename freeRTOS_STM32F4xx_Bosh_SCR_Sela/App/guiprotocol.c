/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : guiprotocol.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 09/05/2014
*
* Description        : gui protocol
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define GUIPROTOCOL_GLOBALS
#include "includes.h"


/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

#define emacBLOCK_TIME_WAITING_FOR_INPUT	( ( portTickType ) 10 )

//extern xSemaphoreHandle USB_TxSem;

INT16U rx_length;		// receive data length 
INT16U r_idx;
INT8U r_sync;		  	// sync count
INT8U r_len;	  	  	// length count

// receive flag
volatile struct {
  INT8U sync      : 1;
  INT8U stx			  :	1;
  INT8U length		: 1;
  INT8U data			:	1;		
  INT8U receive		: 1;
  INT8U reserved	: 3;
} flags;


/*
*********************************************************************************************************
*                                                FUNCTIONS
*********************************************************************************************************
*/

void GUI_hexdisp(char *data, int size)
{
	int i;

	for(i=0; i<size; i++){
		if(i % 20 == 0) printk("\n");
		printk("%02X ",data[i]);
	}
	printk("\n");
}

void InitSCI0(void)
{
 	UART_RxTail = 0;
	UART_RxHead = 0;                                       
}

/*
*******************************************************************************
* void PutChar(char data) 
*
* Description : TERMIO_PutChar
* Arguments   : char data
* Return      : none
* Note        : 
******************************************************************************** 
*/
void PutChar(char data) 
{
  USART_SendData(USART2, data);
}

/*
*******************************************************************************
* uchar DataInReceiveBuffer( void )
*
* Description : none
* Arguments   : none
* Return      : empty : 0
* Note        : 
******************************************************************************** 
*/

INT8U DataInReceiveBuffer(void)
{
	return ( UART_RxHead != UART_RxTail ); /* Return 0 (FALSE) if the receive buffer is empty */
}


/*
*******************************************************************************
* uchar ReceiveByte(uchar data) 
*
* Description : Receive Byte check
* Arguments   : none
* Return      : char data
* Note        : Read and write functions 
******************************************************************************** 
*/
INT8U ReceiveByte(void)
{
	INT16U tmptail;
	
	while ( UART_RxHead == UART_RxTail )  /* Wait for incomming data */
		;
	tmptail = ( UART_RxTail + 1 ) & UART_RX_BUFFER_MASK;/* Calculate buffer index */
	
	UART_RxTail = tmptail;                /* Store new index */
	
	return UART_RxBuf[tmptail];           /* Return data */
}

/*
*******************************************************************************
* Description : communication flag & buffer clean
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/ 
void Flag_clear(void)
{
  *(INT8U *)&flags = 0;	// clear receive flags 
  r_sync = 0;
  r_len = 0;
  r_idx = 0;
}

/*
*******************************************************************************
* Description : MSG_FRAME* msg
* Arguments   : none
* Return      : true or fault
* Note        : 
******************************************************************************** 
*/
INT8U sum_compare(MSG_FRAME* msg)
{
	INT16U i, len;
  INT8U sum8;
	INT8U *t_ptr, *p_ptr;
	
	sum8 = 0;
	len = msg->header.length;
	t_ptr = (INT8U *)&msg->header.length;
	p_ptr = (INT8U *)&msg->header.opcode;

	for(i=0; i<(len+2); i++){				
		sum8 ^= *t_ptr++;
	}
	sum8 += 1;
	if(sum8 == *(p_ptr + len)) return 1;
	else return 0;
}

/*
*******************************************************************************
* Description : receive buffer check
* Arguments   : none
* Return      : none
* Note        : 
* +--------+--------+--------+---------+--------+--------+---------+--------+
* |  SYNC  |  STX   | LENGTH | COMMAND | DEV_ID |  DATA  | SUMCHECK|  ETX   |
* +--------+--------+--------+---------+--------+--------+---------+--------+
* SYNC     : 0xC6 0xC6
* STX      : 0x7E 
* LENGTH   : COMMAND ~ DATA
* SUMCHECK : LEGNTH ~ DATA EXOR+1
* ETX      : 0x7D
******************************************************************************** 
*/
void Message_check(void)
{
  MSG_FRAME* msg;
  INT8U data;
   
  msg = (MSG_FRAME *)MSG_RxBuf; 

  if(DataInReceiveBuffer())
  {
    data = ReceiveByte();
	  if(!flags.sync){											// sync bit set ? 
		  if((r_sync < 2) && (data == SYN)){		// 4 sync receive ?
			  MSG_RxBuf[r_idx] = data; 		      // save sync 
			  r_idx++; r_sync++;
			  if(r_sync==2) flags.sync=1;
		  } 
	  }
 	  else if(!flags.stx){                  // stx bit set? 
      if(data == STX){                    // stx ? 
     	  MSG_RxBuf[r_idx] = data;          // save stx 
     	  r_idx++;
     	  flags.stx=1;                      // rx flag stx bit set 
      } 
 	  }  
    else if(!flags.length){               // length bit set?
      if(!r_len) {
        MSG_RxBuf[r_idx] = data;          // save length 
        rx_length = data;                 // length         
        r_idx++; r_len++;
      }
      else {
        MSG_RxBuf[r_idx] = data;        
        rx_length += (data<<8);           // length 
        r_idx++;
        flags.length=1;                   // rx flag leng bit set 
      }
 	  }
    else if(!flags.data){                 // data receive bit set? 
   	  if(rx_length--){                    // length=0? 
     	  MSG_RxBuf[r_idx] = data;          // save data 
     	  r_idx++;
   	  }    
   	  else{                               // data receive end 
 			  MSG_RxBuf[r_idx] = data;
 			  r_idx++;
     	  flags.data=1;
      }
    }  
    else if(data==ETX){               		// etx? 
   	  MSG_RxBuf[r_idx] = data; 				    // save etx 
   	  flags.receive=1;										// successful data receive 
    }
    else Flag_clear();
  }
  
  if(flags.receive){
		if(sum_compare(msg)) protocol_process(msg);
		else msg_response(msg->header.no, NACK, _CRC);
		Flag_clear();
  }
}

/*
********************************************************************************
* Description : make header
* Arguments   : uchar length, uchar no, uchar opcode
* Return      : none
* Note        : 
******************************************************************************** 
*/
void make_header (INT16U length, INT8U no, INT8U opcode)
{
  HEADER *p_header;
  
  p_header = &msg_f.header;
  
  p_header->sync[0] = SYN;
  p_header->sync[1] = SYN;
  p_header->stx = STX;
  p_header->length = length;
  p_header->opcode = opcode;
  p_header->no = no;
}

/*
********************************************************************************
* Description : Data check_sum
* Arguments   : uchar *in_msg, int length
* Return      : none
* Note        : 
******************************************************************************** 
*/
void check_sum(INT8U *in_msg, int length)
{
  int i;
  INT8U sum8;
  
  sum8=0;
  for (i = 0; i!=(length+2); i++){
 		sum8 ^= *(in_msg+i);								// exclusive or 
  }
  *(in_msg+length+2) = sum8 + 1;				// sum + 1 
  *(in_msg+length+3) = ETX;
}

/*
********************************************************************************
* Description : data send
* Arguments   : INT8U *data, int len
* Return      : none
* Note        : 
********************************************************************************
*/
void data_send (INT8U *data, int len)
{
	int i=0;
  
	for(i=0;i< len; i++) {
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
      USART_SendData(USART2, data[i]);
  }
}

/*
********************************************************************************
* Description : msg_response 
* Arguments   : INT8U res
* Return      : none
* Note        : 
******************************************************************************** 
*/
void msg_response(INT8U no, INT8U opcode, INT8U dat)
{
  INT16U len;
  INT8U *t_ptr;
  
  len=3;
  t_ptr = (INT8U *)msg_f.msg;
  
  make_header(len, no, opcode);
  *t_ptr = dat;
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);  
}

/*
********************************************************************************
* Description : msg_str_response 
* Arguments   : INT8U res
* Return      : none
* Note        : 
******************************************************************************** 
*/
void msg_str_response(INT8U no, INT8U opcode, INT8U* dat, INT16U length)
{
  INT16U len;
  INT8U *t_ptr;
  
  memset((INT8U*)&msg_f, 0, sizeof(MSG_FRAME));
  len = length + 2;
  t_ptr = (INT8U *)msg_f.msg;
  
  make_header(len, no, opcode);
	memcpy((INT8U *)t_ptr, (INT8U *)dat, length); 
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7); 
}

/*
********************************************************************************
* Description : Send SCR Value
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrValue(void)
{
  INT16U len;
  INT8U *t_ptr;

  len = LEN_SYSVALUE+2;   
  t_ptr = (INT8U *)msg_f.msg; 
  
  make_header(len, 0, 0x28);  
  memcpy((INT8U *)t_ptr, (INT8U *)&SysSts, LEN_SYSVALUE);
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);    
}

/*
********************************************************************************
* Description : Send_Map
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrMap(void)
{
  INT16U len;
  INT8U *t_ptr;

  len = sizeof(stMap)+2;   
  t_ptr = (INT8U *)msg_f.msg; 
  
  make_header(len, 0, 0x2E);  
  memcpy((INT8U *)t_ptr, (INT8U *)&ScrMap.AlpaMap, sizeof(stMap));
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);    
}

/*
********************************************************************************
* Description : Set Map data
* Arguments   : MSG_FRAME *msg
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrMap(MSG_FRAME *msg) 
{
	int i, j;

  for(i = 0; i < 72; i++) {
    for(j = 0; j < 8; j++) {
      ScrMap.AlpaMap.row[i].col[j] = msg->msg[(i*8)+j];
    }
  }
  FRAM_page_readwrite(FRAM_SCR_MAP_SAVE, SAVE);//저장
  msg_response(msg->header.no, msg->header.opcode, OK);
}

/*
********************************************************************************
* Description : Send_Map
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrLabel(void)
{
  INT16U len;
  INT8U *t_ptr;

  len = 96+2;   
  t_ptr = (INT8U *)msg_f.msg; 
  
  make_header(len, 0, 0x2C);  
  memcpy((INT8U *)t_ptr, (INT8U *)&ScrMap.BaseTemp[0], 96);
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);    
}
 
/*
********************************************************************************
* Description : Set Label data
* Arguments   : MSG_FRAME *msg
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrLabel(MSG_FRAME *msg) 
{
  memcpy((INT16U *)&ScrMap.BaseTemp[0], (INT16U *)&msg->msg[0], 48);
  memcpy((INT16U *)&ScrMap.BaseMaf[0], (INT16U *)&msg->msg[48], 48); 
  FRAM_page_readwrite(FRAM_SCR_MAP_SAVE, SAVE);//저장
  msg_response(msg->header.no, msg->header.opcode, OK);
}

/*
********************************************************************************
* Description : Send_Factor
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrFactor(void)
{
  INT16U len;
  INT8U *t_ptr;

  len = sizeof(stFactor)+2;   
  t_ptr = (INT8U *)msg_f.msg; 
  
  make_header(len, 0, 0x2A);  
  memcpy((INT8U *)t_ptr, (INT8U *)&ScrFactor.Catalyst, sizeof(stFactor));
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7); 
}

/*
********************************************************************************
* Description : Set Fator data
* Arguments   : MSG_FRAME *msg
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrFactor(MSG_FRAME *msg) 
{
	INT8U *t_ptr;
	
	t_ptr = (INT8U *)msg->msg;

	memcpy((INT8U *)&ScrFactor.Catalyst, (INT8U *)t_ptr, sizeof(stFactor)); 
  FRAM_page_readwrite(FRAM_SCR_FACTOR_SAVE, SAVE);;//저장 
  msg_response(msg->header.no, msg->header.opcode, OK);
}

/*
********************************************************************************
* Description : Send_Parameter
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrParameter(void)
{
  INT16U len;
  INT8U *t_ptr;

  len = sizeof(stSysControl)+2;   
  t_ptr = (INT8U *)msg_f.msg; 
  
  make_header(len, 0, 0x26);  
  memcpy((INT8U *)t_ptr, (INT8U *)&SysSet, sizeof(stSysControl));
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7); 
}

/*
********************************************************************************
* Description : Set Parameter data
* Arguments   : MSG_FRAME *msg
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrParameter(MSG_FRAME *msg) 
{
	INT8U *t_ptr;
	
	t_ptr = (INT8U *)msg->msg;
	
	memcpy((INT8U *)&SysSet.CutMaf+(msg->header.no*8), (INT8U *)t_ptr, 8); 
  FRAM_page_readwrite(FRAM_SCR_SYSTEM_SAVE, SAVE);//저장
  msg_response(msg->header.no, msg->header.opcode, OK);
}

/*
********************************************************************************
* Description : Send_ScrTempCalib
* Arguments   : uchar dev_id
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrTempCalib(INT8U no)
{
  INT16U len;
  INT8U *t_ptr;
  
  len = sizeof(stCalib)+2;
  t_ptr = (INT8U *)msg_f.msg;
  make_header(len, no, 0x33);
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, NO_SAVE);
  memcpy((INT8U *)t_ptr, (INT8U *)ScrCalib.Temp+(sizeof(stCalib)*no), sizeof(stCalib));
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);
}

/*
********************************************************************************
* Description : Write_ScrTempCalib
* Arguments   : uchar dev_id
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrTempCalib(MSG_FRAME *msg)
{
	INT8U *t_ptr;
	
	t_ptr = (INT8U *)msg->msg; 
 
  memcpy((INT8U *)ScrCalib.Temp+(sizeof(stCalib)*msg->header.no), (INT8U *)t_ptr, sizeof(stCalib)); 
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
  Send_ScrTempCalib(msg->header.no);  
}

/*
********************************************************************************
* Description : Send_ScrSupplyCalib
* Arguments   : uchar dev_id
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrSupplyCalib(INT8U no)
{
  INT16U len;
  INT8U *t_ptr;
  
  len = sizeof(stCalib)+2;  
  t_ptr = (INT8U *)msg_f.msg;
  make_header(len, no, 0x36);
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, NO_SAVE);
  memcpy((INT8U *)t_ptr, (INT8U *)&ScrCalib.Supply+(sizeof(stCalib)*no), sizeof(stCalib));
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);
}

/*
********************************************************************************
* Description : Write_ScrSupplyCalib
* Arguments   : uchar dev_id
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrSupplyCalib(MSG_FRAME *msg)
{
	INT8U *t_ptr;
	
	t_ptr = (INT8U *)msg->msg; 
 
  memcpy((INT8U *)&ScrCalib.Supply+(sizeof(stCalib)*msg->header.no), (INT8U *)t_ptr, sizeof(stCalib)); 
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
  Send_ScrSupplyCalib(msg->header.no);  
}


/*
********************************************************************************
* Description : Send_ScrMafCailb
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrMafCalib(void)
{
  INT16U len;
  INT8U *t_ptr;

  len = sizeof(stCalMaf)*5 + 2;   
  t_ptr = (INT8U *)msg_f.msg; 
  
  make_header(len, 0, 0x30);
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, NO_SAVE);
  memcpy((INT8U *)t_ptr, (INT8U *)&ScrCalib.CalMaf, sizeof(stCalMaf)*5);
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7); 
}
    

/*
********************************************************************************
* Description : Write_ScrMafCalib
* Arguments   : MSG_FRAME *msg
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrMafCalib(MSG_FRAME *msg) 
{
	INT8U *t_ptr;
	
	t_ptr = (INT8U *)msg->msg;

	memcpy((INT8U *)&ScrCalib.CalMaf, (INT8U *)t_ptr, sizeof(stCalMaf)*5); 
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
  msg_response(msg->header.no, msg->header.opcode, OK);
}

/*
********************************************************************************
* Description : Send_ScrMafValue
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrMafValue(void)
{
  INT16U len;
  INT8U *t_ptr;

  len = sizeof(stScrMaf) + 2;   
  t_ptr = (INT8U *)msg_f.msg; 
  
  make_header(len, 0, 0x3B);
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, NO_SAVE);
  memcpy((INT8U *)t_ptr, (INT8U *)&ScrMaf, sizeof(stScrMaf));
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7); 
}
    

/*
********************************************************************************
* Description : Write_ScrMafValue
* Arguments   : MSG_FRAME *msg
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrMafValue(MSG_FRAME *msg) 
{
	INT8U *t_ptr;
	
	t_ptr = (INT8U *)msg->msg;

	memcpy((INT8U *)ScrMaf.Adc, (INT8U *)t_ptr, 40); 
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
  msg_response(msg->header.no, msg->header.opcode, OK);
}

/*
********************************************************************************
* Description : Send_ScrNoxCalib
* Arguments   : uchar dev_id
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ScrNoxCalib(INT8U no)
{
  INT16U len;
  INT8U *t_ptr;
  
  len = sizeof(INT8U)+2;  
  t_ptr = (INT8U *)msg_f.msg;
  make_header(len, no, 0x39);
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
  memcpy((INT8U *)t_ptr, (INT8U *)&ScrCalib.NOx_enable+(sizeof(INT8U)*no), sizeof(INT8U));
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);
}

/*
********************************************************************************
* Description : Write_ScrNoxCalib
* Arguments   : uchar dev_id
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Write_ScrNoxCalib(MSG_FRAME *msg)
{
	INT8U *t_ptr;
	
	t_ptr = (INT8U *)msg->msg; 
 
  memcpy((INT8U *)&ScrCalib.NOx_enable+(sizeof(INT8U)*msg->header.no), (INT8U *)t_ptr+1, sizeof(INT8U)); 
  FRAM_page_readwrite(FRAM_SCR_CALIB_SAVE, SAVE);
  Send_ScrNoxCalib(msg->header.no);  
}

/*
********************************************************************************
* Description : Send_ADC_Value
* Arguments   : dev_id : sensor channel
* Return      : none
* Note        : 
******************************************************************************** 
*/
void Send_ADC_Value(INT8U no, INT8U cmd)
{
  INT16U len, adc;
  INT16U *t_ptr;
  double dtemp;
  
  len = sizeof(INT16U)+2;  
  t_ptr = (INT16U *)msg_f.msg;

  if(no == 0) {
    make_header(len, no, cmd);    
    *t_ptr = adcdata[ADC_AVR_T1];
  }
  else if(no == 1) {
    make_header(len, no, cmd);        
    *t_ptr = adcdata[ADC_AVR_T2];    
  }
  else if(no == 2) {
    len += 2;
    make_header(len, 0, cmd);
//    if(SysSet.MafRfactor)
//      adc = MafVal;
//    else 
      adc = adcdata[ADC_AVR_MAF];
    *t_ptr = adc;
    t_ptr++;
    dtemp = MafvoltageOffset(adc);
		*t_ptr = (INT16U)(dtemp*100);
  }
  else {
    make_header(len, 0, cmd);        
    *t_ptr = adcdata[ADC_AVR_P1];    
  }
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);
}

/*
********************************************************************************
* Description : SendOBD_RTC
* Arguments   : none
* Return      : none
* Note        : 
******************************************************************************** 
*/
void SendOBD_RTC(void)
{
  INT16U len;
  INT8U *t_ptr;

  len = 8 + 2;
  t_ptr = (INT8U *)msg_f.msg; 
  
  make_header(len, 0, 0x3E);
  memcpy((INT8U *)t_ptr, (INT8U *)&RTC_Buf, 8);
  check_sum((INT8U *)&msg_f.header.length, len);
  data_send((INT8U *)&msg_f, len+7);
}

/*
********************************************************************************
* Description : OBDtoGUI
* Arguments   : MSG_FRAME *msg
* Return      : none
* Note        : 
******************************************************************************** 
*/
void OBDtoGUI(MSG_FRAME *msg)
{
	INT8U *t_ptr;
	
	t_ptr = (INT8U *)msg->msg; 
 
  memcpy((INT8U *)&RTC_Buf, (INT8U *)t_ptr, 8);  
}
    
/*
********************************************************************************
* Description : percent_display
* Arguments   : cur, max
* Return      : 
* Note        : percent display
******************************************************************************** 
*/
void percent_display(int cur,int max)
{
	printk("cur = %d, max = %d percent = %d%%\r",cur, max, cur * 100 / max);
}

/*
********************************************************************************
* Description : Receive Protocol Process....
* Arguments   : MSG_FRAME *msg
* Return      : none
* Note        : 
******************************************************************************** 
*/
INT16U packetcount;
void protocol_process(MSG_FRAME *msg)
{
	INT8U *pDestAddr;  
	INT16U rcvLastPacketNum;

  INT8U errReport = 0;
  INT8U tempbuff[4]={0};;
	INT16U buf16b;
  u32 buf32b;
  int sectCnt;
  int i, sectOffset;
  INT16U ramCRC=0;
  
  if(msg->header.opcode < 0x20) {
    switch(msg->header.opcode) {
    case CMD_DLSTART:
      if(msg->header.no == 0) {
//        BootState = 0;
//        FRAMWordWrite(BootAddr, BootState);
        
        // 파일 사이즈 4Byte copy
        memcpy((char *)&(dl.ulFileSize), &msg->msg, sizeof(dl.ulFileSize));
        // 한 패킷 사이즈 2Byte copy 
        memcpy((char *)&dl.uSOnePacketSize, (INT8U*)&msg->msg + sizeof(dl.ulFileSize), sizeof(dl.uSOnePacketSize));
        // 총 패킷 사이즈 2Byte copy
        memcpy((char *)&dl.uSTotalPacketCount, (INT8U*)&msg->msg + sizeof(dl.ulFileSize) + sizeof(dl.uSOnePacketSize), sizeof(dl.uSTotalPacketCount));
        memcpy((char *)&dl.uCheckCode, (INT8U*)&msg->msg + 4 + 2 + 2, sizeof(dl.uCheckCode));//4: file size, 2:One's Packet size, 2 : Total Packet Size
        printk("\n##### DownLoad Command #####\n");
        printk("File size          : %dBytes\n",dl.ulFileSize);
        printk("One Packet size    : %dBytes\n",dl.uSOnePacketSize);
        printk("Total Packet Count : %dPacket\n",dl.uSTotalPacketCount);
        printk("Last Packet Byte   : %dBytes\n",dl.ulFileSize % dl.uSOnePacketSize);
        printk("Download Code Check: %dBytes\n",dl.uCheckCode);
        // 전체 패킷 갯수 = (파일 사이즈 / 한 패킷 사이즈) + (파일 사이즈 % 한 패킷 사이즈) 
        dl.uSLastPacketSize = dl.ulFileSize / dl.uSOnePacketSize + (dl.ulFileSize % dl.uSOnePacketSize ? 1 : 0);
        if(dl.uSLastPacketSize == dl.uSTotalPacketCount)		// 전체 패킷 갯수와 총 패킷 사이즈가 같으면 	
        {
          printk("File size, Packet size, TotalPacket count Ok..\n");
          errReport = 1;
          dl.mode = CMD_DLPROGRESS;
          dl.expectedCount = 1;								// 다음 페킷 카운트의 숫자
          memcpy((char *)tPrjcode.boot_seq,"down_gui",8);
          SPI_FLASH_PageErase(PROJECT_BASE);
          if(SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t))) 
          {
            /*-------------------------------------------------------------------------------*/	
            // 다운로드 파일 사이즈 만큼의 SPI Flash의 영역을 지운다.
            sectCnt = (dl.ulFileSize / SPI_FLASH_BLOCK)+1;
            if(sectCnt>FLASH_DOWN_BLOCKCOUNT) sectCnt=FLASH_DOWN_BLOCKCOUNT;
            sectOffset = DOWNLOAD_SECTOR;
            // Block 크기 삭제 
            for(i=0;i<sectCnt;i++)
            {
              SPI_FLASH_BlockErase(sectOffset);
              sectOffset += SPI_FLASH_BLOCK;
              printk("Erase %X at 0x%05X\n",SPI_FLASH_BLOCK,sectOffset);
            }
            /*-------------------------------------------------------------------------------*/	
            dl.last_CRC16 = 0;	
            tPrjcode.file_size = dl.ulFileSize;
          }
          else {
            errReport = 3;
          }
        }
        else {
          errReport = 2;										// total packet size error
        }
        if(errReport != 1) dl.mode = CMD_DLMODE_NORMAL;
        msg_response(msg->header.no, msg->header.opcode, errReport);
      }
      break;
    case CMD_DLPROGRESS:
      if(dl.mode != CMD_DLPROGRESS) {
        return;
      }
      if(msg->header.no == 0) {
        errReport = 0;
        // 패킷 갯수 2Byte copy
        memcpy(&packetcount, &msg->msg, sizeof(packetcount));
        // 예상 카운트와 패킷 카운트가 다르면 에러 처리  
        if(dl.expectedCount != packetcount) {
          errReport = 3;
        }
        else {
          pDestAddr = (INT8U *)(dl.data);									// 받은 패킷을 복사할 주소 = 다운로드 베이스 주소
  /*-----------------------------------------------------------------------------------------------------*/	
  // 데이터를 모두 받을 경우 진입 				
          if(packetcount == dl.uSTotalPacketCount){
            dl.mode = CMD_DLEND;
            dl.uSLastPacketSize = dl.ulFileSize % dl.uSOnePacketSize;
            if(dl.uSLastPacketSize <= (msg->header.length - 2)) {			// 'packet number' 2byte를 뺀다	
              if(packetcount != 0){
                if(dl.uSLastPacketSize == 0)						// 계산이 0이면 한패킷 풀로 보낸다
                  dl.uSLastPacketSize = dl.uSOnePacketSize;
                memcpy(pDestAddr, (INT8U*)&msg->msg + sizeof(dl.uSOnePacketSize), dl.uSLastPacketSize);
                errReport = 1;
                SPI_FLASH_BufferWrite((INT8U *)pDestAddr, (INT32U)(DOWNLOAD_SECTOR + ((packetcount-1)*dl.uSOnePacketSize)), dl.uSLastPacketSize);
                dl.last_CRC16 = GenerateCRC((char*)pDestAddr, dl.uSLastPacketSize, dl.last_CRC16);
                // 마지막 계산된 CRC 값 저장 -> PROJECT_BASE 번지 
                tPrjcode.file_crc = dl.last_CRC16;
                // Ram에서 계산된 CRC 값 Display
                printk("Check calculated ram fileCRC: [%04X][%04X]\n",tPrjcode.file_crc, dl.last_CRC16);
                // Flash에 저장된 데이터에 대한 CRC 값 Display
                ramCRC = GetFlashAreaCRC(DOWNLOAD_SECTOR,dl.ulFileSize);
                printk("Check wrote flash fileCRC: [%04X]\n",(INT16U)ramCRC);
                SPI_FLASH_PageErase(PROJECT_BASE);
                SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));
              }
              else {
                errReport = 2;
              }
            }
            else {
              errReport = 3;
            }
            if(errReport != 1) {
              printk("Last Packet length err, expected:%d, recieved:%d,err:%d\n",dl.uSLastPacketSize,(msg->header.length-2),errReport);
            }
          }
  /*-----------------------------------------------------------------------------------------------------*/	
  // 데이터를 받는 중...	
          else {
            memcpy((char *)pDestAddr, (INT8U*)&msg->msg + 2, dl.uSOnePacketSize);
            errReport = 1;
            // OnePacket 단위로 Write를 실행한다.... 
            SPI_FLASH_BufferWrite((INT8U *)pDestAddr, (INT32U)(DOWNLOAD_SECTOR + ((packetcount-1)*dl.uSOnePacketSize)), dl.uSOnePacketSize);
            dl.last_CRC16 = GenerateCRC((char*)pDestAddr, dl.uSOnePacketSize, dl.last_CRC16);
            if( (packetcount % 10) ==0 ) percent_display(packetcount,dl.uSTotalPacketCount);
          }	
  /*-----------------------------------------------------------------------------------------------------*/	
          dl.expectedCount++;
        }
        if(errReport != 1) dl.mode = CMD_DLMODE_NORMAL;
        
        memcpy(tempbuff, &msg->msg, 2);
        tempbuff[2] = errReport;
        
        msg_str_response(msg->header.no, msg->header.opcode, tempbuff, 3);
      }
      break;
    case CMD_DLEND:
      if(dl.mode != CMD_DLEND) {
        return;
      }
      if(msg->header.no == 0) {
        // 마지막 패킷 갯수 2Byte copy
        memcpy((char *)&rcvLastPacketNum, (char *)(&msg->msg), 2);

        if((dl.expectedCount-1) == rcvLastPacketNum){// 정상 
          errReport = 1;
          msg_response(msg->header.no, msg->header.opcode, errReport);          
          dl.dl_good_finished = 1;
          printk("Good\n");
        }
        else{
          errReport = 2;
          msg_response(msg->header.no, msg->header.opcode, errReport);          
          printk("Not matched\n");
          dl.dl_good_finished = 0;
        }
        if(errReport != 1) dl.mode = CMD_DLMODE_NORMAL;
        if(dl.dl_good_finished == 1){
          memcpy((char*)tPrjcode.boot_seq,"backup",6);
          SPI_FLASH_PageErase(PROJECT_BASE);
          SPI_FLASH_BufferWrite((INT8U *)&tPrjcode, PROJECT_BASE, sizeof(Prjcode_t));
          NVIC_SystemReset();
        }
      }
      break;
    }
  }
  else if(msg->header.opcode < 0x40) {  // SCR
    switch(msg->header.opcode) {
      case 0x20:
        break;
      case 0x21:
        if(SysSts.SCRMode == DOSING_MODE)
        {
          if(msg->msg[0] == 1) { 
            SysSts.MDosingStart = 1;
            memcpy((char *)&buf32b, (char *)&msg->msg[4], 4);
            SysSts.MDosingRatehouer = ((double)buf32b/100.0);
          }
          else if(msg->msg[0] == 2) { 
            SysSts.MDosingStart = 2;        
            memcpy((char *)&buf32b, (char *)&msg->msg[4], 4);
            SysSts.MDosingDuty = ((double)buf32b/100.0);
            if(SysSts.MDosingDuty > 100.0) 
              SysSts.MDosingDuty = 100.0;
          }
          else {
            SysSts.MDosingStart = 0;
          }
        }
        break;
      case 0x22:
        if(SysSts.SCRMode == ALPHA_MODE) {        
          SysSts.AlphaSet = msg->msg[0];
        }
        break;
      case 0x23:
        if(SysSts.SCRMode == MANUAL_MODE) {
          memcpy( (char*)&buf16b, (char *)&msg->msg[0], 2);
          SysSts.mTavg = buf16b;
          memcpy( (char*)&buf16b, (char *)&msg->msg[2], 2);
          SysSts.mMaf = buf16b;
          memcpy( (char*)&buf16b, (char *)&msg->msg[4], 2);
          SysSts.mNoxppm[0] = buf16b;
          memcpy( (char*)&buf16b, (char *)&msg->msg[6], 2);
          SysSts.mNoxppm[1] = buf16b;
        }        
        break;
      case 0x24:
        if((msg->msg[0] & 0x3) == 1)
          SysSts.SCRMode = MANUAL_MODE;
        else if((msg->msg[0] & 0x3) == 2)
          SysSts.SCRMode = ALPHA_MODE;
        else if((msg->msg[0] & 0x3) == 3)
          SysSts.SCRMode = DOSING_MODE;
        else
          SysSts.SCRMode = AUTO_MODE;
        
        if(OldMode != SysSts.SCRMode) {
          OldMode = SysSts.SCRMode;
          if(SysSts.SCRMode <= ALPHA_MODE) {
            FirstFlag = 0;
            //AlarmClear();
            SysSts.TotalDosingRate = 0.0;
            nStatus = SCR_CHECKMODE;
            SCR_Control_Off();
          }
        }
        
        if(msg->msg[1] & 0x01) {
          if(SysSts.SystemSignal.Dosing == OFF) {
            PurgeSolFlag = 1;
            ScrFlag.Flushing = 1;
            ScrFlag.SupplyStart = 1;
            SysSts.SystemSignal.Supply = ON;
          }
        }
        else {
          ScrFlag.Flushing = 0;
          nStatus = SCR_CHECKMODE;      
        }        
        break;
      case 0x25:
        SysSet.ScrTestMode = msg->msg[0];
        
        if(SysSet.ScrTestMode == 1)	//SCR 동작알고리즘 분사 off test mode가 1이면(분사하지 말아라....)
        {
          SysSts.SystemSignal.Operation = ON;	 // Disable
        }
        else
          SysSts.SystemSignal.Operation = OFF; // Enable
            
        printk("Test Mode = %d\n",SysSet.ScrTestMode);
        //FRAMMultiWrite(FRAM_PARMSAVE_BASE,(INT8U *)&SysSet,sizeof(stSysControl));        
        break;
      case 0x26:  //Scr Parameter Read
        Send_ScrParameter(); 
        break;
      case 0x27:  // Scr Paramter Write
        Write_ScrParameter(msg);
        break;
      case 0x28:  //Scr Readvalue
        break;
      case 0x29:
        if(msg->msg[0] == 1)
        {
          SysSts.DosingDuty = 0.0;
          SysSts.DosingRatehouer = 0.0; 
          SysSts.TotalDosingRate = 0.0;
          SysSts.curXk	= 0.0;
          SysSts.preXk	= 0.0;
          SysSts.preUk	= 0.0;
          SysSts.curUk	= 0.0;

          printk("Acc Clear\n");
        }
        if(msg->msg[1] == 1)
        {
          AlarmClear();
          printk("Alarm Clear\n");
        }
        break;
      case 0x2A:  // Scr Factor Read
        Send_ScrFactor();
        break;
      case 0x2B:  // Scr Factor Write
        Write_ScrFactor(msg);
        break;
      case 0x2C:  // Scr label Read
        Send_ScrLabel();
        break;
      case 0x2D:  // Scr label Write
        Write_ScrLabel(msg);
        break;
      case 0x2E:  // Scr Map Read
        Send_ScrMap();
        break;
      case 0x2F:  // Scr Map Write
        Write_ScrMap(msg);
        break;
      case 0x30:  // Scr MAF Read
        Send_ScrMafCalib();
        break;
      case 0x31:  // Scr MAF Write
        Write_ScrMafCalib(msg);
        break;
      case 0x32:  // Scr MAF ADC 
        Send_ADC_Value(2, msg->header.opcode);
        break;
      case 0x33:  // Scr Temp Read
        Send_ScrTempCalib(msg->header.no);
        break;
      case 0x34:  // Scr Temp Write
        Write_ScrTempCalib(msg);
        break;
      case 0x35:  // Scr Temp ADC
        Send_ADC_Value(msg->header.no, msg->header.opcode);
        break;
      case 0x36:  // Scr Supply Read
        Send_ScrSupplyCalib(msg->header.no);
        break;
      case 0x37:  // Scr Supply Write
        Write_ScrSupplyCalib(msg);
        break;
      case 0x38:  // Scr Supply ADC
        Send_ADC_Value(3, msg->header.opcode);
        break;
      case 0x39:  // Scr Nox Read
        Send_ScrNoxCalib(msg->header.no);        
        break;
      case 0x3A:  // Scr Nox Write
        Write_ScrNoxCalib(msg);        
        break;        
      case 0x3B:  // Scr Maf
        Send_ScrMafValue();
        break;
      case 0x3C:  // Scr MAF Write
        Write_ScrMafValue(msg);       
        break;
      case 0x3D:  // OBD To UI
        OBDtoGUI(msg);
        RTC_Flag = 0x01;  //RTC Write
        break;
      case 0x3E:  // OBD To UI
        OBDtoGUI(msg);
        RTC_Flag = 0x02;  //RTC Read
        break;
    }
  }
}

void RS232_RTC(INT8U id)
{
  switch(id) {
  case 0x11:    // RTC Write
    break;
  case 0x12:    // RTC Read
    SendOBD_RTC();
    break;
  default:
    break;
  }
}

//********************************************************************************************* GUI Task

void Task_Gui(void *p_arg)
{
	sysinfo_t *s = (sysinfo_t *)p_arg 	;

	BSP_UART2RX_IntEnable(ENABLE);
			
	for(;;)
	{
		s->twatchGuiFlag = 1;
    Message_check();    // 메세지 체크
    if(RTC_Flag && (RTC_Flag > 0x10)) {
      RS232_RTC(RTC_Flag);
      RTC_Flag = 0;
    }
    
		if(TmrChk(TMR_DPFINTERVAL) == 0) {
      Send_ScrValue();
      TmrSetT(TMR_DPFINTERVAL, 1000);
      TmrStart(TMR_DPFINTERVAL);
    }
		s->twatchGuiFlag = 0;
		vTaskDelay(1);  
	}
}



/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/
