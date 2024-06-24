//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TProg1.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sGauge"
#pragma resource "*.dfm"
TProg1Form *Prog1Form;
//---------------------------------------------------------------------------
__fastcall TProg1Form::TProg1Form(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TProg1Form::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ProgTimer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TProg1Form::TxFile(void)
{
   int handle,tmp;
   unsigned long i,flength;
   unsigned char ch;

   if(!MainForm->ApdComPort1->Open) {
       ShowMessage("Please Open the COM Port First.");
       return;
   }
   if ((handle = open(m_asFName.c_str(), O_RDONLY | O_BINARY)) == -1)
   {
      Memo->Lines->Add("Error : File Read");
      return;
   }
   flength = filelength(handle);
   m_ucFData = new unsigned char[flength];
   read(handle, m_ucFData, flength);

   for(i = 0; i < flength; i++) {
      if(m_bOKflag) break;  
      MainForm->ApdComPort1->PutChar(m_ucFData[i]);
      if(i%200 == 0)  {
        sGauge->Progress = i*100/flength;
      }
      Application->ProcessMessages();
      Sleep(1);
   }

   close(handle);
   delete m_ucFData;
}
//---------------------------------------------------------------------------

void __fastcall TProg1Form::ProgTimerTimer(TObject *Sender)
{
    if(!MainForm->m_bProgFlag) return;

    if(m_nProgStep == 0)
        MainForm->ApdComPort1->PutChar('U');    //Put 'U'
    else if(m_nProgStep == 2){
        ProgTimer->Enabled = false;
        TxFile();
        m_nProgStep = 3;
    }
}
//---------------------------------------------------------------------------

void __fastcall TProg1Form::FileListBoxClick(TObject *Sender)
{
   bool ok;

   for(int i = 0; i < FileListBox->Count; i++) {
      if(FileListBox->Selected[i]) {
         if(FileListBox->Items->Strings[i] == Edit_Fname->Text) {
           ok = false;
           break;
         }
         else ok = true;
         if(ok) {
            Edit_Fname->Text = FileListBox->Directory+"\\"+FileListBox->Items->Strings[i];
            m_asFName = Edit_Fname->Text;
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TProg1Form::AppException(TObject *Sender, Exception *E)
{

if(E->Message == "I/O error 21")
   ShowMessage("Storage device is not ready!");

}

//---------------------------------------------------------------------------

void __fastcall TProg1Form::FormShow(TObject *Sender)
{
   Memo->Clear();

   m_nProgStep = 0;

   Application->OnException = AppException;
   SetErrorMode(SEM_FAILCRITICALERRORS);

   sGauge->Progress = 0;
   if(FileListBox->Count == 0)
       FileListBox->Items->Add(" ");
}
//---------------------------------------------------------------------------

void __fastcall TProg1Form::sButton_ProgramClick(TObject *Sender)
{
    if(!MainForm->ApdComPort1->Open) {
        ShowMessage("Please Open the COM Port First.");
        return;
    }
    if(m_nProgStep == 0) {
        MainForm->m_bProgFlag = 2;
        //MainForm->TxCmd(0xd7, 0x81, 1);
        MainForm->ApdComPort1->SWFlowOptions = swfBoth;
        ProgTimer->Enabled = true;
        m_bOKflag = 0;
    }
    else
        sButton_Program->Down = true;
}
//---------------------------------------------------------------------------

void __fastcall TProg1Form::sButton_CancelClick(TObject *Sender)
{
   m_bOKflag = 1;
   sButton_Program->Down = false;
   ProgTimer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TProg1Form::sButton_ExitClick(TObject *Sender)
{
  Close();
}

//---------------------------------------------------------------------------

