//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TCan.h"
#include "TMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sComboBox"
#pragma resource "*.dfm"
TCanForm *CanForm;

//---------------------------------------------------------------------------
__fastcall TCanForm::TCanForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TCanForm::sButton_CAN_ReadClick(TObject *Sender)
{
    unsigned char rata[20];

    MainForm->TxCANData('Y', 0, rata);
}
//---------------------------------------------------------------------------
void __fastcall TCanForm::sButton_CAN_SetClick(TObject *Sender)
{
    unsigned char sdata[20];
    unsigned char can_config = 0;
    if(sComboBox_CAN_Protocol->ItemIndex < 0)
       sComboBox_CAN_Protocol->ItemIndex = 0;
    if(sComboBox_CAN_Protocol->ItemIndex == 1)
      can_config |= 0x10;
    if(CheckBox_AutoRest->Checked == true)
      can_config |= 0x08;
    if(CheckBox_BUSOff->Checked == true)
      can_config |= 0x04;
    if(CheckBox_CANError->Checked == true)
      can_config |= 0x02;
    if(CheckBox_CANWaring->Checked == true)
      can_config |= 0x01;
    sdata[0] = can_config;
    sdata[1] = BitTime[sComboBox_CAN_BSP->ItemIndex][0];
    sdata[2] = BitTime[sComboBox_CAN_BSP->ItemIndex][1];
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    MainForm->TxCANData('Z', 7, sdata);
}
//---------------------------------------------------------------------------
void __fastcall TCanForm::sButton_CAN_ResetClick(TObject *Sender)
{
    unsigned char sdata[2];

    sdata[0] = 0x00;
    MainForm->TxCANData('R', 0, sdata);
}
//---------------------------------------------------------------------------
void __fastcall TCanForm::sButton_VerClick(TObject *Sender)
{
    unsigned char rata[20];

    MainForm->TxCANData('V', 0, rata);        
}
//---------------------------------------------------------------------------
void __fastcall TCanForm::sButton_EndClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

