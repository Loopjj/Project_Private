//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TRTC.h"
#include "TMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sBitBtn"
#pragma resource "*.dfm"
TRTCForm *RTCForm;
//---------------------------------------------------------------------------
__fastcall TRTCForm::TRTCForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRTCForm::sButton_RTCReadClick(TObject *Sender)
{
    unsigned char sdata[8];

    sdata[0] = 0x01;
    sdata[1] = 0x00;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x115, 8, sdata);
    else
        MainForm->TxData(0xC6, 8, 0x3E, 0, sdata);
}
//---------------------------------------------------------------------------
void __fastcall TRTCForm::sButton_RTCSetClick(TObject *Sender)
{
    unsigned char sdata[8];

    sdata[0] = UpDown_Year->Position-2000;
    sdata[1] = UpDown_Month->Position;
    sdata[2] = UpDown_Day->Position;
    sdata[3] = UpDown_Hour->Position;
    sdata[4] = UpDown_Min->Position;
    sdata[5] = UpDown_Sec->Position;
    sdata[6] = 0;
    sdata[7] = 0;

    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x116, 8, sdata);
    else
        MainForm->TxData(0xC6, 8, 0x3D, 0, sdata);
}
//---------------------------------------------------------------------------
void __fastcall TRTCForm::CheckBox_PCTimeClick(TObject *Sender)
{
    if(CheckBox_PCTime->Checked)
        Timer2->Enabled = true;
    else
        Timer2->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TRTCForm::Timer2Timer(TObject *Sender)
{
    unsigned short y,m,d,h,n,s,ms;
    unsigned char temp0, temp1;
    Now().DecodeDate(&y,&m,&d);
    UpDown_Year->Position = y;
    UpDown_Month->Position = m;
    UpDown_Day->Position = d;
    Now().DecodeTime(&h,&n,&s,&ms);
    UpDown_Hour->Position = h;
    UpDown_Min->Position = n;
    UpDown_Sec->Position = s;
    MainForm->StatusBar1->Panels->Items[1]->Text = IntToStr(y)+":"+IntToStr(m)+":"+IntToStr(d)+" "+IntToStr(h)+":"+IntToStr(n)+":"+IntToStr(s);
}
//---------------------------------------------------------------------------
void __fastcall TRTCForm::sButton_CloseClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

