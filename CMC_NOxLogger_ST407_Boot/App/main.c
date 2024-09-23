/******************** (C) COPYRIGHT 2014 ******************************
* File Name          : main.c
* Author             : Digital Development Team
* Version            : V1.0.0
* Date               : 08/24/2014
* Description        : Main program body
* Programer			 : MKH
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
FATFS SD_Fs;
FIL	 SD_Fil;


/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
********************************************************************************
* History
* 2010/11/25     : SPI Flash Memory ��ü�� ���� ����̹� ���� M25PE80 -> SST25VF080	 
* 2011/05/31     : SPI Flash �ٿ�ε� Address ���� 0x10000 -> 0x20000
*
*******************************************************************************/

int main(void)
{
	INT32U size=0;
	INT8U buf[10];
	INT8U PCode[4]="0407";
	INT8U PCodeSize = 4;
	UINT br;

	int ret;
	
	SystemInit();

	BSP_Init();
	
	printd("\n\nBootloader Start \n");

	SPI_FLASH_BufferRead( (INT8U *)&tPrjcode, PROJECT_INFO_BASE, sizeof(Prjcode_t) );

	if( Bit_SET == GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) )	//Reset Button�� ������ �ֳ�?
	{
		if (f_mount(&SD_Fs, "0:", 1) == FR_OK)
		{
			if(f_open(&SD_Fil, "0:OBDPro.bin", FA_READ) == FR_OK) 
			{
				memset(buf,0x0,sizeof(buf));
				size = f_size(&SD_Fil);												//������ ���������� �̵��Ͽ� ������Ʈ Ȯ���� ���� ����
				f_lseek(&SD_Fil, size-PCodeSize);									//��ü������� 4����Ʈ ������ ����
				f_read(&SD_Fil, buf, PCodeSize, &br);								//���Ͽ��� ������Ʈ �ڵ��� �����Ѵ�.
				if( !strncmp((char const *)buf, (char const *)PCode ,PCodeSize) )	//project Code �� "1404" �ΰ�?
				{
					printd(" project code OK\n");
					ret = AppFileCheck(&tPrjcode, &SD_Fil);
					if(!ret)	//Ok...
					{
						ret = SdFlashToIntFlash(&tPrjcode, &SD_Fil) ;
						if(!ret)
						{
							printd("Download Process Start\n");
							// �ٿ�ε��� �������� CRC�� üũ�Ѵ�
							ret = SpiFlashFileCheck(&tPrjcode);
							if ( ret ){
								printd("SpiFlash File Check Error\n");
								ApplicationJump();
							}

							// IntFlash ������ ����� Gui download �����͸� Write�Ѵ�
							ret = SpiFlashToIntFlash(&tPrjcode,SPIFLASH_DOWNMEM);	
							if ( ret ){
								printd("Download(Spiflash to Intfhash) Error\n");
								ApplicationJump();
							}
							
							memcpy( (char *)tPrjcode.boot_seq, "success", 8 );
							SPI_FLASH_PageErase( PROJECT_INFO_BASE );
							SPI_FLASH_PageWrite( (INT8U *)&tPrjcode, PROJECT_INFO_BASE, sizeof(Prjcode_t) );


							ApplicationJump();
							//System_reset();
						}
					}
				}
				printd("Application Jump(file check)\n\n");
				ApplicationJump();
			}
			else
			{
				f_close(&SD_Fil);
				printd("Application Jump(no file)\n\n");
				ApplicationJump();
			}
		}
		else
		{
			printd("Application Jump(status)\n\n");
			ApplicationJump();
		}
	}
	else
	{

	// ���� �������� Ȯ�� 
	if( (tPrjcode.boot_seq[0] == 0xff) && (tPrjcode.boot_seq[1] == 0xff) ) 
	{
		printd("First Boot\n");
		
		memcpy( (char *)tPrjcode.boot_seq, "status", 8 );
		SPI_FLASH_PageErase( PROJECT_INFO_BASE );
		SPI_FLASH_PageWrite( (INT8U *)&tPrjcode, PROJECT_INFO_BASE, sizeof(Prjcode_t) );

		ApplicationJump();
	}
	else if( !strncmp( (char *)tPrjcode.boot_seq, "down_sd", 7 ) ) 
	{
		printd("SD Card Download Fail\n");
		ApplicationJump();
	}
	
	// GUI �ٿ�ε尡 �ߴܵǸ� ���ø����̼����� �ٽ� �Ѿ�� 
	else if( !strncmp( (char *)tPrjcode.boot_seq, "down_gui", 8 ) ) 
	{
		printd("Gui Download Fail\n");
		ApplicationJump();
	}
	
	// IntFlash ������ �����͸� SpiFlash Backup ������ �������� �����Ѵ� 
	else if( !strncmp( (char *)tPrjcode.boot_seq, "backup", 6 ) ) 
	{ 
		printd("Backup Process Start\n");
		
		ret = BackupIntToSpiflash();			
		if ( ret ){
			printd("Backup(Intflash to Spiflash) Error\n");
			ApplicationJump();
		}

		memcpy( (char *)tPrjcode.boot_seq, "download", 8 );
		SPI_FLASH_PageErase( PROJECT_INFO_BASE );
		SPI_FLASH_PageWrite( (INT8U *)&tPrjcode, PROJECT_INFO_BASE, sizeof(Prjcode_t) );

		System_reset();
	}

	// Backup ���� ������ �Ϸ�Ǹ� Download�� ����
	else if( !strncmp( (char *)tPrjcode.boot_seq, "download", 8 ) ) 
	{ 
		printd("Download Process Start\n");
		// �ٿ�ε��� �������� CRC�� üũ�Ѵ�
		ret = SpiFlashFileCheck(&tPrjcode);
		if ( ret ){
			printd("SpiFlash File Check Error\n");
			ApplicationJump();
		}

		// IntFlash ������ ����� Gui download �����͸� Write�Ѵ�
		ret = SpiFlashToIntFlash(&tPrjcode,SPIFLASH_DOWNMEM);	
		if ( ret ){
			printd("Download(Spiflash to Intfhash) Error\n");
			ApplicationJump();
		}
		
		memcpy( (char *)tPrjcode.boot_seq, "success", 8 );
		SPI_FLASH_PageErase( PROJECT_INFO_BASE );
		SPI_FLASH_PageWrite( (INT8U *)&tPrjcode, PROJECT_INFO_BASE, sizeof(Prjcode_t) );

		System_reset();
	}

	// Rollback ����
	else if( !strncmp( (char *)tPrjcode.boot_seq, "rollback", 8 ) )
	{
		printd("Rollback Process Start\n");
		ret = SpiFlashToIntFlash(&tPrjcode,SPIFLASH_BACKUPBASE);	
		if ( ret ){
			printd("Rollback(Spiflash to Intfhash) Error\n");
		}
		else{
			memcpy( (char *)tPrjcode.boot_seq, "success", 8 );
			SPI_FLASH_PageErase( PROJECT_INFO_BASE );
			SPI_FLASH_PageWrite( (INT8U *)&tPrjcode, PROJECT_INFO_BASE, sizeof(Prjcode_t) );

			System_reset();
		}
	}

	// ���� Flash�� ������ ���簡 �Ϸ�Ǹ� ���ø����̼����� �����Ѵ� 
	else if( !strncmp( (char *)tPrjcode.boot_seq, "success", 7 ) )
	{
		printd("Application Jump(success)\n");
		ApplicationJump();
	}

	else if( !strncmp( (char *)tPrjcode.boot_seq, "status", 6 ) )
	{
		printd("Application Jump(status)\n\n");
		ApplicationJump();
	}

	
	else{
		printd("Abnormal Start!!!!!\n");
		ApplicationJump();
	}
	
	return 0;
	}

}



/******************* (C) COPYRIGHT 2014 ***************END OF FILE****/
