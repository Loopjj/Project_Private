//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFactor.h"
#include "TMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sPanel"
#pragma link "sLabel"
#pragma resource "*.dfm"
TFactorForm *FactorForm;
//---------------------------------------------------------------------------
__fastcall TFactorForm::TFactorForm(TComponent* Owner)
        : TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFactorForm::sButton_PidReadClick(TObject *Sender)
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
        MainForm->TranceiveDataProcess(0x120,8, sdata);
    }
    else {
        MainForm->TxCmd(0xC6, 0x2A, 1);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFactorForm::sButton_PidWriteClick(TObject *Sender)
{
    double dtmp;
    long ltmp;
    unsigned short stmp;
    unsigned char sdata[8];
    stFactor ScrFactor;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        dtmp = StrToFloat(Edit_Ka->Text)*100.0;     // Ka
        ltmp = (long)dtmp;
        sdata[0] = ltmp & 0xff;
        sdata[1] = (ltmp>>8) & 0xff;
        sdata[2] = (ltmp>>16) & 0xff;
        sdata[3] = (ltmp>>24) & 0xff;
        ltmp = (long)StrToInt(Edit_Kd->Text);       // Kd
        sdata[4] = ltmp & 0xff;
        sdata[5] = (ltmp>>8) & 0xff;
        sdata[6] = (ltmp>>16) & 0xff;
        sdata[7] = (ltmp>>24) & 0xff;
        MainForm->TranceiveDataProcess(0x121, 8, sdata);
        Sleep(100);

        ltmp = (long)StrToInt(Edit_Kr->Text);       // Kr
        sdata[0] = ltmp & 0xff;
        sdata[1] = (ltmp>>8) & 0xff;
        sdata[2] = (ltmp>>16) & 0xff;
        sdata[3] = (ltmp>>24) & 0xff;
        dtmp = (StrToFloat(Edit_OFA->Text)*100.0);    // OFA
        stmp = (unsigned short)dtmp;
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        sdata[6] = 0x00;
        sdata[7] = 0x00;
        MainForm->TranceiveDataProcess(0x122, 8, sdata);
        Sleep(100);

        dtmp = StrToFloat(Edit_OmegaS1->Text)*10.0;  // Omega S1
        ltmp = (long)dtmp;
        sdata[0] = ltmp & 0xff;
        sdata[1] = (ltmp>>8) & 0xff;
        sdata[2] = (ltmp>>16) & 0xff;
        sdata[3] = (ltmp>>24) & 0xff;
        dtmp = StrToFloat(Edit_OmegaS2->Text)*1000.0;// Omega S2
        stmp = (unsigned short)dtmp;
        sdata[4] = stmp & 0xff;
        sdata[5] = (stmp>>8) & 0xff;
        sdata[6] = 0;
        sdata[7] = 0;
        MainForm->TranceiveDataProcess(0x123, 8, sdata);
        Sleep(100);

        dtmp = StrToFloat(Edit_Length->Text)*1000.0;     // Length
        stmp = (unsigned short)dtmp;
        sdata[0] = stmp & 0xff;
        sdata[1] = (stmp>>8) & 0xff;
        dtmp = (StrToFloat(Edit_Xsp->Text)*100.0);       // Xsp : ¸ñÇ¥ ÈíÀå
        stmp = (unsigned short)dtmp;
        sdata[2] = stmp & 0xff;
        dtmp = (StrToFloat(Edit_Coeff->Text)*100.0);     // coeff : NOx ¼¾¼­ °è¼ö
        stmp = (unsigned short)dtmp;
        sdata[3] = stmp & 0xff;
        dtmp = StrToFloat(Edit_ScrArea->Text)*100000.0;  // Scr Area
        ltmp = (long)dtmp;
        sdata[4] = ltmp & 0xff;
        sdata[5] = (ltmp>>8) & 0xff;
        sdata[6] = (ltmp>>16) & 0xff;
        sdata[7] = (ltmp>>24) & 0xff;
        MainForm->TranceiveDataProcess(0x124, 8, sdata);
        Sleep(100);

        dtmp = (StrToFloat(Edit_Kl->Text)*1000000.0);   // Kl : °üÃø±â ÀÌµæ
        ltmp = (long)dtmp;
        sdata[0] = ltmp & 0xff;
        sdata[1] = (ltmp>>8) & 0xff;
        sdata[2] = (ltmp>>16) & 0xff;
        sdata[3] = (ltmp>>24) & 0xff;
        dtmp = (StrToFloat(Edit_Kp->Text)*100000.0);    // Kp : ÈíÀåÁ¦¾î ÀÌµæ
        ltmp = (long)dtmp;
        sdata[4] = ltmp & 0xff;
        sdata[5] = (ltmp>>8) & 0xff;
        sdata[6] = (ltmp>>16) & 0xff;
        sdata[7] = (ltmp>>24) & 0xff;
        MainForm->TranceiveDataProcess(0x125, 8, sdata);
        Sleep(100);
    }
    else {
        dtmp = StrToFloat(Edit_Ka->Text)*100.0;     // Ka
        ScrFactor.Ka = (long)dtmp;
        ScrFactor.Kd = (long)StrToInt(Edit_Kd->Text);       // Kd
        ScrFactor.Kr = (long)StrToInt(Edit_Kr->Text);       // Kr
        dtmp = (StrToFloat(Edit_OFA->Text)*100.0);    // OFA
        ScrFactor.OFA = (unsigned short)dtmp;
        dtmp = StrToFloat(Edit_OmegaS1->Text)*10.0;  // Omega S1
        ScrFactor.S1 = (long)dtmp;
        dtmp = StrToFloat(Edit_OmegaS2->Text)*1000.0;// Omega S2
        ScrFactor.S2 = (unsigned short)dtmp;
        dtmp = StrToFloat(Edit_Length->Text)*1000.0;     // Length
        ScrFactor.L = (unsigned short)dtmp;
        dtmp = (StrToFloat(Edit_Xsp->Text)*100.0);       // Xsp : ¸ñÇ¥ ÈíÀå
        ScrFactor.Xsp = (unsigned short)dtmp;
        dtmp = (StrToFloat(Edit_Coeff->Text)*100.0);     // coeff : NOx ¼¾¼­ °è¼ö
        ScrFactor.Coeff = (unsigned short)dtmp;
        dtmp = StrToFloat(Edit_ScrArea->Text)*100000.0;  // Scr Area
        ScrFactor.ScrArea = (long)dtmp;
        dtmp = (StrToFloat(Edit_Kl->Text)*1000000.0);   // Kl : °üÃø±â ÀÌµæ
        ScrFactor.Kl = (long)dtmp;
        dtmp = (StrToFloat(Edit_Kp->Text)*100000.0);    // Kp : ÈíÀåÁ¦¾î ÀÌµæ
        ScrFactor.Kp = (long)dtmp;
        MainForm->TxData(0xC6, sizeof(stFactor), 0x2B, 0, (unsigned char*)&ScrFactor);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFactorForm::sButton_PidDefaultClick(TObject *Sender)
{
    Edit_Ka->Text = "80.00";
    Edit_Kd->Text = "5";
    Edit_Kr->Text = "150";
    Edit_OFA->Text = "1.00";
    Edit_OmegaS1->Text = "60.0";
    Edit_OmegaS2->Text = "0.000";
    Edit_Length->Text = "0.152";
    Edit_ScrArea->Text = "0.05586";
    Edit_Xsp->Text = "0.10";
    Edit_Coeff->Text = "0.77";
    Edit_Kl->Text = "0.002000";
    Edit_Kp->Text = "0.00050";
}
//---------------------------------------------------------------------------

void __fastcall TFactorForm::sButton_EndClick(TObject *Sender)
{
    Close();        
}
//---------------------------------------------------------------------------



