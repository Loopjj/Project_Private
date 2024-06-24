//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TCalib.h"
#include "TMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sBitBtn"
#pragma resource "*.dfm"
TCalibForm *CalibForm;
//---------------------------------------------------------------------------
__fastcall TCalibForm::TCalibForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCalibForm::sButton_TReadClick(TObject *Sender)
{
    unsigned char sdata[8];

    if(CB_TNo->ItemIndex < 0) CB_TNo->ItemIndex = 0;
    sdata[0] = 0x01;
    sdata[1] = CB_TNo->ItemIndex;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;
    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x134, 8, sdata);
    else
        MainForm->TxData(0xC6, 8, 0x33, CB_TNo->ItemIndex, sdata);
}
//---------------------------------------------------------------------------
void __fastcall TCalibForm::sButton_TWriteClick(TObject *Sender)
{
    double dtmp;
    unsigned short stmp;
    unsigned char sdata[16], ctmp;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        sdata[0] = CB_TNo->ItemIndex;
        if(CheckBox_TNo->Checked==true)
                sdata[1] = 0x01;
        else
                sdata[1] = 0x00;
        sdata[2] = 0x00;
        sdata[3] = 0x00;
        stmp = (unsigned short)StrToInt(Edit_TRangeL->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_TRangeH->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x135, 8, sdata);
        Sleep(100);

        if(CB_TNo->ItemIndex < 0) CB_TNo->ItemIndex = 0;
        sdata[0] = CB_TNo->ItemIndex;
        sdata[1] = 0x00;
        sdata[2] = 0x00;
        sdata[3] = 0x00;
        stmp = (unsigned short)StrToInt(Edit_TCalLow->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_TCalHigh->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x136, 8, sdata);
        Sleep(100);
    }
    else {
        if(CheckBox_TNo->Checked==true)
                sdata[0] = 0x01;
        else
                sdata[0] = 0x00;
        stmp = (unsigned short)StrToInt(Edit_TRangeL->Text);
        sdata[1] = stmp & 0xff;
        sdata[2] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_TRangeH->Text);
        sdata[3] = stmp & 0xff;
        sdata[4] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_TCalLow->Text);
        sdata[5] = stmp & 0xff;
        sdata[6] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_TCalHigh->Text);
        sdata[7] = stmp & 0xff;
        sdata[8] = (stmp>>8) & 0xff;
        MainForm->TxData(0xC6, 9, 0x34, CB_TNo->ItemIndex, sdata);
    }
}
//---------------------------------------------------------------------------
void __fastcall TCalibForm::sButton_MAFReadClick(TObject *Sender)
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
        MainForm->TranceiveDataProcess(0x130, 8, sdata);
    else
        MainForm->TxData(0xC6, 8, 0x30, 0, sdata);
}
//---------------------------------------------------------------------------
void __fastcall TCalibForm::sButton_MAFDefaultClick(TObject *Sender)
{
    Edit_ADC1->Text = "839";
    Edit_ADC2->Text = "1639";
    Edit_ADC3->Text = "2434";
    Edit_ADC4->Text = "3235";
    Edit_ADC5->Text = "3637";
    Edit_Offset1->Text = "-0.02";
    Edit_Offset2->Text = "0.00";
    Edit_Offset3->Text = "0.03";
    Edit_Offset4->Text = "0.02";
    Edit_Offset5->Text = "0.07";
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sBitBtn_MAFADCReadClick(TObject *Sender)
{
    unsigned char sdata[8], Btn_st;

    if(!MainForm->ApdComPort1->Open) {
        ShowMessage("Please Open the COM Port First.");
        Timer1->Enabled = false;
        return;
    }

    if(sBitBtn_MAFADCRead->Down == true) {
        sBitBtn_MAFADCRead->Down = false;
        Btn_st = 0;
        sBitBtn_MAFADCRead->Caption = "ADC";
    }
    else {
        sBitBtn_MAFADCRead->Down = true;
        Btn_st = 1;
        sBitBtn_MAFADCRead->Caption = "Reading..";
    }
    sdata[0] = 0x00;
    sdata[1] = Btn_st;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;
    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x130, 8, sdata);
    else {
        if(Btn_st)
            Timer1->Enabled = true;
        else
            Timer1->Enabled = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_MAFWriteClick(TObject *Sender)
{
    double dtmp;
    unsigned short stmp;
    unsigned char sdata[16];
    char ctmp;

    if(Edit_ADC1->Text == "0" && Edit_ADC2->Text == "0" && Edit_ADC3->Text == "0" && Edit_ADC4->Text == "0" && Edit_ADC5->Text == "0")
        return;
    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        stmp = (unsigned short)StrToInt(Edit_ADC1->Text);
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_ADC2->Text);
        sdata[2] = stmp & 0xff;
        sdata[3] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_ADC3->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_ADC4->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x131, 8, sdata);
        Sleep(100);

        stmp = (unsigned short)StrToInt(Edit_ADC5->Text);
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        sdata[2] = 0x00;
        dtmp = (StrToFloat(Edit_Offset1->Text)*100);
        ctmp = (char)dtmp;
        sdata[3] = ctmp & 0xff;
        dtmp = (StrToFloat(Edit_Offset2->Text)*100);
        ctmp = (char)dtmp;
        sdata[4] = ctmp & 0xff;
        dtmp = (StrToFloat(Edit_Offset3->Text)*100);
        ctmp = (char)dtmp;
        sdata[5] = ctmp & 0xff;
        dtmp = (StrToFloat(Edit_Offset4->Text)*100);
        ctmp = (char)dtmp;
        sdata[6] = ctmp & 0xff;
        dtmp = (StrToFloat(Edit_Offset5->Text)*100);
        ctmp = (char)dtmp;
        sdata[7] = ctmp & 0xff;
        MainForm->TranceiveDataProcess(0x132, 8, sdata);
        Sleep(100);
    }
    else {
        stmp = (unsigned short)StrToInt(Edit_ADC1->Text);
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        dtmp = (StrToFloat(Edit_Offset1->Text)*100);
        ctmp = (char)dtmp;
        sdata[2] = ctmp & 0xff;
        stmp = (unsigned short)StrToInt(Edit_ADC2->Text);
        sdata[3] = stmp & 0xff;
        sdata[4] = (stmp>>8) & 0xff;
        dtmp = (StrToFloat(Edit_Offset2->Text)*100);
        ctmp = (char)dtmp;
        sdata[5] = ctmp & 0xff;
        stmp = (unsigned short)StrToInt(Edit_ADC3->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        dtmp = (StrToFloat(Edit_Offset3->Text)*100);
        ctmp = (char)dtmp;
        sdata[8] = ctmp & 0xff;
        stmp = (unsigned short)StrToInt(Edit_ADC4->Text);
        sdata[9] = stmp & 0xff;
        sdata[10] = (stmp>>8) & 0xff;
        dtmp = (StrToFloat(Edit_Offset4->Text)*100);
        ctmp = (char)dtmp;
        sdata[11] = ctmp & 0xff;
        stmp = (unsigned short)StrToInt(Edit_ADC5->Text);
        sdata[12] = stmp & 0xff;
        sdata[13] = (stmp>>8) & 0xff;
        dtmp = (StrToFloat(Edit_Offset5->Text)*100);
        ctmp = (char)dtmp;
        sdata[14] = ctmp & 0xff;
        MainForm->TxData(0xC6, 15, 0x31, 0, sdata);
    }

}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_dPressClick(TObject *Sender)
{
    if(CB_PNo->ItemIndex == 1) {
        Edit_PRangeL->Text = "0";
        Edit_PRangeH->Text = "12000";
        Edit_PCalHigh->Text = "3568";
        Edit_PCalLow->Text = "724";
    }
    else {
        Edit_PRangeL->Text = "0";
        Edit_PRangeH->Text = "12000";
        Edit_PCalHigh->Text = "3568";
        Edit_PCalLow->Text = "724";
    }
    CheckBox_PNo->Checked = true;    
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_PReadClick(TObject *Sender)
{
    unsigned char sdata[8];

    if(CB_PNo->ItemIndex < 0) CB_PNo->ItemIndex = 0;
    sdata[0] = 0x01;
    sdata[1] = CB_PNo->ItemIndex;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x138, 8, sdata);
    else
        MainForm->TxData(0xC6, 8, 0x36, CB_PNo->ItemIndex, sdata);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::BitBtn_PCalHighClick(TObject *Sender)
{
    Edit_PCalHigh->Text = Edit_PCalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::BitBtn_PCalLowClick(TObject *Sender)
{
    Edit_PCalLow->Text = Edit_PCalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_PWriteClick(TObject *Sender)
{
    double dtmp;
    unsigned short stmp;
    unsigned char sdata[8], ctmp;

    if(CB_PNo->ItemIndex < 0) CB_PNo->ItemIndex = 0;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        sdata[0] = CB_PNo->ItemIndex;
        if(CheckBox_PNo->Checked==true)
                sdata[1] = 0x01;
        else
                sdata[1] = 0x00;
        sdata[2] = 0x00;
        sdata[3] = 0x00;
        stmp = (unsigned short)StrToInt(Edit_PRangeL->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_PRangeH->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x139, 8, sdata);
        Sleep(100);

        if(CB_PNo->ItemIndex < 0) CB_PNo->ItemIndex = 0;
        sdata[0] = CB_PNo->ItemIndex;
        sdata[1] = 0x00;
        sdata[2] = 0x00;
        sdata[3] = 0x00;
        stmp = (unsigned short)StrToInt(Edit_PCalLow->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_PCalHigh->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x13A, 8, sdata);
        Sleep(100);
    }
    else {
        if(CheckBox_PNo->Checked==true)
                sdata[0] = 0x01;
        else
                sdata[0] = 0x00;
        stmp = (unsigned short)StrToInt(Edit_PRangeL->Text);
        sdata[1] = stmp & 0xff;
        sdata[2] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_PRangeH->Text);
        sdata[3] = stmp & 0xff;
        sdata[4] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_PCalLow->Text);
        sdata[5] = stmp & 0xff;
        sdata[6] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(Edit_PCalHigh->Text);
        sdata[7] = stmp & 0xff;
        sdata[8] = (stmp>>8) & 0xff;
        MainForm->TxData(0xC6, 9, 0x37, CB_PNo->ItemIndex, sdata);
    }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sBitBtn_PADCReadClick(TObject *Sender)
{
    unsigned char sdata[8], Btn_st;

    if(!MainForm->ApdComPort1->Open) {
        ShowMessage("Please Open the COM Port First.");
        Timer1->Enabled = false;
        return;
    }

    if(sBitBtn_PADCRead->Down == true) {
        sBitBtn_PADCRead->Down = false;
        Btn_st = 0;
        sBitBtn_PADCRead->Caption = "ADC";
    }
    else {
        sBitBtn_PADCRead->Down = true;
        Btn_st = 1;
        sBitBtn_PADCRead->Caption = "Reading..";
    }
    if(CB_PNo->ItemIndex < 0) CB_PNo->ItemIndex = 0;
    sdata[0] = 0x00;
    sdata[1] = CB_PNo->ItemIndex;
    sdata[2] = Btn_st;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;
    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x138, 8, sdata);
    else {
        if(Btn_st)
            Timer1->Enabled = true;
        else
            Timer1->Enabled = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sBitBtn_TADCReadClick(TObject *Sender)
{
    unsigned char sdata[8], Btn_st;

    if(!MainForm->ApdComPort1->Open) {
        ShowMessage("Please Open the COM Port First.");
        Timer1->Enabled = false;
        return;
    }

    if(sBitBtn_TADCRead->Down == true) {
        sBitBtn_TADCRead->Down = false;
        Btn_st = 0;
        sBitBtn_TADCRead->Caption = "ADC";
    }
    else {
        sBitBtn_TADCRead->Down = true;
        Btn_st = 1;
        sBitBtn_TADCRead->Caption = "Reading..";
    }
    if(CB_TNo->ItemIndex < 0) CB_TNo->ItemIndex = 0;
    sdata[0] = 0x00;
    sdata[1] = CB_TNo->ItemIndex;
    sdata[2] = Btn_st;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;
    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x134, 8, sdata);
    else {
        if(Btn_st)
            Timer1->Enabled = true;
        else
            Timer1->Enabled = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::BitBtn_TCalHighClick(TObject *Sender)
{
    Edit_TCalHigh->Text = Edit_TCalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::BitBtn_TCalLowClick(TObject *Sender)
{
    Edit_TCalLow->Text = Edit_TCalADC->Text;        
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_dTressClick(TObject *Sender)
{
    if(CB_TNo->ItemIndex == 1) {
        Edit_TRangeL->Text = "0";
        Edit_TRangeH->Text = "720";
        Edit_TCalHigh->Text = "3858";
        Edit_TCalLow->Text = "296";
    }
    else {
        Edit_TRangeL->Text = "0";
        Edit_TRangeH->Text = "720";
        Edit_TCalHigh->Text = "3858";
        Edit_TCalLow->Text = "296";
    }
    CheckBox_TNo->Checked = true;
}
//---------------------------------------------------------------------------



void __fastcall TCalibForm::sButton_NOxSetClick(TObject *Sender)
{
    double dtmp;
    unsigned short stmp;
    unsigned char sdata[8], ctmp;

    if(CB_NOx->ItemIndex < 0) CB_NOx->ItemIndex = 0;
    sdata[0] = CB_NOx->ItemIndex;
    if(CheckBox_NOx->Checked==true)
        sdata[1] = 0x01;
    else
        sdata[1] = 0x00;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x13D, 8, sdata);
    else
        MainForm->TxData(0xC6, 8, 0x3A, CB_NOx->ItemIndex, sdata);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_NOxReadClick(TObject *Sender)
{
    unsigned char sdata[8];

    if(CB_NOx->ItemIndex < 0) CB_NOx->ItemIndex = 0;
    sdata[0] = CB_NOx->ItemIndex;
    sdata[1] = 0x00;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;
    if(MainForm->sComboBox_Device->ItemIndex == 0)
        MainForm->TranceiveDataProcess(0x13C, 8, sdata);
    else
        MainForm->TxData(0xC6, 8, 0x39, CB_NOx->ItemIndex, sdata);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::FormShow(TObject *Sender)
{
   CMAF[0]  = Edit_MAF00; CMAF[1]  = Edit_MAF01; CMAF[2]  = Edit_MAF02; CMAF[3]  = Edit_MAF03; CMAF[4]  = Edit_MAF04;
   CMAF[5]  = Edit_MAF05; CMAF[6]  = Edit_MAF06; CMAF[7]  = Edit_MAF07; CMAF[8]  = Edit_MAF08; CMAF[9]  = Edit_MAF09;
   CMAF[10] = Edit_MAF10; CMAF[11] = Edit_MAF11; CMAF[12] = Edit_MAF12; CMAF[13] = Edit_MAF13; CMAF[14] = Edit_MAF14;
   CMAF[15] = Edit_MAF15; CMAF[16] = Edit_MAF16; CMAF[17] = Edit_MAF17; CMAF[18] = Edit_MAF18; CMAF[19] = Edit_MAF19;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_CDefaultClick(TObject *Sender)
{
   unsigned short xtmp;
   unsigned short ytmp;
   int i;

   CMAF[0]->Text = 927;
   CMAF[1]->Text = 1236;
   CMAF[2]->Text = 1545;
   CMAF[3]->Text = 1854;
   CMAF[4]->Text = 2163;
   CMAF[5]->Text = 2472;
   CMAF[6]->Text = 2781;
   CMAF[7]->Text = 3090;
   CMAF[8]->Text = 3399;
   CMAF[9]->Text = 3708;

   CMAF[10]->Text = 30;
   CMAF[11]->Text = 92;
   CMAF[12]->Text = 182;
   CMAF[13]->Text = 315;
   CMAF[14]->Text = 509;
   CMAF[15]->Text = 787;
   CMAF[16]->Text = 1181;
   CMAF[17]->Text = 1728;
   CMAF[18]->Text = 2478;
   CMAF[19]->Text = 3488;

    Series_Maf->Clear();
    for(i = 0; i < 10; i++) {
      xtmp = StrToInt(CMAF[i]->Text);
      ytmp = StrToInt(CMAF[i+10]->Text);
      Series_Maf->AddXY(ytmp, xtmp, "", clRed);
    }
}
//---------------------------------------------------------------------------


void __fastcall TCalibForm::sButton_CReadClick(TObject *Sender)
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
        MainForm->TranceiveDataProcess(0x150, 8, sdata);
    else
        MainForm->TxData(0xC6, 8, 0x3B, 0, sdata);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_CSetClick(TObject *Sender)
{
    double dtmp;
    unsigned short stmp;
    unsigned char sdata[128];
    char ctmp;

    unsigned short xtmp;
    unsigned short ytmp;
    int i;

    Series_Maf->Clear();
    for(i = 0; i < 10; i++) {
      xtmp = StrToInt(CMAF[i]->Text);
      ytmp = StrToInt(CMAF[i+10]->Text);
      Series_Maf->AddXY(ytmp, xtmp, "", clRed);
    }

    for(int i=0; i<20; i++) {
      if(CMAF[i]->Text == "")
        return;
    }
    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        stmp = (unsigned short)StrToInt(CMAF[0]->Text);
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[1]->Text);
        sdata[2] = stmp & 0xff;
        sdata[3] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[2]->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[3]->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x151, 8, sdata);
        Sleep(100);

        stmp = (unsigned short)StrToInt(CMAF[4]->Text);
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[5]->Text);
        sdata[2] = stmp & 0xff;
        sdata[3] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[6]->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[7]->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x152, 8, sdata);
        Sleep(100);

        stmp = (unsigned short)StrToInt(CMAF[8]->Text);
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[9]->Text);
        sdata[2] = stmp & 0xff;
        sdata[3] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[10]->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[11]->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x153, 8, sdata);
        Sleep(100);

        stmp = (unsigned short)StrToInt(CMAF[12]->Text);
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[13]->Text);
        sdata[2] = stmp & 0xff;
        sdata[3] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[14]->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[15]->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x154, 8, sdata);
        Sleep(100);

        stmp = (unsigned short)StrToInt(CMAF[16]->Text);
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[17]->Text);
        sdata[2] = stmp & 0xff;
        sdata[3] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[18]->Text);
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        stmp = (unsigned short)StrToInt(CMAF[19]->Text);
        sdata[6] = stmp & 0xff;
        sdata[7] = (stmp>>8) & 0xff;
        MainForm->TranceiveDataProcess(0x155, 8, sdata);
        Sleep(100);
    }
    else {
        for(int i=0; i<20; i++) {
          stmp = (unsigned short)StrToInt(CMAF[i]->Text);
          sdata[(i*2)] = stmp & 0xff;
          sdata[(i*2)+1] = (stmp>>8) & 0xff;
        }
        MainForm->TxData(0xC6, 40, 0x3C, 0, sdata);
    }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_PCReadClick(TObject *Sender)
{
    AnsiString str;
    int v[20];

    Memo->Clear();
    if(OpenDialog->Execute()) {
      Memo->Lines->LoadFromFile(OpenDialog->FileName);

      str = Memo->Lines->Strings[0];
      sscanf(str.c_str(),"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
        &v[0],&v[1],&v[2],&v[3],&v[4],&v[5],&v[6],&v[7],&v[8],&v[9],&v[10],&v[11],&v[12],&v[13],&v[14],&v[15],&v[16],&v[17],&v[18],&v[19]);
      for(int i=0; i<20; i++) {
          CMAF[i]->Text = IntToStr(v[i]);
      }
    }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sButton_PCWriteClick(TObject *Sender)
{
    AnsiString str;

    Memo->Clear();
    str = "";
    for(int i=0; i<20; i++)
      str = str + CMAF[i]->Text + "\t";
    Memo->Lines->Add(str);

    if(SaveDialog->Execute()) {
        Memo->Lines->SaveToFile(SaveDialog->FileName);
    }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Button_DrawClick(TObject *Sender)
{
   unsigned short xtmp;
   unsigned short ytmp;
   int i;
   
    Series_Maf->Clear();
    for(i = 0; i < 10; i++) {
      xtmp = StrToInt(CMAF[i]->Text);
      ytmp = StrToInt(CMAF[i+10]->Text);
      Series_Maf->AddXY(ytmp, xtmp, "", clRed);
    }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Timer1Timer(TObject *Sender)
{
    unsigned char sdata[8], Btn_st;

    sdata[0] = 0x00;
    sdata[1] = Btn_st;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

   if(sBitBtn_MAFADCRead->Down == true) {
        MainForm->TxData(0xC6, 8, 0x32, 0, sdata);   
   }
   if(sBitBtn_TADCRead->Down == true) {
        MainForm->TxData(0xC6, 8, 0x35, CB_TNo->ItemIndex, sdata);
   }
   if(sBitBtn_PADCRead->Down == true) {
        MainForm->TxData(0xC6, 8, 0x38, CB_PNo->ItemIndex, sdata);
   }
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sBitBtn_MAF1VClick(TObject *Sender)
{
    double dtmp;
    AnsiString str;

    Edit_ADC1->Text = Edit_CalADC->Text;
    dtmp = 1.00 - StrToFloat(Edit_CalOffset->Text);
    CalibForm->Edit_Offset1->Text = str.FormatFloat("0.00",dtmp);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sBitBtn_MAF2VClick(TObject *Sender)
{
    double dtmp;
    AnsiString str;

    Edit_ADC2->Text = Edit_CalADC->Text;
    dtmp = 2.00 - StrToFloat(Edit_CalOffset->Text);
    CalibForm->Edit_Offset2->Text = str.FormatFloat("0.00",dtmp);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sBitBtn_MAF3VClick(TObject *Sender)
{
    double dtmp;
    AnsiString str;

    Edit_ADC3->Text = Edit_CalADC->Text;
    dtmp = 3.00 - StrToFloat(Edit_CalOffset->Text);
    CalibForm->Edit_Offset3->Text = str.FormatFloat("0.00",dtmp);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sBitBtn_MAF4VClick(TObject *Sender)
{
    double dtmp;
    AnsiString str;

    Edit_ADC4->Text = Edit_CalADC->Text;
    dtmp = 4.00 - StrToFloat(Edit_CalOffset->Text);
    CalibForm->Edit_Offset4->Text = str.FormatFloat("0.00",dtmp);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::sBitBtn_MAF45VClick(TObject *Sender)
{
    double dtmp;
    AnsiString str;
    
    Edit_ADC5->Text = Edit_CalADC->Text;
    dtmp = 4.50 - StrToFloat(Edit_CalOffset->Text);
    CalibForm->Edit_Offset5->Text = str.FormatFloat("0.00",dtmp);
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF00DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF00->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF01DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF01->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF02DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF02->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF03DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF03->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF04DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF04->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF05DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF05->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF06DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF06->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF07DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF07->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF08DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF08->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCalibForm::Edit_MAF09DblClick(TObject *Sender)
{
    if(sBitBtn_MAFADCRead->Down == true)
      Edit_MAF09->Text = Edit_CalADC->Text;
}
//---------------------------------------------------------------------------

