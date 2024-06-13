/*
*************************************** (C) COPYRIGHT 2013 XNIL *************************************
*
* File Name          : nv.c
*
* Author             : Digital Development Team
*
* Version            : V1.0.0
*
* Date               : 01/28/2013
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

//xSemaphoreHandle BSP_SaveSem = NULL;

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

	ret = SPI_FLASH_ReadID();
	printk("\nSPI flash ID : %06X\n",ret);

	return NO_DISPLAY;
}

INT16U dbg_sflash_dispMap(void)
{
	printk("\n*** SFlash Memory Map ***\n");
	printk("PROJECT_BASE           :%05X (%04X)\n", PROJECT_BASE,           sizeof(Prjcode_t));  
	printk("SYSTEM_BASE            :%05X (%04X)\n", SYSTEM_BASE,			sizeof(SysControl_t));  
	//printk("TABLE_PARAM_SAVE_BASE  :%05X (%04X)\n", TABLE_PARAM_SAVE_BASE,  sizeof(table_nv_t));  
	//printk("DSP_DB_SAVE_BASE       :%05X (%04X)\n", DSP_DB_SAVE_BASE,       sizeof(total_table_t));  
	//printk("DSP_PARAM_SAVE_BASE    :%05X (%04X)\n", DSP_PARAM_SAVE_BASE,    sizeof(dsp_t));  
	//printk("RPT_DOWNLINK_BASE      :%05X (%04X)\n", RPT_DOWNLINK_BASE,      sizeof(RfControl_t));  
	//printk("RPT_UPLINK_BASE        :%05X (%04X)\n", RPT_UPLINK_BASE,        sizeof(RfControl_t));  
	//printk("SELF_CONDITION_BASE    :%05X (%04X)\n", SELF_CONDITION_BASE,    sizeof(Self_Condition_t));  
	//printk("MODEM_CONTROL_SAVE_BASE:%05X (%04X)\n", MODEM_CONTROL_SAVE_BASE,sizeof(ModemRfcontrol_t));  

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
	
#ifdef FLASH_AT45DB642D
	for(i=0; i<4; i++)
	{
		printk("%d\r",i);
		SPI_FLASH_PageErase(0x600000+(i*SPI_FLASH_SECTOR));		// 256*4 Kbytes
	}
#else
	for(i=0; i<16; i++)
	{
		printk("%d\r",i);
		SPI_FLASH_BlockErase(0x600000+(i*SPI_FLASH_BLOCK)); // 64*16 Kbyte
	}
#endif		
	printk("Done.\n");

    return NO_DISPLAY;
}

INT16U dbg_sflash_flash_chip(void)
{
//	int i;

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

	printd("\nSPI Flash(AT45DB642D) Read 1Byte : %X \n",data);

	return NO_DISPLAY;
}

INT16U dbg_sflash_select(void)
{
	int sel;
	
	if ( CLI_GetArgCnt() < 1 ) return INV_PRM;	

	sel = comparison_num((char*)P_ARG(1));

	if     (sel==0) 
	{
		//SPI_FLASH_CS_MCU();
	}

	printd("\nSPI Flash Control Select : %s(%d) \n", sel?"FPGA":"MCU", sel);

	return NO_DISPLAY;
}

INT16U dbg_inittest_select(void)
{

//	if ( CLI_GetArgCnt() < 1 ) return INV_PRM;	

//	sel = comparison_num((char*)P_ARG(1));
//	SPI_FLASH_CS_MCU();
//	SYS_Control_Init();
#ifdef FLASH_AT45DB642D
	SPI_FLASH_BinPageSize();
#endif

	return NO_DISPLAY;
}

INT16U dbg_sflash_crc(void)
{
	int sel_add, filelen;
	INT16U ramCRC;
	
	if(CLI_GetArgCnt() < 2) return INV_PRM;

	sel_add = comparison_num((char*)P_ARG(1));
	filelen = comparison_num((char*)P_ARG(2));

	if(sel_add==0)
	{
		ramCRC = GetFlashAreaCRC(DOWNLOAD_SECTOR,filelen);
		printk("\nCheck wrote flash fileCRC(MCU): [%04X]\n",(INT16U)ramCRC);
	}

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
		ramCRC = GenerateCRC((INT8U*)getCRCBuf, SPI_FLASH_PAGE, ramCRC);
	}

	if(remain){
		SPI_FLASH_BufferRead((u8 *)getCRCBuf, faddr+(i*SPI_FLASH_PAGE), SPI_FLASH_PAGE);
		ramCRC = GenerateCRC((INT8U*)getCRCBuf, remain, ramCRC);
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
		ramCRC = GenerateCRC((INT8U*)pdata, sz, 0);
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
			
		case SYSTEM_SAVE:
			ret = nv_ldstr(SYSTEM_BASE, (void *)&SysSet.Sys_firstboot, sizeof(SysControl_t), rw);
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



/**********************************************************************************************************
*                                      ExcelParam_LoadSave 
*
* ����        : Excel �ٿ�ε�� Excel�� �����͸� ���� Flash�� Ư�� ������ ����� �ִ´�.
* ��������    : pdata  - ������ ������ 
*               sz     - ������ �������� ũ�� 
*               rw     - 'SAVE' or 'NO_SAVE'
* ���ϰ�      : ������ 
**********************************************************************************************************/
#if 0
int ExcelParam_LoadSave(char *pdata, int dataSz, int rw)
{
#if OS_CRITICAL_METHOD == 3                    
	OS_CPU_SR  cpu_sr;
#endif

	INT32U i,pageSz;
	INT16U ramCRC,getflashCRC,rdflashCRC;
	INT16U databuf;
		
	// WRITE ���� 
	if(rw == SAVE){

		printk("\nExcel Parameter Save to IntFlash\n");
		printk("--W--> IntFlashAddr: 0x%08X\n",EXCEL_SAVE_BASE);
		// ���� Flash �������� �󸶳� �������� pageSz�� ��� 
		pageSz = nINTBLKSZ(dataSz);
		
		printk("--W--> dataSz: 0x%04X\n",dataSz);
		printk("--W--> pageSz: 0x%04X\n",pageSz);
		
		// ������ �ϴ� �����Ϳ� ���� CRC�� ��� (Excel���� ������ ������)
		ramCRC = GenerateCRC((char*)pdata, dataSz, 0);
		printk("--W--> ramCRC: 0x%04X\n",ramCRC);

/*-----------------------------------------------------------------------------------------------------*/	
		OSSchedLock();
		portENTER_CRITICAL();
		
		// ���� Flash �޸𸮿� Write�ϱ� ���� Lock�� ����	
		FLASH_Unlock();	
		
		// ���� Flash �޸𸮸� ����
		for(i=0;i<pageSz;i++){
			//printd("Erasing CODE PAGE 0x%08X\n",EXCEL_SAVE_BASE + (i*CODE_MEM_PAGESZ));
			FLASH_ErasePage(EXCEL_SAVE_BASE + (i*CODE_MEM_PAGESZ));
		}
		
		// ���� Flash �޸𸮿� 2Byte������ ������ Write
		//printd("\nWrite data to IntFlash\n");
		for(i=0;i<dataSz;i+=2){
			
			databuf = (*(pdata+1) << 8) + *pdata;
			FLASH_ProgramHalfWord(EXCEL_SAVE_BASE + i, databuf);
			
			//printd("ADDR: 0x%08X, DATA: %04X %04X\n",EXCEL_SAVE_BASE + i, *pdata, *(pdata+1));
			
			pdata += 2;
		}
		
		// ������ ����� CRC���� ���� 
		FLASH_ProgramHalfWord(EXCEL_SAVE_BASE + (CODE_MEM_PAGESZ * nINTBLKSZ(dataSz) - sizeof(ramCRC)), ramCRC);
		//printd("CRC ADDR: 0x%08X, CRC: 0x%04X\n",EXCEL_SAVE_BASE + (CODE_MEM_PAGESZ * nINTBLKSZ(dataSz) - sizeof(ramCRC)), ramCRC);

		portEXIT_CRITICAL();
		OSSchedUnlock();
/*-----------------------------------------------------------------------------------------------------*/	

		// ���� Flash �޸𸮿��� �����͸� �о� �� 
		//printk("\nRead data from IntFlash\n");
		//for(i=0;i<dataSz;i++){
			//printk("%04X ",*(char *)(EXCEL_SAVE_BASE + i));
			//if((i+1)%20==0) printk("\n");
		//}
		//printk("\n");
		// ���� Flash�� ����� CRC�� �о� �� 
		rdflashCRC = *(INT32U *)(EXCEL_SAVE_BASE + (CODE_MEM_PAGESZ * nINTBLKSZ(dataSz) - sizeof(ramCRC)));
		printk("--W--> rdflashCRC: 0x%04X\n",rdflashCRC);
		
		// ���� Flash�� ����� �����Ϳ� ���� CRC ��� 
		getflashCRC = GenerateCRC((char *)EXCEL_SAVE_BASE, dataSz, 0);
		printk("--W--> getflashCRC: 0x%04X\n\n",getflashCRC);

		// ramCRC�� ���� CRC�� ��   
		if(ramCRC != getflashCRC){
			return -1;
		}
		// ramCRC�� �÷����� ������ CRC�� ��
		if(ramCRC != rdflashCRC){
			return -2;
		}
	}
	// Read ���� 
	else{
		printk("\nExcel Parameter Read from IntFlash\n");
		printk("--R--> IntFlashAddr: 0x%08X\n",EXCEL_SAVE_BASE);

		// ���� Flash�� ����� �����Ϳ� ���� CRC ��� 
		getflashCRC = GenerateCRC((char *)EXCEL_SAVE_BASE, dataSz, 0);
		printk("--R--> getflashCRC: 0x%04X\n",getflashCRC);

		// ���� Flash�� ����� CRC�� �о� �� 
		rdflashCRC = *(INT32U *)(EXCEL_SAVE_BASE + (CODE_MEM_PAGESZ * nINTBLKSZ(dataSz) - sizeof(ramCRC)));
		printk("--R--> rdflashCRC: 0x%04X\n\n",rdflashCRC);

		// getflashCRC�� �÷����� ������ CRC�� ��
		if(getflashCRC != rdflashCRC){
			return -2;
		}
		else
			pdata = (char *)EXCEL_SAVE_BASE;
	}

	return 0;
}
#endif





/*
********************* (C) COPYRIGHT 2011 XNIL ***************END OF FILE*****************************
*/
