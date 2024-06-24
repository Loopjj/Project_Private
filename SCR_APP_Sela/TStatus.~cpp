//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TStatus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sLabel"
#pragma resource "*.dfm"
TStatusForm *StatusForm;
//---------------------------------------------------------------------------
__fastcall TStatusForm::TStatusForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TStatusForm::StatusBtnClick(TObject *Sender)
{
        StatusForm->Close();
}
//---------------------------------------------------------------------------
void __fastcall TStatusForm::SetAlarmMsg(AnsiString str)
{
        sLabel_Msg->Caption = str;
}
