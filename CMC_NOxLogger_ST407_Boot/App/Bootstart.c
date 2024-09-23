/******************** (C) COPYRIGHT 2014  ******************************
* File Name          : includes.h
* Author             : Digital Development Team
* Version            : V1.0.0
* Date               : 08/24/2010
* Description        : Bootloader sequence header 
* Programer			 : MKH
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#define BOOT_GLOBALS
#include "includes.h"


/* Private typedef -----------------------------------------------------------*/
typedef void(*pFunction)(void);


static INT16U Crc16_Table_gui [256]  = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};


INT16U GenerateCRC_gui(char *pszBuffer, INT32U nLength, INT16U last_CRC16)
{
	INT16U wData, wCRC_16;

	nLength *= 8;
	
	for (wCRC_16=last_CRC16; nLength>=8; nLength-=8,pszBuffer++) {
		
		wCRC_16 = Crc16_Table_gui[ ((wCRC_16 >> 8) ^ * pszBuffer)&0xFF ] ^ (wCRC_16 << 8);
	} 

	if (nLength != 0) {
		wData = ((INT16U) (*pszBuffer)) << (8);  
		
		while (nLength-- != 0) {
		
			if ( ((wCRC_16 ^ wData) & ((INT16U) 1 << 15)) != 0 ) {  
				wCRC_16 <<= 1;
				wCRC_16 ^= CRC_16_POLYNOMIAL; 
			} 
			else wCRC_16 <<= 1; 
				wData <<= 1;
		}
	} 
	
	return wCRC_16;
}

/* Functions -----------------------------------------------------------------*/
void dput_ch_console(INT8U data)
{
	//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	//USART_SendData(USART1,data);
	USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET)
	{
	}
}

static char dprint_buf[PRINTK_BUFF_SIZE]={0};
void printd(char *fmt, ...) 
{
	INT32S	len,i=0;
	va_list ap;
 
 	va_start(ap, fmt);
 	len = vsprintf(dprint_buf, fmt, ap);
	va_end(ap);

	if ( len > 0 ){
		while(len--){
			if ( dprint_buf[i] == '\n' ){
				dput_ch_console('\n');
				dput_ch_console('\r');
			}
			else {																   
				dput_ch_console(dprint_buf[i]);
			}
			i++;
		}
	}
}


void ApplicationJump(void)
{
	pFunction Jump_To_Application;
	INT32U JumpAddress;

	BSP_Deinit();
	SD_LowLevel_DeInit();
	
	JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDR + 4);
	Jump_To_Application = (pFunction) JumpAddress;
	/* Initialize user application's Stack Pointer */
	__set_MSP(*(__IO uint32_t*) APPLICATION_ADDR);
	Jump_To_Application();
}


void System_reset(void)
{
	NVIC_SystemReset();
}

void percent_display(int cur,int max)
{
	printd("%d%%\r",cur * 100 / max);
}


INT8U SpiFlashFileCheck(Prjcode_t *p)
{
	INT32U file_size, i = 1;
	INT16U last_CRC16 = 0;
	INT8U rdbuf[SPI_FLASH_PAGESIZE];

	INT32U page_cnt, page_cnt_max = 0;
	INT32U last_packetlen;
	
	if( p->file_size != 0xFFFFFFFF )
	{
		file_size = p->file_size;
	}
	else
	{
		printd("File Size Error\n");
		return 1;
	}

	page_cnt_max = (file_size / SPI_FLASH_PAGESIZE);
	last_packetlen = file_size % SPI_FLASH_PAGESIZE;
	//printd("page_cnt_max: %d, last_packetlen: %d\n",page_cnt_max,last_packetlen);
	
	if(last_packetlen)
	{
		page_cnt_max += 1;
	}

	printd("CRC Start\n");
	for( page_cnt = 0; page_cnt < page_cnt_max; page_cnt++ )
	{
		memset(rdbuf, 0, SPI_FLASH_PAGESIZE);

		SPI_FLASH_BufferRead( (INT8U *)rdbuf, 
			             (INT32U)((page_cnt * SPI_FLASH_PAGESIZE)+SPIFLASH_DOWNMEM), 
			             SPI_FLASH_PAGESIZE );

		if(page_cnt == (page_cnt_max-1)){
			if(last_packetlen){ // Last Packet check
				last_CRC16 = GenerateCRC_gui( (char *)rdbuf, last_packetlen, last_CRC16 );
			}
			else{
				last_CRC16 = GenerateCRC_gui( (char *)rdbuf, SPI_FLASH_PAGESIZE, last_CRC16 );
			}
		}
		else{
			last_CRC16 = GenerateCRC_gui( (char *)rdbuf, SPI_FLASH_PAGESIZE, last_CRC16 );
		}

		if( (i++ % 10) == 0 )
		{	
      HPRESSToggle();
			percent_display(page_cnt,page_cnt_max);
		}
	}

	// CRC 비교
	if( last_CRC16 == p->file_crc )
	{
		printd("Done.\n");
		return 0;
	}
	else
	{
		printd("%x != %x\n",last_CRC16,p->file_crc);
	}
	
	return 1;
}

int8_t FLASH_If_Erase(uint32_t StartSector)
{
  //uint32_t FlashAddress;
  
  //FlashAddress = StartSector;

  /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
     be done by word */ 
 
  if (StartSector <= (uint32_t) USER_FLASH_LAST_PAGE_ADDRESS)
  {
    FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3); /* 64 Kbyte */
	printd("#4.\n");
    FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3); /* 128 Kbyte */
	printd("#5.\n");
    FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3); /* 128 Kbyte */
	printd("#6.\n");
    //FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3); /* 128 Kbyte */
    //FLASH_EraseSector(FLASH_Sector_8, VoltageRange_3); /* 128 Kbyte */
    //FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3); /* 128 Kbyte */
    //FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3); /* 128 Kbyte */
    //FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3); /* 128 Kbyte */
  }
  else
  {
    return (1);
  }

  return (0);
}


INT8U SpiFlashToIntFlash(Prjcode_t *p, INT32U SpiflashAddr)
{
	INT32U page_cnt, byte_cnt;//erase_cnt
	INT32U page_cnt_max, wbuf, i = 0;
	INT8U rbuf[SPI_FLASH_PAGESIZE];
	INT8U cbuf[SPI_FLASH_PAGESIZE];
	
	page_cnt_max = (p->file_size / SPI_FLASH_PAGESIZE);

	
	printd("\n*** Intflash 0x%08X ~ 0x%08X Erase / Write ***\n",APPLICATION_ADDR,(APPLICATION_ADDR + APPLICATION_SIZE));
	printd("*** source add:0x%08X, target add:0x%08X, length:0x%08X ***\n",SpiflashAddr,APPLICATION_ADDR,p->file_size);

	
	// 내부 Flash 페이지 삭제
	FLASH_Unlock();
	printd("IntFlash Erase Start\n");
	FLASH_If_Erase( APPLICATION_ADDR );
	/*
	for( erase_cnt = 0; erase_cnt < INT_MAX_PAGETOERASE; erase_cnt++ ) 
	{
		FLASH_EraseSector( (APPLICATION_ADDR + (erase_cnt * INT_FLASH_PAGESIZE)), VoltageRange_3 );
		//FLASH_ErasePage( (APPLICATION_ADDR + (erase_cnt * INT_FLASH_PAGESIZE)));
		if( (erase_cnt % 2) ==0 )
		{
			percent_display(erase_cnt,INT_MAX_PAGETOERASE);
		}
	}
	*/
	printd("Done.\n");

	
	if( p->file_size % SPI_FLASH_PAGESIZE )
	{
		page_cnt_max += 1;
	}

	printd("IntFlash Write Start\n");	
	// SPI Flash의 데이터를 읽어들여 내부 Flash에 저장
	for( page_cnt = 0; page_cnt < page_cnt_max; page_cnt++ ) 
	{
		memset(rbuf,0,SPI_FLASH_PAGESIZE);
		memset(cbuf,0,SPI_FLASH_PAGESIZE);

		// Flash 데이터를size 0x1000만큼 두 번 읽어들여 같은 값인지 확인 
		SPI_FLASH_BufferRead( (INT8U *)rbuf, 
			             (INT32U)((page_cnt*SPI_FLASH_PAGESIZE)+SpiflashAddr), 
			             SPI_FLASH_PAGESIZE );

		SPI_FLASH_BufferRead( (INT8U *)cbuf, 
			             (INT32U)((page_cnt*SPI_FLASH_PAGESIZE)+SpiflashAddr), 
			             SPI_FLASH_PAGESIZE );

		// 읽어들인 데이터가 같으면 내부 Flash에 데이터 WRITE
		if( memcmp(rbuf,cbuf,SPI_FLASH_PAGESIZE)==0 )
		{
			
			for( byte_cnt = 0; byte_cnt < SPI_FLASH_PAGESIZE; byte_cnt += 4 )
			{
				memcpy( &wbuf, (INT8U *)&rbuf[byte_cnt], 4 );
				FLASH_ProgramWord( ((page_cnt*SPI_FLASH_PAGESIZE) + APPLICATION_ADDR + byte_cnt), wbuf );
			}

			if( ( i++ % 10 ) == 0 )
			{
        HPRESSToggle();
				percent_display(page_cnt,page_cnt_max);
			}
		}
		else
		{
			printd("IntFlash Write Error\n");
			return 1;
		}
	}
	printd("Done.\n");
	
	return 0;
}

INT8U BackupIntToSpiflash(void)
{
	INT32U i;
	INT8U pbuf[SPI_FLASH_PAGESIZE];


	printd("\n*** Spiflash 0x%08X ~ 0x%08X Erase / Write ***\n",SPIFLASH_BACKUPBASE,(SPIFLASH_BACKUPBASE + APPLICATION_SIZE));
	printd("*** source add:0x%08X, target add:0x%08X, length:0x%08X ***\n",APPLICATION_ADDR,SPIFLASH_BACKUPBASE,APPLICATION_SIZE);

	
	// SpiFlash Backup 영역 삭제
	printd("Backup Erase Start\n");
	for(i=0; i<8; i++){
		SPI_FLASH_BlockErase(SPIFLASH_BACKUPBASE + i*0x10000);
		percent_display(i,8);
	}
	printd("Done.\n");

	// 내부 Flash의 데이터를 SpiFlash Backup 영역으로 복사
	printd("Backup Write Start\n");
	for(i=0; i< APPLICATION_SIZE/SPI_FLASH_PAGESIZE; i++){
			
		SPI_FLASH_BufferWrite((INT8U *)(APPLICATION_ADDR + i*SPI_FLASH_PAGESIZE),
			                           SPIFLASH_BACKUPBASE + i*SPI_FLASH_PAGESIZE,SPI_FLASH_PAGESIZE);

		SPI_FLASH_BufferRead(pbuf,SPIFLASH_BACKUPBASE + i*SPI_FLASH_PAGESIZE,SPI_FLASH_PAGESIZE);
		if(memcmp((INT8U *)(APPLICATION_ADDR + i*SPI_FLASH_PAGESIZE),pbuf,SPI_FLASH_PAGESIZE))
		{
			printd("Backup Write Error\n");
			return 1;			
		}

		
		if((i % 100)==0){
      HPRESSToggle();
			percent_display(i,APPLICATION_SIZE/SPI_FLASH_PAGESIZE);
		}
	}
	printd("Done.\n");

	// 내부 Flash의 데이터와 Backup된 데이터를 비교
	printd("Backup Compare start\n");
	for(i=0; i< APPLICATION_SIZE/SPI_FLASH_PAGESIZE; i++){
		
		SPI_FLASH_BufferRead(pbuf,SPIFLASH_BACKUPBASE + i*SPI_FLASH_PAGESIZE,SPI_FLASH_PAGESIZE);
		if(memcmp((INT8U *)(APPLICATION_ADDR + i*SPI_FLASH_PAGESIZE),pbuf,SPI_FLASH_PAGESIZE)){
			printd("Backup Compare Error\n");
			return 1;
		}
		else{
			if((i % 100)==0){
        HPRESSToggle();
				percent_display(i,APPLICATION_SIZE/SPI_FLASH_PAGESIZE);
			}
		}
	}
	printd("Done.\n");
	
	return 0;
}

//SDIO Part.......................
INT8U AppFileCheck(Prjcode_t *p, FIL* fp)
{
	INT32U file_size, i = 1,size;
	INT16U last_CRC16 = 0,file_CRC16=0;
	INT8U rdbuf[SPI_FLASH_PAGESIZE];

	INT32U page_cnt, page_cnt_max = 0;
	INT32U last_packetlen;
	UINT br;

	size = f_size(fp);
	if( size != 0xFFFFFFFF)
	{
		file_size = size;
	}
	else
	{
		printd("SD File Size Error\n");
		return 1;
	}

	page_cnt_max = (file_size / SPI_FLASH_PAGESIZE);
	last_packetlen = file_size % SPI_FLASH_PAGESIZE;
	
	if(last_packetlen)
	{
		page_cnt_max += 1;
	}

	printd("CRC Start\n");
	printd("file size = %d\n",file_size);
	printd("max page cnt = %d\n",page_cnt_max);
	printd("last_pack_len = %d\n",last_packetlen);
	for( page_cnt = 0; page_cnt < page_cnt_max; page_cnt++ )
	{
		memset(rdbuf, 0, SPI_FLASH_PAGESIZE);
		f_lseek(fp,page_cnt * SPI_FLASH_PAGESIZE);
		f_read(fp, rdbuf, SPI_FLASH_PAGESIZE, &br);

		if(page_cnt == (page_cnt_max-1)){
			if(last_packetlen){ // Last Packet check
				last_CRC16 = GenerateCRC_gui( (char *)rdbuf, last_packetlen-6, last_CRC16 );
			}
			else{
				last_CRC16 = GenerateCRC_gui( (char *)rdbuf, SPI_FLASH_PAGESIZE-6, last_CRC16 );
			}
		}
		else{
			last_CRC16 = GenerateCRC_gui( (char *)rdbuf, SPI_FLASH_PAGESIZE, last_CRC16 );
		}

		if( (i++ % 10) == 0 )
		{	
      HPRESSToggle();
			percent_display(page_cnt,page_cnt_max);
		}
	}
	memcpy(	(char *)&file_CRC16, (char *)&rdbuf[last_packetlen-6], 2);

	if( last_CRC16 == file_CRC16 )
	{
		printd("Done.\n");
		p->file_size = file_size;
		p->file_crc  = last_CRC16;
		f_close(fp);
		return 0;
	}
	else
	{
		printd("%x != %x\n",last_CRC16,file_CRC16);
	}
	f_close(fp);
	
	return 1;
}

INT8U SdFlashToIntFlash(Prjcode_t *p, FIL* fp)
{
	//INT32U size=0;
	INT32U page_cnt, uSOnePacketSize;//erase_cnt
	INT32U page_cnt_max,  i = 0,last_packetlen=0;
	INT16U last_CRC16 = 0;
	INT8U rbuf[SPI_FLASH_PAGESIZE];
    UINT br;
	
	printd("\n*** Extern flash 0x%08X ~ 0x%08X Erase / Write ***\n",SPIFLASH_DOWNMEM,(SPIFLASH_DOWNMEM + APPLICATION_SIZE));
	printd("*** source add:SD Card, target add:0x%08X, length:0x%08X ***\n",SPIFLASH_DOWNMEM,p->file_size);

	if(f_open(fp, "0:OBDPro.bin", FA_READ) == FR_OK) 
	{
		p->file_size = f_size(fp);

		memcpy( (char *)tPrjcode.boot_seq, "down_sd", 7 );
		SPI_FLASH_PageErase( PROJECT_INFO_BASE );
		SPI_FLASH_PageWrite( (INT8U *)&tPrjcode, PROJECT_INFO_BASE, sizeof(Prjcode_t) );

		// SpiFlash Backup 영역 삭제
		printd("Extern Flash Erase Start\n");
		for(i=0; i<8; i++){
			SPI_FLASH_BlockErase(SPIFLASH_DOWNMEM + i*0x10000);
			percent_display(i,8);
		}
		printd("Done.\n");

		page_cnt_max = (p->file_size / SPI_FLASH_PAGESIZE);
		last_packetlen = p->file_size % SPI_FLASH_PAGESIZE;

		if( p->file_size % SPI_FLASH_PAGESIZE )
		{
			page_cnt_max += 1;
		}
		printd("Extern Flash Write Start\n");
		// SPI Flash의 데이터를 읽어들여 내부 Flash에 저장
		for( page_cnt = 0; page_cnt < page_cnt_max; page_cnt++ ) 
		{
			memset(rbuf,0,SPI_FLASH_PAGESIZE);
			f_lseek(fp,page_cnt * SPI_FLASH_PAGESIZE);
			f_read(fp, rbuf, SPI_FLASH_PAGESIZE, &br);

			if(page_cnt == (page_cnt_max-1))
			{
				if(last_packetlen)
				{ 
					last_CRC16 = GenerateCRC_gui( (char *)rbuf, last_packetlen-6, last_CRC16 );
					uSOnePacketSize = last_packetlen-6;
				}
				else
				{
					last_CRC16 = GenerateCRC_gui( (char *)rbuf, SPI_FLASH_PAGESIZE-6, last_CRC16 );
					uSOnePacketSize = SPI_FLASH_PAGESIZE-6;
				}
			}
			else
			{
				last_CRC16 = GenerateCRC_gui( (char *)rbuf, SPI_FLASH_PAGESIZE, last_CRC16 );
				uSOnePacketSize = SPI_FLASH_PAGESIZE;
			}

			SPI_FLASH_BufferWrite((INT8U *)rbuf,  (INT32U)( SPIFLASH_DOWNMEM + (page_cnt * SPI_FLASH_PAGESIZE)),   uSOnePacketSize);
			if( (i++ % 10) == 0 )
			{	
        HPRESSToggle();
				percent_display(page_cnt,page_cnt_max);
			}
		}
		f_close(fp);
		p->file_size = p->file_size-6;//CRC:2Byte, Project Code 4Byte
		p->file_crc  = last_CRC16 ;
		memcpy( (char *)tPrjcode.boot_seq, "download", 8 );
		SPI_FLASH_PageErase( PROJECT_INFO_BASE );
		SPI_FLASH_PageWrite( (INT8U *)&tPrjcode, PROJECT_INFO_BASE, sizeof(Prjcode_t) );
		
		return 0;
	}
	else
	{
		f_close(fp);
		return 1;
	}

	
}






/******************* (C) COPYRIGHT 2014  ***************END OF FILE****/
