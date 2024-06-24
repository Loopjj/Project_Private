//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TConvert.h"
//---------------------------------------------------------------------------

#define BUFFER_SIZE     64

#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sPanel"
#pragma link "sGauge"
#pragma resource "*.dfm"
TConvertForm *ConvertForm;

//---------------------------------------------------------------------------

__fastcall TConvertForm::TConvertForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::BitBtn_SelectOneClick(TObject *Sender)
{
   bool ok;

   for(int i = 0; i < FileListBox->Count; i++)
      if(FileListBox->Selected[i]) {
         ok = true;
         for(int j = 0; j < ListBox->Count; j++)
            if(FileListBox->Items->Strings[i] == ListBox->Items->Strings[j]) {
               ok = false;
               break;
            }
         if(ok)
            ListBox->Items->Append(FileListBox->Items->Strings[i]);
      }
   Label_SelCount->Caption = IntToStr(ListBox->Count)+" files selected";        
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::BitBtn_DeSelOneClick(TObject *Sender)
{
   for(int i = 0; i < ListBox->Count; i++) {
      if(ListBox->Selected[i]) {
         ListBox->Items->Delete(i);
         i = -1;
      }
   }
   Label_SelCount->Caption = IntToStr(ListBox->Count)+" files selected";        
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::BitBtn_SellectAllClick(TObject *Sender)
{
   bool ok;

   for(int i = 0; i < FileListBox->Count; i++) {
      ok = true;
      for(int j = 0; j < ListBox->Count; j++)
         if(FileListBox->Items->Strings[i] == ListBox->Items->Strings[j]) {
            ok = false;
            break;
         }
      if(ok)
         ListBox->Items->Append(FileListBox->Items->Strings[i]);
   }
   Label_SelCount->Caption = IntToStr(ListBox->Count)+" files selected";        
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::BitBtn_DSelAllClick(TObject *Sender)
{
   sGauge_Total->Progress = 0;
   sGauge_One->Progress = 0;
   m_bOKFlag = true;
   Analyze();        
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::FileListBoxDblClick(TObject *Sender)
{
   bool ok;

   for(int i = 0; i < FileListBox->Count; i++)
      if(FileListBox->Selected[i]) {
         ok = true;
         for(int j = 0; j < ListBox->Count; j++)
            if(FileListBox->Items->Strings[i] == ListBox->Items->Strings[j]) {
               ok = false;
               break;
            }
         if(ok)
            ListBox->Items->Append(FileListBox->Items->Strings[i]);
      }
   Label_SelCount->Caption = IntToStr(ListBox->Count)+" files selected";
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::ListBoxDblClick(TObject *Sender)
{
   for(int i = 0; i < ListBox->Count; i++) {
      if(ListBox->Selected[i]) {
         ListBox->Items->Delete(i);
         i = -1;
      }
   }
   Label_SelCount->Caption = IntToStr(ListBox->Count)+" files selected";        
}
//---------------------------------------------------------------------------
void __fastcall TConvertForm::FormShow(TObject *Sender)
{
   Application->OnException = AppException;
   SetErrorMode(SEM_FAILCRITICALERRORS);

   sGauge_Total->Progress = 0;
   sGauge_One->Progress = 0;

   ListBox->Items->Clear();
   if(FileListBox->Count == 0)
       FileListBox->Items->Add(" ");
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::mem_strcpy(unsigned char *s1, unsigned char *s2, int len)
{
  int i;
  for (i = 0; i != len; i++){
    *s2++ = *s1++;
  }
}
//---------------------------------------------------------------------------

char __fastcall TConvertForm::mem_check(unsigned char *s1, unsigned char ch, int len)
{
  int i;
  for(i=0;i<len;i++)
  {
    if(*(s1+i)!=ch) {
      return 0;
    }
  }
  return 1;
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::Save_data(unsigned char *ReadBuf)
{
  data_t *p_Data;

  p_Data = (data_t *)ReadBuf;

  fprintf(fout,"\n");
  fprintf(fout,"%02d.%02d.%02d\t",
      	CRTC.tm_year, CRTC.tm_mon, CRTC.tm_mday);
  fprintf(fout,"%02d:%02d:%02d\t",
        CRTC.tm_hour, CRTC.tm_min, CRTC.tm_sec);
         // Step, RStart, RTime, T1, T2, T3, T4, P1, P2, PeakP1, PeakP2
  fprintf(fout,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",
        p_Data->f_REG,p_Data->Sts.REGStart,p_Data->t_REG,p_Data->n_REG,p_Data->T1,p_Data->T2,p_Data->T3,p_Data->T4,p_Data->P1,p_Data->P2,p_Data->peakP1,p_Data->peakP2);
         // V, IGC, IGD, IGRly, FPD, FSol, FPRly, CPRly, ENG
  fprintf(fout,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t",
        p_Data->BV,p_Data->IHC,p_Data->Sig.IGNRelay,p_Data->FPD,p_Data->Sig.FPRelay,p_Data->Sig.COMPRelay,p_Data->Sig.Ready);
         // DrvH, DrvM, DrvS, Error, Check
  fprintf(fout,"%d\t%d\t%d\t%d\t%d\t",
        p_Data->Time.tm_hour,p_Data->Time.tm_min,p_Data->Time.tm_sec,p_Data->Error.B,p_Data->Check.B);
 	  // GPS Data Save
  fprintf(fout,"%04d.%04d\t", p_Data->GPSdata.Latitude._Word._High, p_Data->GPSdata.Latitude._Word._Low);
  fprintf(fout,"%05d.%04d\t", p_Data->GPSdata.Longitude._Word._High, p_Data->GPSdata.Longitude._Word._Low);
  fprintf(fout,"%02d.%01d\t", p_Data->GPSdata.Altitude._Word._High, p_Data->GPSdata.Altitude._Word._Low);
  fprintf(fout,"%d", p_Data->GPSdata.Speed);
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::Analyze(void)
{
   struct ftime filet;
   struct stat statbuf;

   data_t *p_Data;
   char *readbuf;

   unsigned char FileName[12];
   unsigned char ReadBuf[BUFFER_SIZE];
   double fsize = 0;
   double lfper;
   long filesize = 0;

   for(int i=0; i<ListBox->Count; i++)
   {
      m_asFileName[i] = ListBox->Items->Strings[i];
      fin = fopen(m_asFileName[i].c_str(),"rb");
      /* get information about the file */
      fstat(fileno(fin), &statbuf);
      readbuf = new char[statbuf.st_size];
      fread(readbuf, statbuf.st_size, 1, fin);
      fclose(fin);

      while((statbuf.st_size>filesize) && (m_bOKFlag==true))
      {
         mem_strcpy((char*)readbuf+filesize, (char*)ReadBuf, BUFFER_SIZE);
         if(!mem_check(ReadBuf, 0xff, BUFFER_SIZE))
         {
            p_Data = (data_t *)ReadBuf;
            mem_strcpy((unsigned char *)&p_Data->Date, (unsigned char *)&CRTC, sizeof(rtc_time));

            if(PreDay != CRTC.tm_mday)
            {
               if(fout != NULL) fclose(fout);
               for(int j = 0; j <12; j++)
      	          FileName[j] = 0;
	       FileName[0] = (CRTC.tm_year/10) + 0x30;
               FileName[1] = (CRTC.tm_year%10) + 0x30;
               FileName[2] = (CRTC.tm_mon/10) + 0x30;
               FileName[3] = (CRTC.tm_mon%10) + 0x30;
               FileName[4] = (CRTC.tm_mday/10) + 0x30;
               FileName[5] = (CRTC.tm_mday%10) + 0x30;
               FileName[6] = '.';
               if(RadioButton_Txt->Checked)
               {
                  FileName[7] = 't';
                  FileName[8] = 'x';
                  FileName[9] = 't';
               }
               else {
                  FileName[7] = 'x';
                  FileName[8] = 'l';
                  FileName[9] = 's';
               }

               if((fchk = fopen(FileName,"rb")) == NULL)
               {
                  fout = fopen(FileName,"wt");
    		  fprintf(fout,"Date\tTime\tStep\tRStart\tRTime\tRCNT\tT1\tT2\tT3\tT4\tP1\tP2\tPeakP1\tPeakP2\t");
  		  fprintf(fout,"V\tIGC\tIGRly\tFPD\tFPRly\tACRly\tReady\t");
  		  fprintf(fout,"drvH\tdrvM\tdrvS\tError\tCheck\tLatitude\tLongitude\tAltitude\tSpeed");
                  Save_data(ReadBuf);
               }
               else
               {
                 fclose(fchk);
                 fout = fopen(FileName,"at");
               }
               PreDay = CRTC.tm_mday;
            }
            else Save_data(ReadBuf);
         }
         fsize += BUFFER_SIZE;
         filesize += BUFFER_SIZE;
         lfper = (fsize/statbuf.st_size)*100.0;
         sGauge_One->Progress = (int)lfper;
         Application->ProcessMessages();
      } // while feof
      if(fout != NULL) fclose(fout);
      delete[] readbuf;
      sGauge_Total->Progress = 100*(i+1)/ListBox->Count;
   } //for ListBox
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::AppException(TObject *Sender, Exception *E)
{

if(E->Message == "I/O error 21")
   ShowMessage("Storage device is not ready!");

}
void __fastcall TConvertForm::sButton_OKClick(TObject *Sender)
{
   sGauge_Total->Progress = 0;
   sGauge_One->Progress = 0;

   m_bOKFlag = true;
   Analyze();          
}
//---------------------------------------------------------------------------

void __fastcall TConvertForm::sButton_CancelClick(TObject *Sender)
{
   sGauge_Total->Progress = 0;
   sGauge_One->Progress = 0;
   
   if(fout!=NULL)fclose(fout);
   m_bOKFlag = false;        
}
//---------------------------------------------------------------------------
void __fastcall TConvertForm::sButton_ExitClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------


