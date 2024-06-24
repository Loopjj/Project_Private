//---------------------------------------------------------------------------

#ifndef TSaveAsH
#define TSaveAsH
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
//---------------------------------------------------------------------------
class TSaveAsForm : public TForm
{
__published:	// IDE-managed Components
        TDriveComboBox *DriveComboBox;
        TDirectoryListBox *DirectoryListBox;
        TsButton *sButton_SavePath;
        TsButton *sButton_Exit;
        TEdit *Edit_Fname;
        void __fastcall AppException(TObject *Sender, Exception *E);
        void __fastcall sButton_ExitClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall DirectoryListBoxChange(TObject *Sender);
        void __fastcall sButton_SavePathClick(TObject *Sender);

private:	// User declarations
    FILE *m_fpFile;
    unsigned long m_ulFLength;
    unsigned char *m_ucFData;
public:		// User declarations
    AnsiString m_asFName, m_asDirName;
    FILE *fin, *fout;


    __fastcall TSaveAsForm(TComponent* Owner);
    void __fastcall TxFile(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TSaveAsForm *SaveAsForm;
//---------------------------------------------------------------------------
#endif
