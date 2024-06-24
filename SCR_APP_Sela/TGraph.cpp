//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TGraph.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sPanel"
#pragma link "sButton"
#pragma resource "*.dfm"
TGraphForm *GraphForm;
//---------------------------------------------------------------------------
__fastcall TGraphForm::TGraphForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::FormShow(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::FormClose(TObject *Sender, TCloseAction &Action)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::GraphTimerTimer(TObject *Sender)
{
    Series_T1->AddY(MainForm->m_uiTVal[0],"",Panel_T1->Color);
    Series_T2->AddY(MainForm->m_uiTVal[1],"",Panel_T2->Color);
    Series_T3->AddY(MainForm->m_uiTVal[2],"",Panel_T3->Color);
    Series_NOxIn->AddY(MainForm->m_uiNOxIn,"",Panel_NI->Color);
    Series_NOxOut->AddY(MainForm->m_uiNOxOut,"",Panel_NO->Color);
    Series_DOSING->AddY(MainForm->m_dDosing,"",Panel_FPD->Color);
    Series_Supply->AddY(MainForm->m_uiPVal,"",Panel_SP->Color);
    Series_MAF->AddY(MainForm->m_uiMAF,"",Panel_MAF->Color);

    if(RG_Left->ItemIndex == 1 || RG_Right->ItemIndex == 1) {
        if(CB_T1->Checked)
          Series_T1->Active = true;
        else  Series_T1->Active = false;

        if(CB_T2->Checked)
          Series_T2->Active = true;
        else  Series_T2->Active = false;

        if(CB_T3->Checked)
          Series_T3->Active = true;
        else  Series_T3->Active = false;
    }
    if(RG_Left->ItemIndex == 2 || RG_Right->ItemIndex == 2) {
        if(CB_NI->Checked)
          Series_NOxIn->Active = true;
        else  Series_NOxIn->Active = false;

        if(CB_NO->Checked)
          Series_NOxOut->Active = true;
        else  Series_NOxOut->Active = false;
    }
    if(RG_Left->ItemIndex == 3 || RG_Right->ItemIndex == 3) {
        if(CB_FPD->Checked)
          Series_DOSING->Active = true;
        else Series_DOSING->Active = false;

        if(CB_SP->Checked)
          Series_Supply->Active = true;
        else  Series_Supply->Active = false;

    }
    if(RG_Left->ItemIndex == 4 || RG_Right->ItemIndex == 4) {
        if(CB_MAF->Checked)
          Series_MAF->Active = true;
        else  Series_MAF->Active = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::Button_ApplyLeftClick(TObject *Sender)
{
    Chart->LeftAxis->Automatic = CB_AutoLeft->Checked;
    if(!CB_AutoLeft->Checked) {
        Chart->LeftAxis->Minimum = StrToInt(Edit_MinLeft->Text);
        Chart->LeftAxis->Maximum = StrToInt(Edit_MaxLeft->Text);
    }
    if(RG_Left->ItemIndex == 1) {
        Series_T1->VertAxis = aLeftAxis;
        Series_T2->VertAxis = aLeftAxis;
        Series_T3->VertAxis = aLeftAxis;
    }
    else if(RG_Left->ItemIndex == 2) {
        Series_NOxIn->VertAxis = aLeftAxis;
        Series_NOxOut->VertAxis = aLeftAxis;
    }
    else if(RG_Left->ItemIndex == 3) {
        Series_DOSING->VertAxis = aLeftAxis;
        Series_Supply->VertAxis = aLeftAxis;
    }
    else if(RG_Left->ItemIndex == 4) {
        Series_MAF->VertAxis = aLeftAxis;
    }
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::Button_ApplyRightClick(TObject *Sender)
{
    Chart->RightAxis->Automatic = CB_AutoRight->Checked;
    if(!CB_AutoRight->Checked) {
        Chart->RightAxis->Minimum = StrToInt(Edit_MinRight->Text);
        Chart->RightAxis->Maximum = StrToInt(Edit_MaxRight->Text);
    }
    if(RG_Right->ItemIndex == 1) {
        Series_T1->VertAxis = aRightAxis;
        Series_T2->VertAxis = aRightAxis;
        Series_T3->VertAxis = aRightAxis;
    }
    else if(RG_Right->ItemIndex == 2) {
        Series_NOxIn->VertAxis = aRightAxis;
        Series_NOxOut->VertAxis = aRightAxis;
    }
    else if(RG_Right->ItemIndex == 3) {
        Series_DOSING->VertAxis = aRightAxis;
        Series_Supply->VertAxis = aRightAxis;
    }
    else if(RG_Right->ItemIndex == 4) {
        Series_MAF->VertAxis = aRightAxis;
    }
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::CB_3DClick(TObject *Sender)
{
    Chart->View3D = CB_3D->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TGraphForm::CB_T1Click(TObject *Sender)
{
   if( CB_T1->Checked == True ) {
      if( ColorDialog1->Execute() )
          Panel_T1->Color = ColorDialog1->Color;
   }
   else {
      Panel_T1->Color = clBtnFace;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::CB_T2Click(TObject *Sender)
{
   if( CB_T2->Checked == True ) {
      if( ColorDialog1->Execute() )
          Panel_T2->Color = ColorDialog1->Color;
   }
   else {
      Panel_T2->Color = clBtnFace;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::CB_T3Click(TObject *Sender)
{
   if( CB_T3->Checked == True ) {
      if( ColorDialog1->Execute() )
          Panel_T3->Color = ColorDialog1->Color;
   }
   else {
      Panel_T3->Color = clBtnFace;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::CB_NIClick(TObject *Sender)
{
   if( CB_NI->Checked == True ) {
      if( ColorDialog1->Execute() )
          Panel_NI->Color = ColorDialog1->Color;
   }
   else {
      Panel_NI->Color = clBtnFace;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::CB_NOClick(TObject *Sender)
{
   if( CB_NO->Checked == True ) {
      if( ColorDialog1->Execute() )
          Panel_NO->Color = ColorDialog1->Color;
   }
   else {
      Panel_NO->Color = clBtnFace;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::CB_FPDClick(TObject *Sender)
{
   if( CB_FPD->Checked == True ) {
      if( ColorDialog1->Execute() )
          Panel_FPD->Color = ColorDialog1->Color;
   }
   else {
      Panel_FPD->Color = clBtnFace;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::CB_SPClick(TObject *Sender)
{
   if( CB_SP->Checked == True ) {
      if( ColorDialog1->Execute() )
          Panel_SP->Color = ColorDialog1->Color;
   }
   else {
      Panel_SP->Color = clBtnFace;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::CB_MAFClick(TObject *Sender)
{
   if( CB_MAF->Checked == True ) {
      if( ColorDialog1->Execute() )
          Panel_MAF->Color = ColorDialog1->Color;
   }
   else {
      Panel_MAF->Color = clBtnFace;
   }
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::sButton_StartClick(TObject *Sender)
{
    GraphTimer->Enabled = true;
    sButton_Start->Down = true;
    sButton_Stop->Down = false;
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::sButton_StopClick(TObject *Sender)
{
    GraphTimer->Enabled = false;
    sButton_Start->Down = false;
    sButton_Stop->Down = true;
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::sButton_ClearClick(TObject *Sender)
{
    Series_T1->Clear();
    Series_T2->Clear();
    Series_T3->Clear();
    Series_NOxIn->Clear();
    Series_NOxOut->Clear();
    Series_DOSING->Clear();
    Series_Supply->Clear();
    Series_MAF->Clear();        
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::sButton_SaveClick(TObject *Sender)
{
    if(SaveDialog->Execute()) {
        Chart->SaveToBitmapFile(SaveDialog->FileName);
    }        
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::sButton_PrintClick(TObject *Sender)
{
    if(PrintDialog->Execute()) {
        Chart->PrintLandscape();
    }        
}
//---------------------------------------------------------------------------

void __fastcall TGraphForm::sButton_EndClick(TObject *Sender)
{
    Close();        
}
//---------------------------------------------------------------------------

