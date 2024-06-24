//---------------------------------------------------------------------------

#ifndef TProgH
#define TProgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include "sButton.hpp"
#include "sGauge.hpp"

#define ONE_PACKET_SIZE       128

#pragma pack(push, before_pack, 1)
typedef struct {
  unsigned int      FileSize;
  unsigned short    OnePacketSize;
  unsigned short    TotalPacketSize;
  unsigned short    CheckCode;
}FILE_INFO;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef struct {
  unsigned short    PacketNo;
  unsigned char     PacketData[ONE_PACKET_SIZE];
}FILE_DATA;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef struct {
  unsigned short    LastPacketNo;
  unsigned short    LastDataLength;
}LAST_DATA;
#pragma pack(pop, before_pack)
//---------------------------------------------------------------------------
class TProgForm : public TForm
{
__published:	// IDE-managed Components
    TTimer *ProgTimer;
    TLabel *Label1;
    TEdit *Edit_Fname;
    TMemo *Memo;
        TDriveComboBox *DriveComboBox;
        TDirectoryListBox *DirectoryListBox;
        TFilterComboBox *FilterComboBox;
        TFileListBox *FileListBox;
        TsButton *sButton_Program;
        TsButton *sButton_BWrite;
        TsButton *sButton_Exit;
        TsGauge *sGauge;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ProgTimerTimer(TObject *Sender);
        void __fastcall FileListBoxClick(TObject *Sender);
        void __fastcall AppException(TObject *Sender, Exception *E);
        void __fastcall sButton_ExitClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall sButton_ProgramClick(TObject *Sender);
private:	// User declarations
    FILE *m_fpFile;
    unsigned char *m_ucFData;
    FILE_INFO FileInfo;
    FILE_DATA FileData;
    LAST_DATA LastData;
public:		// User declarations
    AnsiString m_asFName, m_asDirName;
    unsigned long m_ulFLength;
    unsigned short m_ulFPacket, m_ulDev, m_ulMod, m_uSPacket;
    unsigned short m_uMod, m_uRPacket;
    unsigned int m_ulCheckResponseCount;
    int m_nProgStauts;
    unsigned char m_uStatus;
    unsigned short CRCValue;
    __fastcall TProgForm(TComponent* Owner);
    void __fastcall CAN_TxFile(void);
    void __fastcall RS232_TxFile(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TProgForm *ProgForm;
//---------------------------------------------------------------------------
#endif
