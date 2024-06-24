//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TPassword.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPassForm *PassForm;
//---------------------------------------------------------------------------
__fastcall TPassForm::TPassForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPassForm::BitBtn_OKClick(TObject *Sender)
{
        mPassword = EditPasswrod->Text;
        if((mPassword == "326649") || (mPassword == "10654"))
                MainForm->Show();
        else {
                mPassword = "";
                EditPasswrod->Text = "";
        }        
}
//---------------------------------------------------------------------------
void __fastcall TPassForm::BitBtn_ExitClick(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TPassForm::Button_CPassClick(TObject *Sender)
{
        AnsiString pass;

        if(MaskEdit_CPass1->Text == MaskEdit_CPass2->Text) {
                Memo1->Clear();
                Memo1->Lines->Add(MaskEdit_CPass1->Text);
                Memo1->Lines->SaveToFile("filename");

                Memo1->Clear();
                Memo1->Lines->LoadFromFile("filename");
                pass = Memo1->Lines->Strings[0];
        }
}
//---------------------------------------------------------------------------
void __fastcall TPassForm::EditPasswrodKeyPress(TObject *Sender, char &Key)
{
        if(Key == '\r') {
                mPassword = EditPasswrod->Text;
                if((mPassword == "326649") || (mPassword == "10654"))
                        MainForm->Show();
                else {
                        mPassword = "";
                        EditPasswrod->Text = "";
                }
        }
}
//---------------------------------------------------------------------------


