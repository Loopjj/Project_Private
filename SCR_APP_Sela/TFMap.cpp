//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFMap.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFMapForm *FMapForm;
//---------------------------------------------------------------------------
__fastcall TFMapForm::TFMapForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFMapForm::FormShow(TObject *Sender)
{
    StringGrid_Map->Cells[0][0]="T1/RPM";
    StringGrid_Map->FixedColor = TColor(RGB(255, 128, 0));
    MainForm->TxCmd(0xc6, 0x69,0);
    Sleep(200);
    MainForm->TxCmd(0xc6, 0x67,0);
    Sleep(200);
    MainForm->TxCmd(0xc6, 0x6b,0);
#if 0
    StringGrid_Map->Cells[1][0]="1000";
    StringGrid_Map->Cells[2][0]="1100";
    StringGrid_Map->Cells[3][0]="1200";
    StringGrid_Map->Cells[4][0]="1300";
    StringGrid_Map->Cells[5][0]="1400";
    StringGrid_Map->Cells[6][0]="1500";
    StringGrid_Map->Cells[7][0]="1700";
    StringGrid_Map->Cells[8][0]="1900";
    StringGrid_Map->Cells[9][0]="2100";
    StringGrid_Map->Cells[10][0]="2300";

    StringGrid_Map->Cells[0][1]="200";
    StringGrid_Map->Cells[0][2]="220";
    StringGrid_Map->Cells[0][3]="240";
    StringGrid_Map->Cells[0][4]="260";
    StringGrid_Map->Cells[0][5]="280";
    StringGrid_Map->Cells[0][6]="300";
    StringGrid_Map->Cells[0][7]="320";
    StringGrid_Map->Cells[0][8]="340";
    StringGrid_Map->Cells[0][9]="360";
    StringGrid_Map->Cells[0][10]="380";

    StringGrid_Map->Cells[1][1]="1";
    StringGrid_Map->Cells[2][1]="2";
    StringGrid_Map->Cells[3][1]="3";
    StringGrid_Map->Cells[4][1]="4";
    StringGrid_Map->Cells[5][1]="5";
    StringGrid_Map->Cells[6][1]="6";
    StringGrid_Map->Cells[7][1]="7";
    StringGrid_Map->Cells[8][1]="8";
    StringGrid_Map->Cells[9][1]="9";
    StringGrid_Map->Cells[10][1]="10";

    StringGrid_Map->Cells[1][2]="11";
    StringGrid_Map->Cells[2][2]="12";
    StringGrid_Map->Cells[3][2]="13";
    StringGrid_Map->Cells[4][2]="14";
    StringGrid_Map->Cells[5][2]="15";
    StringGrid_Map->Cells[6][2]="16";
    StringGrid_Map->Cells[7][2]="17";
    StringGrid_Map->Cells[8][2]="18";
    StringGrid_Map->Cells[9][2]="19";
    StringGrid_Map->Cells[10][2]="20";

    StringGrid_Map->Cells[1][3]="21";
    StringGrid_Map->Cells[2][3]="22";
    StringGrid_Map->Cells[3][3]="23";
    StringGrid_Map->Cells[4][3]="24";
    StringGrid_Map->Cells[5][3]="25";
    StringGrid_Map->Cells[6][3]="26";
    StringGrid_Map->Cells[7][3]="27";
    StringGrid_Map->Cells[8][3]="28";
    StringGrid_Map->Cells[9][3]="29";
    StringGrid_Map->Cells[10][3]="30";

    StringGrid_Map->Cells[1][4]="31";
    StringGrid_Map->Cells[2][4]="32";
    StringGrid_Map->Cells[3][4]="33";
    StringGrid_Map->Cells[4][4]="34";
    StringGrid_Map->Cells[5][4]="35";
    StringGrid_Map->Cells[6][4]="36";
    StringGrid_Map->Cells[7][4]="37";
    StringGrid_Map->Cells[8][4]="38";
    StringGrid_Map->Cells[9][4]="39";
    StringGrid_Map->Cells[10][4]="40";

    StringGrid_Map->Cells[1][5]="41";
    StringGrid_Map->Cells[2][5]="42";
    StringGrid_Map->Cells[3][5]="43";
    StringGrid_Map->Cells[4][5]="44";
    StringGrid_Map->Cells[5][5]="45";
    StringGrid_Map->Cells[6][5]="46";
    StringGrid_Map->Cells[7][5]="47";
    StringGrid_Map->Cells[8][5]="48";
    StringGrid_Map->Cells[9][5]="49";
    StringGrid_Map->Cells[10][5]="50";

    StringGrid_Map->Cells[1][6]="51";
    StringGrid_Map->Cells[2][6]="52";
    StringGrid_Map->Cells[3][6]="53";
    StringGrid_Map->Cells[4][6]="54";
    StringGrid_Map->Cells[5][6]="55";
    StringGrid_Map->Cells[6][6]="56";
    StringGrid_Map->Cells[7][6]="57";
    StringGrid_Map->Cells[8][6]="58";
    StringGrid_Map->Cells[9][6]="59";
    StringGrid_Map->Cells[10][6]="60";

    StringGrid_Map->Cells[1][7]="61";
    StringGrid_Map->Cells[2][7]="62";
    StringGrid_Map->Cells[3][7]="63";
    StringGrid_Map->Cells[4][7]="64";
    StringGrid_Map->Cells[5][7]="65";
    StringGrid_Map->Cells[6][7]="66";
    StringGrid_Map->Cells[7][7]="67";
    StringGrid_Map->Cells[8][7]="68";
    StringGrid_Map->Cells[9][7]="69";
    StringGrid_Map->Cells[10][7]="70";

    StringGrid_Map->Cells[1][8]="71";
    StringGrid_Map->Cells[2][8]="72";
    StringGrid_Map->Cells[3][8]="73";
    StringGrid_Map->Cells[4][8]="74";
    StringGrid_Map->Cells[5][8]="75";
    StringGrid_Map->Cells[6][8]="76";
    StringGrid_Map->Cells[7][8]="77";
    StringGrid_Map->Cells[8][8]="78";
    StringGrid_Map->Cells[9][8]="79";
    StringGrid_Map->Cells[10][8]="80";

    StringGrid_Map->Cells[1][9]="81";
    StringGrid_Map->Cells[2][9]="82";
    StringGrid_Map->Cells[3][9]="83";
    StringGrid_Map->Cells[4][9]="84";
    StringGrid_Map->Cells[5][9]="85";
    StringGrid_Map->Cells[6][9]="86";
    StringGrid_Map->Cells[7][9]="87";
    StringGrid_Map->Cells[8][9]="88";
    StringGrid_Map->Cells[9][9]="89";
    StringGrid_Map->Cells[10][9]="90";

    StringGrid_Map->Cells[1][10]="91";
    StringGrid_Map->Cells[2][10]="92";
    StringGrid_Map->Cells[3][10]="93";
    StringGrid_Map->Cells[4][10]="94";
    StringGrid_Map->Cells[5][10]="95";
    StringGrid_Map->Cells[6][10]="96";
    StringGrid_Map->Cells[7][10]="97";
    StringGrid_Map->Cells[8][10]="98";
    StringGrid_Map->Cells[9][10]="99";
    StringGrid_Map->Cells[10][10]="100";
#endif        
}
//---------------------------------------------------------------------------

void __fastcall TFMapForm::RPMReadBtnClick(TObject *Sender)
{
    StringGrid_Map->FixedCols=1;
    StringGrid_Map->FixedRows=1;
    MainForm->TxCmd(0xc6, 0x69,0);
}
//---------------------------------------------------------------------------

void __fastcall TFMapForm::RPMWriteBtnClick(TObject *Sender)
{
    unsigned char sdata[25];
    unsigned int tmp;
    
    StringGrid_Map->FixedCols=1;
    StringGrid_Map->FixedRows=1;

    for(int i=0; i<10; i++) {
        tmp = StrToInt(StringGrid_Map->Cells[i+1][0]);
        sdata[i*2] = (tmp>>8)&0xff;
        sdata[i*2+1] = tmp&0xff;
    }

    MainForm->TxData(0xc6, 20, 0x6a,0, sdata);        
}
//---------------------------------------------------------------------------
void __fastcall TFMapForm::T2ReadBtnClick(TObject *Sender)
{
    StringGrid_Map->FixedCols=1;
    StringGrid_Map->FixedRows=1;
    
    MainForm->TxCmd(0xc6, 0x67,0);
}
//---------------------------------------------------------------------------
void __fastcall TFMapForm::T2WriteBtnClick(TObject *Sender)
{
    unsigned char sdata[25];
    unsigned int tmp;

    StringGrid_Map->FixedCols=1;
    StringGrid_Map->FixedRows=1;
        
    for(int i=0; i<10; i++) {
        tmp = StrToInt(StringGrid_Map->Cells[0][i+1]);
        sdata[i*2] = (tmp>>8)&0xff;
        sdata[i*2+1] = tmp&0xff;
    }
    MainForm->TxData(0xc6, 20, 0x68,0, sdata);        
}
//---------------------------------------------------------------------------
void __fastcall TFMapForm::LoadBtnClick(TObject *Sender)
{
    AnsiString str;
    char s[6];
    int n, v[10];

    Memo->Clear();
    if(OpenDialog->Execute()) {
        Memo->Lines->LoadFromFile(OpenDialog->FileName);

        str = Memo->Lines->Strings[0];
        sscanf(str.c_str(),"%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
               &s,&v[0],&v[1],&v[2],&v[3],&v[4],&v[5],&v[6],&v[7],&v[8],&v[9]);
        for(int j = 0; j < 10; j++)
            StringGrid_Map->Cells[j+1][0] = IntToStr(v[j]);

        for(int i = 0; i < 10; i++) {
            str = Memo->Lines->Strings[i+1];
            sscanf(str.c_str(),"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
                   &n,&v[0],&v[1],&v[2],&v[3],&v[4],&v[5],&v[6],&v[7],&v[8],&v[9]);
            StringGrid_Map->Cells[0][i+1] = IntToStr(n);
            for(int j = 0; j < 10; j++)
                StringGrid_Map->Cells[j+1][i+1] = IntToStr(v[j]);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TFMapForm::SaveBtnClick(TObject *Sender)
{
    AnsiString str;

    Memo->Clear();
    str = "T1/RPM";
    for(int i = 0; i < 10; i++)
       str = str + "\t" + StringGrid_Map->Cells[i+1][0];
    Memo->Lines->Add(str);
    str = "";
    for(int i = 0; i < 10; i++){
        str = StringGrid_Map->Cells[0][i+1];
        for(int j = 0; j < 10; j++) {
            str = str + "\t" + StringGrid_Map->Cells[j+1][i+1];
        }
        Memo->Lines->Add(str);
    }
    if(SaveDialog->Execute()) {
        Memo->Lines->SaveToFile(SaveDialog->FileName);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFMapForm::MapReadBtnClick(TObject *Sender)
{
    StringGrid_Map->FixedCols=1;
    StringGrid_Map->FixedRows=1;

    MainForm->TxCmd(0xc6, 0x6b,0);
}
//---------------------------------------------------------------------------
void __fastcall TFMapForm::MapWriteBtnClick(TObject *Sender)
{
    StringGrid_Map->FixedCols=1;
    StringGrid_Map->FixedRows=1;

    MainForm->Fire_TxMap(0);        
}
//---------------------------------------------------------------------------

void __fastcall TFMapForm::StringGrid_MapSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
    StringGrid_Map->FixedCols=0;
    StringGrid_Map->FixedRows=0;
}
//---------------------------------------------------------------------------

void __fastcall TFMapForm::StringGrid_MapExit(TObject *Sender)
{
    StringGrid_Map->FixedCols=1;
    StringGrid_Map->FixedRows=1;
}
//---------------------------------------------------------------------------

