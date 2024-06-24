//---------------------------------------------------------------------------

#ifndef TProg1H
#define TProg1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include "sButton.hpp"
#include "sGauge.hpp"
//---------------------------------------------------------------------------
class TProg1Form : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *Edit_Fname;
        TMemo *Memo;
        TDriveComboBox *DriveComboBox;
        TDirectoryListBox *DirectoryListBox;
        TFilterComboBox *FilterComboBox;
        TFileListBox *FileListBox;
        TTimer *ProgTimer;
        TsButton *sButton_Program;
        TsButton *sButton_Cancel;
        TsButton *sButton_Exit;
        TsGauge *sGauge;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
 	void __fastcall ProgTimerTimer(TObject *Sender);
        void __fastcall FileListBoxClick(TObject *Sender);
        void __fastcall AppException(TObject *Sender, Exception *E);
        void __fastcall sButton_ProgramClick(TObject *Sender);
        void __fastcall sButton_CancelClick(TObject *Sender);
        void __fastcall sButton_ExitClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    FILE *m_fpFile;
    unsigned long m_ulFLength;
    unsigned char *m_ucFData;

public:		// User declarations
    AnsiString m_asFName, m_asDirName;
    int m_nProgStep;
    unsigned char m_bOKflag;
    unsigned long m_bFLength;    
        __fastcall TProg1Form(TComponent* Owner);
    void __fastcall TxFile(void);        
};
//---------------------------------------------------------------------------
extern PACKAGE TProg1Form *Prog1Form;
//---------------------------------------------------------------------------
#endif
