/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : ECUProtocol.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/18/2014
*
* Description        : CAN2 ECU protocol
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef CANPROTOECU_H
#define CANPROTOECU_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef ECU_GLOBALS
#define CAN2_EXT
#else
#define CAN2_EXT	extern
#endif 

#define DL_NONE		0x00
#define DL_START	0x01
#define DL_PROGRESS 0x02
#define DL_END		0x03


typedef struct{
	INT32U ulFileSize;
	INT32U uSTotalPacketCount;
	INT32U uSLastPacketSize;
	INT8U  uSOnePacketSize;
	INT16U uCRC16;
	
	INT16U WriteOnePacketSize;
	INT16U WritePacketCount;
	INT16U uCheckCode;
	INT16U mode;
	INT32U expectedCount;
	INT16U sectorCount;
	char dl_good_finished;
	char data[512];			// GUI에서 내려오는 패킷 사이즈 256byte 고정 !!
	INT16U last_CRC16;
}do_t;

typedef struct{
	INT8U packnum[3];
	INT8U data[4];
	INT8U sum;
}pack_t;




#if 0
typedef struct
{
  uint32_t id;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint8_t ide;     /*!< Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type */

  uint8_t rtr;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t dlc;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  uint8_t data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */

  uint8_t fmi;     /*!< Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF */
} canrxmsg_t;
#endif

CAN2_EXT do_t	Cdo;


CAN2_EXT void Task_Can2(void *p_arg);
CAN2_EXT void TaskCheckDownload(void);



CAN2_EXT INT16U dbg_CanUI_test(void);



#define CAN2_COMMANDS \
		{"ver",   dbg_CanUI_test, ALL_DIR, "ver"},\



#endif