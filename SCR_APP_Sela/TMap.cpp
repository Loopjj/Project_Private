//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TMap.h"
#include "TMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma resource "*.dfm"
TTMapForm *TMapForm;
//---------------------------------------------------------------------------
__fastcall TTMapForm::TTMapForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
  
void __fastcall TTMapForm::FormCreate(TObject *Sender)
{
    ProgressBar_Map->Position = 0;
    UpDown_Alpha->Max = 150;
    UpDown_Alpha->Min = 0;
    UpDown_Alpha->Increment = 1;
    UpDown_Alpha->Position = (int)(StrToFloat(Edit_Value->Text) * 100.0);
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_ReadMapClick(TObject *Sender)
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
        MainForm->TranceiveDataProcess(0x160, 8, sdata);
    }
    else {
        MainForm->TxCmd(0xC6, 0x2E, 1);
    }
    ProgressBar_Map->Position = 0;
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_WriteMapClick(TObject *Sender)
{
    double dtmp;
    unsigned char sdata[1024];
    int i, j;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        for (i = 0; i < 72; i++) {
                for (j = 0 ; j < 8; j++) {
                        dtmp = StrToFloat(MainForm->Map[i][j]->Text)*100.0;
                        sdata[j] = (unsigned char)dtmp;
                }
                MainForm->TranceiveDataProcess(0x161+i, 8, sdata);
                ProgressBar_Map->Position = i+1;
                Application->ProcessMessages();
        }
    }
    else {
        for (i = 0; i < 72; i++) {
                for (j = 0 ; j < 8; j++) {
                        dtmp = StrToFloat(MainForm->Map[i][j]->Text)*100.0;
                        sdata[(i*8)+j] = (unsigned char)dtmp;
                }
                ProgressBar_Map->Position = i+1;
                Sleep(10);
        }
        MainForm->TxData(0xC6, 72*8, 0x2F, 0, sdata);
    }
}

//---------------------------------------------------------------------------
void __fastcall TTMapForm::sButton_ReadTSVClick(TObject *Sender)
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
        MainForm->TranceiveDataProcess(0x1B0, 8, sdata);
    }
    else {
        MainForm->TxCmd(0xC6, 0x2C, 1);
    }
    ProgressBar_Map->Position = 0;
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_WriteTSVClick(TObject *Sender)
{
    unsigned short stmp;
    unsigned char sdata[96];
    int i, j;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        for (i = 0; i < 6; i++) {
                // Temp Table
                stmp = StrToInt(MainForm->MapT[i][0]->Text);
                sdata[0] = stmp & 0xff;
                sdata[1] = (stmp>>8) & 0xff;
                stmp = StrToInt(MainForm->MapT[i][1]->Text);
                sdata[2] = stmp & 0xff;
                sdata[3] = (stmp>>8) & 0xff;
                stmp = StrToInt(MainForm->MapT[i][2]->Text);
                sdata[4] = stmp & 0xff;
                sdata[5] = (stmp>>8) & 0xff;
                stmp = StrToInt(MainForm->MapT[i][3]->Text);
                sdata[6] = stmp & 0xff;
                sdata[7] = (stmp>>8) & 0xff;
                MainForm->TranceiveDataProcess(0x1B1+i, 8, sdata);
                ProgressBar_Map->Position = (i+1)*6;
                Sleep(10);
                // MAF Table
                stmp = StrToInt(MainForm->MapMAF[i][0]->Text);
                sdata[0] = stmp & 0xff;
                sdata[1] = (stmp>>8) & 0xff;
                stmp = StrToInt(MainForm->MapMAF[i][1]->Text);
                sdata[2] = stmp & 0xff;
                sdata[3] = (stmp>>8) & 0xff;
                stmp = StrToInt(MainForm->MapMAF[i][2]->Text);
                sdata[4] = stmp & 0xff;
                sdata[5] = (stmp>>8) & 0xff;
                stmp = StrToInt(MainForm->MapMAF[i][3]->Text);
                sdata[6] = stmp & 0xff;
                sdata[7] = (stmp>>8) & 0xff;
                MainForm->TranceiveDataProcess(0x1C1+i, 8, sdata);
                ProgressBar_Map->Position = (i+1)*6 + 36;
                Sleep(10);
        }
    }
    else {
        for (i = 0; i < 6; i++) {
                for (j = 0; j < 4; j++) {
                        stmp = StrToInt(MainForm->MapT[i][j]->Text);
                        sdata[(i*8)+(j*2)] = stmp & 0xff;
                        sdata[(i*8)+(j*2)+1] = (stmp>>8) & 0xff;
                        stmp = StrToInt(MainForm->MapMAF[i][j]->Text);
                        sdata[48+(i*8)+(j*2)] = stmp & 0xff;
                        sdata[48+(i*8)+(j*2)+1] = (stmp>>8) & 0xff;
                        ProgressBar_Map->Position = (((i+i)*4) + j) * 3;
                        Sleep(10);
                }
        }
        MainForm->TxData(0xC6, 96, 0x2D, 0, sdata);
    }
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_ReadPCClick(TObject *Sender)
{
    AnsiString str;
    long ltmp[2];
    unsigned short stmp[24];
    unsigned char v[24];
    double dtmp;
    int dev, mod;

    Memo->Clear();
    if(OpenDialog->Execute()) {
        Memo->Lines->LoadFromFile(OpenDialog->FileName);
        Panel_Range1->Visible = false;
        Panel_Range2->Visible = false;
        Panel_Range3->Visible = false;
        Panel_Range4->Visible = false;
        Panel_Range5->Visible = false;
        Panel_Range6->Visible = false;
        Panel_Range7->Visible = false;
        Panel_Range8->Visible = false;
        for(int i = 0; i < 24; i++) {
            str = Memo->Lines->Strings[i];
            sscanf(str.c_str(),"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
               &v[0],&v[1],&v[2],&v[3],&v[4],&v[5],&v[6],&v[7],&v[8],&v[9],&v[10],&v[11],&v[12],&v[13],&v[14],&v[15],&v[16],&v[17],&v[18],&v[19],&v[20],&v[21],&v[22],&v[23]);
            for(int j = 0; j < 24; j++) {
                dtmp = (double)v[j]/100.0;
                dev = j / 8;
                mod = j % 8;
                if(dtmp <= 0.2)                            // 0.2 이하
                  MainForm->Map[i*3 + dev][mod]->Color = clSilver;
                else if(dtmp > 0.2 && dtmp <= 0.6)    // 0.2 ~ 0.6
                  MainForm->Map[i*3 + dev][mod]->Color = 0x0000BB00;
                else if(dtmp > 0.6 && dtmp <= 0.8)    // 0.6 ~ 0.8
                  MainForm->Map[i*3 + dev][mod]->Color = 0x0000EC00;
                else if(dtmp > 0.8 && dtmp <= 0.9)    // 0.8 ~ 0.9
                  MainForm->Map[i*3 + dev][mod]->Color = 0x0028FF94;
                else if(dtmp > 0.9 && dtmp <= 1.0)    // 0.9 ~ 1.0
                  MainForm->Map[i*3 + dev][mod]->Color = 0x0080FFFF;
                else if(dtmp > 1.0 && dtmp <= 1.1)    // 1.0 ~ 1.1
                  MainForm->Map[i*3 + dev][mod]->Color = 0x0000BBFF;
                else if(dtmp > 1.1 && dtmp <= 1.2)    // 1.1 ~ 1.2
                  MainForm->Map[i*3 + dev][mod]->Color = 0x000166FE;
                else                                  // 1.2 이상
                  MainForm->Map[i*3 + dev][mod]->Color = clRed;
                MainForm->Map[i*3 + dev][mod]->Text = str.FormatFloat("0.00",dtmp);
            }
        }
        str = Memo->Lines->Strings[25];
        sscanf(str.c_str(),"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
            &stmp[0],&stmp[1],&stmp[2],&stmp[3],&stmp[4],&stmp[5],&stmp[6],&stmp[7],&stmp[8],&stmp[9],&stmp[10],&stmp[11],&stmp[12],&stmp[13],&stmp[14],&stmp[15],&stmp[16],
            &stmp[17],&stmp[18],&stmp[19],&stmp[20],&stmp[21],&stmp[22],&stmp[23]);
        for(int j = 0; j < 24; j++) {
            dev = j / 4;
            mod = j % 4;
            MainForm->MapT[dev][mod]->Text = stmp[j];
        }

        str = Memo->Lines->Strings[27];
        sscanf(str.c_str(),"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
            &stmp[0],&stmp[1],&stmp[2],&stmp[3],&stmp[4],&stmp[5],&stmp[6],&stmp[7],&stmp[8],&stmp[9],&stmp[10],&stmp[11],&stmp[12],&stmp[13],&stmp[14],&stmp[15],&stmp[16],
            &stmp[17],&stmp[18],&stmp[19],&stmp[20],&stmp[21],&stmp[22],&stmp[23]);
        for(int j = 0; j < 24; j++) {
            dev = j / 4;
            mod = j % 4;
            MainForm->MapMAF[dev][mod]->Text = stmp[j];
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_WritePCClick(TObject *Sender)
{
    AnsiString line, str;

    long ltmp;
    unsigned short stmp;
    unsigned char sdata[8];
    unsigned char data;
    int i, mod, dev;

    Memo->Clear();
    for(int i = 0; i < 24; i++){
        line = "";
        for(int j = 0; j < 24; j++) {
             mod = j % 8;
             dev = j / 8;
            data = (StrToFloat(MainForm->Map[i*3 + dev][mod]->Text)*100.0);
            str = IntToStr(data) + "\t";
            line = line + str;

        }
        Memo->Lines->Add(line);
    }
    line = "====== Temperature range =====";
    Memo->Lines->Add(line);
    line = "";
    for(int i = 0; i < 24; i++){
       mod = i % 4;
       dev = i / 4;
       stmp = StrToInt(MainForm->MapT[dev][mod]->Text);
       str = IntToStr(stmp) + "\t";
       line = line + str;
    }
    Memo->Lines->Add(line);

    line = "========== MAF range =========";
    Memo->Lines->Add(line);
    line = "";
    for(int i = 0; i < 24; i++){
       mod = i % 4;
       dev = i / 4;
       ltmp = StrToFloat(MainForm->MapMAF[dev][mod]->Text);
       str = IntToStr(ltmp) + "\t";
       line = line + str;
    }
    Memo->Lines->Add(line);

    if(SaveDialog->Execute()) {
        Memo->Lines->SaveToFile(SaveDialog->FileName);
    }        
}
//---------------------------------------------------------------------------


void __fastcall TTMapForm::sButton_SetClick(TObject *Sender)
{
   int i, j, start, end, sdev, smod, count;
   double factor;
   AnsiString str;

   factor = StrToFloat(Edit_Value->Text);
   if(factor > 1.50) factor = 1.50;

   if(RadioButton_Col->Checked == true) {
     start = StrToInt(Edit_Start->Text);
     if(start > 23) start = 23;
     start = (start * 3);
     end = StrToInt(Edit_End->Text) + 1;
     if(end > 24) end = 24;
     end = (end * 3);
     for(i=start; i<end; i++) {
        for(j=0; j<8; j++) {
          if(factor <= 0.2)                            // 0.2 이하
            MainForm->Map[i][j]->Color = clSilver;
          else if(factor > 0.2 && factor <= 0.6)    // 0.2 ~ 0.6
            MainForm->Map[i][j]->Color = 0x0000BB00;
          else if(factor > 0.6 && factor <= 0.8)    // 0.6 ~ 0.8
            MainForm->Map[i][j]->Color = 0x0000EC00;
          else if(factor > 0.8 && factor <= 0.9)    // 0.8 ~ 0.9
            MainForm->Map[i][j]->Color = 0x0028FF94;
          else if(factor > 0.9 && factor <= 1.0)    // 0.9 ~ 1.0
            MainForm->Map[i][j]->Color = 0x0080FFFF;
          else if(factor > 1.0 && factor <= 1.1)    // 1.0 ~ 1.1
            MainForm->Map[i][j]->Color = 0x0000BBFF;
          else if(factor > 1.1 && factor <= 1.2)    // 1.1 ~ 1.2
            MainForm->Map[i][j]->Color = 0x000166FE;
          else                                      // 1.2 이상
            MainForm->Map[i][j]->Color = clRed;

           MainForm->Map[i][j]->Text = str.FormatFloat("0.00",factor);
        }
     }
   }
   else {
     start = StrToInt(Edit_Start->Text);
     if(start > 23) start = 23;
     end = StrToInt(Edit_End->Text) + 1;
     if(end > 24) end = 24;
     count = end - start;
     sdev = start / 8;
     smod = start % 8;
     for(i=0; i<count; i++) {
        for(j=0; j<24; j++) {
          if(factor <= 0.2)                            // 0.2 이하
            MainForm->Map[(j*3)+sdev][i+smod]->Color = clSilver;
          else if(factor > 0.2 && factor <= 0.6)    // 0.2 ~ 0.6
            MainForm->Map[(j*3)+sdev][i+smod]->Color = 0x0000BB00;
          else if(factor > 0.6 && factor <= 0.8)    // 0.6 ~ 0.8
            MainForm->Map[(j*3)+sdev][i+smod]->Color = 0x0000EC00;
          else if(factor > 0.8 && factor <= 0.9)    // 0.8 ~ 0.9
            MainForm->Map[(j*3)+sdev][i+smod]->Color = 0x0028FF94;
          else if(factor > 0.9 && factor <= 1.0)    // 0.9 ~ 1.0
            MainForm->Map[(j*3)+sdev][i+smod]->Color = 0x0080FFFF;
          else if(factor > 1.0 && factor <= 1.1)    // 1.0 ~ 1.1
            MainForm->Map[(j*3)+sdev][i+smod]->Color = 0x0000BBFF;
          else if(factor > 1.1 && factor <= 1.2)    // 1.1 ~ 1.2
            MainForm->Map[(j*3)+sdev][i+smod]->Color = 0x000166FE;
          else                                      // 1.2 이상
            MainForm->Map[(j*3)+sdev][i+smod]->Color = clRed;
           MainForm->Map[(j*3)+sdev][i+smod]->Text = str.FormatFloat("0.00",factor);
        }
     }
   }
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_OpenDataClick(TObject *Sender)
{
    AnsiString str;
    unsigned short t1, t2, tavg , maf, reftemp;
    unsigned char x, y, Reduction, Injection, offset;
    double inject, percent;
    short noxin, noxout;
    double ureaconsumption, UreaSum, FuelSum, UreaConsumption, Consumption;
    unsigned int NoxInSum, NoxOutSum, InNoxInSum, InNoxOutSum;

    TStringList *sl = new TStringList;

    if(OpenDialog1->Execute()) {
        for(int i = 0; i < 24; i++) {
          for(int j = 0; j < 24; j++) {
            SumPercent[i][j] = 0;
            Count[i][j] = 0;
            MainForm->Map[i*3+(j/8)][j%8]->Color = clWindow;
          }
        }

        for(int i = 0; i < 24; i++) {
           MainForm->BaseTemp[i] =  StrToInt(MainForm->MapT[i/4][i%4]->Text);
           MainForm->BaseMaf[i] =  StrToInt(MainForm->MapMAF[i/4][i%4]->Text);
        }

        NoxInSum = 0;
        NoxOutSum = 0;
        InNoxInSum = 0;
        InNoxOutSum = 0;
        UreaSum = 0.0;
        x = 0;
        y = 0;
        Panel_Range1->Visible = true;
        Panel_Range2->Visible = true;
        Panel_Range3->Visible = true;
        Panel_Range4->Visible = true;
        Panel_Range5->Visible = true;
        Panel_Range6->Visible = true;
        Panel_Range7->Visible = true;
        Panel_Range8->Visible = true;
        try {
           sl->LoadFromFile(OpenDialog1->FileName);
        }catch(Exception &e) {
           ShowMessage(e.Message);
        }
        TStringList *cl = new TStringList;
        Label_Filename->Caption = OpenDialog1->FileName;
        for(int i = 1; i < sl->Count; i++) {
          cl->CommaText = sl->Strings[i];
          offset = 0;
          noxin   = StrToInt(cl->Strings[2+offset]);     // nox in
          noxout  = StrToInt(cl->Strings[3+offset]);     // nox out
          maf     = StrToInt(cl->Strings[4+offset]);     // maf
          t1      = StrToInt(cl->Strings[5+offset]);     // t1
          t2      = StrToInt(cl->Strings[6+offset]);     // t2
          tavg    = StrToInt(cl->Strings[7+offset]);     // tavg
          inject  = StrToFloat(cl->Strings[8+offset]);   // inject

          if(noxin)
            percent = (1 - (double)noxout / (double)noxin) * 100.0;
          else
            percent =  100.0;

          reftemp = StrToInt(Edit_Temperature->Text);
          if(tavg >= reftemp) {
             NoxInSum += noxin;
             NoxOutSum += noxout;
             if(inject) {
               InNoxInSum += noxin;
               InNoxOutSum += noxout;
             }
          }
          UreaSum += inject;

          if((tavg >= 200 && tavg < 625) && (maf >= 250 && maf < 1000)) {
	    for(int m=0; m<23 ; m++) {
	      if( tavg > MainForm->BaseTemp[23]) {
                x = 23;
                break;
              }
	      else if( tavg <= MainForm->BaseTemp[0]) {
                x = 0;
                break;
              }
	      else if( tavg > MainForm->BaseTemp[m] && tavg <= MainForm->BaseTemp[m+1]) {
	        x = m;
	        break;
	      }
            }
	    for(int n=0; n<23; n++) {
	      if( maf > MainForm->BaseMaf[23]) {
                y = 23;
                break;
              }
	      else if( maf <= MainForm->BaseMaf[0]) {
                y = 0;
                break;
              }
	      else if( maf > MainForm->BaseMaf[n] && maf <= MainForm->BaseMaf[n+1]) {
                y = n;
	        break;
	      }
	    }
            SumPercent[x][y] += percent;
            Count[x][y] += 1;
	  }
	  else {
            x = 0;
            y = 0;
          }
        }
        delete cl;
        //
        Reduction = (unsigned char)((1-(double)NoxOutSum / (double)NoxInSum) * 100);
        Edit_Reduction->Text = IntToStr(Reduction);

        Injection = (unsigned char)((double)InNoxOutSum / (double)InNoxInSum * 100);
        Edit_Injection->Text = IntToStr(Injection);

        ureaconsumption = (double)UreaSum / 3600.0;   // 1초 데이터
        UreaConsumption = (ureaconsumption / 1.09) / 1000.0;
        Edit_UreaConsumption->Text = str.FormatFloat("0.00",UreaConsumption);

        for(int i = 0; i < 24; i++) {
          for(int j = 0; j < 24; j++) {
            if(Count[i][j]) {
              percent = (SumPercent[i][j] / (double)Count[i][j]);
              if(percent < 30.0)                            // 30% 이하
                MainForm->Map[i*3+(j/8)][j%8]->Color = 0x000000FF;
              else if(percent >= 30.0 && percent < 40.0)    // 30% ~ 40%
                MainForm->Map[i*3+(j/8)][j%8]->Color = 0x000166FE;
              else if(percent >= 40.0 && percent < 50.0)    // 40% ~ 50%
                MainForm->Map[i*3+(j/8)][j%8]->Color = 0x0000BBFF;
              else if(percent >= 50.0 && percent < 60.0)    // 50% ~ 60%
                MainForm->Map[i*3+(j/8)][j%8]->Color = 0x0080FFFF;
              else if(percent >= 60.0 && percent < 70.0)    // 60% ~ 70%
                MainForm->Map[i*3+(j/8)][j%8]->Color = 0x0028FF94;
              else if(percent >= 70.0 && percent < 80.0)    // 70% ~ 80%
                MainForm->Map[i*3+(j/8)][j%8]->Color = 0x0000EC00;
              else                                          // 80% 이상
                MainForm->Map[i*3+(j/8)][j%8]->Color = 0x0000BB00;
            }
          }
        }
    }
    delete sl;
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    for(int i = 0; i < 72; i++) {
      for(int j = 0; j < 8; j++) {
        MainForm->Map[i][j]->Color = clWindow;
      }
    }
    Panel_Range1->Visible = false;
    Panel_Range2->Visible = false;
    Panel_Range3->Visible = false;
    Panel_Range4->Visible = false;
    Panel_Range5->Visible = false;
    Panel_Range6->Visible = false;
    Panel_Range7->Visible = false;
    Panel_Range8->Visible = false;
    Label_Filename->Caption = "";

    Edit_UreaConsumption->Text ="";
    Edit_Injection->Text = "";
    Edit_Reduction->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_CloseClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::UpDown_AlphaClick(TObject *Sender,
      TUDBtnType Button)
{
  int factor;
  double dfactor;
  AnsiString str;

  factor = UpDown_Alpha->Position;
  dfactor = (double)factor / 100.0;
  Edit_Value->Text = str.FormatFloat("0.00",dfactor);
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::Edit_ValueChange(TObject *Sender)
{
    UpDown_Alpha->Position = (int)(StrToFloat(Edit_Value->Text) * 100.0);
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_ColorClearClick(TObject *Sender)
{
  int i, j;

  for(i=0; i<72; i++) {
    for(j=0; j<8; j++) {
       MainForm->Map[i][j]->Color = clWindow;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TTMapForm::sButton_ColorSetClick(TObject *Sender)
{
  int i, j;
  double factor;

  for(i=0; i<72; i++) {
    for(j=0; j<8; j++) {
      factor = StrToFloat(MainForm->Map[i][j]->Text);
      if(factor <= 0.2)                            // 0.2 이하
        MainForm->Map[i][j]->Color = clSilver;
      else if(factor > 0.2 && factor <= 0.6)    // 0.2 ~ 0.6
        MainForm->Map[i][j]->Color = 0x0000BB00;
      else if(factor > 0.6 && factor <= 0.8)    // 0.6 ~ 0.8
        MainForm->Map[i][j]->Color = 0x0000EC00;
      else if(factor > 0.8 && factor <= 0.9)    // 0.8 ~ 0.9
        MainForm->Map[i][j]->Color = 0x0028FF94;
      else if(factor > 0.9 && factor <= 1.0)    // 0.9 ~ 1.0
        MainForm->Map[i][j]->Color = 0x0080FFFF;
      else if(factor > 1.0 && factor <= 1.1)    // 1.0 ~ 1.1
        MainForm->Map[i][j]->Color = 0x0000BBFF;
      else if(factor > 1.1 && factor <= 1.2)    // 1.1 ~ 1.2
        MainForm->Map[i][j]->Color = 0x000166FE;
      else                                      // 1.2 이상
        MainForm->Map[i][j]->Color = clRed;
    }
  }
}
//---------------------------------------------------------------------------

