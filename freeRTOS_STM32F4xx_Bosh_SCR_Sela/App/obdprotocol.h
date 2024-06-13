/*
*************************************** (C) COPYRIGHT 2014 Loop *************************************
*
* File Name          : OBDProtocol.h
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 08/18/2014
*
* Description        : can1 protocol
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef CANPROTOOBD_H
#define CANPROTOOBD_H


/*
*********************************************************************************************************
*                                                EXTERNS
*********************************************************************************************************
*/

#ifdef OBD_GLOBALS
#define CAN1_EXT
#else
#define CAN1_EXT	extern
#endif 

#define CAN_STAN	CAN_ID_STD
#define CAN_EXTN	CAN_ID_EXT


CAN1_EXT int obd_debug;




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




CAN1_EXT void Task_Can1(void *p_arg);
CAN1_EXT void MakeCanMassage(CanTxMsg *ptarget, INT32U id, INT8U type, INT8U rtr, INT8U *src, INT8U len);



CAN1_EXT INT16U dbg_NOxSensorOnOff_test(void);
CAN1_EXT INT16U dbg_NOxSensor_test(void);
CAN1_EXT INT16U dbg_ob_debug_test(void);
CAN1_EXT void CAN1_OBD_task(canrxmsg_t* RxMessage);



#define CAN1_COMMANDS \
		{"nox",   dbg_NOxSensorOnOff_test, ALL_DIR, "nox 0 1"},\
		{"nnn",   dbg_ob_debug_test, ALL_DIR, "nox ddd"},\
		{"can",   dbg_NOxSensor_test, ALL_DIR, "can"},\


#endif