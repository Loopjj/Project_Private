//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TSaveAs.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sGauge"
#pragma resource "*.dfm"
TSaveAsForm *SaveAsForm;
//---------------------------------------------------------------------------
__fastcall TSaveAsForm::TSaveAsForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSaveAsForm::AppException(TObject *Sender, Exception *E)
{

if(E->Message == "I/O error 21")
   ShowMessage("Storage device is not ready!");

}

//---------------------------------------------------------------------------

void __fastcall TSaveAsForm::FormShow(TObject *Sender)
{
   Edit_Fname->Text = DirectoryListBox->Directory;
   m_asFName = Edit_Fname->Text;

   Application->OnException = AppException;
   SetErrorMode(SEM_FAILCRITICALERRORS);
}
//---------------------------------------------------------------------------


void __fastcall TSaveAsForm::sButton_ExitClick(TObject *Sender)
{
   Close();
}

//---------------------------------------------------------------------------

void __fastcall TSaveAsForm::DirectoryListBoxChange(TObject *Sender)
{
   Edit_Fname->Text = DirectoryListBox->Directory;
   m_asFName = Edit_Fname->Text;
}

//---------------------------------------------------------------------------

void __fastcall TSaveAsForm::sButton_SavePathClick(TObject *Sender)
{
    TIniFile *ini;

    if(m_asFName != "") {
        ini=new TIniFile(ChangeFileExt(Application->ExeName,".INI"));
        ini->WriteString("PATH","°æ·Î",m_asFName);
        delete ini;
    }
}
//---------------------------------------------------------------------------

