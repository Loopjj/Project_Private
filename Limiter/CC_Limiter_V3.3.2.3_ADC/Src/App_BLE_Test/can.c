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

/* Private define ------------------------------------------------------------*/
#define RESPONSE_DELAY	10
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define CAN_PGN_LIST_NUMBER 15
const u16 CAN_PGN_List[CAN_PGN_LIST_NUMBER] = {
  0xF004,
  0xFEE0,
  0xFEE6,
  0xFEE9,
  0xFEF1,
  0xFEF2,
  0xFEC1,
  0xFE6C,
  0xFF56,
  0xFDC4,
  0xF02F, //AEBS1 : ID:0x0CF02F2A 
  0xF007, //LDWS1 : ID:0x10F007E8 
  0xFE5B,  //LDWS2 : ID:0x18FE5BE8 
  0xFEEE, //Coolant Temp
  0xFD7C  //DPF Signal
};
u8 Can_TxRetry;
u16 Can_TxStep;
u16 Can_TxWait;
u8 CAN_J1979Enable;

stJ1979EcuID CAN_ECU_ID;
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
  if(Setting.Can_Enable == SIG_LINE) return;
	/* CAN configuration ********************************************************/  
  if(Setting.CanBaudrate == 250) can_baud = 2;
  else if(Setting.CanBaudrate == 500) can_baud = 3;
  else can_baud = 2;

  if(Setting.CanMode == CAN_Mode_Normal || Setting.Can_Type < 4) can_mode = CAN_MODE_NORMAL;
  else can_mode = CAN_MODE_SILENT;
  
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
  sFilterConfig.BankNumber = CAN_PGN_LIST_NUMBER;
  
  for(i = 0; i< CAN_PGN_LIST_NUMBER; i++) {   //Initailize filter
    sFilterConfig.FilterNumber = i;
    sFilterConfig.FilterActivation = DISABLE;
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  
  sFilterConfig.FilterActivation = ENABLE;
  
  if(Setting.Can_Type == 1) {   ////J1979 STD_ID
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterIdHigh = 0xFC00;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0xFC00;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else if(Setting.Can_Type == 2) {  ////J1979 EXT_ID
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterIdHigh = 0xC6D7; //18DAF100<<3
    sFilterConfig.FilterIdLow = 0x8800;
    sFilterConfig.FilterMaskIdHigh = 0xC6D7;
    sFilterConfig.FilterMaskIdLow = 0x8800;
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else if(Setting.Can_Type == 5) {  ////HK STD_ID
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterIdHigh = (u16)(0x316<<5); // EMS1
    sFilterConfig.FilterIdLow = (u16)(0x329<<5);  // EMS2
    sFilterConfig.FilterMaskIdHigh = (u16)(0x545<<5);  // EMS4
    sFilterConfig.FilterMaskIdLow = 0x0000;
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else if(Setting.Can_Type == 6) {  ////Ssangyong STD_ID
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterIdHigh = (u16)(0x308<<5); // RPM  
    sFilterConfig.FilterIdLow = (u16)(0x320<<5);  // Brake
    sFilterConfig.FilterMaskIdHigh = (u16)(0x409<<5);  // Speed
    sFilterConfig.FilterMaskIdLow = (u16)(0x608<<5);  // Fuel rate
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else if(Setting.Can_Type == 7) {  ////Renault STD_ID
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterNumber = 0;               //SM7
    sFilterConfig.FilterIdHigh = (u16)(0x17E<<5); // RPM  //0x17E, 0x186
    sFilterConfig.FilterIdLow = (u16)(0x29A<<5);  // Speed  //0x29A,0x29C,0x354,0x4F8,0x5D7
    sFilterConfig.FilterMaskIdHigh = (u16)(0x35C<<5);  // Brake //0x1F6, 0x352, 0x35C
//    sFilterConfig.FilterMaskIdLow = (u16)(0x186<<5);  // Fuel rate
    sFilterConfig.FilterMaskIdLow = (u16)(0x000<<5);  // Fuel rate
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
    sFilterConfig.FilterNumber = 1;                //Renault Master
    sFilterConfig.FilterIdHigh = (u16)(0x181<<5); // RPM  // 0x181, 0x1F9
    sFilterConfig.FilterIdLow = (u16)(0x284<<5);  // Speed  //0x284, 0x354
    sFilterConfig.FilterMaskIdHigh = (u16)(0x35D<<5);  // Brake //0x1F9, 0x35D
    sFilterConfig.FilterMaskIdLow = (u16)(0x000<<5);  // Fuel rate
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else {      ////J1939 EXT_ID                                   
    for(int i = 0; i< CAN_PGN_LIST_NUMBER; i++) {
      if(!(TestJigCAN2 && i == 0)) {
        sFilterConfig.FilterNumber = i;
        sFilterConfig.FilterIdHigh = (u16)((CAN_PGN_List[i]<<11)>>16);
        sFilterConfig.FilterIdLow = (u16)((CAN_PGN_List[i]<<11) | 4);
        sFilterConfig.FilterMaskIdHigh = (u16)((0xFFFF<<11)>>16);
        sFilterConfig.FilterMaskIdLow = (u16)((0xFFFF<<11) | 4);
        if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
      }
    }
  }  
  
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
#ifdef USE_CAN2_ADAS
  if(TestJigCAN2) { 
    can_baud = 2;  //250Kbps
    can_mode = CAN_MODE_SILENT;
  }
  else {
    can_baud = 3;  //500Kbps
    can_mode = CAN_MODE_NORMAL;
  }
#else  
  if(Setting.Can_Enable == SIG_LINE) return;
  if(Setting.CanBaudrate == 250) can_baud = 2;
  else if(Setting.CanBaudrate == 500) can_baud = 3;
  else can_baud = 2;

  if(Setting.CanMode == CAN_Mode_Normal || Setting.Can_Type < 4) can_mode = CAN_MODE_NORMAL;
  else can_mode = CAN_MODE_SILENT;
#endif  

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
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
  sFilterConfig.BankNumber = CAN_PGN_LIST_NUMBER;
  
  for(i = 0; i< 28-CAN_PGN_LIST_NUMBER; i++) {   //Initailize filter
    sFilterConfig.FilterNumber = i+CAN_PGN_LIST_NUMBER;
    sFilterConfig.FilterActivation = DISABLE;
    if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  
  sFilterConfig.FilterActivation = ENABLE;
  
#ifdef USE_CAN2_ADAS
  if(TestJigCAN2) { ////J1394 EXT_ID 
    for(int i = 0; i< 1; i++) {
      sFilterConfig.FilterNumber = i+CAN_PGN_LIST_NUMBER;
      sFilterConfig.FilterIdHigh = (u16)((CAN_PGN_List[i]<<11)>>16);
      sFilterConfig.FilterIdLow = (u16)((CAN_PGN_List[i]<<11) | 4);
      sFilterConfig.FilterMaskIdHigh = (u16)((0xFFFF<<11)>>16);
      sFilterConfig.FilterMaskIdLow = (u16)((0xFFFF<<11) | 4);
      if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
    }
  }
  else {                                        
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    
    sFilterConfig.FilterNumber = 0+CAN_PGN_LIST_NUMBER;
    sFilterConfig.FilterIdHigh = (u16)(0x700<<5); // MOBIL EYE ADAS
    sFilterConfig.FilterIdLow = (u16)(0x760<<5);  // MOBIL EYE ADAS
    sFilterConfig.FilterMaskIdHigh = 0x0000;  
    sFilterConfig.FilterMaskIdLow = 0x0000;
    if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
  }  

#else  
  if(Setting.Can_Type == 1) {   ////J1979 STD_ID
    sFilterConfig.FilterNumber = 0+CAN_PGN_LIST_NUMBER;
    sFilterConfig.FilterIdHigh = 0xFC00;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0xFC00;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else if(Setting.Can_Type == 2) {  ////J1979 EXT_ID
    sFilterConfig.FilterNumber = 0+CAN_PGN_LIST_NUMBER;
    sFilterConfig.FilterIdHigh = 0xC6D7; //18DAF100<<3
    sFilterConfig.FilterIdLow = 0x8800;
    sFilterConfig.FilterMaskIdHigh = 0xC6D7;
    sFilterConfig.FilterMaskIdLow = 0x8800;
    if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else if(Setting.Can_Type == 5) {  ////HK STD_ID
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterNumber = 0+CAN_PGN_LIST_NUMBER;
    sFilterConfig.FilterIdHigh = (u16)(0x316<<5); // EMS1
    sFilterConfig.FilterIdLow = (u16)(0x329<<5);  // EMS2
    sFilterConfig.FilterMaskIdHigh = (u16)(0x545<<5);  // EMS4
    sFilterConfig.FilterMaskIdLow = 0x0000;
    if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else if(Setting.Can_Type == 6) {  ////Ssangyong STD_ID
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterIdHigh = (u16)(0x308<<5); // RPM  
    sFilterConfig.FilterIdLow = (u16)(0x320<<5);  // Brake
    sFilterConfig.FilterMaskIdHigh = (u16)(0x409<<5);  // Speed
    sFilterConfig.FilterMaskIdLow = (u16)(0x608<<5);  // Fuel rate
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else if(Setting.Can_Type == 7) {  ////Renault STD_ID
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterIdHigh = (u16)(0x17E<<5); // RPM  //0x17E, 0x186
    sFilterConfig.FilterIdLow = (u16)(0x29A<<5);  // Speed  //0x29A,0x29C,0x354,0x4F8,0x5D7
    sFilterConfig.FilterMaskIdHigh = (u16)(0x35C<<5);  // Brake //0x1F6, 0x352, 0x35C
//    sFilterConfig.FilterMaskIdLow = (u16)(0x186<<5);  // Fuel rate
    sFilterConfig.FilterMaskIdLow = (u16)(0x000<<5);  // Fuel rate
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
    sFilterConfig.FilterNumber = 1;                //Renault Master
    sFilterConfig.FilterIdHigh = (u16)(0x181<<5); // RPM  // 0x181, 0x1F9
    sFilterConfig.FilterIdLow = (u16)(0x284<<5);  // Speed  //0x284, 0x354
    sFilterConfig.FilterMaskIdHigh = (u16)(0x35D<<5);  // Brake //0x1F9, 0x35D
    sFilterConfig.FilterMaskIdLow = (u16)(0x000<<5);  // Fuel rate
    if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) Error_Handler();
  }
  else {      ////J1394 EXT_ID                                   
    for(int i = 0; i< 28-CAN_PGN_LIST_NUMBER; i++) {
      sFilterConfig.FilterNumber = i+CAN_PGN_LIST_NUMBER;
      sFilterConfig.FilterIdHigh = (u16)((CAN_PGN_List[i]<<11)>>16);
      sFilterConfig.FilterIdLow = (u16)((CAN_PGN_List[i]<<11) | 4);
      sFilterConfig.FilterMaskIdHigh = (u16)((0xFFFF<<11)>>16);
      sFilterConfig.FilterMaskIdLow = (u16)((0xFFFF<<11) | 4);
      if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) Error_Handler();
    }
  }  
#endif  
  /*##-2- Start the Reception process and enable reception interrupt #########*/
  if(HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }    
}

/*
********************************************************************************
* Description : CAN1_TxData
* Arguments   : None
* Return      : 
* Note        : Send CAN Messages(Standard)
******************************************************************************** 
*/
void CAN1_TxData(u16 id, u8 pid)
{ 
  /* Transmit OBD Structure preparation */
  if(id < 0x7DF  || id > 0x7E7) id = 0x7DF;
  hcan1.pTxMsg->IDE = CAN_ID_STD;
  hcan1.pTxMsg->StdId = id;
  hcan1.pTxMsg->RTR = CAN_RTR_DATA;
  hcan1.pTxMsg->DLC = 8;
  hcan1.pTxMsg->Data[0] = 0x02;     // OBD req data length = 2
  hcan1.pTxMsg->Data[1] = 0x01;     // OBD Mode = 1 (Show Current Data)
  hcan1.pTxMsg->Data[2] = pid;      // 0x00  info 0x0c rpm  0x0d: speed
  hcan1.pTxMsg->Data[3] = 0x55;     // Dummy 
  hcan1.pTxMsg->Data[4] = 0x55;   
  hcan1.pTxMsg->Data[5] = 0x55;   
  hcan1.pTxMsg->Data[6] = 0x55;
  hcan1.pTxMsg->Data[7] = 0x55;
  HAL_CAN_Transmit(&hcan1, 1000);  
  Can_TxWait = 0;
}

/*
********************************************************************************
* Description : CAN1_TxExData
* Arguments   : None
* Return      : 
* Note        : Send CAN Messages(Extend)
******************************************************************************** 
*/
void CAN1_TxExData(u16 id, u16 pid)
{ 
  /* Transmit OBD Structure preparation */
  hcan1.pTxMsg->IDE = CAN_ID_EXT;
  hcan1.pTxMsg->ExtId = id;
  hcan1.pTxMsg->RTR = CAN_RTR_DATA;
  hcan1.pTxMsg->DLC = 8;
  hcan1.pTxMsg->Data[0] = 0x03;     // OBD req data length = 2
  hcan1.pTxMsg->Data[1] = 0x022;    // OBD Mode = 1 (Show Current Data)
  hcan1.pTxMsg->Data[2] = (pid>>8)&0xFF;   // 0x00  info 0x0c rpm  0x0d: speed
  hcan1.pTxMsg->Data[3] = pid&0xFF; // Dummy 
  hcan1.pTxMsg->Data[4] = 0x55;   
  hcan1.pTxMsg->Data[5] = 0x55;   
  hcan1.pTxMsg->Data[6] = 0x55;
  hcan1.pTxMsg->Data[7] = 0x55;   
  HAL_CAN_Transmit(&hcan1, 1000);    
  Can_TxWait = 0;
}

/*
********************************************************************************
* Description : TxCan_OBD
* Arguments   : None
* Return      : 
* Note        : Tx Can OBD
******************************************************************************** 
*/
void TxCan_OBD(void)
{     
  if(Setting.Can_Type != 1) return;
  
  switch(Can_TxStep) {
    case SEND_PID_00:
        CAN1_TxData(0x7DF, 0x00);
        Can_TxStep = CHECK_PID_00;
        Can_TxWait = 0;
        CAN_J1979Enable = 0;
        CAN_Key_Off_Time = 0;
      break;
    case CHECK_PID_00:
      Can_TxWait++;
      if(Can_TxWait > 2000 || CAN_J1979Enable == 1)  { 
        if(CAN_J1979Enable == 1) {
          Can_TxStep = SEND_PID_40;
        }
        else
          Can_TxStep = SEND_PID_00;
        Can_TxWait = 0;
      }
      break;
    case SEND_PID_40:
        CAN_Key_Off_Time = 0;
        CAN1_TxData(0x7DF, 0x40);
        Can_TxStep = CHECK_PID_40;
        Can_TxWait = 0;
      break;
    case CHECK_PID_40:
      Can_TxWait++;
      if(Can_TxWait > 2000 || CAN_J1979Enable == 2)  { 
        Can_TxStep = SEND_PID_RPM;
        Can_TxWait = 0;
      }
      break;
    case SEND_PID_RPM:
      if(CAN_TxEnable.RPM) {
        CAN_RxComplete.RPM = 0;
        CAN1_TxData(CAN_ECU_ID.RPM, 0x0C);
        Can_TxStep = CHECK_PID_RPM;
      }
      else {
        Can_TxStep = SEND_PID_SPEED;
      }
      Can_TxWait = 0;
      break;
    case CHECK_PID_RPM:
      Can_TxWait++;
      if(Can_TxWait > 10 || CAN_RxComplete.RPM)  { 
        CAN_RxComplete.RPM = 0;
        Can_TxStep = SEND_PID_SPEED;
        Can_TxWait = 0;
      }
      break;
    case SEND_PID_SPEED:
      if(CAN_TxEnable.Speed) {
        CAN_RxComplete.Speed = 0;
        CAN1_TxData(CAN_ECU_ID.Speed, 0x0D);
        Can_TxStep = CHECK_PID_SPEED;
      }
      else {
        Can_TxStep = SEND_PID_MAF;
      }
      Can_TxWait = 0;
      break;
    case CHECK_PID_SPEED:
      Can_TxWait++;
      if(Can_TxWait > 10 || CAN_RxComplete.Speed)  { 
        CAN_RxComplete.Speed = 0;
        Can_TxStep = SEND_PID_MAF;
        Can_TxWait = 0;
      }
      break;
    case SEND_PID_MAF:
      if(CAN_TxEnable.MAF) {
        CAN_RxComplete.MAF = 0;
        CAN1_TxData(CAN_ECU_ID.MAF, 0x10);
        Can_TxStep = CHECK_PID_MAF;
      }
      else {
        Can_TxStep = SEND_PID_FUELRATE;
      }
      Can_TxWait = 0;
      break;
    case CHECK_PID_MAF:
      Can_TxWait++;
      if(Can_TxWait > 10 || CAN_RxComplete.MAF)  { 
        CAN_RxComplete.MAF = 0;
        Can_TxStep = SEND_PID_FUELRATE;
        Can_TxWait = 0;
      }
      break;
    case SEND_PID_FUELRATE:
      if(CAN_TxEnable.FuelRate) {
        CAN_RxComplete.FuelRate = 0;
        CAN1_TxData(CAN_ECU_ID.FuelRate, 0x5E);
        Can_TxStep = CHECK_PID_FUELRATE;
      }
      else {
        Can_TxStep = J1979_IDLE;
      }
      Can_TxWait = 0;
      break;
    case CHECK_PID_FUELRATE:
      Can_TxWait++;
      if(Can_TxWait > 10 || CAN_RxComplete.FuelRate)  { 
        CAN_RxComplete.FuelRate = 0;
        Can_TxStep = J1979_IDLE;
        Can_TxWait = 0;
      }
      break;
    case J1979_IDLE:
      Can_TxWait++;
      if(Can_TxWait > 100)  { 
        Can_TxStep = SEND_PID_RPM;
        Can_TxWait = 0;
      }
      break;
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
    if(CAN_Enable.Speed) {
      if(Cur_CanEnableCnt.Speed != Pre_CanEnableCnt.Speed)
        Pre_CanEnableCnt.Speed = Cur_CanEnableCnt.Speed;
      else {
        CAN_Enable.Speed = 0;
        if(!CAN_Enable.Speed2)
          CAN_Val.Speed = 0;
      }
    }
    if(CAN_Enable.RPM) {
      if(Cur_CanEnableCnt.RPM != Pre_CanEnableCnt.RPM)
        Pre_CanEnableCnt.RPM = Cur_CanEnableCnt.RPM;
      else {
        CAN_Enable.RPM = 0;
        if(!CAN_Enable.RPM2)
          CAN_Val.RPM = 0;
      }
    }
    if(CAN_Enable.Brake) {
      if(Cur_CanEnableCnt.Brake != Pre_CanEnableCnt.Brake)
        Pre_CanEnableCnt.Brake = Cur_CanEnableCnt.Brake;
      else {
        CAN_Enable.Brake = 0;
        if(!CAN_Enable.Brake2)
          CAN_Val.Brake = 0;
      }
    }
    if(CAN_Enable.FuelRate) {
      if(Cur_CanEnableCnt.FuelRate != Pre_CanEnableCnt.FuelRate)
        Pre_CanEnableCnt.FuelRate = Cur_CanEnableCnt.FuelRate;
      else {
        CAN_Enable.FuelRate = 0;
        CAN_Val.FuelRateSum = 0;
        CAN_Val.FuelRateCnt = 0;
      }
    }
    if(CAN_Enable.KPL) {
      if(Cur_CanEnableCnt.KPL != Pre_CanEnableCnt.KPL)
        Pre_CanEnableCnt.KPL = Cur_CanEnableCnt.KPL;
      else {
        CAN_Enable.KPL = 0;
        if(!CAN_Enable.KPL2) {
          CAN_Val.InstKPLSum = 0;
          CAN_Val.InstKPLCnt = 0;
          CAN_Val.AvgKPL = 0;
        }
      }
    }
    if(CAN_Enable.ODO) {
      if(Cur_CanEnableCnt.ODO != Pre_CanEnableCnt.ODO)
        Pre_CanEnableCnt.ODO = Cur_CanEnableCnt.ODO;
      else 
        CAN_Enable.ODO = 0;
    }
    if(CAN_Enable.Speed2) {
      if(Cur_CanEnableCnt.Speed2 != Pre_CanEnableCnt.Speed2)
        Pre_CanEnableCnt.Speed2 = Cur_CanEnableCnt.Speed2;
      else {
        CAN_Enable.Speed2 = 0;
        if(!CAN_Enable.Speed)
          CAN_Val.Speed = 0;
      }
    }
    if(CAN_Enable.RPM2) {
      if(Cur_CanEnableCnt.RPM2 != Pre_CanEnableCnt.RPM2)
        Pre_CanEnableCnt.RPM2 = Cur_CanEnableCnt.RPM2;
      else {
        CAN_Enable.RPM2 = 0;
        if(!CAN_Enable.RPM)
          CAN_Val.RPM = 0;
      }
    }
    if(CAN_Enable.Brake2) {
      if(Cur_CanEnableCnt.Brake2 != Pre_CanEnableCnt.Brake2)
        Pre_CanEnableCnt.Brake2 = Cur_CanEnableCnt.Brake2;
      else {
        CAN_Enable.Brake2 = 0;
        if(!CAN_Enable.Brake)
          CAN_Val.Brake = 0;
      }
    }
    if(CAN_Enable.TFuel) {
      if(Cur_CanEnableCnt.TFuel != Pre_CanEnableCnt.TFuel)
        Pre_CanEnableCnt.TFuel = Cur_CanEnableCnt.TFuel;
      else
        CAN_Enable.TFuel = 0;
    }
    if(CAN_Enable.KPL2) {
      if(Cur_CanEnableCnt.KPL2 != Pre_CanEnableCnt.KPL2)
        Pre_CanEnableCnt.KPL2 = Cur_CanEnableCnt.KPL2;
      else {
        CAN_Enable.KPL2 = 0;
        if(!CAN_Enable.KPL) {
          CAN_Val.InstKPLSum = 0;
          CAN_Val.InstKPLCnt = 0;
          CAN_Val.AvgKPL = 0;
        }
      }
    }
    if(CAN_Enable.ODO2) {
      if(Cur_CanEnableCnt.ODO2 != Pre_CanEnableCnt.ODO2)
        Pre_CanEnableCnt.ODO2 = Cur_CanEnableCnt.ODO2;
      else
        CAN_Enable.ODO2 = 0;
    }  
    if(CAN_Enable.MAF) {
      if(Cur_CanEnableCnt.MAF != Pre_CanEnableCnt.MAF)
        Pre_CanEnableCnt.MAF = Cur_CanEnableCnt.MAF;
      else
        CAN_Enable.MAF = 0;
    }
    if(CAN_Enable.AEBS) {
      if(Cur_CanEnableCnt.AEBS != Pre_CanEnableCnt.AEBS)
        Pre_CanEnableCnt.AEBS = Cur_CanEnableCnt.AEBS;
      else
        CAN_Enable.AEBS = 0;
    }
    if(CAN_Enable.LDWS) {
      if(Cur_CanEnableCnt.LDWS != Pre_CanEnableCnt.LDWS)
        Pre_CanEnableCnt.LDWS = Cur_CanEnableCnt.LDWS;
      else
        CAN_Enable.LDWS = 0;
    }
    if(CAN_Enable.PBrake) {
      if(Cur_CanEnableCnt.PBrake != Pre_CanEnableCnt.PBrake)
        Pre_CanEnableCnt.PBrake = Cur_CanEnableCnt.PBrake;
      else
        CAN_Enable.PBrake = 0;
    }
    if(CAN_Enable.LPAlarm) {
      if(Cur_CanEnableCnt.LPAlarm != Pre_CanEnableCnt.LPAlarm)
        Pre_CanEnableCnt.LPAlarm = Cur_CanEnableCnt.LPAlarm;
      else
        CAN_Enable.LPAlarm = 0;
    }
    if(CAN_Enable.CoolantTemp) {
      if(Cur_CanEnableCnt.CoolantTemp != Pre_CanEnableCnt.CoolantTemp)
        Pre_CanEnableCnt.CoolantTemp = Cur_CanEnableCnt.CoolantTemp;
      else
        CAN_Enable.CoolantTemp = 0;
    }
    if(CAN_Enable.DPF) {
      if(Cur_CanEnableCnt.DPF != Pre_CanEnableCnt.DPF)
        Pre_CanEnableCnt.DPF = Cur_CanEnableCnt.DPF;
      else
        CAN_Enable.DPF = 0;
    }
    if(CAN_Enable.NGear) {
      if(Cur_CanEnableCnt.NGear != Pre_CanEnableCnt.NGear)
        Pre_CanEnableCnt.NGear = Cur_CanEnableCnt.NGear;
      else
        CAN_Enable.NGear = 0;
    }
  }
}

u8 CAN2_ADAS_TxData(void)
{ 
  HAL_StatusTypeDef state;
  /* Transmit ADAS Structure preparation */
  hcan2.pTxMsg->IDE = CAN_ID_STD;
  hcan2.pTxMsg->StdId = 0x700;
  hcan2.pTxMsg->RTR = CAN_RTR_DATA;
  hcan2.pTxMsg->DLC = 8;
  hcan2.pTxMsg->Data[0] = 0x00;     
  hcan2.pTxMsg->Data[1] = 0x00;    
  hcan2.pTxMsg->Data[2] = 0x00;      
  hcan2.pTxMsg->Data[3] = 0x00;     
  hcan2.pTxMsg->Data[4] = 0x00;   
  hcan2.pTxMsg->Data[5] = 0x00;   
  hcan2.pTxMsg->Data[6] = 0x00;
  hcan2.pTxMsg->Data[7] = 0x00;
  state = HAL_CAN_Transmit(&hcan2, 1000);  
  if(state != HAL_OK) {
    return 0;
  }
  return 1;
}

u8 PreFcwOn, PreRLdwOn, PreLLdwOn, PrePedsFcw, PreHwl, PreUFcw, PreSli, PreFailSafe;
/*
********************************************************************************
* Description : CAN_Std_Decode
* Arguments   : None
* Return      : 
* Note        : CAN Decode(Standard)
******************************************************************************** 
*/
u8 RenaultMasterFlag;
void CAN_Std_Decode(CAN_HandleTypeDef* hcan)
{
  u32 tmp;
  
#ifdef USE_CAN2_ADAS
  u8 fcw_on = 0, r_ldw_on = 0, l_ldw_on = 0, peds_fcw = 0, hwl = 0, ldw_on = 0, u_fcw = 0, sli = 0, failsafe = 0;
  
  if(hcan->Instance == CAN2) {
    if(hcan->pRxMsg->StdId == 0x700) {
      fcw_on = (hcan->pRxMsg->Data[4]>>3)&0x01;
      r_ldw_on = (hcan->pRxMsg->Data[4]>>2)&0x01;
      l_ldw_on = (hcan->pRxMsg->Data[4]>>1)&0x01;
      ldw_on = hcan->pRxMsg->Data[4]&0x01;
      failsafe = (hcan->pRxMsg->Data[4]>>7)&0x01;
      if(ldw_on) ldw_on = 0;
      else ldw_on = 1;
      peds_fcw = (hcan->pRxMsg->Data[5]>>1)&0x01;
      hwl = hcan->pRxMsg->Data[7]&0x03;
      sli = hcan->pRxMsg->Data[6]&0x07;
      CAN_Enable.AEBS = 1;
      Cur_CanEnableCnt.AEBS++;
      CAN_Enable.LDWS = 1;
      Cur_CanEnableCnt.LDWS++;
      CAN_Val.AEBS = (fcw_on<<4)|(peds_fcw<<5)|(hwl<<6); //bit 5,6,7,8
                                                         //bit 5: Mobile eye FCW ON 
                                                         //bit 6: Mobile eye Peds_FCW ON 
                                                         //bit 7,8: Mobile eye Head way warning level (0~2 단계) 
      CAN_Val.LDWS1 = (r_ldw_on<<4)|(l_ldw_on<<6);
      CAN_Val.LDWS2 = (ldw_on<<6);

      // 전방추돌 경고횟수
      if(!PreFcwOn && fcw_on) {
        ADASEventCount.FCWOn++;
      }
      PreFcwOn = fcw_on;
      
      // 우측차선이탈 경고횟수
      if(!PreRLdwOn && ldw_on && r_ldw_on) {
        ADASEventCount.RightLDWOn++;
      }
      PreRLdwOn = r_ldw_on;
      
      // 좌측차선이탈 경고횟수  
      if(!PreLLdwOn && ldw_on && l_ldw_on) {
        ADASEventCount.LeftLDWOn++;
      }
      PreLLdwOn = l_ldw_on;
      
      // 보행자 전방추돌 경고횟수  
      if(!PrePedsFcw && peds_fcw) {
        ADASEventCount.PedesFDWOn++;
      }
      PrePedsFcw = peds_fcw;
      
      //진행중 차간거리 경고횟수
      if(!PreHwl && hwl > 0) {
        ADASEventCount.HWL++;
      }
      PreHwl = hwl;
      
      // 도심지 전방추돌 경고횟수
      if(!PreUFcw && u_fcw > 0) {
        ADASEventCount.U_FCW++;
      }
      PreUFcw = u_fcw;      
     
      // 제한속도 초과 경고횟수
      if(!PreSli && sli > 0) {
        ADASEventCount.SLI++;
      }
      PreSli = sli;      

      // 카메라 이믈질 에러
      if(!PreFailSafe) {    // 발생
        if(failsafe)
          ADASEventCount.FailSafeOn++;
      }
      else {                // 해소
        if(!failsafe)
          ADASEventCount.FailSafeOff++;
      }
      PreFailSafe = failsafe;  
      
      TestJigCAN2_OK = 1;
    }
    else return;
  }
#endif  
  
  if(Setting.Can_Type == 1) { 
    if(hcan->pRxMsg->StdId >= 0x7E8 && hcan->pRxMsg->StdId <= 0x7EF) {
      if(hcan->pRxMsg->Data[1] == 0x41) {
        if(hcan->pRxMsg->Data[2] == 0x00) {
          CAN_J1979Enable = 1;
          if((hcan->pRxMsg->Data[4]&0x10) == 0x10) {
            CAN_TxEnable.RPM = 1;
            CAN_ECU_ID.RPM = hcan->pRxMsg->StdId-8;
          }
          if((hcan->pRxMsg->Data[4]&0x08) == 0x08) {
            CAN_TxEnable.Speed = 1;
            CAN_ECU_ID.Speed = hcan->pRxMsg->StdId-8;
          }
          if((hcan->pRxMsg->Data[4]&0x01) == 0x01) {
            CAN_TxEnable.MAF = 1;
            CAN_ECU_ID.MAF = hcan->pRxMsg->StdId-8;
          }
        }
        else if(hcan->pRxMsg->Data[2] == 0x40) {
          CAN_J1979Enable = 2;
          if((hcan->pRxMsg->Data[6]&0x04) == 0x04) {
            CAN_TxEnable.FuelRate = 1;
            CAN_ECU_ID.FuelRate = hcan->pRxMsg->StdId-8;
          }
        }
        else if(hcan->pRxMsg->Data[2] == 0x0C) {
          tmp = (hcan->pRxMsg->Data[3]<<8)+hcan->pRxMsg->Data[4]; 
          if(tmp < 0xFFFF) {
            CAN_Val.RPM = tmp/4;
            CAN_RxComplete.RPM = 1;
            CAN_Enable.RPM = 1;
            Cur_CanEnableCnt.RPM++;
          }
        }
        else if(hcan->pRxMsg->Data[2] == 0x0D) {
          if(hcan->pRxMsg->Data[3] < 0xFF) {
            CAN_Val.Speed = hcan->pRxMsg->Data[3];   
            CAN_RxComplete.Speed = 1;
            CAN_Enable.Speed = 1;
            Cur_CanEnableCnt.Speed++;
          }
        }  
        else if(hcan->pRxMsg->Data[2] == 0x10) {
          tmp = (hcan->pRxMsg->Data[3]<<8)+hcan->pRxMsg->Data[4]; 
          if(tmp < 0xFFFF && !CAN_TxEnable.FuelRate) {
            CAN_Val.MAF = tmp/100.0;
            CAN_RxComplete.MAF = 1;
            CAN_Enable.MAF = 1;
            CAN_Val.FuelRate = (CAN_Val.MAF/10.88)*1000000.0/3600.0; // ul/sec  10.88 = 14.7*0.74 가솔린  디젤은 12.5 = 14.7*0.85
            CAN_Val.FuelRateSum = CAN_Val.FuelRate;
            CAN_Val.FuelRateCnt = 1;
            CAN_Enable.FuelRate = 1;
            Cur_CanEnableCnt.FuelRate++;
          }
        }
        else if(hcan->pRxMsg->Data[2] == 0x5E) {
          tmp = (hcan->pRxMsg->Data[3]<<8)+hcan->pRxMsg->Data[4]; 
          if(tmp < 0xFFFF) {
            CAN_Val.FuelRate = (double)tmp*50000.0/3600.0; // ul/sec
            CAN_Val.FuelRateSum = CAN_Val.FuelRate;
            CAN_Val.FuelRateCnt = 1;
            CAN_RxComplete.FuelRate = 1;
            CAN_Enable.FuelRate = 1;
            Cur_CanEnableCnt.FuelRate++;
            CAN_TxEnable.MAF = 0;
          }
        }
      }
    }
  }
  else if(Setting.Can_Type == 5) { 
    if(hcan->pRxMsg->StdId == 0x316) {
      tmp = (hcan->pRxMsg->Data[3]<<8)+hcan->pRxMsg->Data[2]; 
      if(tmp < 0xFFFF) {
        CAN_Val.RPM = tmp/4; 
        CAN_Enable.RPM = 1;
        Cur_CanEnableCnt.RPM++;
      }
      if(hcan->pRxMsg->Data[6] < 0xFF) {
        CAN_Val.Speed = hcan->pRxMsg->Data[6];  
        Cur_CanEnableCnt.Speed++;
        CAN_Enable.Speed = 1;
      }
    }
    else if(hcan->pRxMsg->StdId == 0x329) {
      if((hcan->pRxMsg->Data[4]&0x03) == 0x02)
        CAN_Val.Brake = 1;
      else if((hcan->pRxMsg->Data[4]&0x03) == 0x01)
        CAN_Val.Brake = 0;
      CAN_Enable.Brake = 1;
      Cur_CanEnableCnt.Brake++;
    }
    else if(hcan->pRxMsg->StdId == 0x545) {
      tmp = (hcan->pRxMsg->Data[2]<<8)+hcan->pRxMsg->Data[1]; 
      if(tmp < 0xFFFF) {
        CAN_Val.FuelRate = (double)tmp*0.128*100.0;   // ul/s
        CAN_Val.FuelRateSum = CAN_Val.FuelRateSum + CAN_Val.FuelRate;
        CAN_Val.FuelRateCnt++;
        CAN_Enable.FuelRate = 1;
        Cur_CanEnableCnt.FuelRate++;
      }
    }    
  }    
  else if(Setting.Can_Type == 6) { 
    if(hcan->pRxMsg->StdId == 0x308) {
      tmp = (hcan->pRxMsg->Data[1]<<8)+hcan->pRxMsg->Data[2]; 
      if(tmp < 0xFFFF) {
        CAN_Val.RPM = tmp; 
        CAN_Enable.RPM = 1;
        Cur_CanEnableCnt.RPM++;
      }
    }
    else if(hcan->pRxMsg->StdId == 0x320) {
      if((hcan->pRxMsg->Data[4]&0x04) == 0x04)
        CAN_Val.Brake = 1;
      else CAN_Val.Brake = 0;
      CAN_Enable.Brake = 1;
      Cur_CanEnableCnt.Brake++;
    }
    else if(hcan->pRxMsg->StdId == 0x409) {
      if(hcan->pRxMsg->Data[2] < 0xFF) {
        CAN_Val.Speed = hcan->pRxMsg->Data[2];  
        Cur_CanEnableCnt.Speed++;
        CAN_Enable.Speed = 1;
      }
    }
    else if(hcan->pRxMsg->StdId == 0x608) {
      tmp = (hcan->pRxMsg->Data[2]<<8)+hcan->pRxMsg->Data[3]; 
      if(tmp < 0xFFFF) {
        CAN_Val.FuelRate = (double)tmp;   // ul/s
        CAN_Val.FuelRateSum = CAN_Val.FuelRateSum + CAN_Val.FuelRate;
        CAN_Val.FuelRateCnt++;
        CAN_Enable.FuelRate = 1;
        Cur_CanEnableCnt.FuelRate++;
      }
    }    
  }    
  else if(Setting.Can_Type == 7) { 
    if(hcan->pRxMsg->StdId == 0x17E) {  //SM7 //0x186
      tmp = (hcan->pRxMsg->Data[0]<<8)+hcan->pRxMsg->Data[1]; 
      if(tmp < 0xFFFF) {
        CAN_Val.RPM = tmp;    //0x186 CAN_Val.RPM = tmp/8
        CAN_Enable.RPM = 1;
        Cur_CanEnableCnt.RPM++;
        RenaultMasterFlag = 0;
      }
    }   
    else if(hcan->pRxMsg->StdId == 0x181) {  //Renault Master //0x1F9
      tmp = (hcan->pRxMsg->Data[0]<<8)+hcan->pRxMsg->Data[1]; //0x1F9 (Data[2]<<8) + Data[3]
      if(tmp < 0xFFFF) {
        CAN_Val.RPM = tmp/8;    //0x1F9 CAN_Val.RPM = tmp/8
        CAN_Enable.RPM = 1;
        Cur_CanEnableCnt.RPM++;
        RenaultMasterFlag = 1;
      }
    }   
    else if(hcan->pRxMsg->StdId == 0x35C) {   //SM7 //0x1F6(10ms),0x352(40ms),0x35C(100ms)
      if((hcan->pRxMsg->Data[5]&0x80) == 0x80)  //0x1F6 Data[2]&0x08, 0x352 Data[0]&0x08, 0x35C Data[5]&0x80
        CAN_Val.Brake = 1;
      else CAN_Val.Brake = 0;
      CAN_Enable.Brake = 1;
      Cur_CanEnableCnt.Brake++;
    }
    else if(hcan->pRxMsg->StdId == 0x35D) {   //Renault Master //0x1F9
      if((hcan->pRxMsg->Data[4]&0x10) == 0x10)  //0x1F9 Data[5] 0D->FF, 0x35D Data[4]&0x10
        CAN_Val.Brake = 1;
      else CAN_Val.Brake = 0;
      CAN_Enable.Brake = 1;
      Cur_CanEnableCnt.Brake++;
    }
    else if(hcan->pRxMsg->StdId == 0x29A) {  //SM7 //0x29A(20ms,0x354(40ms),0x5D7(100ms),0x4F8(100ms)
      tmp = (hcan->pRxMsg->Data[4]<<8)+hcan->pRxMsg->Data[5]; //0x354(D0D1), 0x5D7(D0D1), 0x4F8(D3D4)
      if(tmp < 50000) {
        CAN_Val.Speed = tmp/100;  
        Cur_CanEnableCnt.Speed++;
        CAN_Enable.Speed = 1;
      }
    }
    else if(hcan->pRxMsg->StdId == 0x284) {  //Renault Master //0x284(20ms,0x354(40ms)
      tmp = (hcan->pRxMsg->Data[4]<<8)+hcan->pRxMsg->Data[5]; //0x354(D0D1)
      if(tmp < 50000) {
        CAN_Val.Speed = tmp/100;  
        Cur_CanEnableCnt.Speed++;
        CAN_Enable.Speed = 1;
      }
    }
//    else if(hcan->pRxMsg->StdId == 0x186) {
//      tmp = (hcan->pRxMsg->Data[6]<<8)+hcan->pRxMsg->Data[7]; 
//      if(tmp < 0xFFFF) {
//        CAN_Val.FuelRate = (double)tmp;   // ul/s
//        CAN_Val.FuelRateSum = CAN_Val.FuelRateSum + CAN_Val.FuelRate;
//        CAN_Val.FuelRateCnt++;
//        CAN_Enable.FuelRate = 1;
//        Cur_CanEnableCnt.FuelRate++;
//      }
//    }    
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
void CAN_Ext_Decode(CAN_HandleTypeDef* hcan)
{
  u16 pgn;
  u32 tmp;
  
  pgn = (hcan->pRxMsg->ExtId>>8) & 0xFFFF;
  
  switch(pgn) {
    case 0xF004:
      tmp = (hcan->pRxMsg->Data[4]<<8)+hcan->pRxMsg->Data[3];
      if(tmp < 0xFFFF) {
        CAN_Val.RPM = tmp/8; 
        CAN_Enable.RPM = 1;
        Cur_CanEnableCnt.RPM++;
        if(hcan->Instance == CAN2) {
          TestJigCAN2_OK = 1;
        }
      }
      break;
    case 0xFD7C:
      if((hcan->pRxMsg->Data[0]&0x07) < 7 || (hcan->pRxMsg->Data[1]&0x0F) < 0x0F ) {
        if((hcan->pRxMsg->Data[0]&0x07) == 1 || (hcan->pRxMsg->Data[0]&0x07) == 4     //DPF LAMP ON / BLINK
           || (hcan->pRxMsg->Data[1]&0x03) == 1 || (hcan->pRxMsg->Data[1]&0x0C) == 4)  //DPF Passive Regen Active / Active Regen Active
          CAN_Val.DPF = 1;   //DPF Lamp ON or Regen Active
        else
          CAN_Val.DPF = 0;
        CAN_Enable.DPF = 1;
        Cur_CanEnableCnt.DPF++;
      }
      break;
    case 0xFEE0:
      tmp = (hcan->pRxMsg->Data[7]<<24)+(hcan->pRxMsg->Data[6]<<16)+(hcan->pRxMsg->Data[5]<<8)+hcan->pRxMsg->Data[4];
      if(tmp > 0 && tmp < 8000000 && !CAN_Enable.ODO) { //ODO 1000000Km 미만
        CAN_Val.ODO = (double)tmp*0.125; //Km
        CAN_Enable.ODO2 = 1;
        Cur_CanEnableCnt.ODO2++;
      }
      break;
    case 0xFEE6:
      if(hcan->pRxMsg->Data[0] < 0xFF) {
        CAN_Val.Time[0] = hcan->pRxMsg->Data[0]/4; // sec
        CAN_Val.Time[1] = hcan->pRxMsg->Data[1];   // min 
        CAN_Val.Time[2] = hcan->pRxMsg->Data[2];   // hour
        CAN_Val.Time[3] = hcan->pRxMsg->Data[3];   // mon
        CAN_Val.Time[4] = hcan->pRxMsg->Data[4]/4+1; // day 
        CAN_Val.Time[5] = hcan->pRxMsg->Data[5]+1985-2000; // year
      }
      break;
    case 0xFEE9:
      tmp = (hcan->pRxMsg->Data[7]<<24)+(hcan->pRxMsg->Data[6]<<16)+(hcan->pRxMsg->Data[5]<<8)+hcan->pRxMsg->Data[4];
      if(tmp < 0xFFFFFFFF) {
        CAN_Val.TFuel = (double)tmp*0.5; //Liter
        CAN_Enable.TFuel = 1;
        Cur_CanEnableCnt.TFuel++;
      }
      break;
    case 0xFEEE:   //coolant temp
      if(hcan->pRxMsg->Data[0] < 0xFF) {
        CAN_Val.CoolantTemp = hcan->pRxMsg->Data[0]-40; //degree
        CAN_Enable.CoolantTemp = 1;
        Cur_CanEnableCnt.CoolantTemp++;
      }
      break;
    case 0xFEF1:
      if((hcan->pRxMsg->Data[0]&0x08) == 0) {
        CAN_Val.PBrake = (hcan->pRxMsg->Data[0]>>2)&0x01;
        CAN_Enable.PBrake = 1;
        Cur_CanEnableCnt.PBrake++;
      }
      if(hcan->pRxMsg->Data[2] < 0xFF) {
        CAN_Val.Speed = hcan->pRxMsg->Data[2];
        CAN_Enable.Speed = 1;
        Cur_CanEnableCnt.Speed++;
      }
      if((hcan->pRxMsg->Data[3]&0x20) == 0) {
        CAN_Val.Brake = (hcan->pRxMsg->Data[3]>>4)&0x01;
        CAN_Enable.Brake = 1;
        Cur_CanEnableCnt.Brake++;
      }
      break;
    case 0xFEF2:
      tmp = (hcan->pRxMsg->Data[1]<<8)+hcan->pRxMsg->Data[0];
      if(tmp < 0xFFFF) {
        CAN_Val.FuelRate = (double)tmp*50000.0/3600.0;   // ul/sec
        CAN_Val.FuelRateSum = CAN_Val.FuelRateSum + CAN_Val.FuelRate;
        CAN_Enable.FuelRate = 1;
        CAN_Val.FuelRateCnt++;
        Cur_CanEnableCnt.FuelRate++;
      }
      tmp = (hcan->pRxMsg->Data[3]<<8)+hcan->pRxMsg->Data[2];
      if(tmp < 0xFFFF) {
        CAN_Val.InstKPL = (double)tmp/512.0;     // Km/L
        CAN_Val.InstKPLSum = CAN_Val.InstKPLSum + CAN_Val.InstKPL;
        CAN_Val.InstKPLCnt++;
        CAN_Enable.KPL = 1;
        Cur_CanEnableCnt.KPL++;
      }
      tmp = (hcan->pRxMsg->Data[5]<<8)+hcan->pRxMsg->Data[4];
      if(tmp < 0xFFFF && !CAN_Enable.KPL) {
        CAN_Val.AvgKPL = (double)tmp/512.0;     // Km/L
        CAN_Enable.KPL2 = 1;
        Cur_CanEnableCnt.KPL2++;
      }
      break;
    case 0xFEC1:
      tmp = (hcan->pRxMsg->Data[3]<<24)+(hcan->pRxMsg->Data[2]<<16)+(hcan->pRxMsg->Data[1]<<8)+hcan->pRxMsg->Data[0];
      if(tmp > 0 && tmp < 200000000) { //ODO 1000000Km 미만
        CAN_Val.ODO = (double)tmp*0.005; //Km
        CAN_Val.PODO = CAN_Val.ODO;
        CAN_Enable.ODO = 1;
        Cur_CanEnableCnt.ODO++;
      }
      break;
    case 0xFE6C:
      if(hcan->pRxMsg->Data[7] < 0xFF && !CAN_Enable.Speed) {
        CAN_Val.Speed = hcan->pRxMsg->Data[7];
        CAN_Enable.Speed2 = 1;
        Cur_CanEnableCnt.Speed2++;
      }
      tmp = (hcan->pRxMsg->Data[5]<<8)+hcan->pRxMsg->Data[4];
      if(tmp < 0xFFFF && !CAN_Enable.RPM) {
        CAN_Val.RPM = tmp/8; 
        CAN_Enable.RPM2 = 1;
        Cur_CanEnableCnt.RPM2++;
      }
      break;
    case 0xFF56:
      if(hcan->pRxMsg->Data[7] < 0xFF && !CAN_Enable.Speed) {
        CAN_Val.Speed = hcan->pRxMsg->Data[7];
        CAN_Enable.Speed2 = 1;
        Cur_CanEnableCnt.Speed2++;
      }
      break;
    case 0xFDC4:
      if((hcan->pRxMsg->Data[1]&0x20) == 0 && !CAN_Enable.Brake) {
        CAN_Val.Brake = (hcan->pRxMsg->Data[1]>>4)&0x01;
        CAN_Enable.Brake2 = 1;
        Cur_CanEnableCnt.Brake2++;
      }
      break;
      
    //- kumho express 
    case 0xF02F:
      CAN_Val.AEBS = hcan->pRxMsg->Data[0];     //bit 1,2,3,4 
                                                        //0010 운전자가 스위치로 AEBS 시스템을 끈 것
                                                        //0011 AEBS 작동 대기의 정상 상태
                                                        //0101 단순 경보 상태
                                                        //0110 부분 제동 동작 상태
                                                        //0111풀제동 동작 상태 
      CAN_Enable.AEBS = 1;
      Cur_CanEnableCnt.AEBS++;
      break;      
    case 0xF007:
      CAN_Val.LDWS1 = hcan->pRxMsg->Data[0];   //bit5,6  Right Lane  (00 : 이탈없음  01: 차선이탈경고, 10:  경고등Error  11: Not Available)
                                                        //bit7,8  Left Lane  (00 : 이탈없음  01: 차선이탈경고, 10:  경고등Error  11: Not Available)  
      break;
    case 0xFE5B:
      CAN_Val.LDWS2 = hcan->pRxMsg->Data[0];   //bit7,8  차선이탈 경고 On/Off  (00 표시등 On (LDWS 꺼짐) , 01 표시등 Off (LDWS 켜짐))
      CAN_Enable.LDWS = 1;
      Cur_CanEnableCnt.LDWS++;
      break;
    default : break;
  }
}

/*
********************************************************************************
* Description : Check_ADAS_Event
* Arguments   : None
* Return      : 
* Note        : Check Tx ADAS Event
******************************************************************************** 
*/
void Check_ADAS_Event(void) 
{
  if(!EventAdasFlag && !EventTxComplete) {
    ADASPckData.EventCode = 0;
    if(ADASEventCount.FCWOn != PreADASEventCount.FCWOn) 
      ADASPckData.EventCode = 'F';
    else if(ADASEventCount.RightLDWOn != PreADASEventCount.RightLDWOn) 
      ADASPckData.EventCode = 'R';
    else if(ADASEventCount.LeftLDWOn != PreADASEventCount.LeftLDWOn) 
      ADASPckData.EventCode = 'L';
    else if(ADASEventCount.PedesFDWOn != PreADASEventCount.PedesFDWOn) 
      ADASPckData.EventCode = 'P';
    else if(ADASEventCount.HWL != PreADASEventCount.HWL) 
      ADASPckData.EventCode = 'H';
    else if(ADASEventCount.U_FCW != PreADASEventCount.U_FCW) 
      ADASPckData.EventCode = 'U';
    else if(ADASEventCount.SLI != PreADASEventCount.SLI) 
      ADASPckData.EventCode = 'T';
    else if(ADASEventCount.FailSafeOn != PreADASEventCount.FailSafeOn)
      ADASPckData.EventCode = 'S';
    else if(ADASEventCount.FailSafeOff != PreADASEventCount.FailSafeOff)
      ADASPckData.EventCode = 'D';    
    if(ADASPckData.EventCode) {
      EventAdasFlag = 1;
      EventAdasRetryCount = 0;
    }
  }        
  if(EventTxComplete) {
    if(ADASPckData.EventCode == 'F') 
      PreADASEventCount.FCWOn = ADASEventCount.FCWOn;
    else if(ADASPckData.EventCode == 'R') 
      PreADASEventCount.RightLDWOn = ADASEventCount.RightLDWOn;
    else if(ADASPckData.EventCode == 'L') 
      PreADASEventCount.LeftLDWOn = ADASEventCount.LeftLDWOn;
    else if(ADASPckData.EventCode == 'P') 
      PreADASEventCount.PedesFDWOn = ADASEventCount.PedesFDWOn;
    else if(ADASPckData.EventCode == 'H') 
      PreADASEventCount.HWL = ADASEventCount.HWL;
    else if(ADASPckData.EventCode == 'U') 
      PreADASEventCount.U_FCW = ADASEventCount.U_FCW;
    else if(ADASPckData.EventCode == 'T') 
      PreADASEventCount.SLI = ADASEventCount.SLI;
    else if(ADASPckData.EventCode == 'S')
      PreADASEventCount.FailSafeOn = ADASEventCount.FailSafeOn;
    else if(ADASPckData.EventCode == 'D')
      PreADASEventCount.FailSafeOff = ADASEventCount.FailSafeOff;
    EventTxComplete = 0;
  }
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
    TxCan_OBD();
    Check_ADAS_Event();
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
