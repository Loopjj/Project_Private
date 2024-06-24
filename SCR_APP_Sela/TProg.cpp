//---------------------------------------------------------------------------

#include <vcl.h>
#include <mem.h>

#pragma hdrstop

#include "TProg.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sGauge"
#pragma resource "*.dfm"
TProgForm *ProgForm;
//---------------------------------------------------------------------------
__fastcall TProgForm::TProgForm(TComponent* Owner)
    : TForm(Owner)
{
}
/*
*********************************************************************************************************
*                                               CONSTANT
*********************************************************************************************************
*/
#define  CRC_16_POLYNOMIAL      0x1021
#define  CRC_TAB_SIZE		256

#define RESPONSE_WAIT 	        2000
#define DOWNLOAD_START	        0
#define DOWNLOAD_CRC	        1
#define DOWNLOAD_PROG	        2
#define DOWNLOAD_END	        3
#define DOWNLOAD_WAIT	        4

static const unsigned short Crc16_Table [CRC_TAB_SIZE]  = {
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



/*
*********************************************************************************************************
*                                                FUNCTIONS
*********************************************************************************************************
*/

/*
********************************************************************************
* Description : GenerateCRC
* Arguments   : INT8U *pszBuffer, INT32U nLength, INT16U last_CRC16
* Return      : 
* Note        : crc16-ccitt
******************************************************************************** 
*/
unsigned short GenerateCRC(unsigned char *pszBuffer, unsigned int nLength, unsigned short last_CRC16)
{

	unsigned short wData, wCRC_16;

	nLength *= 8;
	
	for(wCRC_16=last_CRC16; nLength>=8; nLength-=8,pszBuffer++){
		wCRC_16 = Crc16_Table[((wCRC_16 >> 8) ^ * pszBuffer)&0xFF] ^ (wCRC_16 << 8);
	} 

	if(nLength != 0)
	{
		wData = ((unsigned short)(*pszBuffer)) << (8);

		while(nLength-- != 0)
		{
			if(((wCRC_16 ^ wData) & ((unsigned short) 1 << 15)) != 0){
				wCRC_16 <<= 1;
				wCRC_16 ^= CRC_16_POLYNOMIAL; 
			} 
			else 
				wCRC_16 <<= 1;
			wData <<= 1;
		}
	}

	return wCRC_16;
}

/*
********************************************************************************
* Description : GetCRC16
* Arguments   : INT16U last_CRC16, char data
* Return      : 
* Note        : crc16-ccitt
******************************************************************************** 
*/
unsigned short GetCRC16(unsigned short last_CRC16, char data)
{
  return((last_CRC16<<8) ^ Crc16_Table[((last_CRC16>>8) ^ data)&0x00FF]);
}


/*
********************************************************************************
* Description : CRCCheck
* Arguments   : INT8U *pszData, INT32U nLength
* Return      : 
* Note        : crc16-ccitt
******************************************************************************** 
*/
unsigned int CRCCheck(unsigned char *pszData, unsigned int nLength)
{
	unsigned short wCRC, wTemp;
 
	wCRC=GenerateCRC((unsigned char *)pszData+1,nLength-4,0);

	wTemp = (*(pszData+(nLength-3))<<8) | *(pszData+(nLength-2));

	if(wCRC != wTemp){
		return 0;
	}
 
	return 1;
}


//---------------------------------------------------------------------------

void __fastcall TProgForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    ProgTimer->Enabled = false;
    if(m_asFName != "") {
        Memo->Clear();
        Memo->Lines->Add(m_asFName);
        Memo->Lines->SaveToFile("BMI.ini");
    }
}

//---------------------------------------------------------------------------

void __fastcall TProgForm::CAN_TxFile(void)
{
    int i, handle, tmp, dev, mod;
    unsigned short tempSize;
    unsigned char sdata[8];

    switch(m_uStatus) {
    case DOWNLOAD_START:
        if ((handle = open(m_asFName.c_str(), O_RDONLY | O_BINARY)) == -1)
        {
                Memo->Lines->Add("Error : File Read");
        }
        else {
                m_ulFLength = filelength(handle);
                sdata[0] = m_ulFLength & 0xff;
                sdata[1] = (m_ulFLength>>8) & 0xff;
                sdata[2] = (m_ulFLength>>16) & 0xff;
                sdata[3] = (m_ulFLength>>24) & 0xff;
                m_ulFPacket = m_ulFLength/8 + (m_ulFLength % 8 ? 1 : 0);

                sdata[4] = m_ulFPacket & 0xff;
                sdata[5] = (m_ulFPacket>>8) & 0xff;
                sdata[6] = (m_ulFPacket>>16) & 0xff;
                sdata[7] = (m_ulFPacket>>24) & 0xff;
                MainForm->TranceiveDataProcess(0x10, 8, sdata);
        }
        m_uSPacket = 0;
        m_ulDev = m_ulFLength/8;
        m_ulMod = m_ulFLength%8;
        m_ucFData = new unsigned char[m_ulFLength];
        read(handle, m_ucFData, m_ulFLength);
        close(handle);
        m_ulCheckResponseCount = 0;
        m_uStatus = DOWNLOAD_CRC;
        break;
    case DOWNLOAD_CRC:
       if(m_nProgStauts == 1) {
                CRCValue = 0;
                dev = m_ulFLength/256;
                mod = m_ulFLength%256;
                for(i = 0; i < dev; i++) {
                        CRCValue = GenerateCRC((unsigned char*)&m_ucFData[i*256], 256, CRCValue);
                }
                if(mod) {
                        CRCValue = GenerateCRC((unsigned char*)&m_ucFData[dev*256], mod, CRCValue);
                }
                memset(sdata, 0, 8);
                memcpy(sdata, (char *)&CRCValue, 2);
                MainForm->TranceiveDataProcess(0x11, 8, sdata);
                m_ulCheckResponseCount = 0;
                m_uStatus = DOWNLOAD_PROG;
       }
       if(m_ulCheckResponseCount++ >= RESPONSE_WAIT) {   //2s wait
       	        m_ulCheckResponseCount = 0;
		m_uStatus = DOWNLOAD_WAIT;
       }
       break;
    case DOWNLOAD_PROG:
        if(m_nProgStauts == 1) {
                m_uSPacket++;
                memset(sdata, 0, 8);
                m_ulCheckResponseCount = 0;
                if(m_uSPacket < m_ulFPacket) {
                        memcpy(sdata, (char *)&m_ucFData[(m_uSPacket-1)*8], 8);
                        MainForm->TranceiveDataProcess(0x12, 8, sdata);
                        m_nProgStauts = 0;
                        sGauge->Progress = m_uSPacket*100/m_ulFPacket;
                }
                else {
                        if(m_ulMod == 0) tempSize = 8;
                        else tempSize = m_ulMod;
                        memcpy(sdata, (char *)&m_ucFData[(m_uSPacket-1)*8], tempSize);
                        MainForm->TranceiveDataProcess(0x12, 8, sdata);
                        m_nProgStauts = 0;
                        sGauge->Progress = 100;
                        m_uStatus = DOWNLOAD_END;
                }
        }
        if(m_ulCheckResponseCount++ >= RESPONSE_WAIT) {   //2s wait
                m_ulCheckResponseCount = 0;
                m_uStatus = DOWNLOAD_WAIT;
        }
        break;
    case DOWNLOAD_END:
        if(m_nProgStauts == 1) {
                memset(sdata, 0, 8);
                sdata[0] = 0x01;
                MainForm->TranceiveDataProcess(0x13, 8, sdata);
                m_ulCheckResponseCount = 0;
                m_uStatus = DOWNLOAD_WAIT;
        }
        if(m_ulCheckResponseCount++ >= RESPONSE_WAIT) {   //2s wait
                m_ulCheckResponseCount = 0;
		m_uStatus = DOWNLOAD_WAIT;
        }
        break;
    case DOWNLOAD_WAIT:
        delete m_ucFData;
        m_nProgStauts = 0;
        sGauge->Progress = 0;
        ProgTimer->Enabled = false;
        m_uStatus = DOWNLOAD_START;
        break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TProgForm::RS232_TxFile(void)
{
    int handle;
    unsigned short tempSize;

    switch(m_uStatus) {
      case DOWNLOAD_START:
        if ((handle = open(m_asFName.c_str(), O_RDONLY | O_BINARY)) == -1)
        {
          Memo->Lines->Add("Error : File Read");
          ProgTimer->Enabled = false;
        }
        else {
          FileInfo.FileSize = filelength(handle);
          FileInfo.OnePacketSize = ONE_PACKET_SIZE;
          FileInfo.TotalPacketSize = FileInfo.FileSize/ONE_PACKET_SIZE + (FileInfo.FileSize%ONE_PACKET_SIZE ? 1 : 0);
          FileInfo.CheckCode = 0x1604;
          m_uMod =  FileInfo.FileSize%ONE_PACKET_SIZE;
          m_ucFData = new unsigned char[FileInfo.FileSize];
          read(handle, m_ucFData, FileInfo.FileSize);
          MainForm->TxData(0xc6, sizeof(FILE_INFO), 0x10, 0x00, (unsigned char*)&FileInfo);
        }
        close(handle);
        m_uSPacket = 0;
        m_ulCheckResponseCount = 0;
        m_uStatus = DOWNLOAD_PROG;
        break;
      case DOWNLOAD_PROG:
        if(m_nProgStauts == 1) {
          m_uSPacket++;
          m_ulCheckResponseCount = 0;
          memset((char *)&FileData.PacketData, 0, ONE_PACKET_SIZE);
          FileData.PacketNo = m_uSPacket;
          if(m_uSPacket < FileInfo.TotalPacketSize) {
            memcpy((char *)&FileData.PacketData, (char *)&m_ucFData[(m_uSPacket-1)*ONE_PACKET_SIZE], ONE_PACKET_SIZE);
            MainForm->TxData(0xc6, sizeof(FILE_DATA), 0x11, 0x00, (unsigned char*)&FileData);
            m_nProgStauts = 0;
            sGauge->Progress = (m_uSPacket*100)/FileInfo.TotalPacketSize;
          }
          else {
            if(m_uMod == 0) tempSize = ONE_PACKET_SIZE;
            else tempSize = m_uMod;
            memcpy((char *)&FileData.PacketData, (char *)&m_ucFData[(m_uSPacket-1)*ONE_PACKET_SIZE], tempSize);
            MainForm->TxData(0xc6, tempSize+2, 0x11, 0x00, (unsigned char*)&FileData);
            m_nProgStauts = 0;
            sGauge->Progress = 100;
            m_uStatus = DOWNLOAD_END;
          }
          Application->ProcessMessages();
        }
        if(m_ulCheckResponseCount++ >= RESPONSE_WAIT) {   //2s wait
			    m_ulCheckResponseCount = 0;
			    m_uStatus = DOWNLOAD_WAIT;
        }
        break;
      case DOWNLOAD_END:
        if(m_nProgStauts == 1) {
          LastData.LastPacketNo   = m_uSPacket;
          LastData.LastDataLength = m_uMod;
          MainForm->TxData(0xc6,sizeof(LAST_DATA), 0x12, 0x00, (char *)&LastData);
          m_uStatus = DOWNLOAD_WAIT;
        }
        if(m_ulCheckResponseCount++ >= RESPONSE_WAIT) {   //2s wait
			    m_ulCheckResponseCount = 0;
			    m_uStatus = DOWNLOAD_WAIT;
        }
        break;
      case DOWNLOAD_WAIT:
        delete m_ucFData;
        m_nProgStauts = 0;
        sGauge->Progress = 0;
        m_uStatus = DOWNLOAD_START;
        ProgTimer->Enabled = false;
        break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TProgForm::ProgTimerTimer(TObject *Sender)
{
    if(MainForm->sComboBox_Device->ItemIndex == 0)
        CAN_TxFile();
    else
        RS232_TxFile();
}
//---------------------------------------------------------------------------

void __fastcall TProgForm::FileListBoxClick(TObject *Sender)
{
   bool ok;

   for(int i = 0; i < FileListBox->Count; i++) {
      if(FileListBox->Selected[i]) {
         if(FileListBox->Items->Strings[i] == Edit_Fname->Text) {
           break;
         }
         else ok = true;
         if(ok) {
            Edit_Fname->Text = FileListBox->Directory+"\\"+FileListBox->Items->Strings[i];
            m_asFName = Edit_Fname->Text;
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TProgForm::AppException(TObject *Sender, Exception *E)
{

if(E->Message == "I/O error 21")
   ShowMessage("Storage device is not ready!");

}

//---------------------------------------------------------------------------

void __fastcall TProgForm::FormShow(TObject *Sender)
{
   Memo->Clear();
   if(FileExists("BMI.ini")) {
       Memo->Lines->LoadFromFile("BMI.ini");
       m_asFName = Memo->Lines->Strings[0];
       Edit_Fname->Text = m_asFName;
       Memo->Clear();
   }

   Application->OnException = AppException;
   SetErrorMode(SEM_FAILCRITICALERRORS);

   sGauge->Progress = 0;
   if(FileListBox->Count == 0)
       FileListBox->Items->Add(" ");
}
//---------------------------------------------------------------------------

void __fastcall TProgForm::sButton_ExitClick(TObject *Sender)
{
  ProgTimer->Enabled = false;
  Close();
}

//---------------------------------------------------------------------------

void __fastcall TProgForm::sButton_ProgramClick(TObject *Sender)
{
    if(!MainForm->ApdComPort1->Open) {
        ShowMessage("Please Open the COM Port First.");
        return;
    }
    m_uStatus = DOWNLOAD_START;
    m_nProgStauts = 0;
    sGauge->Progress = 0;
    ProgTimer->Enabled = true;
}
//---------------------------------------------------------------------------


