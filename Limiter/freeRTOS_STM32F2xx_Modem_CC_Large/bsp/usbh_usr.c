/**
  ******************************************************************************
  * @file    Audio_playback_and_record/src/usbh_usr.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-October-2011
  * @brief   This file includes the usb host user callbacks
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_usr.h"
#include "includes.h"

/** @addtogroup STM32F4-Discovery_Audio_Player_Recorder
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */


USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

const uint8_t MSG_HOST_INIT[]        = "> Host Library Initialized\r\n";
const uint8_t MSG_DEV_ATTACHED[]     = "> Device Attached \r\n";
const uint8_t MSG_DEV_DISCONNECTED[] = "> Device Disconnected\r\n";
const uint8_t MSG_DEV_ENUMERATED[]   = "> Enumeration completed \r\n";
const uint8_t MSG_DEV_HIGHSPEED[]    = "> High speed device detected\r\n";
const uint8_t MSG_DEV_FULLSPEED[]    = "> Full speed device detected\r\n";
const uint8_t MSG_DEV_LOWSPEED[]     = "> Low speed device detected\r\n";
const uint8_t MSG_DEV_ERROR[]        = "> Device fault \r\n";

const uint8_t MSG_MSC_CLASS[]        = "> Mass storage device connected\r\n";
const uint8_t MSG_HID_CLASS[]        = "> HID device connected\r\n";
const uint8_t MSG_DISK_SIZE[]        = "> Size of the disk in MBytes: \r\n";
const uint8_t MSG_LUN[]              = "> LUN Available in the device:\r\n";
const uint8_t MSG_ROOT_CONT[]        = "> Exploring disk flash ...\r\n";
const uint8_t MSG_WR_PROTECT[]       = "> The disk is write protected\r\n";
const uint8_t MSG_UNREC_ERROR[]      = "> UNRECOVERED ERROR STATE\r\n";


#define __DEBUG_USB_MSG
/**
  * @brief  USBH_USR_Init
  * @param  None
  * @retval None
  */

extern TM_USB_MSCHOST_Result_t 	TM_USB_MSCHOST_INT_Result;
uint8_t Application = USH_USR_FS_INIT;


void USBH_USR_Init(void)
{
#ifdef __DEBUG_USB_MSG
#ifdef USE_USB_OTG_HS 
  printd(" USB OTG HS MSC Host\r\n");
#else
  printd(" USB OTG FS MSC Host\r\n");
#endif
    printd(">USB Host library started.\r\n"); 
    printd(" USB Host Library v2.1.0\r\n" );
#endif
	/* Device has been initialized, device is not connected yet */
	TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_Disconnected;
	/* We have to initialize first */
	Application = USH_USR_FS_INIT;

}


/**
  * @brief  USBH_USR_DeInit
  *         Deint User state and associated variables
  * @param  None
  * @retval None
  */
void USBH_USR_DeInit(void)
{
	Application = USH_USR_FS_INIT;
}

/**
  * @brief  USBH_USR_DeviceAttached
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAttached(void)
{  
#ifdef __DEBUG_USB_MSG
  printd((void *)MSG_DEV_ATTACHED);  
#endif  
  /* Device has been initialized, device is not connected yet */
	TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_Disconnected;
	/* We have to initialize first */
	Application = USH_USR_FS_INIT;
}

/**
  * @brief  USBH_USR_UnrecoveredError
  * @param  None
  * @retval None
  */
void USBH_USR_UnrecoveredError (void)
{
#ifdef __DEBUG_USB_MSG  
  /* Set default screen color*/ 
  printd((void *)MSG_UNREC_ERROR); 
#endif  
  /* Device has been initialized, device is not connected yet */
	TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_Disconnected;
	/* We have to initialize first */
	Application = USH_USR_FS_INIT;
}

/**
  * @brief  USBH_DisconnectEvent
  *         Device disconnect event
  * @param  None
  * @retval Staus
  */
void USBH_USR_DeviceDisconnected (void)
{    
#ifdef __DEBUG_USB_MSG
  /* Set default screen color*/
  printd((void *)MSG_DEV_DISCONNECTED);
#endif  
  //USB_Detect_Flag = 0;
	/* Device has been initialized, device is not connected yet */
	TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_Disconnected;
	/* We have to initialize first */
	Application = USH_USR_FS_INIT;
	printd("DETECT 0");
}

/**
  * @brief  USBH_USR_ResetUSBDevice
  * @param  None
  * @retval None
  */
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
	Application = USH_USR_FS_INIT;
}


/**
  * @brief  USBH_USR_DeviceSpeedDetected
  *         Displays the message on LCD for device speed
  * @param  Device speed:
  * @retval None
  */
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
#ifdef __DEBUG_USB_MSG  
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    printd((void *)MSG_DEV_HIGHSPEED);
  }  
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    printd((void *)MSG_DEV_FULLSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    printd((void *)MSG_DEV_LOWSPEED);
  }
  else
  {
    printd((void *)MSG_DEV_ERROR);
  }
#endif  
}

/**
  * @brief  USBH_USR_Device_DescAvailable
  * @param  device descriptor
  * @retval None
  */
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  /* callback for device descriptor */
#ifdef __DEBUG_USB_MSG  
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;  
  
  
  printd("VID : %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
  printd("PID : %04Xh\r\n" , (uint32_t)(*hs).idProduct); 
#endif  
}

/**
  * @brief  USBH_USR_DeviceAddressAssigned
  *         USB device is successfully assigned the Address
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAddressAssigned(void)
{
  /* callback for device successfully assigned the Address */
}

/**
  * @brief  USBH_USR_Conf_Desc
  * @param  Configuration descriptor
  * @retval None
  */
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
    USBH_InterfaceDesc_TypeDef *itfDesc,
    USBH_EpDesc_TypeDef *epDesc)
{
  /* callback for configuration descriptor */
#ifdef __DEBUG_USB_MSG  
  USBH_InterfaceDesc_TypeDef *id;
  
  id = itfDesc;  
  
  if((*id).bInterfaceClass  == 0x08)
  {
    printd((void *)MSG_MSC_CLASS);
  }
  else if((*id).bInterfaceClass  == 0x03)
  {
    printd((void *)MSG_HID_CLASS);
  }    
#endif  
}

/**
  * @brief  USBH_USR_Manufacturer_String
  * @param  Manufacturer String
  * @retval None
  */
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
#ifdef __DEBUG_USB_MSG  
  /* callback for  Manufacturer String */
  printd("Manufacturer : %s\r\n", (char *)ManufacturerString);
#endif  
}

/**
  * @brief  USBH_USR_Product_String
  * @param  Product String
  * @retval None
  */
void USBH_USR_Product_String(void *ProductString)
{
#ifdef __DEBUG_USB_MSG
  /* callback for Product String */
  printd("Product : %s\r\n", (char *)ProductString);  
#endif  
}

/**
  * @brief  USBH_USR_SerialNum_String
  * @param  SerialNum_String
  * @retval None
  */
void USBH_USR_SerialNum_String(void *SerialNumString)
{
#ifdef __DEBUG_USB_MSG  
  /* callback for SerialNum_String */
  printd( "Serial Number : %s\r\n", (char *)SerialNumString);    
#endif  
}

/**
  * @brief  EnumerationDone 
  *         User response request is displayed to ask application jump to class
  * @param  None
  * @retval None
  */
void USBH_USR_EnumerationDone(void)
{
#ifdef __DEBUG_USB_MSG  
  printd((void *)MSG_DEV_ENUMERATED);
#endif  
} 

/**
  * @brief  USBH_USR_DeviceNotSupported
  *         Device is not supported
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceNotSupported(void)
{
#ifdef __DEBUG_USB_MSG  
  printd ("> Device not supported.\r\n"); 
#endif  
  TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_DeviceNotSupported;
}


/**
  * @brief  USBH_USR_UserInput
  *         User Action for application state entry
  * @param  None
  * @retval USBH_USR_Status : User response for key button
  */
USBH_USR_Status USBH_USR_UserInput(void)
{
  /* callback for Key botton: set by software in this case */
  return USBH_USR_RESP_OK;
}

/**
  * @brief  USBH_USR_OverCurrentDetected
  *         Over Current Detected on VBUS
  * @param  None
  * @retval None
  */
void USBH_USR_OverCurrentDetected (void)
{
	/* Error */
	TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_Error;
}

/**
  * @brief  USBH_USR_MSC_Application
  * @param  None
  * @retval Staus
  */
//extern static BYTE check_fs (FATFS *fs,WORD sect);
int USBH_USR_MSC_Application(void)
{
	/* Device is connected */
	if (Application == USH_USR_FS_INIT) 
	{
		Application = USH_USR_FS_LOOP;
		/* Device is connected */
		TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_Connected;
		if (USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
		{
			/* Device is write protected */
			TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_WriteProtected;
			printk("Write protected......\n");
		}
	}
	return 0;
}


#if 0
	char fname[20] = {0,};
	sprintf(fname,"1:usbmkh.txt");
	switch (USBH_USR_ApplicationState)
	{
	case USH_USR_FS_INIT:
		if (f_mount( &USB_Fs, "1:", 1 ) != FR_OK ) 
		{
		  /* efs initialisation fails*/
			printk("USB Mount Fail.....\n");
			return(-1);
		}
			/* Flash Disk is write protected */
		if (USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
		{ 
			return(-1);
		}
		USBH_USR_ApplicationState = USH_USR_APP;
		printk("USB Mount OK.....\n");
		printk("USB step.....[%d]\n",USBH_USR_ApplicationState);
		break;
    case USH_USR_APP:
		f_unlink(fname);
		if(f_open(&USB_Fil,fname,FA_READ | FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
		{
			f_printf(&USB_Fil,"First string in my file\n");
			f_close(&USB_Fil);
			printk("USB fine write.....\n");
		}
		USBH_USR_ApplicationState = USH_USR_IDLE;
		printk("USB step.....[%d]\n",USBH_USR_ApplicationState);
		break;
    case USH_USR_IDLE:
		break;
    default:
		break;
  }

  return(0);
}
#endif


#if 0


void ProgramUpdateMsg(void)
{
  if(ProgramUpdateMsgNo > 0 && ProgramUpdateMsgNo < 6) {  
    lcd_clear();
    switch(ProgramUpdateMsgNo) {
      case 1:
            LCD_printf(0,0,0," Incorrect File ");
            LCD_printf(0,2,0,"Can Not Download");  
            break;
      case 2:
            LCD_printf(0,0,0,"Found New SW Ver");
            LCD_printf(0,2,0,"  System Reset  ");   
            break;
      case 3:
            LCD_printf(0,0,0,"NAND Update Fail");
            LCD_printf(0,2,0,"Can not download");  
            break;
      case 4:
            LCD_printf(0,0,0,"  Same SW Ver   ");
            LCD_printf(0,2,0," Updated Already");   
            break;
      case 5:
            LCD_printf(0,0,0,"Previous SW Ver ");
            LCD_printf(0,2,0,"Can Not Download");   
            break;
    }
    delay_ms(1000);  
  }
}
          
void BootUpdateMsg(void)
{
  if(BootUpdateMsgNo > 0 && BootUpdateMsgNo < 6) {
    lcd_clear();
    switch(BootUpdateMsgNo) {
      case 1:
            LCD_printf(0,0,0,"Boot Loader File");
            LCD_printf(0,2,0,"Update...       ");  
            break;
      case 2:
            LCD_printf(0,0,0,"Boot Loader File");
            LCD_printf(0,2,0,"Update Fail!     ");  
            LCD_printf(0,4,0,"Try Again!!     ");  
            break;
      case 3:
            LCD_printf(0,0,0,"Boot Loader File");
            LCD_printf(0,2,0,"NAND Update Fail");
            LCD_printf(0,4,0,"Can Not Download");  
            break;
      case 4:
            LCD_printf(0,0,0,"Boot Loader File");
            LCD_printf(0,2,0," Updated Already");   
            break;
      case 5:
            LCD_printf(0,0,0,"Boot Loader File");
            LCD_printf(0,2,0," Updated Already");   
        break;
    }  
    delay_ms(1000);
  }
}

void ProgramFileUpdate(void)
{
  FIL tfile; 	
  u32 val0, val1, jump = 0, startCode;
	u16 bytesRead;
	u32 i, j, n, checksum = 0, read_checksum = 0, nand_addr, nand_checksum = 0;
	u8 buff[4], error = 0;

  IWDG_ReloadCounter(); 
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return;
  }
  DownFileLength = 0;
  nand_addr = NAND_PROGRAM_ADDR;
  memset(IDE_DnBuff, 0, NAND_PAGE_SIZE);
  if (f_open(&tfile, DOWNLOAD_FILENAME, FA_READ) == FR_OK) {
    if(!(tfile.fsize%4) && tfile.fsize < USER_PROGRAM_SIZE) {
      n = tfile.fsize/4;
      for(i = 0; i < n; i++) {
        IWDG_ReloadCounter(); 
        if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) return;
        if(f_read (&tfile, buff, 4, (void *)&bytesRead) == FR_OK) {
          if(bytesRead != 4) { 
            error = 1;
            break;
          }
          if(i == 0) {
            memcpy((u8*)&startCode, buff, 4);
            if ((startCode & 0x2FFE0000 ) != 0x20000000) {
              error = 1;
              break;
            }
          }
          if(i == n-1)
            memcpy((u8*)&read_checksum, buff, 4);
          else {
            for(j = 0; j < bytesRead; j++)
              checksum += buff[j];
          }
          memcpy(&IDE_DnBuff[DownFileLength%NAND_PAGE_SIZE],buff,4);
          DownFileLength +=4;
          if(DownFileLength%NAND_PAGE_SIZE == 0) {
            IWDG_ReloadCounter();
            Nand_Write_Buffer(IDE_DnBuff, nand_addr, NAND_PAGE_SIZE); 
            nand_addr += NAND_PAGE_SIZE;
            memset(IDE_DnBuff, 0, NAND_PAGE_SIZE);
          }
        }
        else {
          error = 1;
          break;
        }
      }
      IWDG_ReloadCounter();
      if(DownFileLength%NAND_PAGE_SIZE != 0 && DownFileLength > NAND_PAGE_SIZE) {
        Nand_Write_Buffer(IDE_DnBuff, nand_addr, NAND_PAGE_SIZE); 
        memset(IDE_DnBuff, 0, NAND_PAGE_SIZE);
      }
    }
    else
      error = 1;
    f_close (&tfile);
  }
  if(checksum != read_checksum || error) {
    if(HCD_IsDeviceConnected(&USB_OTG_Core) == 1)
      ProgramUpdateMsgNo = 1;
    return;
  }
  jump = FRAMByteRead(offsetof(st_FRAM_Data,UpdateCheck)); 
  val0 = ((u32)finfo.fdate*0x10000) + (u32)finfo.ftime;

  if(jump) {
    FRAMMultiWrite((u8 *)&val0, offsetof(st_FRAM_Data,FW_Date), 4);  
    FRAMByteWrite(0,offsetof(st_FRAM_Data,UpdateCheck));
  }
  else {
    IWDG_ReloadCounter();
    FRAMMultiRead((u8 *)&val1, offsetof(st_FRAM_Data,FW_Date), 4);  
    if(val0 > val1 && DownFileLength > 4) { //1.4.0.0
      n = (DownFileLength-4)/NAND_PAGE_SIZE;
      nand_addr = NAND_PROGRAM_ADDR;
      checksum = 0;
      nand_checksum = 0;
      for(i = 0; i < n; i++) {
        IWDG_ReloadCounter();
        Nand_Read_Buffer(IDE_DnBuff, nand_addr, NAND_PAGE_SIZE); 
        for(j = 0; j < NAND_PAGE_SIZE; j++)
            checksum += IDE_DnBuff[j];
        nand_addr += NAND_PAGE_SIZE;
      }
      n = (DownFileLength-4)%NAND_PAGE_SIZE;
      IWDG_ReloadCounter();
      Nand_Read_Buffer(IDE_DnBuff, nand_addr, n); 
      for(i = 0; i < n; i++)
          checksum += IDE_DnBuff[i];
      nand_addr += n;
      Nand_Read_Buffer((u8*)&nand_checksum, nand_addr, 4); 
      if(checksum == nand_checksum && nand_checksum == read_checksum) {
//        IT_Configuration(DISABLE);
        IWDG_ReloadCounter();
        FRAMMultiWrite((u8 *)&val0, offsetof(st_FRAM_Data,FW_File_Date), 4);  
        FRAMMultiWrite((u8 *)&DownFileLength, offsetof(st_FRAM_Data,FW_Size), 4);  
        FRAMByteWrite(10,offsetof(st_FRAM_Data,UpdateCheck));   
#ifdef __DATE_UPDATE_ENABLE        
        Fram.VuIdentification.vuSoftInstallationDate = 0; 		
        FramWriteData((u8*)&Fram.VuIdentification.vuSoftInstallationDate,4);
#endif        
        Fram.PowerInterruption.powerFlag = 0;
        FramWrite(offsetof(st_FRAM_Data,PowerInterruption),sizeof(Fram.PowerInterruption)); 
        ProgramUpdateMsgNo = 2;
        ProgramResetFlag = 1;
//        NVIC_SystemReset();
      }
      else {
        ProgramUpdateMsgNo = 3;
      }
    }
    else if(val0 == val1) {
      ProgramUpdateMsgNo = 4;
    }
    else {
      ProgramUpdateMsgNo = 5;
    }
  }  
}

void BootFileUpdate(void)
{
  FIL tfile; 	
  u32 startCode;
	u16 bytesRead;
	u32 i, j, n, checksum = 0, read_checksum = 0, nand_addr, nand_checksum = 0, boot_checksum = 0;
	u8 buff[4], error = 0;
  __IO uint32_t address;

  IWDG_ReloadCounter(); 
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return;
  }
  DownFileLength = 0;
  nand_addr = NAND_BOOT_PROGRAM_ADDR;
  memset(IDE_DnBuff, 0, NAND_PAGE_SIZE);
  if (f_open(&tfile, BOOTLOADER_FILENAME, FA_READ) == FR_OK) {
    if(!(tfile.fsize%4) && tfile.fsize < BOOTLOADER_SIZE) {
      n = tfile.fsize/4;
      for(i = 0; i < n; i++) {
        IWDG_ReloadCounter(); 
        if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) return;
        if(f_read (&tfile, buff, 4, (void *)&bytesRead) == FR_OK) {
          if(bytesRead != 4) { 
            error = 1;
            break;
          }
          if(i == 0) {
            memcpy((u8*)&startCode, buff, 4);
            if ((startCode & 0x2FFE0000 ) != 0x20000000) {
              error = 1;
              break;
            }
          }
          if(i == n-1) 
            memcpy((u8*)&read_checksum, buff, 4);
          else {
            for(j = 0; j < bytesRead; j++)
              checksum += buff[j];
          }
          memcpy(&IDE_DnBuff[DownFileLength%NAND_PAGE_SIZE],buff,4);
          DownFileLength +=4;
          if(DownFileLength%NAND_PAGE_SIZE == 0) {
            IWDG_ReloadCounter();
            Nand_Write_Buffer(IDE_DnBuff, nand_addr, NAND_PAGE_SIZE); 
            nand_addr += NAND_PAGE_SIZE;
            memset(IDE_DnBuff, 0, NAND_PAGE_SIZE);
          }
        }
        else {
          error = 1;
          break;
        }
      }
      IWDG_ReloadCounter();
      if(DownFileLength%NAND_PAGE_SIZE != 0 && DownFileLength > NAND_PAGE_SIZE) {
        Nand_Write_Buffer(IDE_DnBuff, nand_addr, NAND_PAGE_SIZE); 
        memset(IDE_DnBuff, 0, NAND_PAGE_SIZE);
      }
    }
    else
      error = 1;
    f_close (&tfile);
  }
  if(checksum == read_checksum && DownFileLength > 4 && !error) {
    IWDG_ReloadCounter();
    address = BOOTLOADER_ADDRESS+DownFileLength-4;
    boot_checksum = 0;
    for(i = 0; i < 4; i++)
      buff[i] = (*(uint8_t*)(address++));
    memcpy((u8*)&boot_checksum, buff, 4);
    if(read_checksum != boot_checksum) {
      IWDG_ReloadCounter(); 
      n = (DownFileLength-4)/NAND_PAGE_SIZE;
      nand_addr = NAND_BOOT_PROGRAM_ADDR;
      checksum = 0;
      nand_checksum = 0;
      for(i = 0; i < n; i++) {
        IWDG_ReloadCounter();
        Nand_Read_Buffer(IDE_DnBuff, nand_addr, NAND_PAGE_SIZE); 
        for(j = 0; j < NAND_PAGE_SIZE; j++)
            checksum += IDE_DnBuff[j];
        nand_addr += NAND_PAGE_SIZE;
      }
      IWDG_ReloadCounter();
      n = (DownFileLength-4)%NAND_PAGE_SIZE;
      Nand_Read_Buffer(IDE_DnBuff, nand_addr, n); 
      for(i = 0; i < n; i++)
          checksum += IDE_DnBuff[i];
      nand_addr += n;
      Nand_Read_Buffer((u8*)&nand_checksum, nand_addr, 4); 
      if(checksum == nand_checksum && nand_checksum == read_checksum) {
        IWDG_ReloadCounter();
        BootUpdateMsgNo = 1;
        if(BOOT_DOWNLOAD() == 0) {
//          IT_Configuration(DISABLE);
          Fram.PowerInterruption.powerFlag = 0;
          FramWrite(offsetof(st_FRAM_Data,PowerInterruption),sizeof(Fram.PowerInterruption)); 
          ProgramResetFlag = 1;
//          NVIC_SystemReset();
        }
        else {
          BootUpdateMsgNo = 2;
        }
      }
      else {
        BootUpdateMsgNo = 3;
      }
    }
    else {
      BootUpdateMsgNo = 4;
    }
  }
  else {
    BootUpdateMsgNo = 5;
  }
}


/**
  * @brief  USBH_USR_MSC_Application
  * @param  None
  * @retval Staus
  */
//extern static BYTE check_fs (FATFS *fs,WORD sect);
int USBH_USR_MSC_Application(void)
{

  switch (USBH_USR_ApplicationState)
  {
    case USH_USR_FS_INIT:

      /* Initialises the File System*/
      IWDG_ReloadCounter();
      if (f_mount( &fatfs, "", 0 ) != FR_OK ) 
      {
        /* efs initialisation fails*/
        return(-1);
      }
  
      /* Flash Disk is write protected */
      if (USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
      { 
        return(-1);
      }
      USB_Detect_Flag = 1;
      if(USB_DL_Flag) {
        USBH_USR_ApplicationState = USH_USR_APP;
        break;
      }
      ProgramResetFlag = 0;
      ProgramUpdateMsgNo = 0;
      BootUpdateMsgNo = 0;
      //- Upgrade File Detected -//
      if(USB_Detect_Flag) {
        IWDG_ReloadCounter();
        if(f_stat(DOWNLOAD_FILENAME,&finfo) == FR_OK) {
          ProgramFileUpdate();
        } 
      }
      if(USB_Detect_Flag) {
        IWDG_ReloadCounter();
        if(f_stat(BOOTLOADER_FILENAME,&finfo) == FR_OK) {
          BootFileUpdate();
        } 
      }
      ProgramUpdateMsg();      
      BootUpdateMsg();
      if(ProgramResetFlag)
        NVIC_SystemReset();
      /* Go to menu */ 
//      if(ProgramUpdateMsgNo == 0 && BootUpdateMsgNo == 0) {  // USB Connect È®ÀÎ¿ë..
//        LCD_printf(0,2,0," USB Connected! ");
//        delay_ms(500);
//      }
      USBH_USR_ApplicationState = USH_USR_IDLE;
      break;

    case USH_USR_APP:
      if(USB_Detect_Flag) {
//        IT_Configuration(DISABLE);
        BUZZER_OFF();
        COMMAND_ExecuteApplication();  
//        IT_Configuration(ENABLE);
      } 
      USB_DL_Flag = 0;
      USBH_USR_ApplicationState = USH_USR_IDLE;
      break;

    case USH_USR_IDLE:
      if(USB_DL_Flag && USB_Detect_Flag) {
        USBH_USR_ApplicationState = USH_USR_APP;
      }

      break;

    default:
      break;
  }
  return(0);
}

/**
  * @brief  Test_USB Func
  * @param  None
  * @retval None
  */
u16 Test_USB(void)
{
  UINT bytesWritten, bytesRead;  
  FIL tfile;  
  u8 usbWrite_err_cnt = 0, usbRead_err_cnt = 0;  
  u8 wbuff[20] = {0,}, rbuff[20] = {0,} ,i;
  char fname[20] = {0,};

  sprintf(fname,"TestDTG.txt");
  IWDG_ReloadCounter();

  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return 0xffff;
  }

  f_unlink(fname);
  IWDG_ReloadCounter(); 
  if(f_open(&tfile, fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
    USB_Error_Flag = 1;
    usbWrite_err_cnt = 0xff;
    return 0xffff;
  }
  IWDG_ReloadCounter(); 
  for(i = 0; i < 10; i++) {
    sprintf((char*)wbuff,"USB TEST %d",i);
    if(f_write (&tfile, wbuff, 10, &bytesWritten) != FR_OK) {
      usbWrite_err_cnt++;
    }    
  }
  if(f_close (&tfile) != FR_OK) {
    USB_Error_Flag = 1;
    return 0xffff;
  }
  IWDG_ReloadCounter(); 
  
  if (f_open(&tfile, fname, FA_READ) != FR_OK)
  {
    USB_Error_Flag = 1;
    usbRead_err_cnt = 0xff;
    return 0xffff;
  }
  IWDG_ReloadCounter(); 
  for(i = 0; i < 10; i++) {
    sprintf((char*)wbuff,"USB TEST %d",i);
    if(f_read (&tfile, rbuff, 10, (void *)&bytesRead) != FR_OK) {
       usbRead_err_cnt++;
    }
    else if(!IsSameMem(wbuff,rbuff,16)) usbRead_err_cnt++;
  }
  if(f_close (&tfile) != FR_OK) {
    USB_Error_Flag = 1;
    return 0xffff;
  }
  return (((usbWrite_err_cnt<<8)&0xff00)|usbRead_err_cnt);
}

/**
  * @brief  Up Calibration Data Func
  * @param  None
  * @retval None
  */

union Chk_Sum{
  u16 w;
  u8 b[2];
};
union W_Data{
  u16 w;
  u8 b[2];
};
u16 UploadCalData(void)
{
  UINT bytesRead;
  FIL tfile;
  u16 chksum = 0, i;
  u8 slot;  
  u8 rbuff[sizeof(st_VuCalibrationRecord)+6+86+2] = {0,};
  char fname[20] = {0,};
  union Chk_Sum rChksum;
  union W_Data V_dataTmp, Work_dataTmp;
  u8 can_baud, elec_leakage;

  sprintf(fname,"DTG-S1.PRM");
  IWDG_ReloadCounter();

  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return 1;
  }
  IWDG_ReloadCounter();
  
  if (f_open(&tfile, fname, FA_READ) != FR_OK)
  {
    USB_Error_Flag = 1;//File Open Error
    return 1;
  }
  IWDG_ReloadCounter();
  
  if(f_read (&tfile, (u8*)&rbuff, sizeof(st_VuCalibrationRecord)+6+86+2, (void*)&bytesRead) != FR_OK) {
    USB_Error_Flag = 1;//File Read Error
    return 1;
  }
  if(f_close (&tfile) != FR_OK) {
    USB_Error_Flag = 1;//File Close Error
    return 1;
  }
  IWDG_ReloadCounter();
  
  for(i = 0; i < sizeof(st_VuCalibrationRecord); i++)
    chksum += rbuff[i];
  for(i = 0; i < 2; i++) {
    V_dataTmp.b[i] = rbuff[sizeof(st_VuCalibrationRecord)+i];
    chksum += rbuff[sizeof(st_VuCalibrationRecord)+i];
  }
  for(i = 0; i < 2; i++) {
    Work_dataTmp.b[i] = rbuff[sizeof(st_VuCalibrationRecord)+i+2];
    chksum += rbuff[sizeof(st_VuCalibrationRecord)+i+2];
  }
  elec_leakage = rbuff[sizeof(st_VuCalibrationRecord)+4];
  chksum += rbuff[sizeof(st_VuCalibrationRecord)+4];
  can_baud = rbuff[sizeof(st_VuCalibrationRecord)+5];
  chksum += rbuff[sizeof(st_VuCalibrationRecord)+5];
  for(i = 0; i < 86; i++)
    chksum += rbuff[sizeof(st_VuCalibrationRecord)+i+6];
  for(i = 0; i < 2; i++)
    rChksum.b[i] = rbuff[sizeof(st_VuCalibrationRecord)+i+92];
  
  IWDG_ReloadCounter();
  
  if(rChksum.w == chksum) {
    if(CardType[0] == WORKSHOP_CARD) slot = 0;
    else if(CardType[1] == WORKSHOP_CARD) slot = 1; 
    else slot = 0xff;
    if(slot < 2) {
      memcpy((u8*)&TmpCalibData, rbuff, sizeof(st_VuCalibrationRecord));
      if(RTC_LVal+(RTC_LVal%(3600*24)) > TmpCalibData.nextCalibrationDate) {
        lcd_clear();
        LCD_printf(0,0,0,"Next Calibration");
        LCD_printf(0,2,0,"  Date Error!   ");
        delay_ms(1000);
        return 1;
      }
      TmpCalibData.oldTimeValue = CalibrationRecord.oldTimeValue;
      TmpCalibData.newTimeValue = CalibrationRecord.newTimeValue;
      memcpy((u8*)&TmpCalibData.oldOdometerValue, (u8*)&CalibrationRecord.oldOdometerValue, 3);
      if(!IsSameMem((u8*)&CalibrationRecord.vehicleIdNumber, (u8*)&TmpCalibData.vehicleIdNumber, sizeof(st_VuCalibrationRecord)-95)) {
        if(IsSameMem((u8*)&TmpCalibData.vehicleRegistration, (u8*)&CalibrationRecord.vehicleRegistration, sizeof(st_VehicleRegistration))) 
          TmpCalibData.calibrationPurpose = 4;
        else
          TmpCalibData.calibrationPurpose = 3;
        memcpy((u8*)&CalibrationRecord, (u8*)&TmpCalibData, sizeof(st_VuCalibrationRecord)-12);
				memcpy((u8*)&CalibrationRecord.workshopName, (u8*)&WorkshopIdentification[slot].WorkshopCardHolderId.workshopName, sizeof(st_NameField));
				memcpy((u8*)&CalibrationRecord.workshopAddress, (u8*)&WorkshopIdentification[slot].WorkshopCardHolderId.workshopAddress, sizeof(st_NameField));
        memcpy((u8*)&CalibrationRecord.workshopCardNumber, (u8*)&VuCardIWRecord[slot].FullCardNumber, sizeof(st_fullCardNumber));
        CalibrationRecord.workshopCardExpiryDate = VuCardIWRecord[slot].cardExpiryDate;
        ODOValToBuff(CalibrationRecord.oldOdometerValue,(u32)Fram.TTrip);
        Fram.TTrip = (double)ODOBuffToVal(CalibrationRecord.newOdometerValue);
        CalibrationRecord.oldTimeValue = RTC_LVal;
        CalibrationRecord.newTimeValue = CalibrationRecord.oldTimeValue;
        CardCalibrationRecord.oldTimeValue = SwapEndian32(RTC_LVal);
        CardCalibrationRecord.newTimeValue = CardCalibrationRecord.oldTimeValue;
        CalibrationRecord.nextCalibrationDate = TmpCalibData.nextCalibrationDate;// RTC_LVal+(2*24*365*3600);
        CardCalibrationRecord.nextCalibrationDate = SwapEndian32(CalibrationRecord.nextCalibrationDate);
        
        memcpy((u8*)&CardCalibrationRecord.vehicleIdentificationNumber, (u8*)&CalibrationRecord.vehicleIdNumber, 17);
        memcpy((u8*)&CardCalibrationRecord.vehicleRegistration, (u8*)&CalibrationRecord.vehicleRegistration, sizeof(st_VehicleRegistration));
        memcpy((u8*)&CardCalibrationRecord.tyreSize, (u8*)&CalibrationRecord.tyreSize, 15);
        MemCopyChangeEndian((u8*)&CardCalibrationRecord.wVehicleCharacteristicConstant, (u8*)&CalibrationRecord.wVehicleCharacteristicConstant, 2);
        MemCopyChangeEndian((u8*)&CardCalibrationRecord.kConstantOfRecordingEquipment, (u8*)&CalibrationRecord.kConstantOfRecordingEquipment, 2);
        MemCopyChangeEndian((u8*)&CardCalibrationRecord.lTyreCircumference, (u8*)&CalibrationRecord.lTyreCircumference, 2);
        MemCopyChangeEndian((u8*)&CardCalibrationRecord.oldOdometerValue, (u8*)&CalibrationRecord.oldOdometerValue, 3);
        MemCopyChangeEndian((u8*)&CardCalibrationRecord.newOdometerValue, (u8*)&CalibrationRecord.newOdometerValue, 3);
        CardCalibrationRecord.authorisedSpeed = CalibrationRecord.authorisedSpeed;
        
        Nand_Write_Calibration(CalibrationRecord);
        Write_Calibration_Data();
        Write_WorkshopDownload(1);
        CalibToFram();
      }
      else {
        if(Fram.V_Factor == V_dataTmp.w && Fram.WorkNumber == Work_dataTmp.w && Fram.CAN_BaudrateFlag == can_baud && Fram.ElectricLeakageFlag == elec_leakage) {
          lcd_clear();
          LCD_printf(0,2,0," Same Parameter!");
          delay_ms(1000);
          return 1;
        }
      }
      Fram.V_Factor = V_dataTmp.w;
      FramWrite(offsetof(st_FRAM_Data,V_Factor),sizeof(Fram.V_Factor));
      Fram.WorkNumber = Work_dataTmp.w;
      FramWrite(offsetof(st_FRAM_Data,WorkNumber),sizeof(Fram.WorkNumber));
      Fram.CAN_BaudrateFlag = can_baud;
      FramWrite(offsetof(st_FRAM_Data,CAN_BaudrateFlag),sizeof(Fram.CAN_BaudrateFlag));
      CAN_Configuration();
      Fram.ElectricLeakageFlag = elec_leakage;
      FramWrite(offsetof(st_FRAM_Data,ElectricLeakageFlag),sizeof(Fram.ElectricLeakageFlag));  
    }
  }
  else {
    lcd_clear();
    LCD_printf(0,2,0," Incorrect File ");
    delay_ms(1000);
    USB_Error_Flag = 1;//Chksum Error
    return 1;
  }
  
  return 0;
}

/**
  * @brief  Down Calibration Data Func
  * @param  None
  * @retval None
  */
__packed typedef struct {
  u8  calibrationPurpose;
  st_NameField workshopName;
  st_NameField workshopAddress;
  st_fullCardNumber workshopCardNumber;
  long workshopCardExpiryDate;
  u8  vehicleIdNumber[17];
  st_VehicleRegistration vehicleRegistration;
  u16 wVehicleCharacteristicConstant;
  u16 kConstantOfRecordingEquipment;
  u16 lTyreCircumference;
  u8  tyreSize[15];
  u8  authorisedSpeed;
  u8  oldOdometerValue[3];
  u8  newOdometerValue[3];
  long oldTimeValue;
  long newTimeValue;
  long nextCalibrationDate;
  // Add Parameger
  u16 V_Factor;
  u16 WorkShopNumber;
  u8  ElectricLeakageFlag;
  u8  CAN_BaudrateFlag;  
  // Add Information
  u8  ManuFactureName[35];
  u8  vuPartNumber[16];
  u8  vuSerialNumber[8];
  long  vuManufacturingDate;
  u8  NCMNumber;
  u8  NCMPartNumber[16];
  u8  NCMExPiredDate[6];
}st_USBCalibrationRecord;

u16 DownloadCalData(void)
{
  UINT bytesRead;
  FIL tfile;
  u16 chksum = 0, i;
  u8 *ptr;
  st_USBCalibrationRecord USBCalibrationRecord;
  char fname[50] = {0,};
  u8 ch, year_ch, mon_ch, day_ch[2];

  BuzzerOff();  
	year_ch = YEAR-14+'A';
	mon_ch = MONTH-1+'A';
	day_ch[0] = (DAY/10)+'0';
	day_ch[1] = (DAY%10)+'0';
  
  IWDG_ReloadCounter(); 
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return 1;
  }
  
  memset(fname,0,50);
  sprintf(fname, "S1_CAL20%02d%02d%02d%02d%02d%02d%c", RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min, RTC_Val.tm_sec,'_');
  for(i=21;i<34;i++) {
    ch = Fram.VRN[i-21+1];
    if((ch<'0' || ch>'9') && (ch<'a' || ch>'z') && (ch<'A' || ch>'Z'))
      fname[i] =  '_';
    else
      fname[i] =  ch;
  }
  fname[34] = '_';
  fname[35] = year_ch;
  fname[36] = mon_ch;
  fname[37] = day_ch[0];
  fname[38] = day_ch[1];

  fname[39] = '.';
  fname[40] = 'C';
  fname[41] = 'A';
  fname[42] = 'L';

  // Remove file if exist on flash disk 

  f_unlink(fname);

  IWDG_ReloadCounter();
  
  // Calibration Record Copy
  memcpy((u8*)&USBCalibrationRecord, (u8*)&CalibrationRecord, sizeof(st_VuCalibrationRecord));
  // Add Parameter
  USBCalibrationRecord.V_Factor = Fram.V_Factor;
  USBCalibrationRecord.WorkShopNumber = Fram.WorkNumber;
  USBCalibrationRecord.ElectricLeakageFlag = Fram.ElectricLeakageFlag;
  USBCalibrationRecord.CAN_BaudrateFlag = Fram.CAN_BaudrateFlag;           
  // Add Information
  memcpy((u8*)USBCalibrationRecord.ManuFactureName, (u8*)Fram.VuIdentification.vuManufacturerName.Name, 35);
  memcpy((u8*)USBCalibrationRecord.vuPartNumber, (u8*)Fram.VuIdentification.vuPartNumber, 16);
  memcpy((u8*)USBCalibrationRecord.vuSerialNumber, (u8*)Fram.VuIdentification.vuSerialNumber, 8);
  USBCalibrationRecord.vuManufacturingDate = Fram.VuIdentification.vuManufacturingDate;
  USBCalibrationRecord.NCMNumber = Fram.NCMNumber;
  memcpy((u8*)USBCalibrationRecord.NCMPartNumber, (u8*)NCMActivationData.PartNumber, 16);;
  memcpy((u8*)USBCalibrationRecord.NCMExPiredDate, (u8*)NCMStatus.ExPiredDate, 6);

  ptr = (u8*)&USBCalibrationRecord;
  
  for(i = 0; i < sizeof(st_USBCalibrationRecord); i++)
    chksum += *(ptr+i);
  
  if (f_open(&tfile, fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
    USB_Error_Flag = 1;//File Open Error
    return 1;
  }
  IWDG_ReloadCounter();
  
  if(f_write(&tfile, (u8*)&USBCalibrationRecord, sizeof(st_USBCalibrationRecord), (void*)&bytesRead) != FR_OK) {
    USB_Error_Flag = 1;//File Read Error
    return 1;
  }
  if(f_write(&tfile, (u8*)&chksum, sizeof(u16), (void*)&bytesRead) != FR_OK) {
    USB_Error_Flag = 1;//File Read Error
    return 1;
  }
  
  if(f_close (&tfile) != FR_OK) {
    USB_Error_Flag = 1;//File Close Error
    return 1;
  }
  IWDG_ReloadCounter();
  
  return 0;
}

#if 1
  /**
  * @brief  File Name
  * @param  None
  * @retval None
  */
void FileName(char *filename, char *ext) 
{
  int i;
  
  memset(filename,0,32);
  sprintf(filename, "%02d%02d%02d%02d%02d%02d%c", RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min, RTC_Val.tm_sec,'-');
  for(i=13;i<26;i++) {
    if((Fram.VRN[(i-13)+1]<'0' || Fram.VRN[(i-13)+1]>'9') && (Fram.VRN[(i-13)+1]<'a' || Fram.VRN[(i-13)+1]>'z') && (Fram.VRN[(i-13)+1]<'A' || Fram.VRN[(i-13)+1]>'Z'))
      filename[i] =  '_';
    else
      filename[i] =  Fram.VRN[(i-13)+1];
  }
  filename[26] = ext[0];
  filename[27] = ext[1];
  filename[28] = ext[2];
  filename[29] = ext[3];
}
#else 
  /**
  * @brief  File Name
  * @param  None
  * @retval None
  */
void FileName(char *filename, char *ext) 
{
  int i;
  
  memset(filename,0,20);
  
  for(i=0;i<13;i++) {
    if((Fram.VRN[i+1]<'0' || Fram.VRN[i+1]>'9') && (Fram.VRN[i+1]<'a' || Fram.VRN[i+1]>'z') && (Fram.VRN[i+1]<'A' || Fram.VRN[i+1]>'Z'))
      filename[i] =  '_';
    else
      filename[i] =  Fram.VRN[i+1];
  }
  filename[13] = ext[0];
  filename[14] = ext[1];
  filename[15] = ext[2];
  filename[16] = ext[3];
}
#endif

/**
  * @brief  USB User Func
  * @param  None
  * @retval None
  */
void DownloadDrvData(u8 count)
{  
  UINT bytesWritten;  
  char fname[50]; 
  u8 buff[NAND_PAGE_SIZE];
  u16 i, n, size;
  u32 start_addr = 0, end_addr = 0;
  long start_date;
  u8 ch;
  u8 year_ch, mon_ch, day_ch[2];

  BuzzerOff();  
  if(count == 0) return;
	year_ch = YEAR-14+'A';
	mon_ch = MONTH-1+'A';
	day_ch[0] = (DAY/10)+'0';
	day_ch[1] = (DAY%10)+'0';
  
  lcd_clear();   

  LCD_printf(0,0,0,"DOWNLOAD TRAJECT");

  IWDG_ReloadCounter(); 
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return;
  }

  start_addr = Fram.VuDailyActRecord.Nand_SecDataAddr;
  if(count < 8) {
    start_date = (RTC_LVal/(3600*24) - (count-1))*3600*24;
    for(i = 0; i < count; i++)  {
      Nand_Read_DailyData(i,(u8*)&TmpDayAct);
      if(TmpDayAct.recordDate >= start_date) {
        start_addr = TmpDayAct.Nand_SecDataAddr;
      }
      else break;
    }
  }
  else {
    if(Fram.NandSaveAddr < NAND_SIZE_DATA) start_addr = 0;
    else 
      start_addr = (Fram.NandSaveAddr + NAND_BLOCK_SIZE)%NAND_SIZE_DATA;
  }
  
  end_addr = Fram.NandSaveAddr - Fram.NandSaveAddr%NAND_PAGE_SIZE;

  if(start_addr >= end_addr) return;

  memset(fname,0,50);
  sprintf(fname, "S1_T20%02d%02d%02d%02d%02d%02d%c", RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min, RTC_Val.tm_sec,'_');
  for(i=19;i<32;i++) {
    ch = Fram.VRN[i-19+1];
    if((ch<'0' || ch>'9') && (ch<'a' || ch>'z') && (ch<'A' || ch>'Z'))
      fname[i] =  '_';
    else
      fname[i] =  ch;
  }
  fname[32] = '_';
  fname[33] = year_ch;
  fname[34] = mon_ch;
  fname[35] = day_ch[0];
  fname[36] = day_ch[1];
  
  fname[37] = '.';
  fname[38] = 'D';
  fname[39] = 'T';
  fname[40] = 'G';

  // Remove file if exist on flash disk 

  f_unlink(fname);
  
  IWDG_ReloadCounter(); 
  // Open the file to write on it 
  if(f_open(&file, fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
    lcd_clear();
    LCD_printf(0,2,0,"Create file fail");
    delay_ms(2000);
    return;
  }

//- drive data save -//
  draw_rectangle(40,140,3);
    
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, (u8*)&Fram.NationCode, 32, &bytesWritten) != FR_OK) {
    USB_Error_Flag = 1;
    return;    
  }    
    
  IWDG_ReloadCounter();  

  size = NAND_PAGE_SIZE - start_addr%NAND_PAGE_SIZE;
  
  if(end_addr > start_addr + size) {
    Nand_Read_Buffer(buff, start_addr%NAND_SIZE_DATA, size);  
    if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
    if(f_write (&file, buff, size, &bytesWritten) != FR_OK) {
      USB_Error_Flag = 1;
      return;
    }    
    start_addr += size;
  }
  
  n = (end_addr-start_addr)/NAND_PAGE_SIZE;
  
  delay_ms(1);
  for(int i = 0; i < n; i++) {    
    IWDG_ReloadCounter();  
    Nand_Read_Buffer(buff, (start_addr+i*NAND_PAGE_SIZE)%NAND_SIZE_DATA, NAND_PAGE_SIZE); 
    if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
    delay_ms(1);
    if(f_write (&file, buff, NAND_PAGE_SIZE, &bytesWritten) != FR_OK) {
      USB_Error_Flag = 1;
      return;
    }        
    if(i%10 == 0) {
      draw_fill_box(40, (i*100/n)+40, 3);      
    }    
    if(IDE_Download_Cancel() == KEY_CANCEL){f_close (&file);  return;} 
  }
  
  IWDG_ReloadCounter();    
  delay_ms(1);
  n = Fram.NandSaveAddr%NAND_PAGE_SIZE;
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, Nand_Save_Buff, n, &bytesWritten) != FR_OK) {
    USB_Error_Flag = 1;
    return;
  }      
  draw_fill_box(40, 140, 3);   
  
  if(f_close (&file) != FR_OK) {
    USB_Error_Flag = 1;
    return;
  }  
}

/**
  * @brief  USB User Func
  * @param  None
  * @retval None
  */
void DownloadVehicleData(void)
{  
  UINT bytesWritten;  
  char fname[50]; 
  u8 buff[NAND_PAGE_SIZE];
  u8 hbuff[2];
  int n, size, i;
  u8 ch;
  u8 year_ch, mon_ch, day_ch[2];
  u8 slot;
  long first_downdate;

  BuzzerOff();  
	year_ch = YEAR-14+'A';
	mon_ch = MONTH-1+'A';
	day_ch[0] = (DAY/10)+'0';
	day_ch[1] = (DAY%10)+'0';
  
  lcd_clear();     
  LCD_printf(0,0,0,"DOWNLOAD VEHICLE");
  
  IWDG_ReloadCounter(); 
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return;
  }
  
  memset(fname,0,50);
  sprintf(fname, "S1_V20%02d%02d%02d%02d%02d%02d%c", RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min, RTC_Val.tm_sec,'_');
  for(i=19;i<32;i++) {
    ch = Fram.VRN[i-19+1];
    if((ch<'0' || ch>'9') && (ch<'a' || ch>'z') && (ch<'A' || ch>'Z'))
      fname[i] =  '_';
    else
      fname[i] =  ch;
  }
  fname[32] = '_';
  fname[33] = year_ch;
  fname[34] = mon_ch;
  fname[35] = day_ch[0];
  fname[36] = day_ch[1];

  fname[37] = '.';
  fname[38] = 'D';
  fname[39] = 'D';
  fname[40] = 'D';

  // Remove file if exist on flash disk 

  f_unlink(fname);
  
  IWDG_ReloadCounter(); 
  // Open the file to write on it 
  if(f_open(&file, fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
    lcd_clear();
    LCD_printf(0,2,0,"Create file fail");
    delay_ms(2000);
    return;
  }

//- drive data save -//
  draw_rectangle(40,140,3);
  
  IDE_DnDate = RTC_LVal;
  
  IWDG_ReloadCounter();    
  MakeOverviewData(1);
  hbuff[0] = 0x76;  hbuff[1] = 1;
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
  if(f_write (&file, IDE_DnBuff, IDE_DnDataNum, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;}    
  
  if(IDE_Download_Cancel() == KEY_CANCEL){f_close (&file);  return;} 
  draw_fill_box(40, 45, 3);   

  IWDG_ReloadCounter();    
  MakeEventFaultData(1);
  hbuff[0] = 0x76;  hbuff[1] = 3;
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
  if(f_write (&file, IDE_DnBuff, IDE_DnDataNum, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;}    

  if(IDE_Download_Cancel() == KEY_CANCEL){f_close (&file);  return;} 
  draw_fill_box(40, 50, 3);   

#if 1  
  IWDG_ReloadCounter();    
  MakeDetailedSpeedData(1);
  if(IDE_Download_Cancel() == KEY_CANCEL || IDE_Down_Cancel_Flag){f_close (&file);  return;}  
  hbuff[0] = 0x76;  hbuff[1] = 4;
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
//  draw_fill_box(40, 70, 3);   
  n = IDE_DnDataNum/NAND_PAGE_SIZE;
  for(int i = 0; i <= n; i++) {    
    IWDG_ReloadCounter();  
    if(i < n) size = NAND_PAGE_SIZE;
    else size = IDE_DnDataNum%NAND_PAGE_SIZE;
    if(size > 0) {
      Nand_Read_Buffer(buff, NAND_DETAILED_DATA_ADDR+i*NAND_PAGE_SIZE, size);  
      if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
      if(f_write (&file, buff, size, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;}    
    }
    if(i%10 == 0) {
      draw_fill_box(70, 100+i*10/n, 3);      
    }    
  } 
#endif  
  IWDG_ReloadCounter();    
  draw_fill_box(40, 110, 3);   
  
  
  IWDG_ReloadCounter();    
  MakeTechnicalData(1);
  if(IDE_Download_Cancel() == KEY_CANCEL){f_close (&file);  return;} 
  hbuff[0] = 0x76;  hbuff[1] = 5;
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
  if(f_write (&file, IDE_DnBuff, IDE_DnDataNum, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;}    
 
  n = CalcDayCount(RTC_LVal,Fram.FirstDailyDataDate);
  if(n > 364) n = 364;
  first_downdate = Fram.FirstDailyDataDate-(Fram.FirstDailyDataDate%(3600*24));
  for(i = n; i >= 0 ; i--) {
    IWDG_ReloadCounter();    
    IDE_DnDate = RTC_LVal-3600*24*i;
    if(IDE_DnDate >= first_downdate) {
      MakeActivityData(1);
      if(IDE_Download_Cancel() == KEY_CANCEL){f_close (&file);  return;} 
      if(!IDE_DataNotAvailable) {
        hbuff[0] = 0x76;  hbuff[1] = 2;
        if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
        if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
        if(f_write (&file, IDE_DnBuff, IDE_DnDataNum, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;}    
      }
    }
    if((n-i)%5 == 0) {
      draw_fill_box(70, 110+(n-i)*20/n, 3);      
    }    
    disp_printf(32, 4, 0, "DAYS: %03d/%03d",n-i+1,n+1);
  }
  draw_fill_box(40, 130, 3);   

  hbuff[0] = 0x76;  hbuff[1] = 0x81;
  if(flag.ncmfind)      
    Request_NCM_Key_Authentication(1);
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
  if(f_write (&file, NCM_RX_BUFFER, NCM_CertiLen, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;} 

  draw_fill_box(40, 135, 3);   

  hbuff[0] = 0x76;  hbuff[1] = 0x82;
  if(flag.ncmfind)      
    Request_NCM_Key_Authentication(0);
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
  if(f_write (&file, NCM_RX_BUFFER, NCM_CertiLen, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;} 

  draw_fill_box(40, 140, 3);   
  if(OperationMode == COMPANY) {
    slot = SearchDownloadCard();
    if(slot < 2)    
      PutCompanyActivity(slot, 2);
  }    
  if(f_close (&file) != FR_OK) {
    USB_Error_Flag = 1;
    return;
  }
}

/**
  * @brief  USB User Func
  * @param  None
  * @retval None
  */
void DownloadCardData(void)
{  
  char fname[50]; 
  int i;
  u8 ch;
  u8 year_ch, mon_ch, day_ch[2];

  BuzzerOff();  
	year_ch = YEAR-14+'A';
	mon_ch = MONTH-1+'A';
	day_ch[0] = (DAY/10)+'0';
	day_ch[1] = (DAY%10)+'0';

  lcd_clear();       
  LCD_printf(0,0,0,"DOWNLOAD CARD");
  IWDG_ReloadCounter(); 
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return;
  }
  if(flag.slot1 && (VuCardIWRecord[0].FullCardNumber.cardType == DRIVER_CARD || VuCardIWRecord[0].FullCardNumber.cardType == WORKSHOP_CARD)) {  
    memset(fname,0,50);
    sprintf(fname, "S1_C20%02d%02d%02d%02d%02d%02d%c", RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min, RTC_Val.tm_sec,'_');
    for(i=19;i<35;i++) {
      ch = VuCardIWRecord[0].FullCardNumber.cardNumber[i-19];
      if((ch<'0' || ch>'9') && (ch<'a' || ch>'z') && (ch<'A' || ch>'Z'))
        fname[i] =  '_';
      else
        fname[i] =  ch;
    }
    fname[35] = '_';
    fname[36] = year_ch;
    fname[37] = mon_ch;
    fname[38] = day_ch[0];
    fname[39] = day_ch[1];

    fname[40] = '.';
    fname[41] = 'D';
    fname[42] = 'D';
    fname[43] = 'D';

    // Remove file if exist on flash disk 
  
    f_unlink(fname);
    
    IWDG_ReloadCounter(); 
    // Open the file to write on it 
    if(f_open(&file, fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
      lcd_clear();
      LCD_printf(0,2,0,"Create file fail");
      delay_ms(2000);
      return;
    }
  //- drive data save -//
    draw_rectangle(40,140,3);
  
    MakeDownloadCardData(VuCardIWRecord[0].FullCardNumber.cardType-1, 1);
    if(USB_Error_Flag) return;
   
    draw_fill_box(40, 140, 3);   
     
    if(f_close (&file) != FR_OK) {
      USB_Error_Flag = 1;
      return;
    }
  }  
  else {
    LCD_printf(0,2,0,"Not Driver Card!");
    delay_ms(2000);
  }
}

/**
  * @brief  USB User Func
  * @param  None
  * @retval None
  */
void DownloadNCMData(u8 count)
{  
  char fname[50]; 
  int i;
  u8 ch;
  UINT bytesWritten;  
  u8 hbuff[2];
  u8 year_ch, mon_ch, day_ch[2];

  BuzzerOff();  
	year_ch = YEAR-14+'A';
	mon_ch = MONTH-1+'A';
	day_ch[0] = (DAY/10)+'0';
	day_ch[1] = (DAY%10)+'0';

  if(!flag.ncmfind || count == 0) return;
  NCM_All_Data_Flag = 0;
  NCM_Down_EndDate = RTC_LVal;
  if(count < 8) {
    NCM_Down_StartDate = (RTC_LVal-RTC_LVal%(3600*24)) - ((count-1)*3600*24);
  }
  else {
    NCM_Down_StartDate = NCM_ActivationTime;
    NCM_All_Data_Flag = 1;
  }

  lcd_clear();       
  LCD_printf(0,0,0,"DOWNLOAD NCM");
  IWDG_ReloadCounter(); 
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    lcd_clear();
    LCD_printf(0,2,0,"Not connected");
    return;
  }

  memset(fname,0,50);
  sprintf(fname, "S1_N20%02d%02d%02d%02d%02d%02d%c", RTC_Val.tm_year, RTC_Val.tm_mon, RTC_Val.tm_mday, RTC_Val.tm_hour, RTC_Val.tm_min, RTC_Val.tm_sec,'_');
  for(i=19;i<35;i++) {
    ch = NCMActivationData.PartNumber[i-19];
    if((ch<'0' || ch>'9') && (ch<'a' || ch>'z') && (ch<'A' || ch>'Z'))
      fname[i] =  '_';
    else
      fname[i] =  ch;
  }
  fname[35] = '_';
  fname[36] = year_ch;
  fname[37] = mon_ch;
  fname[38] = day_ch[0];
  fname[39] = day_ch[1];

  fname[40] = '.';
  fname[41] = 'N';
  fname[42] = 'K';
  fname[43] = 'M';

  // Remove file if exist on flash disk 

  f_unlink(fname);
  
  IWDG_ReloadCounter(); 
  // Open the file to write on it 
  if(f_open(&file, fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
    lcd_clear();
    LCD_printf(0,2,0,"Create file fail");
    delay_ms(2000);
    return;
  }
  draw_rectangle(40,140,3);

  hbuff[0] = 0x76;  hbuff[1] = 0x81;
  if(flag.ncmfind)      
    Request_NCM_Key_Authentication(1);
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
  if(f_write (&file, NCM_RX_BUFFER, NCM_CertiLen, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;} 

  hbuff[0] = 0x76;  hbuff[1] = 0x82;
  if(flag.ncmfind)      
    Request_NCM_Key_Authentication(0);
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
  if(f_write (&file, NCM_RX_BUFFER, NCM_CertiLen, &bytesWritten) != FR_OK) {USB_Error_Flag = 1; return;} 

  hbuff[0] = 0x76;  hbuff[1] = 0x84;
  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {USB_Error_Flag = 1; return;}
  if(f_write (&file, hbuff, 2, &bytesWritten) != FR_OK) {USB_Error_Flag = 1;  return;}
  MakeNCMInfoData(1);
  if(USB_Error_Flag) return;
  MakeNCMArchiveData(1);
  if(USB_Error_Flag) return;
  
  draw_fill_box(40, 140, 3);   
   
  if(f_close (&file) != FR_OK) {
    USB_Error_Flag = 1;
    return;
  }
}
/**
  * @brief  USB User Func
  * @param  None
  * @retval None
  */
void COMMAND_ExecuteApplication(void)
{    
  if(DL_DrvDataCount == 0) DL_DrvDataCount = 1;
  
  if(USB_DL_Flag == 1) {
    if(USB_NCMFlag)
      DownloadNCMData(DL_DrvDataCount);
    else 
      DownloadDrvData(DL_DrvDataCount);
  }
  else if(USB_DL_Flag == 2)
    DownloadVehicleData();
  else if(USB_DL_Flag == 3)
    DownloadCardData();
  else if(USB_DL_Flag == 5)
    DownloadCalData();
  USB_Error_Flag = 0;   
  lcd_clear();    
}

/**
  * @brief  USBH_USR_DeInit
  *         Deint User state and associated variables
  * @param  None
  * @retval None
  */
void USBH_USR_DeInit(void)
{
}

/**
  * @}
  */
void COMMAND_ProgramFlashMemory_boot(void)
{
  u32 cpu_sr;
  
  __IO uint32_t programcounter = 0x00;
  uint8_t readflag = TRUE;
  u32 fsize;
  u32 readsize = 0;
  u32 rcnt;
  uint32_t TmpProgramCounter = 0x00, TmpReadSize = 0x00 , RamAddress = 0x00;
  uint32_t LastPGAddress;

  fsize = DownFileLength;
  /* RAM Address Initialization */
  memset(IDE_DnBuff, 0, NAND_PAGE_SIZE);
  RamAddress = (uint32_t) & IDE_DnBuff;
  
  /* Erase address init */
  LastPGAddress = ((uint32_t)0x08000000);
  
  /* While file still contain data */
  IWDG_ReloadCounter();
  while (readflag == TRUE)
  {
    rcnt = fsize - readsize;
    if(rcnt > NAND_PAGE_SIZE) {
      IWDG_ReloadCounter();
      Nand_Read_Buffer(IDE_DnBuff, NAND_BOOT_PROGRAM_ADDR+readsize, NAND_PAGE_SIZE); 
      TmpReadSize = NAND_PAGE_SIZE;
      readsize += TmpReadSize;
    }
    else {
      IWDG_ReloadCounter();
      Nand_Read_Buffer(IDE_DnBuff, NAND_BOOT_PROGRAM_ADDR+readsize, rcnt); 
      TmpReadSize = rcnt;
      readsize += TmpReadSize;
      readflag = FALSE;
    }
		OS_ENTER_CRITICAL();
    /* Program flash memory */
    for (programcounter = TmpReadSize; programcounter != 0; programcounter -= 4)
    {
      TmpProgramCounter = programcounter;
      /* Write word into flash memory */
      if (FLASH_If_ProgramWord((LastPGAddress - TmpProgramCounter + TmpReadSize), \
        *(__IO uint32_t *)(RamAddress - programcounter + TmpReadSize)) != FLASH_COMPLETE)
      {
        /* Toggle Red LED in infinite loop: Flash programming error */
				OS_EXIT_CRITICAL();
        return;
      }
    }
		OS_EXIT_CRITICAL();
    /* Update last programmed address value */
    LastPGAddress = LastPGAddress + TmpReadSize;
  }
}

/**
  * @brief  IAP write memory
  * @param  None
  * @retval None
  */
int BOOT_DOWNLOAD(void)
{
  IWDG_ReloadCounter();
//  LCD_printf(0,4,0,"Flash Erase...  ");
  FLASH_Unlock();
  IWDG_ReloadCounter();
  if (FLASH_If_EraseSectors_boot(0) != 0x00)
  {
//    LCD_printf(0,4,0,"Erase Error!    ");
    FLASH_Lock();
    IWDG_ReloadCounter();
    return -1;
  }
//  LCD_printf(0,4,0,"Erase Done      ");
//  LCD_printf(0,4,0,"Flash Program...");
  IWDG_ReloadCounter();
  COMMAND_ProgramFlashMemory_boot();
//  LCD_printf(0,4,0,"Done            ");
  return 0;
}

void CopyNand2USB(void)
{
  UINT bytesWritten;  
  FIL tfile;
	u32 i, n;
  char fname[20] = {0,};
     
  sprintf(fname,"nandbak.bin");
  IWDG_ReloadCounter();

  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    printd("Not connected\n");
    return;
  }

  f_unlink(fname);
  IWDG_ReloadCounter(); 
  if(f_open(&tfile, fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
		printd("File Create Error\n");
    return;
  }
  IWDG_ReloadCounter(); 
	n = NAND_SIZE/NAND_PAGE_SIZE;
	printd("\n");
  for(i = 0; i < n; i++) {
		Nand_Read_Buffer(IDE_DnBuff, i*NAND_PAGE_SIZE, NAND_PAGE_SIZE);
    if(f_write (&tfile, IDE_DnBuff, NAND_PAGE_SIZE, &bytesWritten) != FR_OK) {
		  printd("File Write Error\n");
      return;
    }
		
		if(n % 64 == 0){
			printd("%d\r",i/64);
		}
    
	  IWDG_ReloadCounter(); 
  }
	printd("\n");
  f_write (&tfile, Nand_Save_Buff, NAND_PAGE_SIZE, &bytesWritten);
  if(f_close (&tfile) != FR_OK) {
		printd("File Close Error\n");
		return ;
  }
  IWDG_ReloadCounter();    	
}

void CopyUSB2Nand(void)
{
  UINT bytesWritten;  
  FIL tfile;
	u32 i, n;
  char fname[20] = {0,};

  sprintf(fname,"nandbak.bin");
  IWDG_ReloadCounter();

  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    printd("Not connected\n");
    return;
  }

  IWDG_ReloadCounter(); 
  if(f_open(&tfile, fname, FA_READ) != FR_OK)  {
		printd("File Create Error\n");
    return;
  }
  IWDG_ReloadCounter(); 
	n = NAND_SIZE/NAND_PAGE_SIZE;
	printd("\n");
  for(i = 0; i < n; i++) {
    if(f_read (&tfile, IDE_DnBuff, NAND_PAGE_SIZE, &bytesWritten) != FR_OK) {
		  printd("File Write Error\n");
      return;
    }    
		Nand_Write_Buffer(IDE_DnBuff, i*NAND_PAGE_SIZE, NAND_PAGE_SIZE);   
	  IWDG_ReloadCounter();
		
		if(n % 64 == 0){
			printd("%d\r",i/64);
		}		
  }
	printd("\n");
  f_read (&tfile, Nand_Save_Buff, NAND_PAGE_SIZE, &bytesWritten);
  if(f_close (&tfile) != FR_OK) {
		printd("File Close Error\n");
		return ;
  }
  IWDG_ReloadCounter();      
}

void CopyFram2USB(void)
{
  UINT bytesWritten;  
  FIL tfile;
	u32 i, n;
  char fname[20] = {0,};

  sprintf(fname,"frambak.bin");
  IWDG_ReloadCounter();

  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    printd("Not connected\n");
    return;
  }

  f_unlink(fname);
  IWDG_ReloadCounter(); 
  if(f_open(&tfile, fname, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)  {
		printd("File Create Error\n");
    return;
  }
  IWDG_ReloadCounter(); 
	n = 8192/32;
	printd("\n");
  for(i = 0; i < n; i++) {                                           
    if(f_write (&tfile, (u8*)((u8*)&Fram+i*32), 32, &bytesWritten) != FR_OK) {
		  printd("File Write Error\n");
      return;
    }    
	  IWDG_ReloadCounter(); 
		
		printd("%d\r",i);
  }
	printd("\n");                                                             
  if(f_close (&tfile) != FR_OK) {
		printd("File Close Error\n");
		return ;
  }
  IWDG_ReloadCounter(); 
}

void CopyUSB2Fram(void)
{
  UINT bytesWritten;  
  FIL tfile;
	u32 i, n;
  char fname[20] = {0,};

  sprintf(fname,"frambak.bin");
  IWDG_ReloadCounter();

  if(HCD_IsDeviceConnected(&USB_OTG_Core) != 1) {
    printd("Not connected\n");
    return;
  }

  IWDG_ReloadCounter(); 
  if(f_open(&tfile, fname, FA_READ) != FR_OK)  {
		printd("File Create Error\n");
    return;
  }
  IWDG_ReloadCounter(); 
	n = 8192/32;
	printd("\n");
  for(i = 0; i < n; i++) {                                           
    if(f_read (&tfile, (u8*)((u8*)&Fram+i*32), 32, &bytesWritten) != FR_OK) {
		  printd("File Write Error\n");
      return;
    }    
	  IWDG_ReloadCounter(); 
		printd("%d\r",i);
  }
	printd("\n");                                                             
            
	FramWrite(0,sizeof(st_FRAM_Data));
  if(f_close (&tfile) != FR_OK) {
		printd("File Close Error\n");
		return ;
  }
  IWDG_ReloadCounter(); 
}


#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
