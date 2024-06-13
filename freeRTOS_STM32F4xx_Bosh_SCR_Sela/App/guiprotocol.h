/*
*************************************** (C) COPYRIGHT 2014 Loop*************************************
*
* File Name          : guiprotocol.h
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
*                                                MODULE
*********************************************************************************************************
*/

#ifndef GUIPROTOCOL_H
#define GUIPROTOCOL_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef GUIPROTOCOL_GLOBALS
#define GUIPROTOCOL_EXT
#else
#define GUIPROTOCOL_EXT	extern
#endif 

#define  DEF_BIT(bit)                           (1uL << (bit))

/*
*********************************************************************************************************
*                                                DEFINE
*********************************************************************************************************
*/

// uart buffer defines 
#define UART_RX_BUFFER_SIZE 1024       // 2,4,8,16,32,64,128 or 256 bytes
#define MESSAGE_SIZE        1024

#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )
#if ( UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK )
	#error RX buffer size is not a power of 2
#endif

//------------------------------------------------------------------ gui command
#define CMD_STATUS				0x01
#define CMD_SETTING				0x02
#define CMD_VERSION				0x21

#define CMD_DLSTART				0x10
#define CMD_DLPROGRESS		0x11
#define CMD_DLEND					0x12
#define CMD_DLMODE_NORMAL	0x1F

#define SYN			  0xC6	// sync define
#define STX       0x7E  // stx define
#define ETX       0x7D  // etx define

#define NOERR     0xE0  // Parameter

#define ACK       0x06  // ack 
#define NACK      0x15  // nack 

#define	_OK			  0			// no error 
#define _CRC  	  1			// CRC error 
#define _CMD		  2			// invaild command error 
#define _ERR      3     // msg error 
#define _OVR      4     // Buffer overflow error

/*
*********************************************************************************************************
*                                               TYPE DEFINE
*********************************************************************************************************
*/

#define GUI_PACKET_BUFFSZ   1024   //0X400 == 1024   

typedef struct{
	INT32U ulFileSize;
	INT16U uSOnePacketSize;
	INT16U uSTotalPacketCount;
	INT16U uCheckCode;
	INT16U mode;
	INT16U expectedCount;
	INT16U sectorCount;
	char dl_good_finished;
	char data[256];			// GUI에서 내려오는 패킷 사이즈 256byte 고정 !!
	INT16U uSLastPacketSize;
	INT16U last_CRC16;
}dl_t;

__packed typedef struct {
	INT8U	  sync[2];
  INT8U   stx;
  INT16U  length;
  INT8U   opcode;
  INT8U	  no;
}HEADER;

__packed typedef struct {
  HEADER  header;
  INT8U   msg[MESSAGE_SIZE];
}MSG_FRAME;

//#ifdef PACK_STRUCT_USE_INCLUDES
//#  include "arch/bpstruct.h"
//#endif
typedef struct{
	int nPort;
	int bStart_flag;		// if 0x7E recieve then set.
	int bEnd_flag;			// if 0x7E recieve then set.
	int nPacketCnt;			// 
	int nTmr;				// polling interval.
	char pRxBuf[GUI_PACKET_BUFFSZ];		// rxbuff의 위치..[0x400]
	char pRxBuf_dec[GUI_PACKET_BUFFSZ];	// rxbuff의 위치..[0x400]
	char *pTxBuf;			// txbuff의 위치..
	int lastPacketlen;
	INT16U last_CRC16;
}commProcess_t;
//#ifdef PACK_STRUCT_USE_INCLUDES
//#  include "arch/epstruct.h"
//#endif



typedef __packed struct{
	INT8U	project_code[10];			
	INT8U	boot_seq[10];
	INT16U	download_code;
	INT32U	file_size;
	INT16U	file_crc;
}Prjcode_t;


enum HexRecord
{
	HexData = 0,
	EndOfFile,
	ExtendedAddress,
	StartAddress,
	ExtendedLinerAddress,
	FlashAddress,
};


/*
*********************************************************************************************************
*                                           GLOBAL VARIABLE
*********************************************************************************************************
*/

GUIPROTOCOL_EXT dl_t dl;
GUIPROTOCOL_EXT Prjcode_t tPrjcode;
GUIPROTOCOL_EXT MSG_FRAME msg_f;
GUIPROTOCOL_EXT INT8U UART_RxBuf[UART_RX_BUFFER_SIZE];
GUIPROTOCOL_EXT INT8U MSG_RxBuf[MESSAGE_SIZE];
GUIPROTOCOL_EXT INT16U UART_RxHead;
GUIPROTOCOL_EXT INT16U UART_RxTail;
GUIPROTOCOL_EXT INT8U RTC_Buf[8], RTC_Flag;


/*
*********************************************************************************************************
*                                           GLOBAL FUNCTION
*********************************************************************************************************
*/

void protocol_process(MSG_FRAME *msg);
void msg_response(INT8U no, INT8U opcode, INT8U dat);

GUIPROTOCOL_EXT void percent_display(int cur,int max);
GUIPROTOCOL_EXT void GUI_hexdisp(char *data, int size);


GUIPROTOCOL_EXT void Task_Gui(void *p_arg);




#endif
/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/
