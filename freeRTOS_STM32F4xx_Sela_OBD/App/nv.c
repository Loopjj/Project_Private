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

// 특정 데이터 변경 
//static INT8U pNv[SPI_FLASH_PAGE];
static void nv_modify(int addr, INT8U data)
{
	INT8U old_base;

	SPI_FLASH_BufferRead( &old_base, addr, 1);

	printk("\n\n[%05X]ORG:%02X ==> %02X\n", addr, old_base, data);
	
	SPI_FLASH_Member_Write(&data, addr, 1);

}


// SFlash의 sector 사이즈 만큼 Display해준다 
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

// SFlash의 leng*16 사이즈 만큼 Display해준다 
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

	if ( CLI_GetArgCnt() < 1 ) return INV_PRM;	// 중요한 액션이므로 안전 장치...

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
* 설명        : 플레쉬 메모리에 저장된 데이터를 읽어와 CRC를 계산한다.
* 전달인자    : faddr - 읽어올 데이터의 시작 주소
*				size  - 읽어올 데이터 사이즈 
* 리턴값      : CRC 값 
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
* 설명        : 플레쉬 메모리에 데이터를 쓰거나 읽어오는 함수. SPI_FLASH_LoadSave 함수의 하위 함수.		
* 전달인자    : fbase - 메모리의 시작 번지 
*               pdata - 데이터 버퍼
*               sz    - 데이터 사이즈 
*               rw    - 'read' 'write'
* 리턴값      : 에러값 
**********************************************************************************************************/
//Non-volatile (비 휘 발 성) memory write or readout.
static int nv_ldstr(int fbase, void *pdata, int sz, int rw)
{
	INT16U ramCRC,getflashCRC,rdflashCRC;

	// WRITE 동작 
	if(rw == SAVE){
		// 쓰고자 하는 데이터에 대한 CRC를 계산
		ramCRC = GenerateCRC((INT8U*)pdata, sz, 0);
		//printk("\n--W--> ramCRC: 0x%04X\n",ramCRC);
		// 데이터를 플레쉬에 저장 
		SPI_FLASH_Member_Write((u8 *)pdata, fbase, sz);//write data
		// Page의 마지막 2Byte 위치에 CRC 저장.
		SPI_FLASH_Member_Write((u8 *)&ramCRC, fbase + (SPI_FLASH_PAGE * nEXTBLKSZ(sz) - sizeof(ramCRC)), sizeof(ramCRC));//write crc
		// 저장한 CRC를 읽어온다 
		SPI_FLASH_BufferRead((u8 *)&rdflashCRC, fbase + (SPI_FLASH_PAGE * nEXTBLKSZ(sz) - sizeof(rdflashCRC)), sizeof(rdflashCRC));//read crc
		//printk("--W--> rdflashCRC: 0x%04X\n",rdflashCRC);
		// 저장한 데이터에 대한 CRC를 계산 
		getflashCRC = GetFlashAreaCRC(fbase, sz);
		//printk("--W--> getflashCRC: 0x%04X\n",getflashCRC);
		// ramCRC와 계산된 CRC를 비교   
		if(ramCRC != getflashCRC){
			printk("-> ramCRC:%X, getflashCRC:%X \n", ramCRC, getflashCRC);
			return -1;
		}
		// ramCRC와 플레쉬에 저장한 CRC를 비교
		if(ramCRC != rdflashCRC){
			printk("-> ramCRC:%X, rdflashCRC:%X \n", ramCRC, rdflashCRC);
			return -2;
		}
        
	}
	// READ 동작 
	else{
		// 저장한 데이터에 대한 CRC를 계산 
		getflashCRC = GetFlashAreaCRC(fbase, sz);
		//printk("\n--R--> getflashCRC: 0x%04X\n",getflashCRC);
		// 저장한 CRC를 읽어온다 
		SPI_FLASH_BufferRead((u8 *)&rdflashCRC, fbase + (SPI_FLASH_PAGE * nEXTBLKSZ(sz) - sizeof(rdflashCRC)), sizeof(rdflashCRC));//read crc
		//printk("--R--> rdflashCRC: 0x%04X\n",rdflashCRC);
		// 계산된 CRC와 저장한 CRC를 비교	
		if(getflashCRC != rdflashCRC){
			printk("-> getflashCRC:%X, rdflashCRC:%X \n", getflashCRC, rdflashCRC);
			return -2;
		}
		// 플레쉬에 저장된 데이터를 읽어 온다 
		SPI_FLASH_BufferRead((u8 *)pdata, fbase, sz);
	}

	return 0;
}


/**********************************************************************************************************
*                                      SPI_FLASH_LoadSave
*
* 설명        : 플레쉬 메모리에 구조체 데이터를 쓰거나 읽어 오는 함수. 구조체 사이즈 만큼 쓰거나 읽는다  
* 전달인자    : itemNum - 구조체 영역에 해당하는 넘버
*               rw      - 'SAVE' or 'NO_SAVE'
* 리턴값      : 에러값 
**********************************************************************************************************/
int SPI_FLASH_LoadSave(int itemNum, int rw)
{
	int ret = 0;
	//INT8U back_amp_onoff[2];

	switch(itemNum){
		case PROJECT_SAVE:
			// 부트로더와의 연관 관계로 인해 PROJECT_BASE에 대해서는 CRC 체크를 하지 않는다  
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
* 설명        : 데이터를 쓰고자 하는 주소 위치에 해당하는 Page를 자동으로 계산하여 해당 Sector만 데이터를 업데이트한다 	
* 전달인자    : pBuffer        - 저장할 데이터 
*				flashWaddr     - 저장할 구조체의 시작 주소 
*				NumByteToWrite - 저장할 데이터 사이즈 	
* 리턴값      : 없음 
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
	
		CurPageAddr = flashWaddr & ~(SPI_FLASH_PAGE-1);			// 처음 시도될 Sector의 base주소.
		inPgWoffset = flashWaddr % SPI_FLASH_PAGE; 				// Sector옵셋사이즈 즉변경시킬 시작포인트 .

		if( NumByteToWrite >= (SPI_FLASH_PAGE - inPgWoffset) )
			inPgWsz = SPI_FLASH_PAGE - inPgWoffset;
		else
			inPgWsz = NumByteToWrite;
			
	/*-----------------------------------------------------------------------------------------------------*/	
		SPI_FLASH_BufferRead(pDumpBuff, CurPageAddr ,SPI_FLASH_PAGE);			// 첫번쩨 블럭에 대한 처리

		if (memcmp((char *)((int)pDumpBuff + inPgWoffset), pBuffer, inPgWsz )){ // 다르냐??
			memcpy((char *)((int)pDumpBuff + inPgWoffset), pBuffer, inPgWsz );	// 데이터 변경.
			SPI_FLASH_PageErase(CurPageAddr);
			SPI_FLASH_BufferWrite(pDumpBuff, CurPageAddr, SPI_FLASH_PAGE);		// 데이터 쓰기.
		}
	/*-----------------------------------------------------------------------------------------------------*/	

		reMainWsz = NumByteToWrite - inPgWsz;							// 총 써야할 데이터의 갯수 중 이미 쓴 데이터의 갯수를 구한다.

		WrittenSz = inPgWsz;

		while(1)
		{	
			CurPageAddr += SPI_FLASH_PAGE; 						// 다음Sector로 주소 이동. 

			if( reMainWsz == 0 ){
				printk("Write Complete..\n");
				break;
			}															// 두블럭 이상 지웠다 써야 할 정도의 큰 데이터 관리 							 
			else if( reMainWsz >= SPI_FLASH_PAGE ){				// 다음Sector에서 해결될 경우.
																			
				inPgWsz = SPI_FLASH_PAGE;
				
	/*-----------------------------------------------------------------------------------------------------*/	
				SPI_FLASH_BufferRead(pDumpBuff, CurPageAddr, SPI_FLASH_PAGE);

				if (memcmp((char *)pDumpBuff, (char *)((int)pBuffer+WrittenSz), inPgWsz)){	// 다르냐??
					memcpy((char *)pDumpBuff, (char *)((int)pBuffer+WrittenSz), inPgWsz);	// 데이터 변경.
					SPI_FLASH_PageErase(CurPageAddr);
					SPI_FLASH_BufferWrite((u8 *)pDumpBuff, CurPageAddr, SPI_FLASH_PAGE); 	// 데이터 쓰기.
				}
	/*-----------------------------------------------------------------------------------------------------*/	

				WrittenSz += inPgWsz;
				reMainWsz -= SPI_FLASH_PAGE;
			}
			else{														// Sector보다 작으므로 이곳에서는 단순히 Sector를 새로 쓴다.
				
				inPgWsz = reMainWsz;
				
	/*-----------------------------------------------------------------------------------------------------*/				
				SPI_FLASH_BufferRead(pDumpBuff, CurPageAddr, SPI_FLASH_PAGE);

				if (memcmp((char *)pDumpBuff, (char *)((int)pBuffer+WrittenSz) ,inPgWsz)){	// 다르냐??
					memcpy((char *)pDumpBuff, (char *)((int)pBuffer+WrittenSz) ,inPgWsz);	// 데이터 변경.
					SPI_FLASH_PageErase(CurPageAddr);
					SPI_FLASH_BufferWrite((u8 *)pDumpBuff, CurPageAddr, SPI_FLASH_PAGE); 	// 데이터 쓰기.
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
* 설명        : Excel 다운로드시 Excel의 데이터를 내부 Flash의 특정 번지에 기록해 넣는다.
* 전달인자    : pdata  - 저장할 데이터 
*               sz     - 저장할 데이터의 크기 
*               rw     - 'SAVE' or 'NO_SAVE'
* 리턴값      : 에러값 
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
		
	// WRITE 동작 
	if(rw == SAVE){

		printk("\nExcel Parameter Save to IntFlash\n");
		printk("--W--> IntFlashAddr: 0x%08X\n",EXCEL_SAVE_BASE);
		// 내부 Flash 페이지를 얼마나 삭제할지 pageSz를 계산 
		pageSz = nINTBLKSZ(dataSz);
		
		printk("--W--> dataSz: 0x%04X\n",dataSz);
		printk("--W--> pageSz: 0x%04X\n",pageSz);
		
		// 쓰고자 하는 데이터에 대한 CRC를 계산 (Excel에서 내려온 데이터)
		ramCRC = GenerateCRC((char*)pdata, dataSz, 0);
		printk("--W--> ramCRC: 0x%04X\n",ramCRC);

/*-----------------------------------------------------------------------------------------------------*/	
		OSSchedLock();
		portENTER_CRITICAL();
		
		// 내부 Flash 메모리에 Write하기 위해 Lock을 해제	
		FLASH_Unlock();	
		
		// 내부 Flash 메모리를 삭제
		for(i=0;i<pageSz;i++){
			//printd("Erasing CODE PAGE 0x%08X\n",EXCEL_SAVE_BASE + (i*CODE_MEM_PAGESZ));
			FLASH_ErasePage(EXCEL_SAVE_BASE + (i*CODE_MEM_PAGESZ));
		}
		
		// 내부 Flash 메모리에 2Byte단위로 데이터 Write
		//printd("\nWrite data to IntFlash\n");
		for(i=0;i<dataSz;i+=2){
			
			databuf = (*(pdata+1) << 8) + *pdata;
			FLASH_ProgramHalfWord(EXCEL_SAVE_BASE + i, databuf);
			
			//printd("ADDR: 0x%08X, DATA: %04X %04X\n",EXCEL_SAVE_BASE + i, *pdata, *(pdata+1));
			
			pdata += 2;
		}
		
		// 위에서 계산한 CRC값을 저장 
		FLASH_ProgramHalfWord(EXCEL_SAVE_BASE + (CODE_MEM_PAGESZ * nINTBLKSZ(dataSz) - sizeof(ramCRC)), ramCRC);
		//printd("CRC ADDR: 0x%08X, CRC: 0x%04X\n",EXCEL_SAVE_BASE + (CODE_MEM_PAGESZ * nINTBLKSZ(dataSz) - sizeof(ramCRC)), ramCRC);

		portEXIT_CRITICAL();
		OSSchedUnlock();
/*-----------------------------------------------------------------------------------------------------*/	

		// 내부 Flash 메모리에서 데이터를 읽어 옴 
		//printk("\nRead data from IntFlash\n");
		//for(i=0;i<dataSz;i++){
			//printk("%04X ",*(char *)(EXCEL_SAVE_BASE + i));
			//if((i+1)%20==0) printk("\n");
		//}
		//printk("\n");
		// 내부 Flash에 저장된 CRC를 읽어 옴 
		rdflashCRC = *(INT32U *)(EXCEL_SAVE_BASE + (CODE_MEM_PAGESZ * nINTBLKSZ(dataSz) - sizeof(ramCRC)));
		printk("--W--> rdflashCRC: 0x%04X\n",rdflashCRC);
		
		// 내부 Flash에 저장된 데이터에 대한 CRC 계산 
		getflashCRC = GenerateCRC((char *)EXCEL_SAVE_BASE, dataSz, 0);
		printk("--W--> getflashCRC: 0x%04X\n\n",getflashCRC);

		// ramCRC와 계산된 CRC를 비교   
		if(ramCRC != getflashCRC){
			return -1;
		}
		// ramCRC와 플레쉬에 저장한 CRC를 비교
		if(ramCRC != rdflashCRC){
			return -2;
		}
	}
	// Read 동작 
	else{
		printk("\nExcel Parameter Read from IntFlash\n");
		printk("--R--> IntFlashAddr: 0x%08X\n",EXCEL_SAVE_BASE);

		// 내부 Flash에 저장된 데이터에 대한 CRC 계산 
		getflashCRC = GenerateCRC((char *)EXCEL_SAVE_BASE, dataSz, 0);
		printk("--R--> getflashCRC: 0x%04X\n",getflashCRC);

		// 내부 Flash에 저장된 CRC를 읽어 옴 
		rdflashCRC = *(INT32U *)(EXCEL_SAVE_BASE + (CODE_MEM_PAGESZ * nINTBLKSZ(dataSz) - sizeof(ramCRC)));
		printk("--R--> rdflashCRC: 0x%04X\n\n",rdflashCRC);

		// getflashCRC와 플레쉬에 저장한 CRC를 비교
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
