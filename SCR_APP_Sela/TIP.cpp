//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TIP.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma resource "*.dfm"
TIPForm *IPForm;
//---------------------------------------------------------------------------
__fastcall TIPForm::TIPForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TIPForm::sButton_ReadClick(TObject *Sender)
{
    MainForm->TranceiveDataProcess(0x1D0, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TIPForm::sButton_SetClick(TObject *Sender)
{
    unsigned char sdata[8];
    unsigned long ltmp;

    ltmp = StrToInt(Edit_Port->Text);
    sdata[0] = StrToInt(Edit_IP1->Text);
    sdata[1] = StrToInt(Edit_IP2->Text);
    sdata[2] = StrToInt(Edit_IP3->Text);
    sdata[3] = StrToInt(Edit_IP4->Text);
    sdata[4] = ltmp & 0xff;
    sdata[5] = (ltmp>>8) & 0xff;
    sdata[6] = (ltmp>>16) & 0xff;
    sdata[7] = (ltmp>>24) & 0xff;

    MainForm->TranceiveDataProcess(0x1D1, 8, sdata);
}
//---------------------------------------------------------------------------
void __fastcall TIPForm::sButton_EndClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

