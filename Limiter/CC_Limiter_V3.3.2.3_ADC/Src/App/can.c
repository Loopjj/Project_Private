/*
*************************************** (C) COPYRIGHT 2017 LOOP *************************************
*
* File Name          : can.c
*
* Author             : Reserch & Developing 1 Team
*
* Version            : V1.0.0
*
* Date               : 012/21/2017
*
* Description        : can process 
*
*********************************************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#define CAN_GLOBALS
#include "includes.h"
#define TRAP_ST		  0x01

INT8U CanTrapMode= TRAP_ST;
/* Private define ------------------------------------------------------------*/
#define RESPONSE_DELAY	10
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define CAN1_FILTER_NUMBER 14
#define CAN1_ID_LIST_NUMBER 28
const u32 CAN1_ID_List[CAN1_ID_LIST_NUMBER] = {
  0x18F00E51,  //Nox In  Sender : 0x51
  0x18F00F51,  //Nox Out Sender : Cleanearth 0x51 
  0x18F00F52,  //Nox Out Sender : Iljin 0x52
  0x18FD3E3D,  //Cleanearth Temp. ASDS Sender: 0x3D
  0x18FDB43D,  //Ceracomb Temp. In Sender 0x3D
  0x18FDB33D,  //Ceracomb Temp. Out Sender 0x3D
  0x18FFE53D,  //Iljin, EnD Temp. In Sender: 0x3D
  0x18FFE43D,  //EnD, Iljin, Cleanearth Urea Level*** Sender: 0x3D
  0x18FE563D,  //EnD GENIII, CeraComb Urea Level Sender: 0x3D
  0x18FFDE3D,  //Iljin, EnD, Cleanearth Dosing Rate, EnD Pump State Sender: 0x3D
  0x18FFC33D,  //EnD GENIII Dosing Rate Sender: 0x3D
  0x18FFDF00,  //CeraComb Dosing Rate Sender: 0x00
  0x18B2003D,  //Cleanearth ASDS Dosing Rate Sender: 0x3D
  0x18FFE63D,  //Iljin, EnD, Cleanearth MAF Sender: 0x3D
  0x0CF00A3D,  //CeraComb MAF Sender: 0x00
  0x18FEDF3D,  //Cleanearth ASDS MAF Sender: 0x3D
  0x18FD6E3D,  //Cleanearth ASDS Concentration Sender: 0x3D
  0x18FFE33D,  //Iljin, EnDSystem Error Sender: 0x3D
  0x18FFE23D,  //Iljin Doser Error Sender: 0x3D
  0x18FFE03D,  //Iljin Vbat Sender: 0x3D
  0x18FEF73D,  //Cleanearth ASDS Vbat Sender: 0x3D
  0x18BA003D,  //Cleanearth ASDS SCR State Sender: 0x3D
  0x10000000,  //Cleanearth Pressure, SCR State Sender: 0x00
  0x00000000,   //dummy for even
  0x00000000,   //dummy for reserve
  0x00000000,   //dummy for reserve
  0x00000000,   //dummy for reserve
  0x00000000    //dummy for reserve
};
#define CAN2_FILTER_NUMBER 1
#define CAN2_ID_LIST_NUMBER 2
const u32 CAN2_ID_List[CAN2_ID_LIST_NUMBER] = {
  0x18FD9BA3,  //Urea Concentration Sender: 0xA3
  0x18FE56A3  //Urea Level Sender: 0xA3
};
u8 Can_TxRetry;
u16 Can_TxStep;
u16 Can_TxWait;
u8 CAN_J1979Enable;

stCanEnable CAN_TxEnable, CAN_RxComplete;
stCanEnableCnt Cur_CanEnableCnt, Pre_CanEnableCnt;


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Function Name  : MX_CAN1_Init
* Description    : Configures the CAN1.
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/  
void MX_CAN1_Init(void)
{
  CAN_FilterConfTypeDef sFilterConfig;
  static CanTxMsgTypeDef TxMessage1;
  static CanRxMsgTypeDef RxMessage1;
  u32 can_mode, i;
  u8 can_baud = 2;
  
  HAL_CAN_DeInit(&hcan1);
	/* CAN configuration ********************************************************/  
  if((Setting.CanBPS&0x0F)) can_baud = 3; //500kbps
  else can_baud = 3;  //250kbps //Default 250kbps

  can_mode = CAN_Mode_Normal;
  
  /*##-1- Configure the CAN peripheral #######################################*/
  hcan1.Instance = CAN1;
  hcan1.pTxMsg = &TxMessage1;
  hcan1.pRxMsg = &RxMessage1;
    
  hcan1.Init.TTCM = DISABLE;
  hcan1.Init.ABOM = DISABLE;
  hcan1.Init.AWUM = DISABLE;
  hcan1.Init.NART = ENABLE;
  hcan1.Init.RFLM = DISABLE;
  hcan1.Init.TXFP = DISABLE;
  hcan1.Init.Mode = can_mode;
  hcan1.Init.SJW = CAN_SJW_1TQ;

  hcan1.Init.Prescaler = 14;

  if(can_baud == 0) {
    hcan1.Init.BS1 = CAN_BS1_13TQ; 
    hcan1.Init.BS2 = CAN_BS2_1TQ;       //  100 kbit/s
  }
  else if(can_baud == 1) {
    hcan1.Init.BS1 = CAN_BS1_16TQ; 
    hcan1.Init.BS2 = CAN_BS2_7TQ;        //  125 kbit/s
  }
  else if(can_baud == 2) {
    hcan1.Init.BS1 = CAN_BS1_10TQ; 
    hcan1.Init.BS2 = CAN_BS2_1TQ;         //  250 kbit/s
  }
  else if(can_baud == 3) {
    hcan1.Init.BS1 = CAN_BS1_4TQ; 
    hcan1.Init.BS2 = CAN_BS2_1TQ;        //  500 kbit/s
  }
  else if(can_baud == 4) {
    hcan1.Init.BS1 = CAN_BS1_1TQ; 
    hcan1.Init.BS2 = CAN_BS2_1TQ;        //  1000 kbit/s
  }
  else {
    hcan1.Init.BS1 = CAN_BS1_10TQ; 
    hcan1.Init.BS2 = CAN_BS2_1TQ;        //  250 kbit/s
  }
  
  if(HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the CAN Filter ###########################################*/
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  
  sFilterConfig.FilterNumber = 0;
  sFilterConfig.FilterIdHigh = (u16)(0x0100<<5);  // 0x010 ~ 0x013 수신
  sFilterConfig.FilterIdLow = (u16)0x0000;  // 0x098 ~ 0x099 수신
  sFilterConfig.FilterMaskIdHigh = (0x0700<<5);
  sFilterConfig.FilterMaskIdLow = 0x0000;
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  
  sFilterConfig.FilterNumber = 1;
  sFilterConfig.FilterIdHigh = (u16)(0x0030<<5);  // 0x010 ~ 0x013 수신
  sFilterConfig.FilterIdLow = (u16)0x0000;  // 0x098 ~ 0x099 수신
  sFilterConfig.FilterMaskIdHigh = (0x07C8<<5);
  sFilterConfig.FilterMaskIdLow = 0x0000;
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  
  sFilterConfig.FilterNumber = 2;
  sFilterConfig.FilterIdHigh = (u16)(0x100<<5);  // 0x010 ~ 0x013 수신
  sFilterConfig.FilterIdLow = (u16)0x0000;  // 0x098 ~ 0x099 수신
  sFilterConfig.FilterMaskIdHigh = (0x17C8<<5);
  sFilterConfig.FilterMaskIdLow = 0x0000;
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
 /*추가*/ 
  sFilterConfig.FilterNumber = 3;
  sFilterConfig.FilterIdHigh = (u16)(0x0010<<5);  // 0x010 ~ 0x013 수신    //0X30 
  sFilterConfig.FilterIdLow = (u16)0x0000;  // 0x098 ~ 0x099 수신
  sFilterConfig.FilterMaskIdHigh = (0x0700<<5);
  sFilterConfig.FilterMaskIdLow = 0x0000;                               
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();

  /*##-2- Start the Reception process and enable reception interrupt #########*/
  if(HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }    
}


/******************************************************************************
* Function Name  : MX_CAN2_Init
* Description    : Configures the CAN2.
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/    
void MX_CAN2_Init(void)
{
  CAN_FilterConfTypeDef sFilterConfig;
  static CanTxMsgTypeDef TxMessage2;
  static CanRxMsgTypeDef RxMessage2;
  u32 can_mode, i;
  u8 can_baud = 2;
  
  HAL_CAN_DeInit(&hcan2);
	/* CAN configuration ********************************************************/ 
  if((Setting.CanBPS&0xF0)) can_baud = 2; //250kbps
  else can_baud = 3;  //500kbps           //Default 500kbps

  can_mode = CAN_MODE_SILENT;

  /*##-1- Configure the CAN peripheral #######################################*/
  hcan2.Instance = CAN2;
  hcan2.pTxMsg = &TxMessage2;
  hcan2.pRxMsg = &RxMessage2;
    
  hcan2.Init.TTCM = DISABLE;
  hcan2.Init.ABOM = DISABLE;
  hcan2.Init.AWUM = DISABLE;
  hcan2.Init.NART = ENABLE;
  hcan2.Init.RFLM = DISABLE;
  hcan2.Init.TXFP = DISABLE;
  hcan2.Init.Mode = can_mode; 
  hcan2.Init.SJW = CAN_SJW_1TQ;

  hcan2.Init.Prescaler = 14;

  if(can_baud == 0) {
    hcan2.Init.BS1 = CAN_BS1_13TQ; 
    hcan2.Init.BS2 = CAN_BS2_1TQ;       //  100 kbit/s
  }
  else if(can_baud == 1) {
    hcan2.Init.BS1 = CAN_BS1_16TQ; 
    hcan2.Init.BS2 = CAN_BS2_7TQ;        //  125 kbit/s
  }
  else if(can_baud == 2) {
    hcan2.Init.BS1 = CAN_BS1_10TQ; 
    hcan2.Init.BS2 = CAN_BS2_1TQ;         //  250 kbit/s
  }
  else if(can_baud == 3) {
    hcan2.Init.BS1 = CAN_BS1_4TQ; 
    hcan2.Init.BS2 = CAN_BS2_1TQ;        //  500 kbit/s
  }
  else if(can_baud == 4) {
    hcan2.Init.BS1 = CAN_BS1_1TQ; 
    hcan2.Init.BS2 = CAN_BS2_1TQ;        //  1000 kbit/s
  }
  else {
    hcan2.Init.BS1 = CAN_BS1_10TQ; 
    hcan2.Init.BS2 = CAN_BS2_1TQ;        //  250 kbit/s
  }
  
  if(HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the CAN Filter ###########################################*/
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST; //CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
  sFilterConfig.BankNumber = CAN1_FILTER_NUMBER;
  
  for(i = 0; i< CAN2_FILTER_NUMBER; i++) {   //Initailize filter
    sFilterConfig.FilterNumber = i+CAN1_FILTER_NUMBER;
    sFilterConfig.FilterActivation = DISABLE;
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  
  sFilterConfig.FilterActivation = ENABLE;
  
  for(int i = 0; i < CAN2_FILTER_NUMBER; i++) {
    sFilterConfig.FilterNumber = i+CAN1_FILTER_NUMBER;
    sFilterConfig.FilterIdHigh = (u16)((CAN2_ID_List[i*2]<<3)>>16);
    sFilterConfig.FilterIdLow = (u16)((CAN2_ID_List[i*2]<<3) | 4);
    sFilterConfig.FilterMaskIdHigh = (u16)((CAN2_ID_List[i*2+1]<<3)>>16);
    sFilterConfig.FilterMaskIdLow = (u16)((CAN2_ID_List[i*2+1]<<3) | 4);
    if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  
//  for(int i = 0; i< 28-CAN_PGN_LIST_NUMBER; i++) {
//    sFilterConfig.FilterNumber = i+CAN_PGN_LIST_NUMBER;
//    sFilterConfig.FilterIdHigh = (u16)((CAN_PGN_List[i]<<11)>>16);
//    sFilterConfig.FilterIdLow = (u16)((CAN_PGN_List[i]<<11) | 4);
//    sFilterConfig.FilterMaskIdHigh = (u16)((0xFFFF<<11)>>16);
//    sFilterConfig.FilterMaskIdLow = (u16)((0xFFFF<<11) | 4);
//    if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
//  }
  
  /*##-2- Start the Reception process and enable reception interrupt #########*/
  if(HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }    
}

/*
********************************************************************************
* Description : CheckCanDisable
* Arguments   : None
* Return      : 
* Note        : Can Disable Check
******************************************************************************** 
*/
INT32U CanDisableCnt;
void CheckCanDisable(void) 
{
  CanDisableCnt++;
  if(CanDisableCnt > 3) {
    CanDisableCnt = 0;
    if(CAN_Enable.NOxIn) {
      if(Cur_CanEnableCnt.NOxIn != Pre_CanEnableCnt.NOxIn)
        Pre_CanEnableCnt.NOxIn = Cur_CanEnableCnt.NOxIn;
      else 
        CAN_Enable.NOxIn = 0;
    }
    if(CAN_Enable.NOxOut) {
      if(Cur_CanEnableCnt.NOxOut != Pre_CanEnableCnt.NOxOut)
        Pre_CanEnableCnt.NOxOut = Cur_CanEnableCnt.NOxOut;
      else 
        CAN_Enable.NOxOut = 0;
    }
    if(CAN_Enable.TempIn) {
      if(Cur_CanEnableCnt.TempIn != Pre_CanEnableCnt.TempIn)
        Pre_CanEnableCnt.TempIn = Cur_CanEnableCnt.TempIn;
      else 
        CAN_Enable.TempIn = 0;
    }
    if(CAN_Enable.TempOut) {
      if(Cur_CanEnableCnt.TempOut != Pre_CanEnableCnt.TempOut)
        Pre_CanEnableCnt.TempOut = Cur_CanEnableCnt.TempOut;
      else 
        CAN_Enable.TempOut = 0;
    }
    if(CAN_Enable.UreaLevel) {
      if(Cur_CanEnableCnt.UreaLevel != Pre_CanEnableCnt.UreaLevel)
        Pre_CanEnableCnt.UreaLevel = Cur_CanEnableCnt.UreaLevel;
      else 
        CAN_Enable.UreaLevel = 0;
    }
    if(CAN_Enable.DosingRate) {
      if(Cur_CanEnableCnt.DosingRate != Pre_CanEnableCnt.DosingRate)
        Pre_CanEnableCnt.DosingRate = Cur_CanEnableCnt.DosingRate;
      else 
        CAN_Enable.DosingRate = 0;
    }
    if(CAN_Enable.MAF) {
      if(Cur_CanEnableCnt.MAF != Pre_CanEnableCnt.MAF)
        Pre_CanEnableCnt.MAF = Cur_CanEnableCnt.MAF;
      else 
        CAN_Enable.MAF = 0;
    }
    if(CAN_Enable.Concentration) {
      if(Cur_CanEnableCnt.Concentration != Pre_CanEnableCnt.Concentration)
        Pre_CanEnableCnt.Concentration = Cur_CanEnableCnt.Concentration;
      else 
        CAN_Enable.Concentration = 0;
    }
    if(CAN_Enable.SystemError) {
      if(Cur_CanEnableCnt.SystemError != Pre_CanEnableCnt.SystemError)
        Pre_CanEnableCnt.SystemError = Cur_CanEnableCnt.SystemError;
      else 
        CAN_Enable.SystemError = 0;
    }
    if(CAN_Enable.DoserError) {
      if(Cur_CanEnableCnt.DoserError != Pre_CanEnableCnt.DoserError)
        Pre_CanEnableCnt.DoserError = Cur_CanEnableCnt.DoserError;
      else 
        CAN_Enable.DoserError = 0;
    }
    if(CAN_Enable.Vbat) {
      if(Cur_CanEnableCnt.Vbat != Pre_CanEnableCnt.Vbat)
        Pre_CanEnableCnt.Vbat = Cur_CanEnableCnt.Vbat;
      else 
        CAN_Enable.Vbat = 0;
    }
    if(CAN_Enable.State) {
      if(Cur_CanEnableCnt.State != Pre_CanEnableCnt.State)
        Pre_CanEnableCnt.State = Cur_CanEnableCnt.State;
      else 
        CAN_Enable.State = 0;
    }
    if(CAN_Enable.DpfPress) {
      if(Cur_CanEnableCnt.DpfPress != Pre_CanEnableCnt.DpfPress)
        Pre_CanEnableCnt.DpfPress = Cur_CanEnableCnt.DpfPress;
      else 
        CAN_Enable.DpfPress = 0;
    }
  }
}
INT16U TEST;
/*
********************************************************************************
* Description : CAN_Std_Decode
* Arguments   : None
* Return      : 
* Note        : CAN Decode(Standard)
******************************************************************************** 
*/
void CAN_Std_Decode(CAN_HandleTypeDef* hcan) //ID 2.0A 세라컴 전용 
{
	INT16U buf16b;
  INT32U buf32b;
  INT32U diff;
  INT32U RTC_OBD_Val;
 //rtc_time rtc_val;
  
  switch(hcan->pRxMsg->StdId) {

		case 0x30:    //Temp Out         
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[6],2);
        SecData.TempOut = buf16b;
      //CAN_Enable.TempOut = 1;
        Cur_CanEnableCnt.TempOut++;        
        break;  
		case 0x101:    //UreaQuality 
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],1);
        SecData.Err.B = buf16b;// SCR Error
        if(SecData.Err.Noxin) CAN_Enable.NOxIn = 0; //에러 
        else {CAN_Enable.NOxIn = 1; // 정상
        Cur_CanEnableCnt.NOxIn++;}
        if(SecData.Err.Noxout) CAN_Enable.NOxOut = 0;
        else {CAN_Enable.NOxOut = 1;
        Cur_CanEnableCnt.NOxOut++;}        
        if(SecData.Err.ULevel) CAN_Enable.UreaLevel = 0;
        else {CAN_Enable.UreaLevel = 1;
        Cur_CanEnableCnt.UreaLevel++;}
        if(SecData.Err.MAF) CAN_Enable.MAF = 0;
        else {CAN_Enable.MAF = 1;
        Cur_CanEnableCnt.MAF++;}
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
        SecData.Chk.W = buf16b;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[4],2);
        Sig.W = buf16b;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[6],2);
        SecData.Concentration = buf16b/10; // 627-> 62.7 
        //SecData.Concentration = 40; 테스트용 
        CAN_Enable.Concentration = 1;
        Cur_CanEnableCnt.Concentration++;      
        break;        
		case 0x307:    //NOxIn,NOxOut,MAF 
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        SecData.NOxIn = (INT16S)buf16b;
        if (SecData.NOxIn <= 0 || SecData.NOxIn > 3012) 
          SecData.NOxIn = 0;  
        //Cur_CanEnableCnt.NOxIn++;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
        SecData.NOxOut = (INT16S)buf16b;
        if (SecData.NOxOut <= 0 || SecData.NOxOut > 3012)
          SecData.NOxOut = 0;
        //Cur_CanEnableCnt.NOxOut++;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[4],2);
        SecData.MAF = buf16b;
        if (SecData.MAF > 5000) SecData.MAF = 0;  // MAF 최초값 65526 수신문제 방어      
        //CAN_Enable.MAF = 1;
        //Cur_CanEnableCnt.MAF++;
        break;          
		case 0x103:    //Temp In,Out,  
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        SecData.TempIn = buf16b;
       //CAN_Enable.TempIn = 1;
        Cur_CanEnableCnt.TempIn++;        
//        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
//        SecData.TempOut = buf16b;
      //CAN_Enable.TempOut = 1;
//        Cur_CanEnableCnt.TempOut++;        
        break;
		case 0x104:    //Dosing Rate g/h   
        memcpy((char *)&buf32b,&hcan->pRxMsg->Data[0],4);
        SecData.DosingRate = buf32b;
        //CAN_Enable.DosingRate = 1;
        Cur_CanEnableCnt.DosingRate++;
        
        break;        
		case 0x107:    //Urea Level   
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
        SecData.UreaLevel = buf16b;
//        if(SecData.UreaLevel < Setting.UreaLvlHi) SecData.Chk.UreaLevel = 1;
//        else SecData.Chk.UreaLevel = 0;
        //CAN_Enable.UreaLevel = 1;
        //Cur_CanEnableCnt.UreaLevel++;        
        break;
		case 0x110:    //NOxIn,NOxOut,MAF                
        if((hcan->pRxMsg->Data[1] & 0x0F) == 0x0F) NOxValidFlag[0] = 0;
        else NOxValidFlag[0] = 1;
        if((hcan->pRxMsg->Data[5] & 0x0F) == 0x0F) NOxValidFlag[1] = 0;
        else NOxValidFlag[1] = 1;
        break;         
		case 0x119:    //Master OBD의 시간을 받아온다.     
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],1);
        SecData.Speed = (unsigned char)buf16b;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],1); 
        RTC_Val.tm_year = buf16b;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[3],1);
        RTC_Val.tm_mon = buf16b;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[4],1);
        RTC_Val.tm_mday = buf16b;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[5],1);
        RTC_Val.tm_hour = buf16b;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[6],1);
        RTC_Val.tm_min = buf16b;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[7],1);
        RTC_Val.tm_sec = buf16b;
        
        if(LimitDelay < RTC_DELAY) {
          RTC_OBD_Val = RTCValTotime(RTC_Val); //OBD시간을 UNIX time 변환 
            diff = abs(RTC_OBD_Val - RTC_LVal); // OBD와 Limiter의 시간차 절대값을 구한다. 
            if(diff > 3) {                      //OBD 시간과 Limiter 시간차가 3초이상 일경우    
              RTC_SetDateTime((u8 *)&RTC_Val);   // OBD 시간으로 RTC를 설정한다.  
            }
          LimitDelay = RTC_DELAY;
        }
        break;       
      default:
       break;
   }
}

/*
********************************************************************************
* Description : CAN_Ext_Decode
* Arguments   : None
* Return      : 
* Note        : CAN Decode(Extend)
******************************************************************************** 
*/
#define TRAP_0x010	0x010
#define TRAP_0x011	0x011
#define TRAP_0x012	0x012
#define TRAP_0x013	0x013

u8 Urea_QSensor_Use;
void CAN_Ext_Decode(CAN_HandleTypeDef* hcan) //ID 2.0B
{
	INT16U buf16b;
	INT32U buf32b;
  
	switch(hcan->pRxMsg->ExtId) {
//		case TRAP_0x010:							//CAN ID 0x10 Download Start(file size, Packet Count)
//			if(hcan->pRxMsg->Data[0] == 0x00)
//			{
////				nDlStatus = 1;				//Packet Count Ok...
////				printk("DL Packet Count Ok....\n");
//			}
//			else
//			{
////				nDlStatus = 4;				//Packet Count Error.....
////				printk("DL Packet Count Error....\n");
//			}
//			break;
//		case TRAP_0x011:
//			if (hcan->pRxMsg->Data[0] == 0x00)//CAN ID 0x11 Download Start(File CRC)
//			{
////				nDlStatus = 2;
////				printk("DL CRC Response Ok....\n");
//			}
//			else
//			{
////				nDlStatus = 4;
//			}
//			break;
//		case TRAP_0x013:
//			if (hcan->pRxMsg->Data[0] == 0x00) //CAN ID 0x13 Download End
//			{
////				nDlStatus = 3;
////				printk("DL soucess Ok....\n");
//			}
//			else
//			{
////				nDlStatus = 4;
////				printk("DL CRC Error....\n");
//			}
//			break;  
		case 0x18F00E51:  //NOx in
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        if(hcan->pRxMsg->Data[4] & 0x10) NOxValidFlag[0] = 0;
        else NOxValidFlag[0] = 1;
        //SecData.NOxIn = (INT16U)(buf16b * 0.05) - 200;
        //if(SecData.NOxIn > 3012) SecData.NOxIn = 0;
        CAN_Enable.NOxIn = 1;
        Cur_CanEnableCnt.NOxIn++;
			break;
		case 0x18F00F52:  //NOx out
      if(Setting.UserNumber == USER_ILJIN35L
         || Setting.UserNumber == USER_ILJIN50L
         || Setting.UserNumber == USER_EnD
         || Setting.UserNumber == USER_EnDGENIII
         || Setting.UserNumber == USER_CERACOMB
         || Setting.UserNumber == USER_CLEANEARTH) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        if(hcan->pRxMsg->Data[4] & 0x10) NOxValidFlag[1] = 1;
        else NOxValidFlag[1] = 0;
        //SecData.NOxOut = (INT16U)(buf16b * 0.05) - 200;
        if(SecData.NOxOut > 3012) SecData.NOxOut = 0;
        CAN_Enable.NOxOut = 1;
        Cur_CanEnableCnt.NOxOut++;
      }
			break;
		case 0x18F00F51:  //NOx out
      if(Setting.UserNumber == USER_CLEANASDS) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        if(hcan->pRxMsg->Data[4] & 0x10) NOxValidFlag[1] = 1;
        else NOxValidFlag[1] = 0;
        //SecData.NOxOut = (INT16U)(buf16b * 0.05) - 200;
        if(SecData.NOxOut > 3012) SecData.NOxOut = 0;
        CAN_Enable.NOxOut = 1;
        Cur_CanEnableCnt.NOxOut++;
      }
			break;
		case 0x18FD3E3D:  //Cleanearth ASDS Temp. in/out
      if(Setting.UserNumber == USER_CLEANASDS) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        SecData.TempIn = (INT16U)(buf16b * 0.03125) - 273;
        if(SecData.TempIn >= 1734) SecData.TempIn = 0;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[3],2);
        SecData.TempOut = (INT16U)(buf16b * 0.03125) - 273;
        if(SecData.TempOut >= 1734) SecData.TempOut = 0;
        CAN_Enable.TempIn = 1;
        Cur_CanEnableCnt.TempIn++;
        CAN_Enable.TempOut = 1;
        Cur_CanEnableCnt.TempOut++;
      }
			break;
		case 0x18FDB43D:  //CeraComb Temp. in
      if(Setting.UserNumber == USER_CERACOMB) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        SecData.TempIn = (INT16U)(buf16b * 0.03125) - 273;
        if(SecData.TempIn >= 1734) SecData.TempIn = 0;
        CAN_Enable.TempIn = 1;
        Cur_CanEnableCnt.TempIn++;
      }
			break;
		case 0x18FDB33D:  //CeraComb Temp. out
      if(Setting.UserNumber == USER_CERACOMB) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        SecData.TempOut = (INT16U)(buf16b * 0.03125) - 273;
        if(SecData.TempOut >= 1734) SecData.TempOut = 0;
        CAN_Enable.TempOut = 1;
        Cur_CanEnableCnt.TempOut++;
      }
			break;
		case 0x18FFE53D:  //Iljin, Cleanearth Temp. in
      if(Setting.UserNumber == USER_ILJIN35L
         || Setting.UserNumber == USER_ILJIN50L
         || Setting.UserNumber == USER_CLEANEARTH) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        SecData.TempIn = buf16b;
        if(SecData.TempIn > 800) SecData.TempIn = 0;
        CAN_Enable.TempIn = 1;
        Cur_CanEnableCnt.TempIn++;
      }
      //EnD Temp. in/out
      else if(Setting.UserNumber == USER_EnD
         || Setting.UserNumber == USER_EnDGENIII) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        SecData.TempIn = buf16b;
        if(SecData.TempIn > 800) SecData.TempIn = 0;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
        SecData.TempOut = buf16b;
        if(SecData.TempOut > 800) SecData.TempOut = 0;
        CAN_Enable.TempOut = 1;
        Cur_CanEnableCnt.TempOut++;
      }
			break;
		case 0x18FFE43D:  //Cleanearth Temp. out, Urea Level***
      //Iljin, Cleanearth Urea Level
      if(Setting.UserNumber == USER_EnD  || Setting.UserNumber == USER_CLEANEARTH) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[4],2);
        SecData.UreaLevel = (INT16U)(buf16b * 0.0025);
        if(SecData.UreaLevel > 160) SecData.UreaLevel = 0;
        CAN_Enable.UreaLevel = 1;
        Cur_CanEnableCnt.UreaLevel++;
      }
      //Iljin
      else if(Setting.UserNumber == USER_ILJIN35L) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
        buf16b &= 0x03FF;  
        if(buf16b > 690 || buf16b < 340) SecData.UreaLevel = 0;
        else if(buf16b < 448) SecData.UreaLevel = 100;
        else if(buf16b <= 535) SecData.UreaLevel = 90;
        else if(buf16b <= 608) SecData.UreaLevel = 80;
        else if(buf16b <= 625) SecData.UreaLevel = 70;
        else if(buf16b <= 645) SecData.UreaLevel = 60;
        else if(buf16b <= 652) SecData.UreaLevel = 50;
        else if(buf16b <= 665) SecData.UreaLevel = 40;
        else if(buf16b <= 673) SecData.UreaLevel = 30;
        else if(buf16b <= 681) SecData.UreaLevel = 20;
        else if(buf16b <= 688) SecData.UreaLevel = 10;
        CAN_Enable.UreaLevel = 1;
        Cur_CanEnableCnt.UreaLevel++;
      }
      else if(Setting.UserNumber == USER_ILJIN50L) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
        buf16b &= 0x03FF;  
        if(buf16b >= 775 || buf16b < 170) SecData.UreaLevel = 0;
        else if(buf16b < 205) SecData.UreaLevel = 100;
        else if(buf16b <= 387) SecData.UreaLevel = 90;
        else if(buf16b <= 470) SecData.UreaLevel = 80;
        else if(buf16b <= 533) SecData.UreaLevel = 70;
        else if(buf16b <= 584) SecData.UreaLevel = 60;
        else if(buf16b <= 626) SecData.UreaLevel = 50;
        else if(buf16b <= 670) SecData.UreaLevel = 40;
        else if(buf16b <= 723) SecData.UreaLevel = 30;
        else if(buf16b <= 750) SecData.UreaLevel = 20;
        else if(buf16b < 775) SecData.UreaLevel = 10;
        CAN_Enable.UreaLevel = 1;
        Cur_CanEnableCnt.UreaLevel++;
      }
			break;
		case 0x18FE563D:  //EnD GENIII, CeraComb Urea Level
      if(Setting.UserNumber == USER_EnDGENIII
         || Setting.UserNumber == USER_CERACOMB) {
        SecData.UreaLevel = (INT8U)(hcan->pRxMsg->Data[0] * 0.4);
        if(SecData.UreaLevel > 100) SecData.UreaLevel = 0;
        CAN_Enable.UreaLevel = 1;
        Cur_CanEnableCnt.UreaLevel++;
      }
			break;
		case 0x18FFDE3D:  //Iljin, EnD, Cleanearth Dosing Rate, EnD Pump State
      if(Setting.UserNumber == USER_ILJIN35L
         || Setting.UserNumber == USER_ILJIN50L
         || Setting.UserNumber == USER_EnD
         || Setting.UserNumber == USER_CLEANEARTH) {
        memcpy((char *)&buf32b,&hcan->pRxMsg->Data[0],4);
        SecData.DosingRate = buf32b;
        CAN_Enable.DosingRate = 1;
        Cur_CanEnableCnt.DosingRate++;
      }
			break;
		case 0x18FFC33D:  //EnD GENIII Dosing Rate
      if(Setting.UserNumber == USER_EnDGENIII) {
        memcpy((char *)&buf32b,&hcan->pRxMsg->Data[0],4);
        SecData.DosingRate = buf32b;
        CAN_Enable.DosingRate = 1;
        Cur_CanEnableCnt.DosingRate++;
      }
			break;
		case 0x18FFDF00:  //CeraComb Dosing Rate
      if(Setting.UserNumber == USER_CERACOMB) {
        memcpy((char *)&buf32b,&hcan->pRxMsg->Data[0],4);
        SecData.DosingRate = buf32b;
        if(SecData.DosingRate > 0xB71B0) SecData.DosingRate = 0;
        CAN_Enable.DosingRate = 1;
        Cur_CanEnableCnt.DosingRate++;
      }
			break;
		case 0x18B2003D:  //Cleanearth ASDS Dosing Rate
      if(Setting.UserNumber == USER_CLEANASDS) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
        SecData.DosingRate = (INT32U)buf16b;
        if(SecData.DosingRate > 0xFAFF) SecData.DosingRate = 0;
        CAN_Enable.DosingRate = 1;
        Cur_CanEnableCnt.DosingRate++;
      }
			break;
		case 0x18FFE63D:  //Iljin, EnD, Cleanearth MAF
      if(Setting.UserNumber == USER_ILJIN35L
         || Setting.UserNumber == USER_ILJIN50L
         || Setting.UserNumber == USER_EnD
         || Setting.UserNumber == USER_EnDGENIII
         || Setting.UserNumber == USER_CLEANEARTH) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[0],2);
        SecData.MAF = buf16b;
        if(SecData.MAF > 8000) SecData.MAF = 0;
        CAN_Enable.MAF = 1;
        Cur_CanEnableCnt.MAF++;
      }
			break;
		case 0x0CF00A00:  //CeraComb MAF
      if(Setting.UserNumber == USER_CERACOMB) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[2],2);
        SecData.MAF = (INT16U)(buf16b * 0.05);
        if(SecData.MAF > 3212) SecData.MAF = 0;
        CAN_Enable.MAF = 1;
        Cur_CanEnableCnt.MAF++;
      }
			break;
		case 0x18FEDF3D:  //Cleanearth ASDS MAF
      if(Setting.UserNumber == USER_CLEANASDS
         || Setting.UserNumber == USER_EnDGENIII) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[5],2);
        SecData.MAF = (INT16U)(buf16b * 0.2);
        if(SecData.MAF > 12851) SecData.MAF = 0;
        CAN_Enable.MAF = 1;
        Cur_CanEnableCnt.MAF++;
      }
			break;
		case 0x18FD6E3D:
      if(Setting.UserNumber == USER_CLEANASDS && !Urea_QSensor_Use) {
        SecData.Concentration = (INT16U)(hcan->pRxMsg->Data[4] * 0.4);
        if(SecData.Concentration > 100) SecData.Concentration = 0;
        CAN_Enable.Concentration = 1;
        Cur_CanEnableCnt.Concentration++;
      }
			break;
		case 0x18FFE33D:  //System Error
      if(Setting.UserNumber == USER_ILJIN35L
         || Setting.UserNumber == USER_ILJIN50L) {
        memcpy((char *)&SecData.SystemError,&hcan->pRxMsg->Data[0],8);
        CAN_Enable.SystemError = 1;
        Cur_CanEnableCnt.SystemError++;
      }
      else if(Setting.UserNumber == USER_EnD) {
        memcpy((char *)&SecData.SystemError,&hcan->pRxMsg->Data[0],8);
        SecData.SystemError[0] &= 0x07;
        SecData.SystemError[1] &= 0x80;
        SecData.SystemError[2] &= 0xFF;
        SecData.SystemError[3] &= 0xFF;
        SecData.SystemError[4] &= 0x00;
        SecData.SystemError[5] &= 0x00;
        SecData.SystemError[6] &= 0x1F;
        SecData.SystemError[7] &= 0x00;
        CAN_Enable.SystemError = 1;
        Cur_CanEnableCnt.SystemError++;
      }
			break;
		case 0x18FFE23D:  //Iljin Doser Error
      if(Setting.UserNumber == USER_ILJIN35L
         || Setting.UserNumber == USER_ILJIN50L) {
        memcpy((char *)&SecData.DoserError,&hcan->pRxMsg->Data[0],8);
        CAN_Enable.DoserError = 1;
        Cur_CanEnableCnt.DoserError++;
      }
			break;
		case 0x18FFE03D:  //Iljin Vbat
      if(Setting.UserNumber == USER_ILJIN35L
         || Setting.UserNumber == USER_ILJIN50L) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[5],2);
        SecData.Vbat = (0x03FE & buf16b) * 10;
        if(SecData.Vbat > 32) SecData.Vbat = 0;
        CAN_Enable.Vbat = 1;
        Cur_CanEnableCnt.Vbat++;
      }//Cleanearth Vbat
      else if(Setting.UserNumber == USER_CLEANEARTH) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[5],2);
        SecData.Vbat = (0x01FF & buf16b) * 10;
        if(SecData.Vbat > 3200) SecData.Vbat = 0;
        CAN_Enable.Vbat = 1;
        Cur_CanEnableCnt.Vbat++;
      }
			break;
		case 0x18FEF73D:  //Cleanearth ASDS Vbat
      if(Setting.UserNumber == USER_CLEANASDS) {
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[4],2);
        SecData.Vbat = (INT16U)(buf16b * 0.05*10);
        if(SecData.Vbat > 3212) SecData.Vbat = 0;
        CAN_Enable.Vbat = 1;
        Cur_CanEnableCnt.Vbat++;
      }
			break;
		case 0x18BA003D:  //SCR State
      if(Setting.UserNumber == USER_CLEANASDS) {
        SecData.UreaLevel = hcan->pRxMsg->Data[0];
        if(SecData.UreaLevel > 100) SecData.UreaLevel = 0;
        SecData.State = hcan->pRxMsg->Data[1];
        if(SecData.State > 2) SecData.State = 0;
        CAN_Enable.State = 1;
        Cur_CanEnableCnt.State++;
      }
			break;
		case 0x10000000:  //Cleanearth Pressure, SCR State
      if(Setting.UserNumber == USER_CLEANASDS
         || Setting.UserNumber == USER_CLEANEARTH) {
        SecData.State = hcan->pRxMsg->Data[2];
        if(SecData.State > 3) SecData.State = 0;
        memcpy((char *)&buf16b,&hcan->pRxMsg->Data[4],2);
        //SecData.DpfPress = buf16b;
        //if(SecData.DpfPress > 1000) SecData.DpfPress = 0;
        CAN_Enable.State = 1;
        Cur_CanEnableCnt.State++;
        CAN_Enable.DpfPress = 1;
        Cur_CanEnableCnt.DpfPress++;
      }
			break;
		case 0x18FD9BA3:
        SecData.Concentration = (INT8U)(hcan->pRxMsg->Data[1]/4);
        if(SecData.Concentration > 62.5) SecData.Concentration = 0;
        CAN_Enable.Concentration = 1;
        Cur_CanEnableCnt.Concentration++;
        Urea_QSensor_Use = 1;
			break;
		case 0x18FE56A3: //Urea level D0*0.4%
			break;
 	 default:
		 break;
	}  
}
/*
********************************************************************************
* Description : SendCanMassage
* Arguments   : src
* Return      : 
* Note        : Send CAN Messages
******************************************************************************** 
*/
void SendCanMassage(CanTxMsgTypeDef *src)
{ 
  u16 i;
  
  /* Transmit OBD Structure preparation */
  hcan1.pTxMsg->IDE = src->IDE;
  hcan1.pTxMsg->StdId = src->StdId;
  hcan1.pTxMsg->RTR = src->RTR;
  hcan1.pTxMsg->DLC = src->DLC;
  for(i=0;i<8;i++)
  {
     hcan1.pTxMsg->Data[i] = src->Data[i];
  }  
  HAL_CAN_Transmit(&hcan1, 100);    
}
/*
********************************************************************************
* Description : MakePackMassage
* Arguments   : None
* Return      : 
* Note        : Send CAN Messages(Extend)
******************************************************************************** 
*/
int MakePackMassage(CanTxMsgTypeDef *ptarget, INT32U id, INT8U type, INT8U rtr, INT8U *src, INT8U len)
{
	int i=0;
	if(type == CAN_ID_STD)
		ptarget->StdId = id;
	else
		ptarget->ExtId = id;

	ptarget->IDE = type;
	ptarget->RTR = rtr;
	ptarget->DLC = len;

	for(i=0;i<len;i++)
	{
		ptarget->Data[i] = src[i];
	}
	return i;
	
}

INT32U CanSendWaitTime;
/*
********************************************************************************
* Description : StepSendTrap
* Arguments   : none
* Return      : 
* Note        : 
******************************************************************************** 
*/
//void StepSendTrap(void)
//{
//	INT16U SendCount=0;
//	INT8U buf[20];
//	INT16U buf16b;
	//INT32U buf32b;
//	CanTxMsgTypeDef  msg;
//	double dtemp;
//	static INT8U ParmSendCount = 1;
//	static INT16U TrapSendCount = 0x30;
//  
//  CanSendWaitTime++;
//	if(CanSendWaitTime > 50) {  
//    memset(buf,0x0,sizeof(buf)); 
//    if(CanTrapMode == TRAP_ST) {
//      if(TrapSendCount == 0x30) {
//        buf16b = SecData.UreaAccFlag;
//        memcpy(buf, (char *)&buf16b, 2);
//        buf16b = SecData.UreaAccFlag;
//        memcpy(&buf[2], (char *)&buf16b, 2);
//        buf16b = SecData.UreaAccFlag;
//        memcpy(&buf[4], (char *)&buf16b, 2);
//        buf16b = SecData.UreaAccFlag;
//        memcpy(&buf[6], (char *)&buf16b, 2);
//        SendCount = MakePackMassage(&msg, 0x111, CAN_ID_STD, CAN_RTR_DATA, buf, 8);
//        TrapSendCount = 0x30;
//      }
//      CanSendWaitTime = 0;      
//    }
//    if (SendCount)
//      SendCanMassage(&msg);
//  }
//}
/*
********************************************************************************
* Description : Send 'Limit Step' to OBD 
* Arguments   : *none
* Return      : *none
* Note        : 1 Second Proccess  
******************************************************************************** 
*/
void Send_LimitStep(void)
{
  INT8U buf[8];
	CanTxMsgTypeDef msg;	
 
   buf[0] = SecData.UreaAccFlag; //Limit Step 
   buf[1] = UreaTestFlag;        //Test Flag 판독을 위함   
   memcpy(&buf[2],(char *)&Pedal_ADC,4);  //(1 - (Pedal_ADC[1] / Pedal_ADC[0])) * 100.0;
   buf[6] = 0; //Reserved 6
   buf[7] = 0; //Reserved 7 
   MakePackMassage(&msg, 0x11A, CAN_STAN, CAN_RTR_DATA, buf, 8);
   SendCanMassage(&msg);
}

/******************************************************************************
* Function Name  : Task_CAN
* Description    : Task_CAN function
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void Task_CAN(void const * argument)
{
  /* USER CODE BEGIN Task_CAN */
	sysinfo_t *s = (sysinfo_t *)argument;  
  /* Infinite loop */
  
  for(;;)
  {
		s->twatchCanFlag = 1;    
    if(CAN_ErrorFlag) {
      MX_CAN1_Init();
      MX_CAN2_Init();
      CAN_ErrorFlag = 0;
      CAN_ErrorCnt++;
    }
    osDelay(1);
		s->twatchCanFlag = 0;    
  }
  /* USER CODE END Task_CAN */
}
