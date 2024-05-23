/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : test_jig.h
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 06/03/2018
*
* Description        : uart test jig
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                                MODULE
*********************************************************************************************************
*/

#ifndef __TEST_JIG_H
#define __TEST_JIG_H

#ifdef TEST_JIG_GLOBALS
#define TEST_JIG_EXT
#else
#define TEST_JIG_EXT	extern
#endif 

TEST_JIG_EXT u8 TestJigComm[3];
TEST_JIG_EXT u8 TestJigCAN2;
TEST_JIG_EXT u8 TestJigCAN2_OK;
TEST_JIG_EXT void DecodeRxData(unsigned char rdata);
TEST_JIG_EXT void Send_SecData(void);
TEST_JIG_EXT void Decode_Command(void);
TEST_JIG_EXT void TestJIgTxData(u8 type, u8 command, u8 *data, u16 len);
TEST_JIG_EXT void CheckUreaAlarm(void); 
#endif