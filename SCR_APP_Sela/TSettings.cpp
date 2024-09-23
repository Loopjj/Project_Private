    //---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TSettings.h"
#include "TMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sCheckBox"
#pragma link "sLabel"
#pragma resource "*.dfm"
TParmForm *ParmForm;
//---------------------------------------------------------------------------
__fastcall TParmForm::TParmForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TParmForm::FormClose(TObject *Sender, TCloseAction &Action)
{
 //   
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::sButton_DefaultClick(TObject *Sender)
{
    MainForm->SData[0][0]->Text =  "180";
    MainForm->SData[0][1]->Text =  "10";
    MainForm->SData[0][2]->Text =  "-10";
    MainForm->SData[0][3]->Text =  "1";

    MainForm->SData[1][0]->Text =  "230";
    MainForm->SData[1][1]->Text =  "575";
    MainForm->SData[1][2]->Text =  "7000";
    MainForm->SData[1][3]->Text =  "10700";

    MainForm->SData[2][0]->Text =  "20.00";
    MainForm->SData[2][1]->Text =  "0.80";
    MainForm->SData[2][2]->Text =  "1";
    MainForm->SData[2][3]->Text =  "1";

    MainForm->SData[3][0]->Text =  "300";
    MainForm->SData[3][1]->Text =  "0";
    MainForm->SData[3][2]->Text =  "60";
    MainForm->SData[3][3]->Text =  "0";

    MainForm->SData[4][0]->Text =  "0.750";
    MainForm->SData[4][1]->Text =  "130";
    MainForm->SData[4][2]->Text =  "1.000";
    MainForm->SData[4][3]->Text =  "1";
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::sButton_SelectAllClick(TObject *Sender)
{
    sCheckBox_P0->Checked = true;
    sCheckBox_P1->Checked = true;
    sCheckBox_P2->Checked = true;
    sCheckBox_P3->Checked = true;
    sCheckBox_P4->Checked = true;
    for(int i = 0; i < 5; i++)
        m_bPWriteEnable[i] = true;
}

//---------------------------------------------------------------------------

void __fastcall TParmForm::sButton_ParReadClick(TObject *Sender)
{
    unsigned char sdata[8];
    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        sdata[0] = 0x01;
        sdata[1] = 0x00;
        sdata[2] = 0x00;
        sdata[3] = 0x00;
        sdata[4] = 0x00;
        sdata[5] = 0x00;
        sdata[6] = 0x00;
        sdata[7] = 0x00;
        MainForm->TranceiveDataProcess(0x140, 8, sdata);
    }
    else {
        MainForm->TxCmd(0xC6, 0x26, 1);
    }
}

//---------------------------------------------------------------------------
void __fastcall TParmForm::TxParm(unsigned char id)
{
    double dtmp;
    unsigned short stmp;
    signed short sstmp;
    long ltmp;
    unsigned char sdata[8];

    switch(id) {
    case 0:
        stmp = StrToInt(MainForm->SData[0][0]->Text);
        sdata[0] = stmp & 0xff;                                         // Cut MAF
        sdata[1] = (stmp>>8) & 0xff;
        stmp = StrToInt(MainForm->SData[0][1]->Text);
        sdata[2] = stmp & 0xff;                                         // Tank Level
        sdata[3] = (stmp>>8) & 0xff;
        stmp = StrToInt(MainForm->SData[0][2]->Text);
        sdata[4] = stmp & 0xff;                                         // Tank Temp
        sdata[5] = (stmp>>8) & 0xff;
        stmp = StrToInt(MainForm->SData[0][3]->Text);
        sdata[6] = stmp & 0xff;                                         // Slip
        sdata[7] = (stmp>>8) & 0xff;
    break;
    case 1:
        stmp = StrToInt(MainForm->SData[1][0]->Text);
        sdata[0] = stmp & 0xff;                                         // T Low
        sdata[1] = (stmp>>8) & 0xff;
        stmp = StrToInt(MainForm->SData[1][1]->Text);
        sdata[2] = stmp & 0xff;                                         // T High
        sdata[3] = (stmp>>8) & 0xff;
        stmp = StrToInt(MainForm->SData[1][2]->Text);
        sdata[4] = stmp & 0xff;                                         // P Low
        sdata[5] = (stmp>>8) & 0xff;
        stmp = StrToInt(MainForm->SData[1][3]->Text);
        sdata[6] = stmp & 0xff;                                         // P High
        sdata[7] = (stmp>>8) & 0xff;
    break;
    case 2:
        dtmp = (StrToFloat(MainForm->SData[2][0]->Text)*100.0);         // Low Bat
        sstmp = (signed short)dtmp;
        sdata[0] = sstmp & 0xff;
        sdata[1] = (sstmp>>8) & 0xff;
        dtmp = (StrToFloat(MainForm->SData[2][1]->Text)*100.0);         // T ratio
        sstmp = (signed short)dtmp;
        sdata[2] = sstmp & 0xff;
        sdata[3] = StrToInt(MainForm->SData[2][2]->Text) & 0xff;        // Scr Mode
        sdata[4] = StrToInt(MainForm->SData[2][3]->Text) & 0xff;        // Dosing Poriod
        sdata[5] = 0;
        sdata[6] = 0;
        sdata[7] = 0;
    break;
    case 3:
        stmp = StrToInt(MainForm->SData[3][0]->Text);
        sdata[0] = stmp & 0xff;                                         // t Idle
        sdata[1] = (stmp>>8) & 0xff;
        sdata[2] = StrToInt(MainForm->SData[3][1]->Text) & 0xff;        // Purge On/Off
        sdata[3] = 0;
        stmp = StrToInt(MainForm->SData[3][2]->Text);
        sdata[4] = stmp & 0xff;                                         // Purge Time
        sdata[5] = (stmp>>8) & 0xff;
        sdata[6] = StrToInt(MainForm->SData[3][3]->Text) & 0xff;        // Maf Cal type    
    break;
    case 4:
        dtmp = (StrToFloat(MainForm->SData[4][0]->Text)*1000.0);        // Maf Offset  :
        sstmp = (signed short)dtmp;
        sdata[0] = sstmp & 0xff;
        sdata[1] = (sstmp>>8) & 0xff;
        stmp = StrToInt(MainForm->SData[4][1]->Text);
        sdata[2] = stmp & 0xff;                                         // Maf Diameter
        sdata[3] = (stmp>>8) & 0xff;
        dtmp = (StrToFloat(MainForm->SData[4][2]->Text)*1000.0);        // Map Offset  :
        sstmp = (signed short)dtmp;
        sdata[4] = sstmp & 0xff;
        sdata[5] = (sstmp>>8) & 0xff;
        sdata[6] = StrToInt(MainForm->SData[4][3]->Text) & 0xff;        // Nozzle 0: Small 1: Large        
    break;
    }
    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        MainForm->TranceiveDataProcess(0x141+id, 8, sdata);
    }
    else {
        MainForm->TxData(0xC6, 8, 0x27, id, sdata);
    }

    ParmForm->m_bPWriteEnable[id] = false;

    if(id == 0)
        ParmForm->sCheckBox_P0->Checked = false;
    else if(id == 1)
        ParmForm->sCheckBox_P1->Checked = false;
    else if(id == 2)
        ParmForm->sCheckBox_P2->Checked = false;
    else if(id == 3)
        ParmForm->sCheckBox_P3->Checked = false;
    else if(id == 4)
        ParmForm->sCheckBox_P4->Checked = false;
}

//---------------------------------------------------------------------------

void __fastcall TParmForm::sButton_ParWriteClick(TObject *Sender)
{
    signed short sstmp;

    for(int i = 0; i < 5; i++) {
        if(m_bPWriteEnable[i]) {
            TxParm(i);
//            Sleep(100);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::sCheckBox_P0Click(TObject *Sender)
{
    m_bPWriteEnable[0] = sCheckBox_P0->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::sCheckBox_P1Click(TObject *Sender)
{
    m_bPWriteEnable[1] = sCheckBox_P1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::sCheckBox_P2Click(TObject *Sender)
{
    m_bPWriteEnable[2] = sCheckBox_P2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::sCheckBox_P3Click(TObject *Sender)
{
    m_bPWriteEnable[3] = sCheckBox_P3->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::sCheckBox_P4Click(TObject *Sender)
{
    m_bPWriteEnable[4] = sCheckBox_P4->Checked;
}
//---------------------------------------------------------------------------



void __fastcall TParmForm::sButton_EndClick(TObject *Sender)
{
        Close();                
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::ComboBox_TankChange(TObject *Sender)
{
  if(ComboBox_Tank->ItemIndex < 0)
     ComboBox_Tank->ItemIndex = 0;

  if(ComboBox_Tank->ItemIndex == 0) {
     Edit_P03->Text = "1";
  }
  else {
     Edit_P03->Text = "2";
  }        
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::ComboBox_PeriodChange(TObject *Sender)
{
  if(ComboBox_Period->ItemIndex < 0)
     ComboBox_Period->ItemIndex = 0;

  if(ComboBox_Period->ItemIndex == 0) {
     Edit_P23->Text = "1";
  }
  else if(ComboBox_Period->ItemIndex == 1) {
     Edit_P23->Text = "2";
  }
  else {
     Edit_P23->Text = "3";
  }        
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::ComboBox_ModeChange(TObject *Sender)
{
  if(ComboBox_Mode->ItemIndex < 0)
     ComboBox_Mode->ItemIndex = 0;

  if(ComboBox_Mode->ItemIndex == 0) {
     Edit_P22->Text = "1";
  }
  else if(ComboBox_Mode->ItemIndex == 1) {
     Edit_P22->Text = "2";
  }
  else if(ComboBox_Mode->ItemIndex == 2) {
     Edit_P22->Text = "3";
  }
  else {
     Edit_P22->Text = "4";
  }        
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::ComboBox_PurgeChange(TObject *Sender)
{
  if(ComboBox_Purge->ItemIndex < 0)
     ComboBox_Purge->ItemIndex = 0;

  if(ComboBox_Purge->ItemIndex == 0) {
     Edit_P31->Text = "0";
  }
  else {
     Edit_P31->Text = "1";
  }        
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::ComboBox_NozzleChange(TObject *Sender)
{
  if(ComboBox_Nozzle->ItemIndex < 0)
     ComboBox_Nozzle->ItemIndex = 0;

  if(ComboBox_Nozzle->ItemIndex == 0) {
     Edit_P43->Text = "1";
  }
  else {
     Edit_P43->Text = "2";
  }        
}
//---------------------------------------------------------------------------

void __fastcall TParmForm::ComboBox_MAFChange(TObject *Sender)
{
  if(ComboBox_MAF->ItemIndex < 0)
     ComboBox_MAF->ItemIndex = 0;

  if(ComboBox_MAF->ItemIndex == 0) {
     Edit_P40->Text = "0.590";
     Edit_P41->Text = "50";
  }
  else if(ComboBox_MAF->ItemIndex == 1) {
     Edit_P40->Text = "0.740";
     Edit_P41->Text = "62";
  }
  else if(ComboBox_MAF->ItemIndex == 2) {
     Edit_P40->Text = "0.700";
     Edit_P41->Text = "71";
  }
  else if(ComboBox_MAF->ItemIndex == 3) {
     Edit_P40->Text = "0.750";
     Edit_P41->Text = "78";
  }
  else if(ComboBox_MAF->ItemIndex == 4) {
     Edit_P40->Text = "1.000";
     Edit_P41->Text = "82";
  }
  else if(ComboBox_MAF->ItemIndex == 5) {
     Edit_P40->Text = "0.700";
     Edit_P41->Text = "130";
  }
  else {
     Edit_P40->Text = "0.670";
     Edit_P41->Text = "153";
  }        
}
//---------------------------------------------------------------------------
