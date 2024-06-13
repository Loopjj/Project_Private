/*
*************************************** (C) COPYRIGHT 2014 Loop*************************************
*
* File Name          : nv.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 09/05/2014
*
* Description        :  
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define NV_GLOBALS
#include "includes.h"


#define DUMMY_BYTE	0x55

void nvxSemaphore_init(void)
{
	BSP_SaveSem = xSemaphoreCreateMutex();
}


/*
*********************************************************************************************************
*                                                FUNCTIONS
*********************************************************************************************************
*/

// Ư�� ������ ���� 
//static INT8U pNv[SPI_FLASH_PAGE];
static void nv_modify(int addr, INT8U data)
{
	INT8U old_base;

	SPI_FLASH_BufferRead( &old_base, addr, 1);

	printk("\n\n[%05X]ORG:%02X ==> %02X\n", addr, old_base, data);
	
	SPI_FLASH_Member_Write(&data, addr, 1);

}


// SFlash�� sector ������ ��ŭ Display���ش� 
static void nv_sflash_md(int addr)
{
	int i,j;
	INT8U temp,buf[0x10];

	printk("\n");

	for(i=0; i<SPI_FLASH_PAGE; i++)
	{
		SPI_FLASH_BufferRead((INT8U *)&temp, addr+i, 1);

		if((i&0xF) == 0){
			if(i){
				for(j=0; j<0x10; j++){
					if( buf[j] >= 0x20 && buf[j] <= 0x7E ){
						printk("%c",buf[j]);
					}
					else{
						printk(".");
					}
				}
			}
			printk("\n%05X : ",addr+i);
		}
		
		buf[i&0xF] = temp;
		
		printk("%02X ",temp);
	}

	for(j=0; j<0x10; j++){
		if( buf[j] >= 0x20 && buf[j] <= 0x7E ){
			printk("%c",buf[j]);
		}
		else{
			printk(".");
		}
	}
	
	printk("\n");	
}

// SFlash�� leng*16 ������ ��ŭ Display���ش� 
static void nv_sflash_md_1(int addr, int leng)
{
	int i,j;
	INT8U temp,buf[0x10];

	printk("\n");

	for(i=0; i<(0x10*leng); i++)
	{
		SPI_FLASH_BufferRead((INT8U *)&temp, addr+i, 1);

		if((i&0xF) == 0){
			if(i){
				for(j=0; j<0x10; j++){
					if( buf[j] >= 0x20 && buf[j] <= 0x7E ){
						printk("%c",buf[j]);
					}
					else{
						printk(".");
					}
				}
			}
			printk("\n%05X : ",addr+i);
		}
		
		buf[i&0xF] = temp;
		
		printk("%02X ",temp);
	}

	for(j=0; j<0x10; j++){
		if( buf[j] >= 0x20 && buf[j] <= 0x7E ){
			printk("%c",buf[j]);
		}
		else{
			printk(".");
		}
	}
	
	printk("\n");	
}


/*
*********************************************************************************************************
*                                              DEBUG FUNCTIONS
*********************************************************************************************************
*/

INT16U dbg_sflash_read_id(void)
{
	INT32U ret = 0;
	INT8U buf[2];

	ret = SPI_FLASH_ReadID();
	buf[0] = (ret >> 8 ) & 0xFF;
	buf[1] = ret  & 0xFF;

	printk("\nManufacturer ID : ");
	
	if( buf[0] == 0xEF ){
		 printk("Winbond Serial Flash [0x%02X]\n",buf[0]);
	}
	else{
		 printk("Unknown\n");
	}
	printk("Device ID       : ");
	switch(buf[1]){
		case 0x15:printk("W25Q32FVSSIG 32Mb [0x%02X] \n",buf[1]);
			break;
		case 0x16:printk("W25Q64FVSSIG 64Mb [0x%02X]\n",buf[1]);
			break;
		default:printk("Unknown device ID [0x%02X]\n",buf[1]);
			break;
	}

	return NO_DISPLAY;
}

INT16U dbg_sflash_dispMap(void)
{
	printk("\n*** SFlash Memory Map ***\n");
	printk("PROJECT_BASE           :%05X (%04X)\n", PROJECT_BASE,       sizeof(Prjcode_t));  
	printk("SYSTEM_BASE	           :%05X (%04X)\n", SYSTEM_BASE,        sizeof(stSysControl));  
 

	return NO_DISPLAY;
}


INT16U dbg_sflash_smd(void)
{
	int address;
	
	if(CLI_GetArgCnt() < 1) return INV_PRM;

	address = comparison_num((char*)P_ARG(1));

	nv_sflash_md(address);

	return NO_DISPLAY;
}

INT16U dbg_sflash_modify(void)
{
	int address;
	INT8U data;
	
	if ( CLI_GetArgCnt() < 2 ) return INV_PRM;

	address = comparison_num((char*)P_ARG(1));
	data = comparison_num((char*)P_ARG(2));

	nv_sflash_md_1(address&0xFFFFF0,1);

	nv_modify(address,data);

	nv_sflash_md_1(address&0xFFFFF0,1);

	return NO_DISPLAY;
}

INT16U dbg_sflash_flush(void)
{
	int address,pagebase;

	if ( CLI_GetArgCnt() < 1 ) return INV_PRM;	// �߿��� �׼��̹Ƿ� ���� ��ġ...

	address = comparison_num((char*)P_ARG(1));

	pagebase = (address / SPI_FLASH_PAGE) * SPI_FLASH_PAGE;
	
	nv_sflash_md(address);
	
	printk("\nErasing.. sector\n");

	SPI_FLASH_PageErase(pagebase);		// 1Kbytes

	nv_sflash_md(address);

    return NO_DISPLAY;
}

INT16U dbg_sflash_flush_all(void)
{
	int i;

	printk("\nErasing.. MCU Area\n");
	
	for(i=0; i<16; i++)
	{
		printk("%d\r",i);
		SPI_FLASH_BlockErase(0x600000+(i*SPI_FLASH_BLOCK)); // 64*16 Kbyte
	}
	
	printk("Done.\n");

    return NO_DISPLAY;
}

INT16U dbg_sflash_flash_chip(void)
{

	printk("\nChip Erasing..(C7H) \n");
	
	SPI_FLASH_ChipErase(); 

	printk("Done.\n");

	return NO_DISPLAY;
}


INT16U dbg_sflash_write(void)
{
	int address,data;

	if ( CLI_GetArgCnt() < 2 ) return INV_PRM;	

	address = comparison_num((char*)P_ARG(1));
	data = comparison_num((char*)P_ARG(2));

	SPI_FLASH_Member_Write( (INT8U *)&data, address, 1 );

	return NO_DISPLAY;

}

INT16U dbg_sflash_read(void)
{
	int address;
	INT8U data;

	if ( CLI_GetArgCnt() < 1 ) return INV_PRM;	

	address = comparison_num((char*)P_ARG(1));
	
	SPI_FLASH_BufferRead( (INT8U *)&data, address, 1);

	printd("\nSPI Flash Read 1Byte : %X \n",data);

	return NO_DISPLAY;
}


INT16U dbg_sflash_crc(void)
{
	int  filelen;
	INT16U ramCRC;
	
	if(CLI_GetArgCnt() < 1) return INV_PRM;

	filelen = comparison_num((char*)P_ARG(1));


	ramCRC = GetFlashAreaCRC(DOWNLOAD_SECTOR,filelen);
	printk("\nCheck wrote flash fileCRC(MCU): [%04X]\n",(INT16U)ramCRC);


	return NO_DISPLAY;
}


/**********************************************************************************************************
*                                      GetFlashAreaCRC
*
* ����        : �÷��� �޸𸮿� ����� �����͸� �о�� CRC�� ����Ѵ�.
* ��������    : faddr - �о�� �������� ���� �ּ�
*				size  - �о�� ������ ������ 
* ���ϰ�      : CRC �� 
**********************************************************************************************************/
static char getCRCBuf[SPI_FLASH_PAGE];
INT16U GetFlashAreaCRC(int faddr, int size)
{
	INT16U ramCRC=0;
	int i, quot, remain;

	quot = size / SPI_FLASH_PAGE;
	remain  = size % SPI_FLASH_PAGE;

	for(i=0; i<quot; i++)
	{
		SPI_FLASH_BufferRead((u8 *)getCRCBuf, faddr+(i*SPI_FLASH_PAGE), SPI_FLASH_PAGE);
		ramCRC = GenerateCRC((char*)getCRCBuf, SPI_FLASH_PAGE, ramCRC);
	}

	if(remain){
		SPI_FLASH_BufferRead((u8 *)getCRCBuf, faddr+(i*SPI_FLASH_PAGE), SPI_FLASH_PAGE);
		ramCRC = GenerateCRC((char*)getCRCBuf, remain, ramCRC);
	}
	
	return ramCRC;
}


/**********************************************************************************************************
*                                      nv_ldstr
*
* ����        : �÷��� �޸𸮿� �����͸� ���ų� �о���� �Լ�. SPI_FLASH_LoadSave �Լ��� ���� �Լ�.		
* ��������    : fbase - �޸��� ���� ���� 
*               pdata - ������ ����
*               sz    - ������ ������ 
*               rw    - 'read' 'write'
* ���ϰ�      : ������ 
**********************************************************************************************************/
//Non-volatile (�� �� �� ��) memory write or readout.
static int nv_ldstr(int fbase, void *pdata, int sz, int rw)
{
	INT16U ramCRC,getflashCRC,rdflashCRC;

	// WRITE ���� 
	if(rw == SAVE){
		// ������ �ϴ� �����Ϳ� ���� CRC�� ���
		ramCRC = GenerateCRC((char*)pdata, sz, 0);
		//printk("\n--W--> ramCRC: 0x%04X\n",ramCRC);
		// �����͸� �÷����� ���� 
		SPI_FLASH_Member_Write((u8 *)pdata, fbase, sz);//write data
		// Page�� ������ 2Byte ��ġ�� CRC ����.
		SPI_FLASH_Member_Write((u8 *)&ramCRC, fbase + (SPI_FLASH_PAGE * nEXTBLKSZ(sz) - sizeof(ramCRC)), sizeof(ramCRC));//write crc
		// ������ CRC�� �о�´� 
		SPI_FLASH_BufferRead((u8 *)&rdflashCRC, fbase + (SPI_FLASH_PAGE * nEXTBLKSZ(sz) - sizeof(rdflashCRC)), sizeof(rdflashCRC));//read crc
		//printk("--W--> rdflashCRC: 0x%04X\n",rdflashCRC);
		// ������ �����Ϳ� ���� CRC�� ��� 
		getflashCRC = GetFlashAreaCRC(fbase, sz);
		//printk("--W--> getflashCRC: 0x%04X\n",getflashCRC);
		// ramCRC�� ���� CRC�� ��   
		if(ramCRC != getflashCRC){
			printk("-> ramCRC:%X, getflashCRC:%X \n", ramCRC, getflashCRC);
			return -1;
		}
		// ramCRC�� �÷����� ������ CRC�� ��
		if(ramCRC != rdflashCRC){
			printk("-> ramCRC:%X, rdflashCRC:%X \n", ramCRC, rdflashCRC);
			return -2;
		}
        
	}
	// READ ���� 
	else{
		// ������ �����Ϳ� ���� CRC�� ��� 
		getflashCRC = GetFlashAreaCRC(fbase, sz);
		//printk("\n--R--> getflashCRC: 0x%04X\n",getflashCRC);
		// ������ CRC�� �о�´� 
		SPI_FLASH_BufferRead((u8 *)&rdflashCRC, fbase + (SPI_FLASH_PAGE * nEXTBLKSZ(sz) - sizeof(rdflashCRC)), sizeof(rdflashCRC));//read crc
		//printk("--R--> rdflashCRC: 0x%04X\n",rdflashCRC);
		// ���� CRC�� ������ CRC�� ��	
		if(getflashCRC != rdflashCRC){
			printk("-> getflashCRC:%X, rdflashCRC:%X \n", getflashCRC, rdflashCRC);
			return -2;
		}
		// �÷����� ����� �����͸� �о� �´� 
		SPI_FLASH_BufferRead((u8 *)pdata, fbase, sz);
	}

	return 0;
}


/**********************************************************************************************************
*                                      SPI_FLASH_LoadSave
*
* ����        : �÷��� �޸𸮿� ����ü �����͸� ���ų� �о� ���� �Լ�. ����ü ������ ��ŭ ���ų� �д´�  
* ��������    : itemNum - ����ü ������ �ش��ϴ� �ѹ�
*               rw      - 'SAVE' or 'NO_SAVE'
* ���ϰ�      : ������ 
**********************************************************************************************************/
int SPI_FLASH_LoadSave(int itemNum, int rw)
{
	int ret = 0;
	//INT8U back_amp_onoff[2];

	switch(itemNum){
		case PROJECT_SAVE:
			// ��Ʈ�δ����� ���� ����� ���� PROJECT_BASE�� ���ؼ��� CRC üũ�� ���� �ʴ´�  
			if(rw == SAVE)
			{
				SPI_FLASH_PageErase(PROJECT_BASE);
				SPI_FLASH_BufferWrite((INT8U *)&tPrjcode.project_code[0], PROJECT_BASE, sizeof(Prjcode_t));
			}
			else
			{
				SPI_FLASH_BufferRead((INT8U *)&tPrjcode.project_code[0], PROJECT_BASE, sizeof(Prjcode_t));
			}
			break;
			
		default:
			ret = 100;
			break;		
	}

	if(ret){
		printk("--> NV mem [%s] errorcode %d item:%d\n",rw?"Write":"Read",ret,itemNum);
	}

	
	return ret;
	
}

/**********************************************************************************************************
*                                      SPI_FLASH_Member_Write
*
* ����        : �����͸� ������ �ϴ� �ּ� ��ġ�� �ش��ϴ� Page�� �ڵ����� ����Ͽ� �ش� Sector�� �����͸� ������Ʈ�Ѵ� 	
* ��������    : pBuffer        - ������ ������ 
*				flashWaddr     - ������ ����ü�� ���� �ּ� 
*				NumByteToWrite - ������ ������ ������ 	
* ���ϰ�      : ���� 
**********************************************************************************************************/
static INT8U pDumpBuff[SPI_FLASH_PAGE];

void SPI_FLASH_Member_Write(u8* pBuffer, u32 flashWaddr, u16 NumByteToWrite)
{
	//INT8U err;
	int CurPageAddr,inPgWsz,inPgWoffset,reMainWsz,WrittenSz;

	if(BSP_SaveSem == NULL)
	{
		printk("%s err\n",__func__);
		return;
	}
	if(xSemaphoreTake(BSP_SaveSem, ( portTickType ) 10 )  == pdTRUE)
	{
	
		CurPageAddr = flashWaddr & ~(SPI_FLASH_PAGE-1);			// ó�� �õ��� Sector�� base�ּ�.
		inPgWoffset = flashWaddr % SPI_FLASH_PAGE; 				// Sector�ɼ»����� �ﺯ���ų ��������Ʈ .

		if( NumByteToWrite >= (SPI_FLASH_PAGE - inPgWoffset) )
			inPgWsz = SPI_FLASH_PAGE - inPgWoffset;
		else
			inPgWsz = NumByteToWrite;
			
	/*-----------------------------------------------------------------------------------------------------*/	
		SPI_FLASH_BufferRead(pDumpBuff, CurPageAddr ,SPI_FLASH_PAGE);			// ù���� ���� ���� ó��

		if (memcmp((char *)((int)pDumpBuff + inPgWoffset), pBuffer, inPgWsz )){ // �ٸ���??
			memcpy((char *)((int)pDumpBuff + inPgWoffset), pBuffer, inPgWsz );	// ������ ����.
			SPI_FLASH_PageErase(CurPageAddr);
			SPI_FLASH_BufferWrite(pDumpBuff, CurPageAddr, SPI_FLASH_PAGE);		// ������ ����.
		}
	/*-----------------------------------------------------------------------------------------------------*/	

		reMainWsz = NumByteToWrite - inPgWsz;							// �� ����� �������� ���� �� �̹� �� �������� ������ ���Ѵ�.

		WrittenSz = inPgWsz;

		while(1)
		{	
			CurPageAddr += SPI_FLASH_PAGE; 						// ����Sector�� �ּ� �̵�. 

			if( reMainWsz == 0 ){
				printk("Write Complete..\n");
				break;
			}															// �κ� �̻� ������ ��� �� ������ ū ������ ���� 							 
			else if( reMainWsz >= SPI_FLASH_PAGE ){				// ����Sector���� �ذ�� ���.
																			
				inPgWsz = SPI_FLASH_PAGE;
				
	/*-----------------------------------------------------------------------------------------------------*/	
				SPI_FLASH_BufferRead(pDumpBuff, CurPageAddr, SPI_FLASH_PAGE);

				if (memcmp((char *)pDumpBuff, (char *)((int)pBuffer+WrittenSz), inPgWsz)){	// �ٸ���??
					memcpy((char *)pDumpBuff, (char *)((int)pBuffer+WrittenSz), inPgWsz);	// ������ ����.
					SPI_FLASH_PageErase(CurPageAddr);
					SPI_FLASH_BufferWrite((u8 *)pDumpBuff, CurPageAddr, SPI_FLASH_PAGE); 	// ������ ����.
				}
	/*-----------------------------------------------------------------------------------------------------*/	

				WrittenSz += inPgWsz;
				reMainWsz -= SPI_FLASH_PAGE;
			}
			else{														// Sector���� �����Ƿ� �̰������� �ܼ��� Sector�� ���� ����.
				
				inPgWsz = reMainWsz;
				
	/*-----------------------------------------------------------------------------------------------------*/				
				SPI_FLASH_BufferRead(pDumpBuff, CurPageAddr, SPI_FLASH_PAGE);

				if (memcmp((char *)pDumpBuff, (char *)((int)pBuffer+WrittenSz) ,inPgWsz)){	// �ٸ���??
					memcpy((char *)pDumpBuff, (char *)((int)pBuffer+WrittenSz) ,inPgWsz);	// ������ ����.
					SPI_FLASH_PageErase(CurPageAddr);
					SPI_FLASH_BufferWrite((u8 *)pDumpBuff, CurPageAddr, SPI_FLASH_PAGE); 	// ������ ����.
				}
	/*-----------------------------------------------------------------------------------------------------*/	

				WrittenSz += inPgWsz;
				reMainWsz -= inPgWsz;
			}
		}
		
		xSemaphoreGive(BSP_SaveSem);
	}
}





/*
********************* (C) COPYRIGHT 2014 Loop ***************END OF FILE*****************************
*/
