//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TMain.h"
#include "TSettings.h"
#include "TGraph.h"
#include "TStatus.h"
#include "TConvert.h"
#include "TMap.h"
#include "TCalib.h"
#include "TFactor.h"
#include "TRTC.h"
#include "TSaveAs.h"
#include "TCan.h"
#include "TIP.h"
#include "TPopup.h"
#include "TProg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdPort"
#pragma link "OoMisc"
#pragma link "sSkinManager"
#pragma link "sPanel"
#pragma link "sLabel"
#pragma link "sButton"
#pragma link "sComboBox"
#pragma link "AdStatLt"
#pragma resource "*.dfm"


TMainForm *MainForm;


//---------------------------------------------------------------------------
typedef union {
    unsigned short id;
    struct {
        unsigned char l;
        unsigned char h;
    }b;
}st_std;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
unsigned char __fastcall TMainForm::asciiHexToNibble(unsigned char  character)
{
	if((character >= 0x30) && (character <= 0x39))
		return character-0x30;
	else if((character >=  'A') && (character <=  'F'))
		return character-'A'+10;
	else if((character >=  'a') && (character <=  'f'))
		return character-'a'+10;
	else return 0;
}

unsigned char __fastcall TMainForm::asciiHexToByte(unsigned char * string)
{
	return (asciiHexToNibble(string[0])<<4) + asciiHexToNibble(string[1]);
}


unsigned char __fastcall TMainForm::nibbleHexToAscii(unsigned char  nibble) {
        if (nibble <= 9)
                return nibble + '0';
        return nibble + 'A' - 10;
}

void __fastcall TMainForm::byteToAsciiHex(unsigned char character, unsigned char * string)
{
        string[1] = nibbleHexToAscii(character & 0xF);
        string[0] = nibbleHexToAscii((character>>4) & 0xF);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
    sComboBox_ComPort->Clear();
    for(int i = 0; i < 20; i++) {
        if(IsPortAvailable(i))
            sComboBox_ComPort->Items->Add("COM"+IntToStr(i));
    }
    sComboBox_ComPort->ItemIndex = 0;

    RLED[0][0] = PER0; RLED[0][1] = PER1; RLED[0][2] = PER2; RLED[0][3] = PER3;
    RLED[0][4] = PER4; RLED[0][5] = PER5; RLED[0][6] = PER6; RLED[0][7] = PER7;

    RLED[2][0] = PCK0; RLED[2][1] = PCK1; RLED[2][2] = PCK2; RLED[2][3] = PCK3;
    RLED[2][4] = PCK4; RLED[2][5] = PCK5; RLED[2][6] = PCK6; RLED[2][7] = PCK7;
    RLED[3][0] = PCK8; RLED[3][1] = PCK9;

    RLED[4][0] = PSG0; RLED[4][1] = PSG1; RLED[4][2] = PSG2; RLED[4][3] = PSG3;
    RLED[4][4] = PSG4; RLED[4][5] = PSG5; RLED[4][6] = PSG6; RLED[4][7] = PSG7;
    RLED[5][0] = PSG8; RLED[5][1] = PSG9;

    SData[0][0]  = ParmForm->Edit_P00; SData[0][1]  = ParmForm->Edit_P01; SData[0][2]  = ParmForm->Edit_P02; SData[0][3]  = ParmForm->Edit_P03;
    SData[1][0]  = ParmForm->Edit_P10; SData[1][1]  = ParmForm->Edit_P11; SData[1][2]  = ParmForm->Edit_P12; SData[1][3]  = ParmForm->Edit_P13;
    SData[2][0]  = ParmForm->Edit_P20; SData[2][1]  = ParmForm->Edit_P21; SData[2][2]  = ParmForm->Edit_P22; SData[2][3]  = ParmForm->Edit_P23;
    SData[3][0]  = ParmForm->Edit_P30; SData[3][1]  = ParmForm->Edit_P31; SData[3][2]  = ParmForm->Edit_P32; SData[3][3]  = ParmForm->Edit_P33;
    SData[4][0]  = ParmForm->Edit_P40; SData[4][1]  = ParmForm->Edit_P41; SData[4][2]  = ParmForm->Edit_P42; SData[4][3]  = ParmForm->Edit_P43;
    // Alpa Factor Map
    Map[0][0] = TMapForm->Edit_Map0_00; Map[0][1] = TMapForm->Edit_Map0_01; Map[0][2] = TMapForm->Edit_Map0_02; Map[0][3] = TMapForm->Edit_Map0_03;
    Map[0][4] = TMapForm->Edit_Map0_04; Map[0][5] = TMapForm->Edit_Map0_05; Map[0][6] = TMapForm->Edit_Map0_06; Map[0][7] = TMapForm->Edit_Map0_07;
    Map[1][0] = TMapForm->Edit_Map0_08; Map[1][1] = TMapForm->Edit_Map0_09; Map[1][2] = TMapForm->Edit_Map0_10; Map[1][3] = TMapForm->Edit_Map0_11;
    Map[1][4] = TMapForm->Edit_Map0_12; Map[1][5] = TMapForm->Edit_Map0_13; Map[1][6] = TMapForm->Edit_Map0_14; Map[1][7] = TMapForm->Edit_Map0_15;
    Map[2][0] = TMapForm->Edit_Map0_16; Map[2][1] = TMapForm->Edit_Map0_17; Map[2][2] = TMapForm->Edit_Map0_18; Map[2][3] = TMapForm->Edit_Map0_19;
    Map[2][4] = TMapForm->Edit_Map0_20; Map[2][5] = TMapForm->Edit_Map0_21; Map[2][6] = TMapForm->Edit_Map0_22; Map[2][7] = TMapForm->Edit_Map0_23;
    Map[3][0] = TMapForm->Edit_Map1_00; Map[3][1] = TMapForm->Edit_Map1_01; Map[3][2] = TMapForm->Edit_Map1_02; Map[3][3] = TMapForm->Edit_Map1_03;
    Map[3][4] = TMapForm->Edit_Map1_04; Map[3][5] = TMapForm->Edit_Map1_05; Map[3][6] = TMapForm->Edit_Map1_06; Map[3][7] = TMapForm->Edit_Map1_07;
    Map[4][0] = TMapForm->Edit_Map1_08; Map[4][1] = TMapForm->Edit_Map1_09; Map[4][2] = TMapForm->Edit_Map1_10; Map[4][3] = TMapForm->Edit_Map1_11;
    Map[4][4] = TMapForm->Edit_Map1_12; Map[4][5] = TMapForm->Edit_Map1_13; Map[4][6] = TMapForm->Edit_Map1_14; Map[4][7] = TMapForm->Edit_Map1_15;
    Map[5][0] = TMapForm->Edit_Map1_16; Map[5][1] = TMapForm->Edit_Map1_17; Map[5][2] = TMapForm->Edit_Map1_18; Map[5][3] = TMapForm->Edit_Map1_19;
    Map[5][4] = TMapForm->Edit_Map1_20; Map[5][5] = TMapForm->Edit_Map1_21; Map[5][6] = TMapForm->Edit_Map1_22; Map[5][7] = TMapForm->Edit_Map1_23;
    Map[6][0] = TMapForm->Edit_Map2_00; Map[6][1] = TMapForm->Edit_Map2_01; Map[6][2] = TMapForm->Edit_Map2_02; Map[6][3] = TMapForm->Edit_Map2_03;
    Map[6][4] = TMapForm->Edit_Map2_04; Map[6][5] = TMapForm->Edit_Map2_05; Map[6][6] = TMapForm->Edit_Map2_06; Map[6][7] = TMapForm->Edit_Map2_07;
    Map[7][0] = TMapForm->Edit_Map2_08; Map[7][1] = TMapForm->Edit_Map2_09; Map[7][2] = TMapForm->Edit_Map2_10; Map[7][3] = TMapForm->Edit_Map2_11;
    Map[7][4] = TMapForm->Edit_Map2_12; Map[7][5] = TMapForm->Edit_Map2_13; Map[7][6] = TMapForm->Edit_Map2_14; Map[7][7] = TMapForm->Edit_Map2_15;
    Map[8][0] = TMapForm->Edit_Map2_16; Map[8][1] = TMapForm->Edit_Map2_17; Map[8][2] = TMapForm->Edit_Map2_18; Map[8][3] = TMapForm->Edit_Map2_19;
    Map[8][4] = TMapForm->Edit_Map2_20; Map[8][5] = TMapForm->Edit_Map2_21; Map[8][6] = TMapForm->Edit_Map2_22; Map[8][7] = TMapForm->Edit_Map2_23;
    Map[9][0] = TMapForm->Edit_Map3_00; Map[9][1] = TMapForm->Edit_Map3_01; Map[9][2] = TMapForm->Edit_Map3_02; Map[9][3] = TMapForm->Edit_Map3_03;
    Map[9][4] = TMapForm->Edit_Map3_04; Map[9][5] = TMapForm->Edit_Map3_05; Map[9][6] = TMapForm->Edit_Map3_06; Map[9][7] = TMapForm->Edit_Map3_07;
    Map[10][0] = TMapForm->Edit_Map3_08; Map[10][1] = TMapForm->Edit_Map3_09; Map[10][2] = TMapForm->Edit_Map3_10; Map[10][3] = TMapForm->Edit_Map3_11;
    Map[10][4] = TMapForm->Edit_Map3_12; Map[10][5] = TMapForm->Edit_Map3_13; Map[10][6] = TMapForm->Edit_Map3_14; Map[10][7] = TMapForm->Edit_Map3_15;
    Map[11][0] = TMapForm->Edit_Map3_16; Map[11][1] = TMapForm->Edit_Map3_17; Map[11][2] = TMapForm->Edit_Map3_18; Map[11][3] = TMapForm->Edit_Map3_19;
    Map[11][4] = TMapForm->Edit_Map3_20; Map[11][5] = TMapForm->Edit_Map3_21; Map[11][6] = TMapForm->Edit_Map3_22; Map[11][7] = TMapForm->Edit_Map3_23;
    Map[12][0] = TMapForm->Edit_Map4_00; Map[12][1] = TMapForm->Edit_Map4_01; Map[12][2] = TMapForm->Edit_Map4_02; Map[12][3] = TMapForm->Edit_Map4_03;
    Map[12][4] = TMapForm->Edit_Map4_04; Map[12][5] = TMapForm->Edit_Map4_05; Map[12][6] = TMapForm->Edit_Map4_06; Map[12][7] = TMapForm->Edit_Map4_07;
    Map[13][0] = TMapForm->Edit_Map4_08; Map[13][1] = TMapForm->Edit_Map4_09; Map[13][2] = TMapForm->Edit_Map4_10; Map[13][3] = TMapForm->Edit_Map4_11;
    Map[13][4] = TMapForm->Edit_Map4_12; Map[13][5] = TMapForm->Edit_Map4_13; Map[13][6] = TMapForm->Edit_Map4_14; Map[13][7] = TMapForm->Edit_Map4_15;
    Map[14][0] = TMapForm->Edit_Map4_16; Map[14][1] = TMapForm->Edit_Map4_17; Map[14][2] = TMapForm->Edit_Map4_18; Map[14][3] = TMapForm->Edit_Map4_19;
    Map[14][4] = TMapForm->Edit_Map4_20; Map[14][5] = TMapForm->Edit_Map4_21; Map[14][6] = TMapForm->Edit_Map4_22; Map[14][7] = TMapForm->Edit_Map4_23;
    Map[15][0] = TMapForm->Edit_Map5_00; Map[15][1] = TMapForm->Edit_Map5_01; Map[15][2] = TMapForm->Edit_Map5_02; Map[15][3] = TMapForm->Edit_Map5_03;
    Map[15][4] = TMapForm->Edit_Map5_04; Map[15][5] = TMapForm->Edit_Map5_05; Map[15][6] = TMapForm->Edit_Map5_06; Map[15][7] = TMapForm->Edit_Map5_07;
    Map[16][0] = TMapForm->Edit_Map5_08; Map[16][1] = TMapForm->Edit_Map5_09; Map[16][2] = TMapForm->Edit_Map5_10; Map[16][3] = TMapForm->Edit_Map5_11;
    Map[16][4] = TMapForm->Edit_Map5_12; Map[16][5] = TMapForm->Edit_Map5_13; Map[16][6] = TMapForm->Edit_Map5_14; Map[16][7] = TMapForm->Edit_Map5_15;
    Map[17][0] = TMapForm->Edit_Map5_16; Map[17][1] = TMapForm->Edit_Map5_17; Map[17][2] = TMapForm->Edit_Map5_18; Map[17][3] = TMapForm->Edit_Map5_19;
    Map[17][4] = TMapForm->Edit_Map5_20; Map[17][5] = TMapForm->Edit_Map5_21; Map[17][6] = TMapForm->Edit_Map5_22; Map[17][7] = TMapForm->Edit_Map5_23;
    Map[18][0] = TMapForm->Edit_Map6_00; Map[18][1] = TMapForm->Edit_Map6_01; Map[18][2] = TMapForm->Edit_Map6_02; Map[18][3] = TMapForm->Edit_Map6_03;
    Map[18][4] = TMapForm->Edit_Map6_04; Map[18][5] = TMapForm->Edit_Map6_05; Map[18][6] = TMapForm->Edit_Map6_06; Map[18][7] = TMapForm->Edit_Map6_07;
    Map[19][0] = TMapForm->Edit_Map6_08; Map[19][1] = TMapForm->Edit_Map6_09; Map[19][2] = TMapForm->Edit_Map6_10; Map[19][3] = TMapForm->Edit_Map6_11;
    Map[19][4] = TMapForm->Edit_Map6_12; Map[19][5] = TMapForm->Edit_Map6_13; Map[19][6] = TMapForm->Edit_Map6_14; Map[19][7] = TMapForm->Edit_Map6_15;
    Map[20][0] = TMapForm->Edit_Map6_16; Map[20][1] = TMapForm->Edit_Map6_17; Map[20][2] = TMapForm->Edit_Map6_18; Map[20][3] = TMapForm->Edit_Map6_19;
    Map[20][4] = TMapForm->Edit_Map6_20; Map[20][5] = TMapForm->Edit_Map6_21; Map[20][6] = TMapForm->Edit_Map6_22; Map[20][7] = TMapForm->Edit_Map6_23;
    Map[21][0] = TMapForm->Edit_Map7_00; Map[21][1] = TMapForm->Edit_Map7_01; Map[21][2] = TMapForm->Edit_Map7_02; Map[21][3] = TMapForm->Edit_Map7_03;
    Map[21][4] = TMapForm->Edit_Map7_04; Map[21][5] = TMapForm->Edit_Map7_05; Map[21][6] = TMapForm->Edit_Map7_06; Map[21][7] = TMapForm->Edit_Map7_07;
    Map[22][0] = TMapForm->Edit_Map7_08; Map[22][1] = TMapForm->Edit_Map7_09; Map[22][2] = TMapForm->Edit_Map7_10; Map[22][3] = TMapForm->Edit_Map7_11;
    Map[22][4] = TMapForm->Edit_Map7_12; Map[22][5] = TMapForm->Edit_Map7_13; Map[22][6] = TMapForm->Edit_Map7_14; Map[22][7] = TMapForm->Edit_Map7_15;
    Map[23][0] = TMapForm->Edit_Map7_16; Map[23][1] = TMapForm->Edit_Map7_17; Map[23][2] = TMapForm->Edit_Map7_18; Map[23][3] = TMapForm->Edit_Map7_19;
    Map[23][4] = TMapForm->Edit_Map7_20; Map[23][5] = TMapForm->Edit_Map7_21; Map[23][6] = TMapForm->Edit_Map7_22; Map[23][7] = TMapForm->Edit_Map7_23;
    Map[24][0] = TMapForm->Edit_Map8_00; Map[24][1] = TMapForm->Edit_Map8_01; Map[24][2] = TMapForm->Edit_Map8_02; Map[24][3] = TMapForm->Edit_Map8_03;
    Map[24][4] = TMapForm->Edit_Map8_04; Map[24][5] = TMapForm->Edit_Map8_05; Map[24][6] = TMapForm->Edit_Map8_06; Map[24][7] = TMapForm->Edit_Map8_07;

    Map[25][0] = TMapForm->Edit_Map8_08; Map[25][1] = TMapForm->Edit_Map8_09; Map[25][2] = TMapForm->Edit_Map8_10; Map[25][3] = TMapForm->Edit_Map8_11;
    Map[25][4] = TMapForm->Edit_Map8_12; Map[25][5] = TMapForm->Edit_Map8_13; Map[25][6] = TMapForm->Edit_Map8_14; Map[25][7] = TMapForm->Edit_Map8_15;
    Map[26][0] = TMapForm->Edit_Map8_16; Map[26][1] = TMapForm->Edit_Map8_17; Map[26][2] = TMapForm->Edit_Map8_18; Map[26][3] = TMapForm->Edit_Map8_19;
    Map[26][4] = TMapForm->Edit_Map8_20; Map[26][5] = TMapForm->Edit_Map8_21; Map[26][6] = TMapForm->Edit_Map8_22; Map[26][7] = TMapForm->Edit_Map8_23;

    Map[27][0] = TMapForm->Edit_Map9_00; Map[27][1] = TMapForm->Edit_Map9_01; Map[27][2] = TMapForm->Edit_Map9_02; Map[27][3] = TMapForm->Edit_Map9_03;
    Map[27][4] = TMapForm->Edit_Map9_04; Map[27][5] = TMapForm->Edit_Map9_05; Map[27][6] = TMapForm->Edit_Map9_06; Map[27][7] = TMapForm->Edit_Map9_07;
    Map[28][0] = TMapForm->Edit_Map9_08; Map[28][1] = TMapForm->Edit_Map9_09; Map[28][2] = TMapForm->Edit_Map9_10; Map[28][3] = TMapForm->Edit_Map9_11;
    Map[28][4] = TMapForm->Edit_Map9_12; Map[28][5] = TMapForm->Edit_Map9_13; Map[28][6] = TMapForm->Edit_Map9_14; Map[28][7] = TMapForm->Edit_Map9_15;
    Map[29][0] = TMapForm->Edit_Map9_16; Map[29][1] = TMapForm->Edit_Map9_17; Map[29][2] = TMapForm->Edit_Map9_18; Map[29][3] = TMapForm->Edit_Map9_19;
    Map[29][4] = TMapForm->Edit_Map9_20; Map[29][5] = TMapForm->Edit_Map9_21; Map[29][6] = TMapForm->Edit_Map9_22; Map[29][7] = TMapForm->Edit_Map9_23;
    Map[30][0] = TMapForm->Edit_Map10_00; Map[30][1] = TMapForm->Edit_Map10_01; Map[30][2] = TMapForm->Edit_Map10_02; Map[30][3] = TMapForm->Edit_Map10_03;
    Map[30][4] = TMapForm->Edit_Map10_04; Map[30][5] = TMapForm->Edit_Map10_05; Map[30][6] = TMapForm->Edit_Map10_06; Map[30][7] = TMapForm->Edit_Map10_07;
    Map[31][0] = TMapForm->Edit_Map10_08; Map[31][1] = TMapForm->Edit_Map10_09; Map[31][2] = TMapForm->Edit_Map10_10; Map[31][3] = TMapForm->Edit_Map10_11;
    Map[31][4] = TMapForm->Edit_Map10_12; Map[31][5] = TMapForm->Edit_Map10_13; Map[31][6] = TMapForm->Edit_Map10_14; Map[31][7] = TMapForm->Edit_Map10_15;
    Map[32][0] = TMapForm->Edit_Map10_16; Map[32][1] = TMapForm->Edit_Map10_17; Map[32][2] = TMapForm->Edit_Map10_18; Map[32][3] = TMapForm->Edit_Map10_19;
    Map[32][4] = TMapForm->Edit_Map10_20; Map[32][5] = TMapForm->Edit_Map10_21; Map[32][6] = TMapForm->Edit_Map10_22; Map[32][7] = TMapForm->Edit_Map10_23;
    Map[33][0] = TMapForm->Edit_Map11_00; Map[33][1] = TMapForm->Edit_Map11_01; Map[33][2] = TMapForm->Edit_Map11_02; Map[33][3] = TMapForm->Edit_Map11_03;
    Map[33][4] = TMapForm->Edit_Map11_04; Map[33][5] = TMapForm->Edit_Map11_05; Map[33][6] = TMapForm->Edit_Map11_06; Map[33][7] = TMapForm->Edit_Map11_07;
    Map[34][0] = TMapForm->Edit_Map11_08; Map[34][1] = TMapForm->Edit_Map11_09; Map[34][2] = TMapForm->Edit_Map11_10; Map[34][3] = TMapForm->Edit_Map11_11;
    Map[34][4] = TMapForm->Edit_Map11_12; Map[34][5] = TMapForm->Edit_Map11_13; Map[34][6] = TMapForm->Edit_Map11_14; Map[34][7] = TMapForm->Edit_Map11_15;
    Map[35][0] = TMapForm->Edit_Map11_16; Map[35][1] = TMapForm->Edit_Map11_17; Map[35][2] = TMapForm->Edit_Map11_18; Map[35][3] = TMapForm->Edit_Map11_19;
    Map[35][4] = TMapForm->Edit_Map11_20; Map[35][5] = TMapForm->Edit_Map11_21; Map[35][6] = TMapForm->Edit_Map11_22; Map[35][7] = TMapForm->Edit_Map11_23;
    Map[36][0] = TMapForm->Edit_Map12_00; Map[36][1] = TMapForm->Edit_Map12_01; Map[36][2] = TMapForm->Edit_Map12_02; Map[36][3] = TMapForm->Edit_Map12_03;
    Map[36][4] = TMapForm->Edit_Map12_04; Map[36][5] = TMapForm->Edit_Map12_05; Map[36][6] = TMapForm->Edit_Map12_06; Map[36][7] = TMapForm->Edit_Map12_07;
    Map[37][0] = TMapForm->Edit_Map12_08; Map[37][1] = TMapForm->Edit_Map12_09; Map[37][2] = TMapForm->Edit_Map12_10; Map[37][3] = TMapForm->Edit_Map12_11;
    Map[37][4] = TMapForm->Edit_Map12_12; Map[37][5] = TMapForm->Edit_Map12_13; Map[37][6] = TMapForm->Edit_Map12_14; Map[37][7] = TMapForm->Edit_Map12_15;
    Map[38][0] = TMapForm->Edit_Map12_16; Map[38][1] = TMapForm->Edit_Map12_17; Map[38][2] = TMapForm->Edit_Map12_18; Map[38][3] = TMapForm->Edit_Map12_19;
    Map[38][4] = TMapForm->Edit_Map12_20; Map[38][5] = TMapForm->Edit_Map12_21; Map[38][6] = TMapForm->Edit_Map12_22; Map[38][7] = TMapForm->Edit_Map12_23;
    Map[39][0] = TMapForm->Edit_Map13_00; Map[39][1] = TMapForm->Edit_Map13_01; Map[39][2] = TMapForm->Edit_Map13_02; Map[39][3] = TMapForm->Edit_Map13_03;
    Map[39][4] = TMapForm->Edit_Map13_04; Map[39][5] = TMapForm->Edit_Map13_05; Map[39][6] = TMapForm->Edit_Map13_06; Map[39][7] = TMapForm->Edit_Map13_07;
    Map[40][0] = TMapForm->Edit_Map13_08; Map[40][1] = TMapForm->Edit_Map13_09; Map[40][2] = TMapForm->Edit_Map13_10; Map[40][3] = TMapForm->Edit_Map13_11;
    Map[40][4] = TMapForm->Edit_Map13_12; Map[40][5] = TMapForm->Edit_Map13_13; Map[40][6] = TMapForm->Edit_Map13_14; Map[40][7] = TMapForm->Edit_Map13_15;
    Map[41][0] = TMapForm->Edit_Map13_16; Map[41][1] = TMapForm->Edit_Map13_17; Map[41][2] = TMapForm->Edit_Map13_18; Map[41][3] = TMapForm->Edit_Map13_19;
    Map[41][4] = TMapForm->Edit_Map13_20; Map[41][5] = TMapForm->Edit_Map13_21; Map[41][6] = TMapForm->Edit_Map13_22; Map[41][7] = TMapForm->Edit_Map13_23;
    Map[42][0] = TMapForm->Edit_Map14_00; Map[42][1] = TMapForm->Edit_Map14_01; Map[42][2] = TMapForm->Edit_Map14_02; Map[42][3] = TMapForm->Edit_Map14_03;
    Map[42][4] = TMapForm->Edit_Map14_04; Map[42][5] = TMapForm->Edit_Map14_05; Map[42][6] = TMapForm->Edit_Map14_06; Map[42][7] = TMapForm->Edit_Map14_07;
    Map[43][0] = TMapForm->Edit_Map14_08; Map[43][1] = TMapForm->Edit_Map14_09; Map[43][2] = TMapForm->Edit_Map14_10; Map[43][3] = TMapForm->Edit_Map14_11;
    Map[43][4] = TMapForm->Edit_Map14_12; Map[43][5] = TMapForm->Edit_Map14_13; Map[43][6] = TMapForm->Edit_Map14_14; Map[43][7] = TMapForm->Edit_Map14_15;
    Map[44][0] = TMapForm->Edit_Map14_16; Map[44][1] = TMapForm->Edit_Map14_17; Map[44][2] = TMapForm->Edit_Map14_18; Map[44][3] = TMapForm->Edit_Map14_19;
    Map[44][4] = TMapForm->Edit_Map14_20; Map[44][5] = TMapForm->Edit_Map14_21; Map[44][6] = TMapForm->Edit_Map14_22; Map[44][7] = TMapForm->Edit_Map14_23;
    Map[45][0] = TMapForm->Edit_Map15_00; Map[45][1] = TMapForm->Edit_Map15_01; Map[45][2] = TMapForm->Edit_Map15_02; Map[45][3] = TMapForm->Edit_Map15_03;
    Map[45][4] = TMapForm->Edit_Map15_04; Map[45][5] = TMapForm->Edit_Map15_05; Map[45][6] = TMapForm->Edit_Map15_06; Map[45][7] = TMapForm->Edit_Map15_07;
    Map[46][0] = TMapForm->Edit_Map15_08; Map[46][1] = TMapForm->Edit_Map15_09; Map[46][2] = TMapForm->Edit_Map15_10; Map[46][3] = TMapForm->Edit_Map15_11;
    Map[46][4] = TMapForm->Edit_Map15_12; Map[46][5] = TMapForm->Edit_Map15_13; Map[46][6] = TMapForm->Edit_Map15_14; Map[46][7] = TMapForm->Edit_Map15_15;
    Map[47][0] = TMapForm->Edit_Map15_16; Map[47][1] = TMapForm->Edit_Map15_17; Map[47][2] = TMapForm->Edit_Map15_18; Map[47][3] = TMapForm->Edit_Map15_19;
    Map[47][4] = TMapForm->Edit_Map15_20; Map[47][5] = TMapForm->Edit_Map15_21; Map[47][6] = TMapForm->Edit_Map15_22; Map[47][7] = TMapForm->Edit_Map15_23;
    Map[48][0] = TMapForm->Edit_Map16_00; Map[48][1] = TMapForm->Edit_Map16_01; Map[48][2] = TMapForm->Edit_Map16_02; Map[48][3] = TMapForm->Edit_Map16_03;
    Map[48][4] = TMapForm->Edit_Map16_04; Map[48][5] = TMapForm->Edit_Map16_05; Map[48][6] = TMapForm->Edit_Map16_06; Map[48][7] = TMapForm->Edit_Map16_07;
    Map[49][0] = TMapForm->Edit_Map16_08; Map[49][1] = TMapForm->Edit_Map16_09; Map[49][2] = TMapForm->Edit_Map16_10; Map[49][3] = TMapForm->Edit_Map16_11;
    Map[49][4] = TMapForm->Edit_Map16_12; Map[49][5] = TMapForm->Edit_Map16_13; Map[49][6] = TMapForm->Edit_Map16_14; Map[49][7] = TMapForm->Edit_Map16_15;
    Map[50][0] = TMapForm->Edit_Map16_16; Map[50][1] = TMapForm->Edit_Map16_17; Map[50][2] = TMapForm->Edit_Map16_18; Map[50][3] = TMapForm->Edit_Map16_19;
    Map[50][4] = TMapForm->Edit_Map16_20; Map[50][5] = TMapForm->Edit_Map16_21; Map[50][6] = TMapForm->Edit_Map16_22; Map[50][7] = TMapForm->Edit_Map16_23;
    Map[51][0] = TMapForm->Edit_Map17_00; Map[51][1] = TMapForm->Edit_Map17_01; Map[51][2] = TMapForm->Edit_Map17_02; Map[51][3] = TMapForm->Edit_Map17_03;
    Map[51][4] = TMapForm->Edit_Map17_04; Map[51][5] = TMapForm->Edit_Map17_05; Map[51][6] = TMapForm->Edit_Map17_06; Map[51][7] = TMapForm->Edit_Map17_07;
    Map[52][0] = TMapForm->Edit_Map17_08; Map[52][1] = TMapForm->Edit_Map17_09; Map[52][2] = TMapForm->Edit_Map17_10; Map[52][3] = TMapForm->Edit_Map17_11;
    Map[52][4] = TMapForm->Edit_Map17_12; Map[52][5] = TMapForm->Edit_Map17_13; Map[52][6] = TMapForm->Edit_Map17_14; Map[52][7] = TMapForm->Edit_Map17_15;
    Map[53][0] = TMapForm->Edit_Map17_16; Map[53][1] = TMapForm->Edit_Map17_17; Map[53][2] = TMapForm->Edit_Map17_18; Map[53][3] = TMapForm->Edit_Map17_19;
    Map[53][4] = TMapForm->Edit_Map17_20; Map[53][5] = TMapForm->Edit_Map17_21; Map[53][6] = TMapForm->Edit_Map17_22; Map[53][7] = TMapForm->Edit_Map17_23;
    Map[54][0] = TMapForm->Edit_Map18_00; Map[54][1] = TMapForm->Edit_Map18_01; Map[54][2] = TMapForm->Edit_Map18_02; Map[54][3] = TMapForm->Edit_Map18_03;
    Map[54][4] = TMapForm->Edit_Map18_04; Map[54][5] = TMapForm->Edit_Map18_05; Map[54][6] = TMapForm->Edit_Map18_06; Map[54][7] = TMapForm->Edit_Map18_07;
    Map[55][0] = TMapForm->Edit_Map18_08; Map[55][1] = TMapForm->Edit_Map18_09; Map[55][2] = TMapForm->Edit_Map18_10; Map[55][3] = TMapForm->Edit_Map18_11;
    Map[55][4] = TMapForm->Edit_Map18_12; Map[55][5] = TMapForm->Edit_Map18_13; Map[55][6] = TMapForm->Edit_Map18_14; Map[55][7] = TMapForm->Edit_Map18_15;
    Map[56][0] = TMapForm->Edit_Map18_16; Map[56][1] = TMapForm->Edit_Map18_17; Map[56][2] = TMapForm->Edit_Map18_18; Map[56][3] = TMapForm->Edit_Map18_19;
    Map[56][4] = TMapForm->Edit_Map18_20; Map[56][5] = TMapForm->Edit_Map18_21; Map[56][6] = TMapForm->Edit_Map18_22; Map[56][7] = TMapForm->Edit_Map18_23;
    Map[57][0] = TMapForm->Edit_Map19_00; Map[57][1] = TMapForm->Edit_Map19_01; Map[57][2] = TMapForm->Edit_Map19_02; Map[57][3] = TMapForm->Edit_Map19_03;
    Map[57][4] = TMapForm->Edit_Map19_04; Map[57][5] = TMapForm->Edit_Map19_05; Map[57][6] = TMapForm->Edit_Map19_06; Map[57][7] = TMapForm->Edit_Map19_07;
    Map[58][0] = TMapForm->Edit_Map19_08; Map[58][1] = TMapForm->Edit_Map19_09; Map[58][2] = TMapForm->Edit_Map19_10; Map[58][3] = TMapForm->Edit_Map19_11;
    Map[58][4] = TMapForm->Edit_Map19_12; Map[58][5] = TMapForm->Edit_Map19_13; Map[58][6] = TMapForm->Edit_Map19_14; Map[58][7] = TMapForm->Edit_Map19_15;
    Map[59][0] = TMapForm->Edit_Map19_16; Map[59][1] = TMapForm->Edit_Map19_17; Map[59][2] = TMapForm->Edit_Map19_18; Map[59][3] = TMapForm->Edit_Map19_19;
    Map[59][4] = TMapForm->Edit_Map19_20; Map[59][5] = TMapForm->Edit_Map19_21; Map[59][6] = TMapForm->Edit_Map19_22; Map[59][7] = TMapForm->Edit_Map19_23;
    Map[60][0] = TMapForm->Edit_Map20_00; Map[60][1] = TMapForm->Edit_Map20_01; Map[60][2] = TMapForm->Edit_Map20_02; Map[60][3] = TMapForm->Edit_Map20_03;
    Map[60][4] = TMapForm->Edit_Map20_04; Map[60][5] = TMapForm->Edit_Map20_05; Map[60][6] = TMapForm->Edit_Map20_06; Map[60][7] = TMapForm->Edit_Map20_07;
    Map[61][0] = TMapForm->Edit_Map20_08; Map[61][1] = TMapForm->Edit_Map20_09; Map[61][2] = TMapForm->Edit_Map20_10; Map[61][3] = TMapForm->Edit_Map20_11;
    Map[61][4] = TMapForm->Edit_Map20_12; Map[61][5] = TMapForm->Edit_Map20_13; Map[61][6] = TMapForm->Edit_Map20_14; Map[61][7] = TMapForm->Edit_Map20_15;
    Map[62][0] = TMapForm->Edit_Map20_16; Map[62][1] = TMapForm->Edit_Map20_17; Map[62][2] = TMapForm->Edit_Map20_18; Map[62][3] = TMapForm->Edit_Map20_19;
    Map[62][4] = TMapForm->Edit_Map20_20; Map[62][5] = TMapForm->Edit_Map20_21; Map[62][6] = TMapForm->Edit_Map20_22; Map[62][7] = TMapForm->Edit_Map20_23;
    Map[63][0] = TMapForm->Edit_Map21_00; Map[63][1] = TMapForm->Edit_Map21_01; Map[63][2] = TMapForm->Edit_Map21_02; Map[63][3] = TMapForm->Edit_Map21_03;
    Map[63][4] = TMapForm->Edit_Map21_04; Map[63][5] = TMapForm->Edit_Map21_05; Map[63][6] = TMapForm->Edit_Map21_06; Map[63][7] = TMapForm->Edit_Map21_07;
    Map[64][0] = TMapForm->Edit_Map21_08; Map[64][1] = TMapForm->Edit_Map21_09; Map[64][2] = TMapForm->Edit_Map21_10; Map[64][3] = TMapForm->Edit_Map21_11;
    Map[64][4] = TMapForm->Edit_Map21_12; Map[64][5] = TMapForm->Edit_Map21_13; Map[64][6] = TMapForm->Edit_Map21_14; Map[64][7] = TMapForm->Edit_Map21_15;
    Map[65][0] = TMapForm->Edit_Map21_16; Map[65][1] = TMapForm->Edit_Map21_17; Map[65][2] = TMapForm->Edit_Map21_18; Map[65][3] = TMapForm->Edit_Map21_19;
    Map[65][4] = TMapForm->Edit_Map21_20; Map[65][5] = TMapForm->Edit_Map21_21; Map[65][6] = TMapForm->Edit_Map21_22; Map[65][7] = TMapForm->Edit_Map21_23;
    Map[66][0] = TMapForm->Edit_Map22_00; Map[66][1] = TMapForm->Edit_Map22_01; Map[66][2] = TMapForm->Edit_Map22_02; Map[66][3] = TMapForm->Edit_Map22_03;
    Map[66][4] = TMapForm->Edit_Map22_04; Map[66][5] = TMapForm->Edit_Map22_05; Map[66][6] = TMapForm->Edit_Map22_06; Map[66][7] = TMapForm->Edit_Map22_07;
    Map[67][0] = TMapForm->Edit_Map22_08; Map[67][1] = TMapForm->Edit_Map22_09; Map[67][2] = TMapForm->Edit_Map22_10; Map[67][3] = TMapForm->Edit_Map22_11;
    Map[67][4] = TMapForm->Edit_Map22_12; Map[67][5] = TMapForm->Edit_Map22_13; Map[67][6] = TMapForm->Edit_Map22_14; Map[67][7] = TMapForm->Edit_Map22_15;
    Map[68][0] = TMapForm->Edit_Map22_16; Map[68][1] = TMapForm->Edit_Map22_17; Map[68][2] = TMapForm->Edit_Map22_18; Map[68][3] = TMapForm->Edit_Map22_19;
    Map[68][4] = TMapForm->Edit_Map22_20; Map[68][5] = TMapForm->Edit_Map22_21; Map[68][6] = TMapForm->Edit_Map22_22; Map[68][7] = TMapForm->Edit_Map22_23;
    Map[69][0] = TMapForm->Edit_Map23_00; Map[69][1] = TMapForm->Edit_Map23_01; Map[69][2] = TMapForm->Edit_Map23_02; Map[69][3] = TMapForm->Edit_Map23_03;
    Map[69][4] = TMapForm->Edit_Map23_04; Map[69][5] = TMapForm->Edit_Map23_05; Map[69][6] = TMapForm->Edit_Map23_06; Map[69][7] = TMapForm->Edit_Map23_07;
    Map[70][0] = TMapForm->Edit_Map23_08; Map[70][1] = TMapForm->Edit_Map23_09; Map[70][2] = TMapForm->Edit_Map23_10; Map[70][3] = TMapForm->Edit_Map23_11;
    Map[70][4] = TMapForm->Edit_Map23_12; Map[70][5] = TMapForm->Edit_Map23_13; Map[70][6] = TMapForm->Edit_Map23_14; Map[70][7] = TMapForm->Edit_Map23_15;
    Map[71][0] = TMapForm->Edit_Map23_16; Map[71][1] = TMapForm->Edit_Map23_17; Map[71][2] = TMapForm->Edit_Map23_18; Map[71][3] = TMapForm->Edit_Map23_19;
    Map[71][4] = TMapForm->Edit_Map23_20; Map[71][5] = TMapForm->Edit_Map23_21; Map[71][6] = TMapForm->Edit_Map23_22; Map[71][7] = TMapForm->Edit_Map23_23;

    MapT[0][0] = TMapForm->Edit_MapT00; MapT[0][1] = TMapForm->Edit_MapT01; MapT[0][2] = TMapForm->Edit_MapT02; MapT[0][3] = TMapForm->Edit_MapT03;
    MapT[1][0] = TMapForm->Edit_MapT04; MapT[1][1] = TMapForm->Edit_MapT05; MapT[1][2] = TMapForm->Edit_MapT06; MapT[1][3] = TMapForm->Edit_MapT07;
    MapT[2][0] = TMapForm->Edit_MapT08; MapT[2][1] = TMapForm->Edit_MapT09; MapT[2][2] = TMapForm->Edit_MapT10; MapT[2][3] = TMapForm->Edit_MapT11;
    MapT[3][0] = TMapForm->Edit_MapT12; MapT[3][1] = TMapForm->Edit_MapT13; MapT[3][2] = TMapForm->Edit_MapT14; MapT[3][3] = TMapForm->Edit_MapT15;
    MapT[4][0] = TMapForm->Edit_MapT16; MapT[4][1] = TMapForm->Edit_MapT17; MapT[4][2] = TMapForm->Edit_MapT18; MapT[4][3] = TMapForm->Edit_MapT19;
    MapT[5][0] = TMapForm->Edit_MapT20; MapT[5][1] = TMapForm->Edit_MapT21; MapT[5][2] = TMapForm->Edit_MapT22; MapT[5][3] = TMapForm->Edit_MapT23;

    MapMAF[0][0] = TMapForm->Edit_MapMAF00; MapMAF[0][1] = TMapForm->Edit_MapMAF01; MapMAF[0][2] = TMapForm->Edit_MapMAF02; MapMAF[0][3] = TMapForm->Edit_MapMAF03;
    MapMAF[1][0] = TMapForm->Edit_MapMAF04; MapMAF[1][1] = TMapForm->Edit_MapMAF05; MapMAF[1][2] = TMapForm->Edit_MapMAF06; MapMAF[1][3] = TMapForm->Edit_MapMAF07;
    MapMAF[2][0] = TMapForm->Edit_MapMAF08; MapMAF[2][1] = TMapForm->Edit_MapMAF09; MapMAF[2][2] = TMapForm->Edit_MapMAF10; MapMAF[2][3] = TMapForm->Edit_MapMAF11;
    MapMAF[3][0] = TMapForm->Edit_MapMAF12; MapMAF[3][1] = TMapForm->Edit_MapMAF13; MapMAF[3][2] = TMapForm->Edit_MapMAF14; MapMAF[3][3] = TMapForm->Edit_MapMAF15;
    MapMAF[4][0] = TMapForm->Edit_MapMAF16; MapMAF[4][1] = TMapForm->Edit_MapMAF17; MapMAF[4][2] = TMapForm->Edit_MapMAF18; MapMAF[4][3] = TMapForm->Edit_MapMAF19;
    MapMAF[5][0] = TMapForm->Edit_MapMAF20; MapMAF[5][1] = TMapForm->Edit_MapMAF21; MapMAF[5][2] = TMapForm->Edit_MapMAF22; MapMAF[5][3] = TMapForm->Edit_MapMAF23;

    m_flag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ApdComPort1TriggerAvail(TObject *CP, WORD Count)
{
    unsigned char rdata;
    char buff[8192];

    ApdStatusLight2->Lit = true;
    ApdComPort1->GetBlock(buff, Count);
    for(int i = 0; i < Count; i++) {
        if(sComboBox_Device->ItemIndex == 0) {
           CAN_RxData(buff[i]);
        }
        else {
           RS232_RxData(buff[i]);
        }
    }

    ApdStatusLight2->Lit = false;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TxData(unsigned char sync, unsigned short len, unsigned char command, unsigned char id, unsigned char *data)
{
    AnsiString str = "<-";
    unsigned char check_sum;
    unsigned short length;
    int i;

    if(!ApdComPort1->Open) {
        ShowMessage("Please Open the COM Port First.");
        return;
    }
    ApdStatusLight3->Lit = true;
    length = len+2;
    m_ucTxBuf[0] = sync;
    m_ucTxBuf[1] = sync;
    m_ucTxBuf[2] = 0x7e;
    m_ucTxBuf[3] = length&0xff;
    m_ucTxBuf[4] = (length>>8)&0xff;
    m_ucTxBuf[5] = command;
    m_ucTxBuf[6] = id;
    check_sum = (length&0xff)^((length>>8)&0xff)^command^id;
    for(i = 0; i < len; i++) {
        m_ucTxBuf[i+7] = data[i];
        check_sum = check_sum^m_ucTxBuf[i+7];
    }
    check_sum++;
    m_ucTxBuf[len+7] = check_sum;
    m_ucTxBuf[len+8] = 0x7d;

    ApdComPort1->PutBlock(m_ucTxBuf, len+9);
    ApdStatusLight3->Lit = false;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TxCmd(unsigned char sync, unsigned char command, unsigned char sig)
{
    TxData(sync, 1, command, 0, &sig);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CAN_RxData(unsigned char rdata)
{
    switch(m_ucRxStep) {
        case 0:
                if(rdata == ':' || rdata == '?') {
                    m_ucRxStep = 1;
                    m_ucRxDataCnt = 0;
                }
                else m_ucRxStep = 0;
                break;
        case 1:
                if(rdata == 0x0D) {
                    m_ucRxStep = 0;
                    CAN_DecodeData();
                }
                else {
                    m_ucRxData[m_ucRxDataCnt] = rdata;
                    m_ucRxDataCnt++;
                }
                break;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::RS232_RxData(unsigned char rdata)
{
    unsigned char checksum;

    switch(m_ucRxStep) {
        case 0:
                if(rdata == 0xC6)
                    m_ucSyncCnt++;
                else
                    m_ucSyncCnt = 0;
                if(m_ucSyncCnt == 2) {
                    m_ucRxStep = 1;
                    m_ucSyncCnt = 0;
                }
                break;
        case 1:
                if(rdata == 0x7E)
                    m_ucRxStep = 2;
                else
                    m_ucRxStep = 0;
                break;
        case 2:
                if(!m_ucLenCnt) {
                    ucLength = rdata;
                    m_ucLenCnt++;
                }
                else {
                  ucLength += (rdata<<8);
                  m_ucRxLength = ucLength-2;
                  m_ucLenCnt = 0;
                  m_ucRxStep = 3;
                }
                break;
        case 3:
                m_ucRxCommand = rdata;
                m_ucRxStep = 4;
                break;
        case 4:
                m_ucRxID = rdata;
                m_ucRxDataCnt = 0;
                m_ucRxStep = 5;
                break;
        case 5:
                m_ucRxData[m_ucRxDataCnt] = rdata;
                m_ucRxDataCnt++;
                if(m_ucRxDataCnt == m_ucRxLength)
                    m_ucRxStep = 6;
                break;
        case 6:
                checksum = (ucLength&0xff)^((ucLength>>8)&0xff)^m_ucRxCommand^m_ucRxID;
                for(int i = 0; i < m_ucRxLength; i++)
                    checksum ^= m_ucRxData[i];
                checksum++;
                if(checksum == rdata)
                    m_ucRxStep = 7;
                else
                    m_ucRxStep = 0;
                break;
        case 7:
                m_ucRxStep = 0;
                m_ucSyncCnt = 0;
                m_ucLenCnt = 0;
                if(rdata == 0x7D)
                    RS232_DecodeData();
                break;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::RS232_DecodeData(void)
{
    AnsiString str;
    unsigned int data ,i, j ,k, m;
    double dtmp;
    unsigned short stmp;
    unsigned char htmp, ltmp;
    signed short sstmp, cur_dev = 0, cur_mod = 0, pre_dev = 0, pre_mod = 0;

    stSysValue *pScrValue;
    stLabel *pLabel;
    stFactor *pFactor;
    stSysControl *pParameter;
    stScrMaf *pScrMaf;

    switch(m_ucRxCommand) {
        case 0x10:     // Download Start
            if(m_ucRxData[0] == 1) {
                ProgForm->Memo->Lines->Add("Download Start Ok...");
                ProgForm->m_nProgStauts = 1;
            }
            else {
                if(m_ucRxData[0] == 2)
                    ProgForm->Memo->Lines->Add("Total Packet Size Error...");
                else if(m_ucRxData[0] == 6)
                    ProgForm->Memo->Lines->Add("Check Code Error...");
                else
                    ProgForm->Memo->Lines->Add("Unknown Error...");
                ProgForm->m_nProgStauts = 0;
            }
            break;
        case 0x11:     // Download Progress
            if(m_ucRxData[2] == 1) {
              ProgForm->m_uRPacket = m_ucRxData[0];
              ProgForm->m_uRPacket += m_ucRxData[1]<<8;
              if(ProgForm->m_uSPacket == ProgForm->m_uRPacket)
                ProgForm->m_nProgStauts = 1;
              else
                ProgForm->Memo->Lines->Add("Receive Packet No Error...");
            }
            else {
                if(m_ucRxData[2] == 2)
                    ProgForm->Memo->Lines->Add("Total Packet Size Error...");
                else if(m_ucRxData[2] == 3)
                    ProgForm->Memo->Lines->Add("Packet Sequence Error...");
                else
                    ProgForm->Memo->Lines->Add("Unknown Error...");
                ProgForm->m_nProgStauts = 0;
            }
            break;
        case 0x12:     // Download End
            if(m_ucRxData[0] == 1)
                ProgForm->Memo->Lines->Add("Download End Ok...");
            else if(m_ucRxData[0] == 2)
                ProgForm->Memo->Lines->Add("Total Packet Size Error...");
            else if(m_ucRxData[0] == 3)
                ProgForm->Memo->Lines->Add("Packet Sequence Error...");
            else if(m_ucRxData[0] == 4)
                ProgForm->Memo->Lines->Add("Last Packet No Error...");
            else if(m_ucRxData[0] == 5)
                ProgForm->Memo->Lines->Add("File Size Error...");
            else
                ProgForm->Memo->Lines->Add("Unknown Error...");
            ProgForm->m_nProgStauts = 0;
            break;
        case 0x26:
                pParameter = (stSysControl *)&m_ucRxData[0];
                //PAR 1
                stmp = pParameter->CutMaf;
                SData[0][0]->Text = IntToStr(stmp);
                stmp = pParameter->LowTankLevel;
                SData[0][1]->Text = IntToStr(stmp);
                sstmp = pParameter->CutTankTemp;
                SData[0][2]->Text = IntToStr(sstmp);
                stmp = pParameter->TankType;
                SData[0][3]->Text = IntToStr(stmp);
                //PAR 2
                stmp = pParameter->LowT1;
                SData[1][0]->Text = IntToStr(stmp);
                stmp = pParameter->HighT1;
                SData[1][1]->Text = IntToStr(stmp);
                stmp = pParameter->LowP1;
                SData[1][2]->Text = IntToStr(stmp);
                sstmp = pParameter->HighP1;
                SData[1][3]->Text = IntToStr(sstmp);
                //PAR 3
                dtmp    = (double)pParameter->BatVoltageLow / 100.0;
                SData[2][0]->Text = str.FormatFloat("0.00",dtmp);
                dtmp = (double)pParameter->Temp_Ratio / 100.0;
                SData[2][1]->Text = str.FormatFloat("0.00",dtmp);
                stmp = pParameter->Scr_Mode;
                SData[2][2]->Text = IntToStr(stmp);
                stmp = pParameter->Scr_Period;
                SData[2][3]->Text = IntToStr(stmp);
                //PAR 4
                stmp = pParameter->ScrWaitTime;
                SData[3][0]->Text = IntToStr(stmp);
                stmp = pParameter->ScrPurge;
                SData[3][1]->Text = IntToStr(stmp);
                stmp = pParameter->ScrPurgeTime;
                SData[3][2]->Text = IntToStr(stmp);
                //PAR 5
                dtmp    = (double)pParameter->MafOffset / 1000.0;
                SData[4][0]->Text = str.FormatFloat("0.000",dtmp);
                stmp = pParameter->MafDiameter;
                SData[4][1]->Text = IntToStr(stmp);
                dtmp    = (double)pParameter->MapOffset / 1000.0;
                SData[4][2]->Text = str.FormatFloat("0.000",dtmp);
                stmp = pParameter->Nozzle;
                SData[4][3]->Text = IntToStr(stmp);
                //PAR 6
                stmp = pParameter->Reserved[0];
                SData[5][0]->Text = IntToStr(stmp);
                stmp = pParameter->Reserved[1];
                SData[5][1]->Text = IntToStr(stmp);
                stmp = pParameter->Reserved[2];
                SData[5][2]->Text = IntToStr(stmp);
                stmp = pParameter->Reserved[3];
                SData[5][3]->Text = IntToStr(stmp);
                AlarmWindow("Receive OK!");
                break;
        case 0x27:
                //ShowMessage("Write OK");
                break;
        case 0x28:
                pScrValue = (stSysValue *)&m_ucRxData[0];

                dtmp    =  pScrValue->curXk;
                Edit_Xk->Text = str.FormatFloat("0.000000", dtmp);
                dtmp    = pScrValue->Xc;
                Edit_Xc->Text = str.FormatFloat("0.000000", dtmp);
                dtmp    = pScrValue->H1k;
                Edit_H1->Text = str.FormatFloat("0.00", dtmp);
                dtmp    = pScrValue->H2k;
                Edit_H2->Text = str.FormatFloat("0.00", dtmp);
                dtmp    = pScrValue->Yk;
                Edit_Y->Text = str.FormatFloat("0.00", dtmp);

                htmp = (pScrValue->Version>>8) & 0xff;
                ltmp = pScrValue->Version & 0xff;
                MainForm->StatusBar1->Panels->Items[0]->Text = "ECU Ver : " \
                + IntToStr(htmp/0x10) + IntToStr(htmp%0x10) \
                + "." + IntToStr(ltmp/0x10) + IntToStr(ltmp%0x10);

                m_bManual = pScrValue->SCRMode&0x03;
                if(m_bManual == 0) {
                        Label_Mode->Caption = "AUTO MODE";
                        Edit_mDosing->Enabled     = false;
                        sButton_Inject->Enabled   = false;
                        RadioButton_gh->Enabled   = false;
                        RadioButton_duty->Enabled = false;
                        Edit_mAlpha->Enabled      = false;
                        sButton_Alpha->Enabled    = false;
                        Edit_mTavg->Enabled       = false;
                        Edit_mMaf->Enabled        = false;
                        Edit_mNoxUp->Enabled      = false;
                        Edit_mNoxDn->Enabled      = false;
                        sButton_ManualSet->Enabled = false;
                        sPanel_m1->Enabled        = false;
                        sPanel_m2->Enabled        = false;
                        sPanel_m3->Enabled        = false;
                        sPanel_m4->Enabled        = false;
                        sButton_MODE->Down        = false;
                }
                else if(m_bManual == 1) {
                        Label_Mode->Caption = "MANUAL MODE";
                        Edit_mDosing->Enabled     = false;
                        sButton_Inject->Enabled   = false;
                        RadioButton_gh->Enabled   = false;
                        RadioButton_duty->Enabled = false;
                        Edit_mAlpha->Enabled      = false;
                        sButton_Alpha->Enabled    = false;
                        Edit_mTavg->Enabled       = true;
                        Edit_mMaf->Enabled        = true;
                        Edit_mNoxUp->Enabled      = true;
                        Edit_mNoxDn->Enabled      = true;
                        sButton_ManualSet->Enabled = true;
                        sPanel_m1->Enabled        = true;
                        sPanel_m2->Enabled        = true;
                        sPanel_m3->Enabled        = true;
                        sPanel_m4->Enabled        = true;
                        sButton_MODE->Down = false;
                }
                else if(m_bManual == 2) {
                        Label_Mode->Caption = "ALPHA MODE";
                        Edit_mDosing->Enabled     = false;
                        sButton_Inject->Enabled   = false;
                        RadioButton_gh->Enabled   = false;
                        RadioButton_duty->Enabled = false;
                        Edit_mAlpha->Enabled      = true;
                        sButton_Alpha->Enabled    = true;
                        Edit_mTavg->Enabled       = false;
                        Edit_mMaf->Enabled        = false;
                        Edit_mNoxUp->Enabled      = false;
                        Edit_mNoxDn->Enabled      = false;
                        sButton_ManualSet->Enabled = false;
                        sPanel_m1->Enabled        = false;
                        sPanel_m2->Enabled        = false;
                        sPanel_m3->Enabled        = false;
                        sPanel_m4->Enabled        = false;
                        sButton_MODE->Down        = false;
                }
                else {
                        Label_Mode->Caption = "DOSING MODE";
                        Edit_mDosing->Enabled     = true;
                        sButton_Inject->Enabled   = true;
                        RadioButton_gh->Enabled   = true;
                        RadioButton_duty->Enabled = true;
                        Edit_mAlpha->Enabled      = false;
                        sButton_Alpha->Enabled    = false;
                        Edit_mTavg->Enabled       = false;
                        Edit_mMaf->Enabled        = false;
                        Edit_mNoxUp->Enabled      = false;
                        Edit_mNoxDn->Enabled      = false;
                        sButton_ManualSet->Enabled = false;
                        sPanel_m1->Enabled        = false;
                        sPanel_m2->Enabled        = false;
                        sPanel_m3->Enabled        = false;
                        sPanel_m4->Enabled        = false;
                        sButton_MODE->Down        = false;
                }
                m_MapX = pScrValue->Map_X;
                TMapForm->Edit_MapX->Text = IntToStr(m_MapX);
                m_MapY = pScrValue->Map_Y;
                TMapForm->Edit_MapY->Text = IntToStr(m_MapY);
                cur_dev = m_MapY / 8;
                cur_mod = m_MapY % 8;
                pre_dev = m_PreMapY / 8;
                pre_mod = m_PreMapY % 8;
                //colortmp = Map[(m_MapX*3)+cur_dev][cur_mod]->Color;
                if((m_MapX != m_PreMapX) || (m_MapY != m_PreMapY))
                {
                        //Map[(m_MapX*3)+cur_dev][cur_mod]->Color = clRed;
                        //Map[(m_PreMapX*3)+pre_dev][pre_mod]->Color = colortmp;
                        Map[(m_PreMapX*3)+pre_dev][pre_mod]->Font->Style = TFontStyles();
                        Map[(m_MapX*3)+cur_dev][cur_mod]->Font->Style = TFontStyles()<< fsBold;
                }
                else {
                        //Map[(m_MapX*3)+cur_dev][cur_mod]->Color = colortmp;
                        Map[(m_MapX*3)+cur_dev][cur_mod]->Font->Style = TFontStyles()<< fsBold;
                }
                m_PreMapX = m_MapX;
                m_PreMapY = m_MapY;

                for(int i = 0; i < 8; i++) {
                        ltmp = pScrValue->SystemError.B;
                        if((ltmp>>i)&0x01) RLED[0][i]->Color = clRed;
                        else RLED[0][i]->Color = clBtnFace;
                        ltmp = pScrValue->SystemCheck.W & 0xff;
                        if((ltmp>>i)&0x01) RLED[2][i]->Color = clFuchsia;
                        else RLED[2][i]->Color = clBtnFace;
                        ltmp = pScrValue->SystemSignal.W & 0xff;
                        if((ltmp>>i)&0x01) RLED[4][i]->Color = clBlue;
                        else RLED[4][i]->Color = clBtnFace;
                }
                for(int i = 0; i < 2; i++) {
                        htmp = (pScrValue->SystemSignal.W>>8) &0xff;
                        if((htmp>>i)&0x01) RLED[5][i]->Color = clBlue;
                        else RLED[5][i]->Color = clBtnFace;
                }
                htmp = (pScrValue->SystemSignal.W>>8) &0xff;
                if((htmp>>3) & 0x01) { //
                        m_ScrFlag = 1;
                        sButton_SCRDsiable->Down = true;
                        sButton_SCRDsiable->Caption = "Scr Start";
                }
                else {
                        m_ScrFlag = 0;
                        sButton_SCRDsiable->Down = false;
                        sButton_SCRDsiable->Caption = "Scr Stop";
                }
                if((htmp>>5) & 0x01) {
                        m_uiAirPChk = 1;
                        sButton_AirPressure->Down = true;
                        sButton_AirPressure->Caption = "Flushing Off";
                }
                else {
                        m_uiAirPChk = 0;
                        sButton_AirPressure->Down = false;
                        sButton_AirPressure->Caption = "Flushing On";
                }
                m_uiNOxIn  = pScrValue->Noxppm[0];
                Edit_NOxIn->Text  = IntToStr(m_uiNOxIn);

                m_uiNOxOut = pScrValue->Noxppm[1];
                Edit_NOxOut->Text = IntToStr(m_uiNOxOut);

                m_uiMAF    = pScrValue->MafKg_H;
                Edit_MAF->Text    = IntToStr(m_uiMAF);
                TMapForm->Edit_MAF->Text = IntToStr(m_uiMAF);

                m_Maf_T    = pScrValue->Maf_temp;
                Edit_Maf_T->Text = IntToStr(m_Maf_T);

                if(m_uiNOxIn > 0)
                        dtmp = (double)(m_uiNOxIn-m_uiNOxOut)/(double)m_uiNOxIn*100.0;
                else
                        dtmp = 0;
                Edit_NOxRealReduce->Text = str.FormatFloat("0.00", dtmp);
                m_uiTVal[0] = pScrValue->T1_temp;
                m_uiTVal[1] = pScrValue->T2_temp;
                m_uiTVal[2] = pScrValue->Tavg_temp;
                m_UreaTemp  = pScrValue->TankTemp;
                if(m_uiTVal[0]<750)
                        Edit_T1->Text = IntToStr(m_uiTVal[0]);
                else
                        Edit_T1->Text = "NC";
                if(m_uiTVal[1]<750)
                        Edit_T2->Text = IntToStr(m_uiTVal[1]);
                else
                        Edit_T2->Text = "NC";
                if(m_uiTVal[2]<750) {
                        TMapForm->Edit_Tavg->Text = IntToStr(m_uiTVal[2]);
                        Edit_Tavg->Text = IntToStr(m_uiTVal[2]);
                }
                else {
                        TMapForm->Edit_Tavg->Text = "NC";
                        Edit_Tavg->Text = "NC";
                }
                Edit_TankTemp->Text = IntToStr(m_UreaTemp);

                m_dDosing    = pScrValue->DosingRatehouer;
                Edit_Dosing->Text = str.FormatFloat("0.00",m_dDosing);
                m_dSumDosing = pScrValue->TotalDosingRate;
                Edit_Sum->Text = str.FormatFloat("0.00",m_dSumDosing);

                m_dKI  = pScrValue->Kl;
                Edit_KI->Text = str.FormatFloat("0.000000",m_dKI);
                m_dKP  = pScrValue->Kp;
                Edit_KP->Text = str.FormatFloat("0.00000",m_dKP);

                m_dVelocity  = pScrValue->Vk;
                Edit_Velocity->Text = str.FormatFloat("0.0000",m_dVelocity);
                m_dGamma  = pScrValue->Gamma;
                Edit_Gamma->Text = str.FormatFloat("0.0000",m_dGamma);

                m_dBattery  = pScrValue->BattVoltage / 100.0;
                Edit_B0->Text= str.FormatFloat("0.00",m_dBattery);
                m_UreaLevelP = pScrValue->TankLevelP;
                Edit_TankLevelP->Text = IntToStr(m_UreaLevelP);
                m_uiPVal = pScrValue->P1_bar;
                Edit_P1->Text = IntToStr(m_uiPVal);
                m_Duty = pScrValue->DosingDuty;
                Edit_DosingDuty->Text = IntToStr(m_Duty);

                m_Xh    = pScrValue->curXh;
                Edit_Xh->Text = str.FormatFloat("0.000000",m_Xh);
                m_Factor = pScrValue->StatusAlpha;
                Edit_Pulse->Text= str.FormatFloat("0.00",m_Factor);
                m_Reduce  = pScrValue->NoxReduction;
                Edit_NOxReduce->Text = IntToStr(m_Reduce);

                sstmp  = (unsigned short)pScrValue->Formula1;
                Edit_1PPM->Text = IntToStr(sstmp);
                sstmp  = (unsigned short)pScrValue->Formula2;
                Edit_2PPM->Text = IntToStr(sstmp);
                sstmp  = (unsigned short)pScrValue->Formula3;
                Edit_3PPM->Text = IntToStr(sstmp);
                dtmp    = (unsigned short)pScrValue->NH3Up;
                Edit_NH3Up->Text = str.FormatFloat("0.00",dtmp);
                dtmp    = (unsigned short)pScrValue->NH3Dn;
                Edit_NH3Dn->Text = str.FormatFloat("0.00",dtmp);
                break;
        case 0x2A:
                pFactor = (stFactor *)&m_ucRxData[0];
                dtmp    = (double)pFactor->Ka / 100.0;
                FactorForm->Edit_Ka->Text = str.FormatFloat("0.00",dtmp);
                ltmp    = pFactor->Kd;
                FactorForm->Edit_Kd->Text = IntToStr(ltmp);
                ltmp    = pFactor->Kr;
                FactorForm->Edit_Kr->Text = IntToStr(ltmp);
                dtmp    = (double)pFactor->OFA / 100.0;
                FactorForm->Edit_OFA->Text = str.FormatFloat("0.00",dtmp);
                dtmp    = (double)pFactor->S1 / 10.0;
                FactorForm->Edit_OmegaS1->Text = str.FormatFloat("0.0",dtmp);
                dtmp    = (double)pFactor->S2 / 1000.0;
                FactorForm->Edit_OmegaS2->Text = str.FormatFloat("0.000",dtmp);
                dtmp    = (double)pFactor->L / 1000.0;
                FactorForm->Edit_Length->Text = str.FormatFloat("0.000",dtmp);
                dtmp    = (double)pFactor->Xsp / 100.0;
                FactorForm->Edit_Xsp->Text = str.FormatFloat("0.00",dtmp);
                dtmp    = (double)pFactor->Coeff / 100.0;
                FactorForm->Edit_Coeff->Text = str.FormatFloat("0.00",dtmp);
                dtmp    = (double)pFactor->ScrArea / 100000.0;
                FactorForm->Edit_ScrArea->Text = str.FormatFloat("0.00000",dtmp);
                dtmp    = (double)pFactor->Kl / 1000000.0;
                FactorForm->Edit_Kl->Text = str.FormatFloat("0.000000",dtmp);
                dtmp    = (double)pFactor->Kp / 100000.0;
                FactorForm->Edit_Kp->Text = str.FormatFloat("0.00000",dtmp);
                AlarmWindow("Receive OK!");                
                break;
        case 0x2B:
                ShowMessage("Write OK");
                break;
        case 0x2C:
                pLabel = (stLabel *)&m_ucRxData[0];
                
                for(i=0; i<6; i++) {
                  for(j=0; j<4; j++) {
                      MapT[i][j]->Text = IntToStr(pLabel->BaseTemp[(i*4)+j]);
                      MapMAF[i][j]->Text = IntToStr(pLabel->BaseMaf[(i*4)+j]);
                  }
                }
                Popup->Close();
                AlarmWindow("Receive OK!");                
                break;
        case 0x2D:
                ShowMessage("Write OK");
                break;
        case 0x2E:
                Popup->Show();
                for(i=0; i<72; i++) {
                  for(j=0; j<8; j++) {
                    dtmp = (double)m_ucRxData[(i*8) + j]/100.0;
                    Map[i][j]->Text = str.FormatFloat("0.00",dtmp);
                    TMapForm->ProgressBar_Map->Position = (i*8) + j;
                  }
                }
                TMapForm->ProgressBar_Map->Position = 0;
                TMapForm->sButton_ReadTSVClick(NULL);
                TMapForm->sButton_ColorSetClick(NULL);
                break;
        case 0x2F:
                ShowMessage("Write OK");
                break;
        case 0x30:
                stmp    = (m_ucRxData[1]<<8)+m_ucRxData[0];
                CalibForm->Edit_ADC1->Text = IntToStr(stmp);
                dtmp    = (signed char)m_ucRxData[2]/100.0;
                CalibForm->Edit_Offset1->Text = str.FormatFloat("0.00",dtmp);

                stmp    = (m_ucRxData[4]<<8)+m_ucRxData[3];
                CalibForm->Edit_ADC2->Text = IntToStr(stmp);
                dtmp    = (signed char)m_ucRxData[5]/100.0;
                CalibForm->Edit_Offset2->Text = str.FormatFloat("0.00",dtmp);

                stmp    = (m_ucRxData[7]<<8)+m_ucRxData[6];
                CalibForm->Edit_ADC3->Text = IntToStr(stmp);
                dtmp    = (signed char)m_ucRxData[8]/100.0;
                CalibForm->Edit_Offset3->Text = str.FormatFloat("0.00",dtmp);

                stmp    = (m_ucRxData[10]<<8)+m_ucRxData[9];
                CalibForm->Edit_ADC4->Text = IntToStr(stmp);
                dtmp    = (signed char)m_ucRxData[11]/100.0;
                CalibForm->Edit_Offset4->Text = str.FormatFloat("0.00",dtmp);

                stmp    = (m_ucRxData[13]<<8)+m_ucRxData[12];
                CalibForm->Edit_ADC5->Text = IntToStr(stmp);
                dtmp    = (signed char)m_ucRxData[14]/100.0;
                CalibForm->Edit_Offset5->Text = str.FormatFloat("0.00",dtmp);

                AlarmWindow("Receive OK!");
                break;
        case 0x31:
                ShowMessage("Write OK");
                break;
        case 0x32:
                stmp    = (m_ucRxData[1]<<8)+m_ucRxData[0];
                CalibForm->Edit_CalADC->Text = IntToStr(stmp);
                dtmp    = (double)((m_ucRxData[3]<<8)+m_ucRxData[2])/100.0;
                CalibForm->Edit_CalOffset->Text = str.FormatFloat("0.00",dtmp);
                break;
        case 0x33:
                CalibForm->CB_TNo->ItemIndex = m_ucRxID;
                if(m_ucRxData[0]==0)
                        CalibForm->CheckBox_TNo->Checked = false;
                else
                        CalibForm->CheckBox_TNo->Checked = true;
                stmp = (m_ucRxData[2]<<8)+m_ucRxData[1];
                CalibForm->Edit_TRangeL->Text = IntToStr(stmp);
                stmp = (m_ucRxData[4]<<8)+m_ucRxData[3];
                CalibForm->Edit_TRangeH->Text = IntToStr(stmp);
                stmp = (m_ucRxData[6]<<8)+m_ucRxData[5];
                CalibForm->Edit_TCalLow->Text = IntToStr(stmp);
                stmp = (m_ucRxData[8]<<8)+m_ucRxData[7];
                CalibForm->Edit_TCalHigh->Text = IntToStr(stmp);
                AlarmWindow("Receive OK!");
                break;
        case 0x34:
                ShowMessage("Write OK");
                break;
        case 0x35:
                CalibForm->CB_TNo->ItemIndex = m_ucRxID;
                stmp    = (m_ucRxData[1]<<8)+m_ucRxData[0];
                CalibForm->Edit_TCalADC->Text = IntToStr(stmp);
                break;
        case 0x36:
                CalibForm->CB_PNo->ItemIndex = m_ucRxID;
                if(m_ucRxData[0]==0)
                        CalibForm->CheckBox_PNo->Checked = false;
                else
                        CalibForm->CheckBox_PNo->Checked = true;
                stmp = (m_ucRxData[2]<<8)+m_ucRxData[1];
                CalibForm->Edit_PRangeL->Text = IntToStr(stmp);
                stmp = (m_ucRxData[4]<<8)+m_ucRxData[3];
                CalibForm->Edit_PRangeH->Text = IntToStr(stmp);
                stmp = (m_ucRxData[6]<<8)+m_ucRxData[5];
                CalibForm->Edit_PCalLow->Text = IntToStr(stmp);
                stmp = (m_ucRxData[8]<<8)+m_ucRxData[7];
                CalibForm->Edit_PCalHigh->Text = IntToStr(stmp);
                AlarmWindow("Receive OK!");
                break;
        case 0x37:
                ShowMessage("Write OK");
                break;
        case 0x38:
                CalibForm->CB_PNo->ItemIndex = m_ucRxID;
                stmp    = (m_ucRxData[1]<<8)+m_ucRxData[0];
                CalibForm->Edit_PCalADC->Text = IntToStr(stmp);
                break;
        case 0x39:
                CalibForm->CB_NOx->ItemIndex = m_ucRxID;
                if(m_ucRxData[0]==0)
                        CalibForm->CheckBox_NOx->Checked = false;
                else
                        CalibForm->CheckBox_NOx->Checked = true;
                AlarmWindow("Receive OK!");
                break;
        case 0x3A:
                ShowMessage("Write OK");
                break;
        case 0x3B:
                pScrMaf = (stScrMaf *)&m_ucRxData[0];

                CalibForm->CMAF[0]->Text = IntToStr(pScrMaf->Adc[0]);
                CalibForm->CMAF[1]->Text = IntToStr(pScrMaf->Adc[1]);
                CalibForm->CMAF[2]->Text = IntToStr(pScrMaf->Adc[2]);
                CalibForm->CMAF[3]->Text = IntToStr(pScrMaf->Adc[3]);
                CalibForm->CMAF[4]->Text = IntToStr(pScrMaf->Adc[4]);
                CalibForm->CMAF[5]->Text = IntToStr(pScrMaf->Adc[5]);
                CalibForm->CMAF[6]->Text = IntToStr(pScrMaf->Adc[6]);
                CalibForm->CMAF[7]->Text = IntToStr(pScrMaf->Adc[7]);
                CalibForm->CMAF[8]->Text = IntToStr(pScrMaf->Adc[8]);
                CalibForm->CMAF[9]->Text = IntToStr(pScrMaf->Adc[9]);
                CalibForm->CMAF[10]->Text = IntToStr(pScrMaf->Value[0]);
                CalibForm->CMAF[11]->Text = IntToStr(pScrMaf->Value[1]);
                CalibForm->CMAF[12]->Text = IntToStr(pScrMaf->Value[2]);
                CalibForm->CMAF[13]->Text = IntToStr(pScrMaf->Value[3]);
                CalibForm->CMAF[14]->Text = IntToStr(pScrMaf->Value[4]);
                CalibForm->CMAF[15]->Text = IntToStr(pScrMaf->Value[5]);
                CalibForm->CMAF[16]->Text = IntToStr(pScrMaf->Value[6]);
                CalibForm->CMAF[17]->Text = IntToStr(pScrMaf->Value[7]);
                CalibForm->CMAF[18]->Text = IntToStr(pScrMaf->Value[8]);
                CalibForm->CMAF[19]->Text = IntToStr(pScrMaf->Value[9]);
                AlarmWindow("Receive OK!");
                CalibForm->Button_DrawClick(NULL);
                break;
        case 0x3D:
                ShowMessage("Write OK");
                break;
        case 0x3E:
                str = "20";
                str = str+IntToStr(m_ucRxData[0]/10)+IntToStr(m_ucRxData[0]%10)+"/";
                str = str+IntToStr(m_ucRxData[1]/10)+IntToStr(m_ucRxData[1]%10)+"/";
                str = str+IntToStr(m_ucRxData[2]/10)+IntToStr(m_ucRxData[2]%10)+"    ";
                str = str+IntToStr(m_ucRxData[3]/10)+IntToStr(m_ucRxData[3]%10)+":";
                str = str+IntToStr(m_ucRxData[4]/10)+IntToStr(m_ucRxData[4]%10)+":";
                str = str+IntToStr(m_ucRxData[5]/10)+IntToStr(m_ucRxData[5]%10);
                RTCForm->Panel_RTC->Caption = str;
                ShowMessage("Write OK");
                break;
    }

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ReceiveDataProcess(unsigned short id, unsigned char length, unsigned char *data)
{
    int idx;
    AnsiString str;
    double dtmp;
    unsigned long ltmp;
    unsigned int colortmp;
    unsigned short stmp;
    unsigned char ctmp;

    signed short sstmp, cur_dev = 0, cur_mod = 0, pre_dev = 0, pre_mod = 0;
    int i, j;

    if(id == 0x10) { // Download Start
        if(data[0] == 0) {
          ProgForm->m_nProgStauts = 1;
          ProgForm->Memo->Lines->Add("DL Packet Count Ok....");
            }
        else {
          ProgForm->m_nProgStauts = 0;
        }
    }
    else if(id == 0x11) { // CRC Send
        if(data[0] == 0) {
          ProgForm->m_nProgStauts = 1;
          ProgForm->Memo->Lines->Add("DL CRC Response Ok....");
        }
        else {
          ProgForm->m_nProgStauts = 0;
        }
    }
    else if(id == 0x12) { // Program Send
        if(data[0] == 0) {
          ProgForm->m_nProgStauts = 1;
        }
        else {
          ProgForm->m_nProgStauts = 0;
        }
    }
    else if(id == 0x13) {     // Download End
        if(data[0] == 0) {
          ProgForm->m_nProgStauts = 1;
          ProgForm->Memo->Lines->Add("DL soucess Ok....");
        }
        else {
          ProgForm->m_nProgStauts = 0;
        }
    }
    else if(id == 0x98) {     // Xk, Xc
        dtmp    = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0])/1000000.0;
        Edit_Xk->Text = str.FormatFloat("0.000000", dtmp);
        dtmp    = (double)((data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4])/1000000.0;
        Edit_Xc->Text = str.FormatFloat("0.000000", dtmp);
    }
    else if(id == 0x99) {     // H1, H2
        dtmp    = (double)((data[1]<<8)+data[0])/100.0;
        Edit_H1->Text = str.FormatFloat("0.00", dtmp);
        dtmp    = (double)((data[3]<<8)+data[2])/100.0;
        Edit_H2->Text = str.FormatFloat("0.00", dtmp);
        dtmp    = (double)((data[5]<<8)+data[4])/100.0;
        Edit_Y->Text = str.FormatFloat("0.00", dtmp);

    }
    else if(id == 0x100) {     // ECU Version
        MainForm->StatusBar1->Panels->Items[0]->Text = "ECU Ver : " \
         + IntToStr(data[1]/0x10) + IntToStr(data[1]%0x10) \
         + "." + IntToStr(data[0]/0x10) + IntToStr(data[0]%0x10);

        m_bManual = data[2]&0x03;
        if(m_bManual == 0) {
            Label_Mode->Caption = "AUTO MODE";
            Edit_mDosing->Enabled     = false;
            sButton_Inject->Enabled   = false;
            RadioButton_gh->Enabled   = false;
            RadioButton_duty->Enabled = false;
            Edit_mAlpha->Enabled      = false;
            sButton_Alpha->Enabled    = false;
            Edit_mTavg->Enabled       = false;
            Edit_mMaf->Enabled        = false;
            Edit_mNoxUp->Enabled      = false;
            Edit_mNoxDn->Enabled      = false;
            sButton_ManualSet->Enabled = false;
            sPanel_m1->Enabled        = false;
            sPanel_m2->Enabled        = false;
            sPanel_m3->Enabled        = false;
            sPanel_m4->Enabled        = false;
            sButton_MODE->Down        = false;
        }
        else if(m_bManual == 1) {
            Label_Mode->Caption = "MANUAL MODE";
            Edit_mDosing->Enabled     = false;
            sButton_Inject->Enabled   = false;
            RadioButton_gh->Enabled   = false;
            RadioButton_duty->Enabled = false;
            Edit_mAlpha->Enabled      = false;
            sButton_Alpha->Enabled    = false;
            Edit_mTavg->Enabled       = true;
            Edit_mMaf->Enabled        = true;
            Edit_mNoxUp->Enabled      = true;
            Edit_mNoxDn->Enabled      = true;
            sButton_ManualSet->Enabled = true;
            sPanel_m1->Enabled        = true;
            sPanel_m2->Enabled        = true;
            sPanel_m3->Enabled        = true;
            sPanel_m4->Enabled        = true;
            sButton_MODE->Down = false;
        }
        else if(m_bManual == 2) {
            Label_Mode->Caption = "ALPHA MODE";
            Edit_mDosing->Enabled     = false;
            sButton_Inject->Enabled   = false;
            RadioButton_gh->Enabled   = false;
            RadioButton_duty->Enabled = false;
            Edit_mAlpha->Enabled      = true;
            sButton_Alpha->Enabled    = true;
            Edit_mTavg->Enabled       = false;
            Edit_mMaf->Enabled        = false;
            Edit_mNoxUp->Enabled      = false;
            Edit_mNoxDn->Enabled      = false;
            sButton_ManualSet->Enabled = false;
            sPanel_m1->Enabled        = false;
            sPanel_m2->Enabled        = false;
            sPanel_m3->Enabled        = false;
            sPanel_m4->Enabled        = false;
            sButton_MODE->Down        = false;
        }
        else {
            Label_Mode->Caption = "DOSING MODE";
            Edit_mDosing->Enabled     = true;
            sButton_Inject->Enabled   = true;
            RadioButton_gh->Enabled   = true;
            RadioButton_duty->Enabled = true;
            Edit_mAlpha->Enabled      = false;
            sButton_Alpha->Enabled    = false;
            Edit_mTavg->Enabled       = false;
            Edit_mMaf->Enabled        = false;
            Edit_mNoxUp->Enabled      = false;
            Edit_mNoxDn->Enabled      = false;
            sButton_ManualSet->Enabled = false;
            sPanel_m1->Enabled        = false;
            sPanel_m2->Enabled        = false;
            sPanel_m3->Enabled        = false;
            sPanel_m4->Enabled        = false;
            sButton_MODE->Down        = false;
        }

        m_MapX = data[3];
        m_MapY = data[4];
        TMapForm->Edit_MapX->Text = IntToStr(m_MapX);
        TMapForm->Edit_MapY->Text = IntToStr(m_MapY);

        cur_dev = m_MapY / 8;
        cur_mod = m_MapY % 8;
        pre_dev = m_PreMapY / 8;
        pre_mod = m_PreMapY % 8;

        //colortmp = Map[(m_MapX*3)+cur_dev][cur_mod]->Color;
        if((m_MapX != m_PreMapX) || (m_MapY != m_PreMapY))
        {
            //Map[(m_MapX*3)+cur_dev][cur_mod]->Color = clRed;
            //Map[(m_PreMapX*3)+pre_dev][pre_mod]->Color = colortmp;
            Map[(m_PreMapX*3)+pre_dev][pre_mod]->Font->Style = TFontStyles();            
            Map[(m_MapX*3)+cur_dev][cur_mod]->Font->Style = TFontStyles()<< fsBold;
        }
        else {
            //Map[(m_MapX*3)+cur_dev][cur_mod]->Color = colortmp;
            Map[(m_MapX*3)+cur_dev][cur_mod]->Font->Style = TFontStyles()<< fsBold;
        }
        m_PreMapX = m_MapX;
        m_PreMapY = m_MapY;
        m_uiAirPChk = data[5];
    }
    else if(id == 0x101) {     // Alarm
      for(int i = 0; i < 8; i++) {
          if((data[0]>>i)&0x01) RLED[0][i]->Color = clRed;
          else RLED[0][i]->Color = clBtnFace;
          if((data[2]>>i)&0x01) RLED[2][i]->Color = clFuchsia;
          else RLED[2][i]->Color = clBtnFace;
          if((data[4]>>i)&0x01) RLED[4][i]->Color = clBlue;
          else RLED[4][i]->Color = clBtnFace;
      }
      for(int i = 0; i < 2; i++) {
          if((data[3]>>i)&0x01) RLED[3][i]->Color = clFuchsia;
          else RLED[3][i]->Color = clBtnFace;
          if((data[5]>>i)&0x01) RLED[5][i]->Color = clBlue;
          else RLED[5][i]->Color = clBtnFace;
      }
      if((data[4]>>3) & 0x01) { //
         m_ScrFlag = 1;
         sButton_SCRDsiable->Down = true;
         sButton_SCRDsiable->Caption = "Scr Start";
      }
      else {
         m_ScrFlag = 0;
         sButton_SCRDsiable->Down = false;
         sButton_SCRDsiable->Caption = "Scr Stop";
      }
      if((data[4]>>5) & 0x01) {
         m_uiAirPChk = 1;
         sButton_AirPressure->Down = true;
         sButton_AirPressure->Caption = "Flushing Off";
      }
      else {
         m_uiAirPChk = 0;
         sButton_AirPressure->Down = false;
         sButton_AirPressure->Caption = "Flushing On";
      }
    }
    else if(id == 0x102) {     // NOx In, NOx Out, MAF
        m_uiNOxIn  = (data[1]<<8)+data[0];
        Edit_NOxIn->Text  = IntToStr(m_uiNOxIn);

        m_uiNOxOut = (data[3]<<8)+data[2];
        Edit_NOxOut->Text = IntToStr(m_uiNOxOut);

        m_uiMAF    = (data[5]<<8)+data[4];
        Edit_MAF->Text    = IntToStr(m_uiMAF);
        TMapForm->Edit_MAF->Text = IntToStr(m_uiMAF);

        Edit_Maf_T->Text = IntToStr(m_Maf_T);
        m_Maf_T    = (data[7]<<8)+data[6];
        if(m_uiNOxIn > 0)
            dtmp = (double)(m_uiNOxIn-m_uiNOxOut)/(double)m_uiNOxIn*100.0;
        else
            dtmp = 0;
        Edit_NOxRealReduce->Text = str.FormatFloat("0.00", dtmp);
    }
    else if(id == 0x103) {     // T1, T2, T3, UREA
        m_uiTVal[0] = (data[1]<<8)+data[0];
        m_uiTVal[1] = (data[3]<<8)+data[2];
        m_uiTVal[2] = (data[5]<<8)+data[4];
        m_UreaTemp  = (data[7]<<8)+data[6];
        if(m_uiTVal[0]<750)
            Edit_T1->Text = IntToStr(m_uiTVal[0]);
        else
            Edit_T1->Text = "NC";
        if(m_uiTVal[1]<750)
            Edit_T2->Text = IntToStr(m_uiTVal[1]);
        else
            Edit_T2->Text = "NC";
        if(m_uiTVal[2]<750) {
            TMapForm->Edit_Tavg->Text = IntToStr(m_uiTVal[2]);
            Edit_Tavg->Text = IntToStr(m_uiTVal[2]);
        }
        else {
            TMapForm->Edit_Tavg->Text = "NC";
            Edit_Tavg->Text = "NC";
        }
        Edit_TankTemp->Text = IntToStr(m_UreaTemp);
    }
    else if(id == 0x104) {     // Dosing, SumDosing
        m_dDosing    = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0])/100.0;
        m_dSumDosing = (double)((data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4])/100.0;
        Edit_Dosing->Text = str.FormatFloat("0.00",m_dDosing);
        Edit_Sum->Text = str.FormatFloat("0.00",m_dSumDosing);
    }
    else if(id == 0x105) {
        m_dKI  = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0])/1000000.0;
        m_dKP  = (double)((data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4])/100000.0;
        Edit_KI->Text = str.FormatFloat("0.000000",m_dKI);
        Edit_KP->Text = str.FormatFloat("0.00000",m_dKP);
    }
    else if(id == 0x106) {
        m_dVelocity  = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0])/10000.0;
        m_dGamma  = (double)((data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4])/10000.0;
        Edit_Velocity->Text = str.FormatFloat("0.0000",m_dVelocity);
        Edit_Gamma->Text = str.FormatFloat("0.0000",m_dGamma);
    }
    else if(id == 0x107) {
        m_dBattery  = (double)((data[1]<<8)+data[0])/100.0;
        m_UreaLevelP = (data[3]<<8)+data[2];
        m_uiPVal = (data[5]<<8)+data[4];
        m_Duty = (data[7]<<8)+data[6];
        Edit_B0->Text= str.FormatFloat("0.00",m_dBattery);
        Edit_TankLevelP->Text = IntToStr(m_UreaLevelP);
        Edit_P1->Text = IntToStr(m_uiPVal);
        Edit_DosingDuty->Text = IntToStr(m_Duty);
    }
    else if(id == 0x108) {
        m_Xh    = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0])/1000000.0;
        Edit_Xh->Text = str.FormatFloat("0.000000",m_Xh);
        m_Factor = (double)((data[5]<<8)+data[4])/100.0;
        Edit_Pulse->Text= str.FormatFloat("0.00",m_Factor);
        m_Reduce  = (data[7]<<8)+data[6];
//        Edit_NOxRealReduce->Text = str.FormatFloat("0.0",dtmp);
        Edit_NOxReduce->Text = IntToStr(m_Reduce);
    }
    else if(id == 0x116) {
        str = "20";
        str = str+IntToStr(data[0]/10)+IntToStr(data[0]%10)+"/";
        str = str+IntToStr(data[1]/10)+IntToStr(data[1]%10)+"/";
        str = str+IntToStr(data[2]/10)+IntToStr(data[2]%10)+"    ";
        str = str+IntToStr(data[3]/10)+IntToStr(data[3]%10)+":";
        str = str+IntToStr(data[4]/10)+IntToStr(data[4]%10)+":";
        str = str+IntToStr(data[5]/10)+IntToStr(data[5]%10);
        RTCForm->Panel_RTC->Caption = str;
    }
    else if(id == 0x117) {
        sstmp  = (data[1]<<8)+data[0];
        Edit_1PPM->Text = IntToStr(sstmp);
        sstmp  = (data[3]<<8)+data[2];
        Edit_2PPM->Text = IntToStr(sstmp);
        sstmp  = (data[5]<<8)+data[4];
        Edit_3PPM->Text = IntToStr(sstmp);
    }
    else if(id == 0x118) {
        dtmp    = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0]) / 100.0;
        Edit_NH3Up->Text = str.FormatFloat("0.00",dtmp);
        dtmp    = (double)((data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4]) / 100.0;
        Edit_NH3Dn->Text = str.FormatFloat("0.00",dtmp);
    }
    else if(id == 0x121) {     // Ka Kd
        dtmp    = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0]) / 100.0;
        FactorForm->Edit_Ka->Text = str.FormatFloat("0.00",dtmp);
        ltmp    = (data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4];
        FactorForm->Edit_Kd->Text = IntToStr(ltmp);
    }
    else if(id == 0x122) {     // Kr OFA
        ltmp    = (data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0];
        FactorForm->Edit_Kr->Text = IntToStr(ltmp);
        dtmp    = (double)((data[5]<<8)+data[4])/100.0;
        FactorForm->Edit_OFA->Text = str.FormatFloat("0.00",dtmp);
    }
    else if(id == 0x123) {     // OmegaS1, OmegaS2
        dtmp    = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0]) / 10.0;
        FactorForm->Edit_OmegaS1->Text = str.FormatFloat("0.0",dtmp);
        dtmp    = (double)((data[5]<<8)+data[4])/1000.0;
        FactorForm->Edit_OmegaS2->Text = str.FormatFloat("0.000",dtmp);
    }
    else if(id == 0x124) {    // L, A
        dtmp    = (double)((data[1]<<8)+data[0]) / 1000.0;
        FactorForm->Edit_Length->Text = str.FormatFloat("0.000",dtmp);
        dtmp    = (double)data[2] / 100.0;
        FactorForm->Edit_Xsp->Text = str.FormatFloat("0.00",dtmp);
        dtmp    = (double)data[3] / 100.0;
        FactorForm->Edit_Coeff->Text = str.FormatFloat("0.00",dtmp);
        dtmp    = (double)((data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4]) / 100000.0;
        FactorForm->Edit_ScrArea->Text = str.FormatFloat("0.00000",dtmp);
        AlarmWindow("Receive OK!");
    }
    else if(id == 0x125) {    // Kl, Kp
        dtmp    = (double)((data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0])/1000000.0;
        FactorForm->Edit_Kl->Text = str.FormatFloat("0.000000",dtmp);
        dtmp    = (double)((data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4])/100000.0;
        FactorForm->Edit_Kp->Text = str.FormatFloat("0.00000",dtmp);
    }
    else if(id == 0x131) {
        stmp    = (data[1]<<8)+data[0];
        CalibForm->Edit_ADC1->Text = IntToStr(stmp);
        stmp    = (data[3]<<8)+data[2];
        CalibForm->Edit_ADC2->Text = IntToStr(stmp);
        stmp    = (data[5]<<8)+data[4];
        CalibForm->Edit_ADC3->Text = IntToStr(stmp);
        stmp    = (data[7]<<8)+data[6];
        CalibForm->Edit_ADC4->Text = IntToStr(stmp);
    }
    else if(id == 0x132) {
        stmp    = (data[1]<<8)+data[0];
        CalibForm->Edit_ADC5->Text = IntToStr(stmp);
        dtmp    = (signed char)data[3]/100.0;
        CalibForm->Edit_Offset1->Text = str.FormatFloat("0.00",dtmp);
        dtmp    = (signed char)data[4]/100.0;
        CalibForm->Edit_Offset2->Text = str.FormatFloat("0.00",dtmp);
        dtmp    = (signed char)data[5]/100.0;
        CalibForm->Edit_Offset3->Text = str.FormatFloat("0.00",dtmp);
        dtmp    = (signed char)data[6]/100.0;
        CalibForm->Edit_Offset4->Text = str.FormatFloat("0.00",dtmp);
        dtmp    = (signed char)data[7]/100.0;
        CalibForm->Edit_Offset5->Text = str.FormatFloat("0.00",dtmp);
        AlarmWindow("Receive OK!");
    }
    else if(id == 0x133) {
        stmp    = (data[1]<<8)+data[0];
        CalibForm->Edit_CalADC->Text = IntToStr(stmp);
        dtmp    = (double)((data[3]<<8)+data[2])/100.0;
        CalibForm->Edit_CalOffset->Text = str.FormatFloat("0.00",dtmp);
    }
    else if(id == 0x135) {
       CalibForm->CB_TNo->ItemIndex = data[0];
       if(data[1]==0)
          CalibForm->CheckBox_TNo->Checked = false;
       else
          CalibForm->CheckBox_TNo->Checked = true;
       stmp = (data[5]<<8)+data[4];
       CalibForm->Edit_TRangeL->Text = IntToStr(stmp);
       stmp = (data[7]<<8)+data[6];
       CalibForm->Edit_TRangeH->Text = IntToStr(stmp);
    }
    else if(id == 0x136) {
       CalibForm->CB_TNo->ItemIndex = data[0];
       stmp = (data[5]<<8)+data[4];
       CalibForm->Edit_TCalLow->Text = IntToStr(stmp);
       stmp = (data[7]<<8)+data[6];
       CalibForm->Edit_TCalHigh->Text = IntToStr(stmp);
       AlarmWindow("Receive OK!");
    }
    else if(id == 0x137) {
        CalibForm->CB_TNo->ItemIndex = data[0];
        stmp    = (data[3]<<8)+data[2];
        CalibForm->Edit_TCalADC->Text = IntToStr(stmp);
    }
    else if(id == 0x139) {
         CalibForm->CB_PNo->ItemIndex = data[0];
         if(data[1]==0)
            CalibForm->CheckBox_PNo->Checked = false;
         else
            CalibForm->CheckBox_PNo->Checked = true;
         stmp = (data[5]<<8)+data[4];
         CalibForm->Edit_PRangeL->Text = IntToStr(stmp);
         stmp = (data[7]<<8)+data[6];
         CalibForm->Edit_PRangeH->Text = IntToStr(stmp);
    }
    else if(id == 0x13A) {
        if(data[0])
           CalibForm->CB_PNo->ItemIndex = 1;
        else
           CalibForm->CB_PNo->ItemIndex = 0;
         stmp = (data[5]<<8)+data[4];
         CalibForm->Edit_PCalLow->Text = IntToStr(stmp);
         stmp = (data[7]<<8)+data[6];
         CalibForm->Edit_PCalHigh->Text = IntToStr(stmp);
         AlarmWindow("Receive OK!");
    }
    else if(id == 0x13B) {
        if(data[0])
           CalibForm->CB_PNo->ItemIndex = 1;
        else
           CalibForm->CB_PNo->ItemIndex = 0;
        stmp    = (data[3]<<8)+data[2];
        CalibForm->Edit_PCalADC->Text = IntToStr(stmp);
    }
    else if(id == 0x13D) {
         CalibForm->CB_NOx->ItemIndex = data[0];
         if(data[1]==0)
            CalibForm->CheckBox_NOx->Checked = false;
         else
            CalibForm->CheckBox_NOx->Checked = true;
    }
    else if(id == 0x141) {      // CutMaf, Cut T1, Cut P1, Slip
        stmp = (data[1]<<8)+data[0];
        SData[0][0]->Text = IntToStr(stmp);
        stmp = (data[3]<<8)+data[2];
        SData[0][1]->Text = IntToStr(stmp);
        sstmp = (data[5]<<8)+data[4];
        SData[0][2]->Text = IntToStr(sstmp);
        stmp = (data[7]<<8)+data[6];
        SData[0][3]->Text = IntToStr(stmp);
    }
    else if(id == 0x142) {      // TLow, Thigh, TankLevel, Cut TankT
        stmp = (data[1]<<8)+data[0];
        SData[1][0]->Text = IntToStr(stmp);
        stmp = (data[3]<<8)+data[2];
        SData[1][1]->Text = IntToStr(stmp);
        stmp = (data[5]<<8)+data[4];
        SData[1][2]->Text = IntToStr(stmp);
        sstmp = (data[7]<<8)+data[6];
        SData[1][3]->Text = IntToStr(sstmp);
    }
    else if(id == 0x143) {      // LowBat, Tratio, ScrMode
        dtmp    = (double)((data[1]<<8)+data[0])/100.0;
        SData[2][0]->Text = str.FormatFloat("0.00",dtmp);
        dtmp = (double)data[2]/100.0;
        SData[2][1]->Text = str.FormatFloat("0.00",dtmp);
        stmp = data[3];
        SData[2][2]->Text = IntToStr(stmp);
        stmp = data[4];
        SData[2][3]->Text = IntToStr(stmp);
    }
    else if(id ==0x144) {       // WaitTime, ScrPeriod, Xsp, nox_Coeff
        stmp = (data[1]<<8)+data[0];
        SData[3][0]->Text = IntToStr(stmp);
        stmp = data[2];
        SData[3][1]->Text = IntToStr(stmp);
        stmp = (data[5]<<8)+data[4];
        SData[3][2]->Text = IntToStr(stmp);
        stmp = data[6];
        SData[3][3]->Text = IntToStr(stmp);
    }
    else if(id == 0x145) {      //Maf Offset, Map Offset
        dtmp    = (double)((data[1]<<8)+data[0])/1000.0;
        SData[4][0]->Text = str.FormatFloat("0.000",dtmp);
        stmp = (data[3]<<8)+data[2];
        SData[4][1]->Text = IntToStr(stmp);
        dtmp    = (double)((data[5]<<8)+data[4])/1000.0;
        SData[4][2]->Text = str.FormatFloat("0.000",dtmp);
        stmp = data[6];
        SData[4][3]->Text = IntToStr(stmp);
        AlarmWindow("Receive OK!");
    }
    else if(id == 0x151) {
        stmp    = (data[1]<<8)+data[0];
        CalibForm->CMAF[0]->Text = IntToStr(stmp);
        stmp    = (data[3]<<8)+data[2];
        CalibForm->CMAF[1]->Text = IntToStr(stmp);
        stmp    = (data[5]<<8)+data[4];
        CalibForm->CMAF[2]->Text = IntToStr(stmp);
        stmp    = (data[7]<<8)+data[6];
        CalibForm->CMAF[3]->Text = IntToStr(stmp);
    }
    else if(id == 0x152) {
        stmp    = (data[1]<<8)+data[0];
        CalibForm->CMAF[4]->Text = IntToStr(stmp);
        stmp    = (data[3]<<8)+data[2];
        CalibForm->CMAF[5]->Text = IntToStr(stmp);
        stmp    = (data[5]<<8)+data[4];
        CalibForm->CMAF[6]->Text = IntToStr(stmp);
        stmp    = (data[7]<<8)+data[6];
        CalibForm->CMAF[7]->Text = IntToStr(stmp);
    }
    else if(id == 0x153) {
        stmp    = (data[1]<<8)+data[0];
        CalibForm->CMAF[8]->Text = IntToStr(stmp);
        stmp    = (data[3]<<8)+data[2];
        CalibForm->CMAF[9]->Text = IntToStr(stmp);
        stmp    = (data[5]<<8)+data[4];
        CalibForm->CMAF[10]->Text = IntToStr(stmp);
        stmp    = (data[7]<<8)+data[6];
        CalibForm->CMAF[11]->Text = IntToStr(stmp);
    }
    else if(id == 0x154) {
        stmp    = (data[1]<<8)+data[0];
        CalibForm->CMAF[12]->Text = IntToStr(stmp);
        stmp    = (data[3]<<8)+data[2];
        CalibForm->CMAF[13]->Text = IntToStr(stmp);
        stmp    = (data[5]<<8)+data[4];
        CalibForm->CMAF[14]->Text = IntToStr(stmp);
        stmp    = (data[7]<<8)+data[6];
        CalibForm->CMAF[15]->Text = IntToStr(stmp);
    }
    else if(id == 0x155) {
        stmp    = (data[1]<<8)+data[0];
        CalibForm->CMAF[16]->Text = IntToStr(stmp);
        stmp    = (data[3]<<8)+data[2];
        CalibForm->CMAF[17]->Text = IntToStr(stmp);
        stmp    = (data[5]<<8)+data[4];
        CalibForm->CMAF[18]->Text = IntToStr(stmp);
        stmp    = (data[7]<<8)+data[6];
        CalibForm->CMAF[19]->Text = IntToStr(stmp);
        AlarmWindow("Receive OK!");
    }    
    else if(id > 0x160 && id < 0x1A9) {
        idx = (int)(id - 0x161);
        dtmp = (double)data[0]/100.0;
        Map[idx][0]->Text = str.FormatFloat("0.00",dtmp);
        dtmp = (double)data[1]/100.0;
        Map[idx][1]->Text = str.FormatFloat("0.00",dtmp);
        dtmp = (double)data[2]/100.0;
        Map[idx][2]->Text = str.FormatFloat("0.00",dtmp);
        dtmp = (double)data[3]/100.0;
        Map[idx][3]->Text = str.FormatFloat("0.00",dtmp);
        dtmp = (double)data[4]/100.0;
        Map[idx][4]->Text = str.FormatFloat("0.00",dtmp);
        dtmp = (double)data[5]/100.0;
        Map[idx][5]->Text = str.FormatFloat("0.00",dtmp);
        dtmp = (double)data[6]/100.0;
        Map[idx][6]->Text = str.FormatFloat("0.00",dtmp);
        dtmp = (double)data[7]/100.0;
        Map[idx][7]->Text = str.FormatFloat("0.00",dtmp);
        TMapForm->ProgressBar_Map->Position = idx;

        if(idx==0 && MapRead==1)
           Popup->Show();
        else if(idx==71 && MapRead==1) {
            MapRead = 0;
            TMapForm->sButton_ReadTSVClick(NULL);
        }
        else if(idx==71 && MapRead!=1) {
            TMapForm->ProgressBar_Map->Position = 0;
            TMapForm->sButton_ColorSetClick(NULL);
            AlarmWindow("Receive OK!");
        }
    }
    else if((id > 0x1B0) && (id < 0x1B7)) {
        idx = (int)(id - 0x1B1);
        stmp = (data[1]<<8)+data[0];
        MapT[idx][0]->Text = IntToStr(stmp);
        stmp = (data[3]<<8)+data[2];
        MapT[idx][1]->Text = IntToStr(stmp);
        stmp = (data[5]<<8)+data[4];
        MapT[idx][2]->Text = IntToStr(stmp);
        stmp = (data[7]<<8)+data[6];
        MapT[idx][3]->Text = IntToStr(stmp);
        TMapForm->ProgressBar_Map->Position = (idx + 1) * 6;
    }
    else if((id > 0x1C0) && (id < 0x1C7)) {
        idx = id - 0x1C1;
        stmp = (data[1]<<8)+data[0];
        MapMAF[idx][0]->Text = IntToStr(stmp);
        stmp = (data[3]<<8)+data[2];
        MapMAF[idx][1]->Text = IntToStr(stmp);
        stmp = (data[5]<<8)+data[4];
        MapMAF[idx][2]->Text = IntToStr(stmp);
        stmp = (data[7]<<8)+data[6];
        MapMAF[idx][3]->Text = IntToStr(stmp);
        TMapForm->ProgressBar_Map->Position = (idx + 1) * 6 + 36;
        if(idx==5) {
            Popup->Close();
            TMapForm->ProgressBar_Map->Position = 0;
            TMapForm->sButton_ColorSetClick(NULL);
            AlarmWindow("Receive OK!");
        }
    }
    else if(id == 0x1D0) {
        ltmp = (data[7]<<24)+(data[6]<<16)+(data[5]<<8)+data[4];
        IPForm->Edit_IP1->Text = IntToStr(data[0]);
        IPForm->Edit_IP2->Text = IntToStr(data[1]);
        IPForm->Edit_IP3->Text = IntToStr(data[2]);
        IPForm->Edit_IP4->Text = IntToStr(data[3]);
        IPForm->Edit_Port->Text = IntToStr(ltmp);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TranceiveDataProcess(unsigned short id, unsigned char length, unsigned char *data)
{
    unsigned char sdata[256];
    st_std stdid;
    int i;

    stdid.id = id;
    sdata[0] = 0x08;
    sdata[1] = stdid.b.h;
    sdata[2] = stdid.b.l;

    for(i = 0;i < length; i++) {
        sdata[3+i] = *(data+i);
    }
    TxCANData('W', length+3, sdata);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CAN_DecodeData(void)
{
    AnsiString str;
    double tmp;
    unsigned char rdata[256], canid[4], length;
    unsigned char chk, crc, hex, scode;
    st_std stdid;
    int i, can_config = 0;

    m_ucRxCommand = m_ucRxData[0];
    crc = m_ucRxCommand;            // command
    switch(m_ucRxCommand) {
        case 'Z':
        case 'Y':
                m_bAscii[0] = m_ucRxData[1];       // spec code
                crc += m_bAscii[0];
                m_bAscii[1] = m_ucRxData[2];
                crc += m_bAscii[1];
                scode = asciiHexToByte(m_bAscii);
                length    = (scode & 0x0F);
                if((scode>>0 & 0x01) == 0x01)
                   CanForm->CheckBox_CANWaring->Checked = true;
                else
                   CanForm->CheckBox_CANWaring->Checked = false;
                if((scode>>1 & 0x01) == 0x01)
                   CanForm->CheckBox_CANError->Checked = true;
                else
                   CanForm->CheckBox_CANError->Checked = false;
                if((scode>>2 & 0x01) == 0x01)
                   CanForm->CheckBox_BUSOff->Checked = true;
                else
                   CanForm->CheckBox_BUSOff->Checked = false;
                if((scode>>3 & 0x01) == 0x01)
                   CanForm->CheckBox_AutoRest->Checked = true;
                else
                   CanForm->CheckBox_AutoRest->Checked = false;
                if((scode>>4 & 0x01) != 0x01) {   // CAN 2.0A?
                  for(i = 0; i < 2; i++) {      // ID
                     m_bAscii[0] = m_ucRxData[3+(2*i)];
                     crc += m_bAscii[0];
                     m_bAscii[1] = m_ucRxData[3+(2*i)+1];
                     crc += m_bAscii[1];
                     hex = asciiHexToByte(m_bAscii);
                     m_bBitTime[i] = hex;
                  }
                  for(i = 0; i < 2; i++) {      // Arbitor
                     m_bAscii[0] = m_ucRxData[7+(2*i)];
                     crc += m_bAscii[0];
                     m_bAscii[1] = m_ucRxData[7+(2*i)+1];
                     crc += m_bAscii[1];
                     hex = asciiHexToByte(m_bAscii);
                     canid[i] = hex;
                  }
                  for(i = 0; i < 2; i++) {      // Mask
                     m_bAscii[0] = m_ucRxData[11+(2*i)];
                     crc += m_bAscii[0];
                     m_bAscii[1] = m_ucRxData[11+(2*i)+1];
                     crc += m_bAscii[1];
                     hex = asciiHexToByte(m_bAscii);
                     canid[i] = hex;
                  }
                  m_bAscii[0] = m_ucRxData[15+(length*2)];
                  m_bAscii[1] = m_ucRxData[15+(length*2)+1];
                  chk = asciiHexToByte(m_bAscii);

                  if(crc == chk) {
                     for(i=0; i<9; i++) {
                        if((BitTime[i][0] == m_bBitTime[0]) && (BitTime[i][1] == m_bBitTime[1])) {
                          can_config = i;
                          break;
                        }
                     }
                     CanForm->sComboBox_CAN_Protocol->ItemIndex = (scode>>4 & 0x01);
                     CanForm->sComboBox_CAN_BSP->ItemIndex = can_config;
                     ApdStatusLight_Link->Lit = true;
                     ApdStatusLight_Link1->Lit = true;
                  }
                  else {
                     ApdStatusLight_Link->Lit = false;
                     ApdStatusLight_Link1->Lit = false;
                     AlarmWindow("Receive Fault");
                  }
                }
                else AlarmWindow("CAN Only 2.0A");
                break;
        case 'U':
        case 'W':
                m_bAscii[0] = m_ucRxData[1];       // spec code
                crc += m_bAscii[0];
                m_bAscii[1] = m_ucRxData[2];
                crc += m_bAscii[1];
                scode = asciiHexToByte(m_bAscii);
                if((scode>>5 & 0x01) != 0x01) {   // CAN 2.0A?
                  for(i = 0; i < 2; i++) {      // ID
                    m_bAscii[0] = m_ucRxData[3+(2*i)];
                    crc += m_bAscii[0];
                    m_bAscii[1] = m_ucRxData[3+(2*i)+1];
                    crc += m_bAscii[1];
                    hex = asciiHexToByte(m_bAscii);
                    canid[i] = hex;
                  }
                  stdid.b.h = canid[0];
                  stdid.b.l = canid[1];
                  length    = (scode & 0x0F);
                  for(i = 0; i < length; i++) {
                    m_bAscii[0] = m_ucRxData[7+(2*i)];
                    crc += m_bAscii[0];
                    m_bAscii[1] = m_ucRxData[7+(2*i)+1];
                    crc += m_bAscii[1];
                    hex = asciiHexToByte(m_bAscii);
                    rdata[i] = hex;
                  }
                  m_bAscii[0] = m_ucRxData[7+(length*2)];
                  m_bAscii[1] = m_ucRxData[7+(length*2)+1];
                  chk = asciiHexToByte(m_bAscii);
                  if(crc == chk) {
                     ReceiveDataProcess(stdid.id, length, rdata);
                  }
                }
                else AlarmWindow("CAN Only 2.0A");

                break;
        case 'G':
                crc = m_ucRxData[0];
                m_bAscii[0] = m_ucRxData[1];
                crc += m_ucRxData[1];
                m_bAscii[1] = m_ucRxData[2];
                crc += m_ucRxData[2];
                scode = asciiHexToByte(m_bAscii);
                m_bAscii[0] = m_ucRxData[3];
                m_bAscii[1] = m_ucRxData[4];
                chk = asciiHexToByte(m_bAscii);
                if(crc != chk) 
                   AlarmWindow("Receive Fault");
                break;
        case 'R':
                m_bAscii[0] = m_ucRxData[1];
                crc += m_bAscii[0];
                m_bAscii[1] = m_ucRxData[2];
                crc += m_bAscii[1];
                scode = asciiHexToByte(m_bAscii);
                m_bAscii[0] = m_ucRxData[3];
                m_bAscii[1] = m_ucRxData[4];
                chk = asciiHexToByte(m_bAscii);
                if(crc == chk) {
                   if(scode == 0x00)
                      AlarmWindow("CAN Reset");
                   else
                      AlarmWindow("CAN Bus-Off Auto Reset");
                }
                else
                   AlarmWindow("Receive Fault");
                break;
        case 'I':
                m_bAscii[0] = m_ucRxData[1];
                crc += m_bAscii[0];
                m_bAscii[1] = m_ucRxData[2];
                crc += m_bAscii[1];
                scode = asciiHexToByte(m_bAscii);
                m_bAscii[0] = m_ucRxData[3];
                m_bAscii[1] = m_ucRxData[4];
                chk = asciiHexToByte(m_bAscii);
                if(crc == chk) {
                    if((scode>>0 & 0x01) == 0x01)
                        AlarmWindow("CAN Bus-off error");
                    else if((scode>>1 & 0x01) == 0x01)
                        AlarmWindow("CAN Receiver error");
                    else if((scode>>2 & 0x01) == 0x01)
                        AlarmWindow("CAN Transmiter error");
                    else if((scode>>3 & 0x01) == 0x01)
                        AlarmWindow("CAN Receiver warning");
                    else if((scode>>4 & 0x01) == 0x01)
                        AlarmWindow("CAN Transmiter warning");
                }
                else
                   ShowMessage("Receive Fault");
                break;
        case 'V':
                m_bAscii[0] = m_ucRxData[1];
                crc += m_bAscii[0];
                m_bAscii[1] = m_ucRxData[2];
                crc += m_bAscii[1];
                scode = asciiHexToByte(m_bAscii);
                m_bAscii[0] = m_ucRxData[3];
                m_bAscii[1] = m_ucRxData[4];
                chk = asciiHexToByte(m_bAscii);
                if(crc == chk)
                   AlarmWindow("CAN Ver " + IntToStr((scode>>4)& 0x0f) + '.' + IntToStr(scode & 0x0f));
                else
                   AlarmWindow("Receive Fault");
                break;

    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TxCANData(unsigned char command, unsigned char length, unsigned char *data)
{
    AnsiString str = "<-";
    int i;
    unsigned char sdata[1024], check_sum;
    unsigned char ascii[2], sum, crc;

    if(!ApdComPort1->Open) {
        ShowMessage("Please Open the COM Port First.");
        MapRead = 0;
        return;
    }
    ApdStatusLight3->Lit = true;

    sdata[0] = ':';
    sdata[1] = command;
    for(i = 0; i < length; i++) {
        byteToAsciiHex(*(data+i), ascii);
        sdata[2+(i*2)] = ascii[0];
        sdata[2+(i*2)+1] = ascii[1];
    }
    sum = command;
    for(i = 0; i < (length*2); i++) {
        sum += sdata[2+i];
    }
    crc = (sum & 0xff);
    byteToAsciiHex(crc, ascii);
    sdata[2+(length*2)] = ascii[0];
    sdata[2+(length*2)+1] = ascii[1];

    sdata[2+(length*2)+2] = 0x0D;
    ApdComPort1->PutBlock(sdata, (length*2)+5);

    ApdStatusLight3->Lit = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AlarmWindow(AnsiString str)
{
    StatusForm->SetAlarmMsg(str);
    StatusForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AppException(TObject *Sender, Exception *E)
{

  if(E->Message == "ie_Open - device already open")
  {
     ApdComPort1->Open = false;
     ShowMessage("Device already open");
   }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    TIniFile *ini;

    if (FileExists(ChangeFileExt(Application->ExeName,".INI"))) {
      ini = new TIniFile(ChangeFileExt(Application->ExeName,".INI"));
      m_asFName = ini->ReadString("PATH","경로",0);
      delete ini;
    }
    else {
      ini=new TIniFile(ChangeFileExt(Application->ExeName,".INI"));
      m_asFName = ExtractFilePath(Application->ExeName);
      ini->WriteString("PATH","경로",m_asFName);
      delete ini;
    }

    Application->OnException = AppException;
    SetErrorMode(SEM_FAILCRITICALERRORS);        
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_SettingsClick(TObject *Sender)
{
    CalibForm->Show();
    CalibForm->sButton_MAFReadClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_ExitClick(TObject *Sender)
{
    ApdComPort1->Open = false;
    ApdStatusLight1->Lit = false;
    Close();
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_GraphClick(TObject *Sender)
{
    GraphForm->Show();
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_TMapClick(TObject *Sender)
{
    TMapForm->Show();
    TMapForm->sButton_ReadMapClick(NULL);
    MapRead = 1;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_ParamClick(TObject *Sender)
{
    ParmForm->Show();
    ParmForm->sButton_ParReadClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_FactorClick(TObject *Sender)
{
    FactorForm->Show();
    FactorForm->sButton_PidReadClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_ComOpenClick(TObject *Sender)
{
    int index, port, baud;
    unsigned char sdata[10];
    unsigned char rdata[4096];
    TIniFile *ini;

    if (FileExists(ChangeFileExt(Application->ExeName,".INI"))) {
      ini = new TIniFile(ChangeFileExt(Application->ExeName,".INI"));
      m_asFName = ini->ReadString("PATH","경로",0);
      delete ini;

      if (!DirectoryExists(m_asFName)) {
        ShowMessage("저장 경로가 정상적이지 않습니다.\n경로를 설정하세요!");
      }
      else {
            if(sComboBox_Device->ItemIndex < 0)
              sComboBox_Device->ItemIndex = 0;
            index = sComboBox_ComPort->ItemIndex;
            ApdComPort1->Open = false;
            port = StrToInt(sComboBox_ComPort->Items->Strings[index].SubString(4,sComboBox_ComPort->Items->Strings[index].Length()-3));
            baud = StrToInt(sComboBox_Baud->Text);
            ApdComPort1->ComNumber = port;
            ApdComPort1->Baud = baud;
            ApdComPort1->Open = true;
            sButton_ComClose->Down = false;
            sButton_ComOpen->Down = true;
            ApdStatusLight1->Lit = true;
            sdata[0] = 0x11;
            TxCANData('G', 1, sdata);
            Sleep(1);
            TxCANData('Y', 0, rdata);
            DispTimer->Enabled = true;
      }
    }
    else {
      ini=new TIniFile(ChangeFileExt(Application->ExeName,".INI"));
      m_asFName = ExtractFilePath(Application->ExeName);
      ini->WriteString("PATH","경로",m_asFName);
      delete ini;
      AlarmWindow("새로운 저장 경로를 생성 했습니다.");
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_ComCloseClick(TObject *Sender)
{
    unsigned char sdata[10];
    sdata[0] = 0x10;
    TxCANData('G', 1, sdata);
    ApdComPort1->Open = false;
    sButton_ComClose->Down = true;
    sButton_ComOpen->Down = false;
    ApdStatusLight_Link->Lit = false;
    ApdStatusLight_Link1->Lit = false;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_MODEClick(TObject *Sender)
{
    unsigned char sdata[8];

    if(sComboBox_Mode->ItemIndex < 0)
        sComboBox_Mode->ItemIndex = 0;
    m_bManual = sComboBox_Mode->ItemIndex;
    sdata[0] = m_bManual;
    sdata[1] = m_uiAirPChk;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;


    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x110, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x24, 0, sdata);
    }
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_MANUALClick(TObject *Sender)
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


    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x110, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x24, 0, sdata);
    }
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_ACOffClick(TObject *Sender)
{
    unsigned char sdata[8];

    sdata[0] = 0x00;
    sdata[1] = 0x00;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    TranceiveDataProcess(0x111, 8, sdata);
}
//---------------------------------------------------------------------------
   
void __fastcall TMainForm::sButton_InjectClick(TObject *Sender)
{
    unsigned char val, st_Inject, st_Pump;
    unsigned char sdata[8], cmd;
    double dtmp;
    long ltmp;

    if(sComboBox_Mode->ItemIndex < 0)
        sComboBox_Mode->ItemIndex = 0;
    if(sButton_Inject->Down == true) {
        sButton_VauleSet->Enabled = false;
        sButton_Inject->Caption = "Pump OFF";
        sButton_Inject->Down = false;

        sdata[0] = 0x00;
        sdata[1] = 0x00;
        sdata[2] = 0x00;
        sdata[3] = 0x00;
        sdata[4] = 0x00;
        sdata[5] = 0x00;
        sdata[6] = 0x00;
        sdata[7] = 0x00;
    }
    else {
        sButton_VauleSet->Enabled = true;
        if(RadioButton_gh->Checked == true)
           cmd = 0x01;
        else
           cmd = 0x02;

        sButton_Inject->Caption = "Pump ON";
        sButton_Inject->Down = true;
        sdata[0] = cmd;
        sdata[1] = 0x00;
        sdata[2] = 0x00;
        sdata[3] = 0x00;
        dtmp = (StrToFloat(Edit_mDosing->Text)*100.0);
        ltmp = (long)dtmp;
        sdata[4] = ltmp&0xff;
        sdata[5] = (ltmp>>8)&0xff;
        sdata[6] = (ltmp>>16)&0xff;
        sdata[7] = (ltmp>>24)&0xff;
    }

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x112, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x21, 0, sdata);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_VauleSetClick(TObject *Sender)
{
    unsigned char sdata[8], cmd;
    double dtmp;
    long ltmp;

    if(sComboBox_Mode->ItemIndex < 0)
        sComboBox_Mode->ItemIndex = 0;
    if(RadioButton_gh->Checked == true)
       cmd = 0x01;
    else
       cmd = 0x02;

    sButton_Inject->Caption = "Pump ON";
    sButton_Inject->Down = true;
    sdata[0] = cmd;
    sdata[1] = 0x00;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    dtmp = (StrToFloat(Edit_mDosing->Text)*100.0);
    ltmp = (long)dtmp;
    sdata[4] = ltmp&0xff;
    sdata[5] = (ltmp>>8)&0xff;
    sdata[6] = (ltmp>>16)&0xff;
    sdata[7] = (ltmp>>24)&0xff;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x112, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x21, 0, sdata);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_InjectOffClick(TObject *Sender)
{
    unsigned char sdata[8];

    sdata[0] = 0x00;
    sdata[1] = 0x00;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    TranceiveDataProcess(0x112, 8, sdata);
}
//---------------------------------------------------------------------------
  
void __fastcall TMainForm::sButton_InitGegenCntClick(TObject *Sender)
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

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x113, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x29, 0, sdata);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_AlarmClearClick(TObject *Sender)
{
    unsigned char sdata[8];

    sdata[0] = 0x00;
    sdata[1] = 0x01;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x113, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x29, 0, sdata);
    }
}
//---------------------------------------------------------------------------
            
void __fastcall TMainForm::Monitor_ON(void)
{
    unsigned char sdata[10];

    sdata[0] = 0x11;
    TxCANData('G', 1, sdata);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Monitor_OFF(void)
{
    unsigned char sdata[10];

    sdata[0] = 0x10;
    TxCANData('G', 1, sdata);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_AirPressureClick(TObject *Sender)
{
    unsigned char sdata[8];

    if(m_uiAirPChk) {
        m_uiAirPChk = 0;
        sButton_AirPressure->Down = true;
    }
    else {
        m_uiAirPChk = 1;
        sButton_AirPressure->Down = false;
    }

    sdata[0] = m_bManual;
    sdata[1] = m_uiAirPChk;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x110, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x24, 0, sdata);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_AlphaClick(TObject *Sender)
{
    double dtmp;
    unsigned short stmp;
    unsigned char sdata[8];

    if(sComboBox_Mode->ItemIndex < 0)
        sComboBox_Mode->ItemIndex = 0;

    dtmp = StrToFloat(Edit_mAlpha->Text)*100.0;
    stmp = (unsigned short)dtmp;
    sdata[0] = stmp & 0xff;
    sdata[1] = (stmp>>8) & 0xff;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x114, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x22, 0, sdata);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sButton_RTCClick(TObject *Sender)
{
    RTCForm->Show();
    RTCForm->sButton_RTCSetClick(NULL);
}

void __fastcall TMainForm::sButton_ECUClick(TObject *Sender)
{
    ProgForm->Show();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Save_data(void)
{
  if((fout = fopen(dir.c_str(),"a")) != NULL) {
        fprintf(fout,"\n");
        // Date
        fprintf(fout,"%02d.%02d.%02d\t", (Year-2000), Month, Day);
        // Time
        fprintf(fout,"%02d:%02d:%02d\t", Hour, Min, Sec);
        // NOxIn NOxOut MAF T1 T2 Tavg Inject TotalInject
        fprintf(fout, "%d\t%d\t%d\t%d\t%d\t%d\t%1.2f\t%1.2f\t",m_uiNOxIn,m_uiNOxOut,m_uiMAF,m_uiTVal[0],m_uiTVal[1],m_uiTVal[2],m_dDosing, m_dSumDosing);
        // Battery Level p1 factor
        fprintf(fout,"%2.2f\t%d\t%d\t%1.2f\t", m_dBattery, m_UreaLevelP, m_uiPVal, m_Factor);
        // Error Check Signal,
        fprintf(fout, "%d\t%d\t%d", ErrFlag,ChkFlag,SigFlag);
        fclose(fout);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DispTimerTimer(TObject *Sender)
{
    TDateTime dtPresent = Now();
    DecodeDate(dtPresent,Year,Month,Day);
    DecodeTime(dtPresent,Hour,Min,Sec,MSec);

    if(PreDay != Day)
    {
        memset(RecFileName, 0 , 12);
	RecFileName[0] = ((Year-2000)/10) + 0x30;
        RecFileName[1] = ((Year-2000)%10) + 0x30;
        RecFileName[2] = (Month/10) + 0x30;
        RecFileName[3] = (Month%10) + 0x30;
        RecFileName[4] = (Day/10) + 0x30;
        RecFileName[5] = (Day%10) + 0x30;
        RecFileName[6] = '.';
        RecFileName[7] = 'x';
        RecFileName[8] = 'l';
        RecFileName[9] = 's';

        strcpy(dir.c_str(), RecFileName);
        dir = m_asFName+dir.c_str();
       if((fchk = fopen(dir.c_str(), "r")) == NULL) {
          fout = fopen(dir.c_str(), "w");
          fprintf(fout, "Date\tTime\tNOxIn\tNOxOut\tMAF\tT1\tT2\tTavg\tInject\tTotalInject\t");
          fprintf(fout, "Battery\tLevel\tP1\tfactor\tError\tCheck\tSignal");
          fclose(fout);
        }
        fclose(fchk);
        PreDay = Day;
    }
    else {
      Save_data();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::sButton_SCRDsiableClick(TObject *Sender)
{
    unsigned char sdata[8];

    if(m_ScrFlag) {
        m_ScrFlag = 0x00;
        sButton_SCRDsiable->Down = true;
    }
    else {
        m_ScrFlag = 0x01;
        sButton_SCRDsiable->Down = false;
    }

    sdata[0] = m_ScrFlag & 0xff;
    sdata[1] = 0x00;
    sdata[2] = 0x00;
    sdata[3] = 0x00;
    sdata[4] = 0x00;
    sdata[5] = 0x00;
    sdata[6] = 0x00;
    sdata[7] = 0x00;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x147, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x25, 0, sdata);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::sButton_ManualSetClick(TObject *Sender)
{
    unsigned short stmp;
    unsigned char sdata[8];

    if(sComboBox_Mode->ItemIndex < 0)
        sComboBox_Mode->ItemIndex = 0;
    stmp = StrToInt(Edit_mTavg->Text);
    sdata[0] = stmp & 0xff;
    sdata[1] = (stmp>>8) & 0xff;
    stmp = StrToInt(Edit_mMaf->Text);
    sdata[2] = stmp & 0xff;
    sdata[3] = (stmp>>8) & 0xff;
    stmp = StrToInt(Edit_mNoxUp->Text);
    sdata[4] = stmp & 0xff;
    sdata[5] = (stmp>>8) & 0xff;
    stmp = StrToInt(Edit_mNoxDn->Text);
    sdata[6] = stmp & 0xff;
    sdata[7] = (stmp>>8) & 0xff;

    if(MainForm->sComboBox_Device->ItemIndex == 0) {
        TranceiveDataProcess(0x111, 8, sdata);
    }
    else {
        TxData(0xC6, 8, 0x23, 0, sdata);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::sButton_CANConfigClick(TObject *Sender)
{
    CanForm->Show();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::sButton1Click(TObject *Sender)
{
    IPForm->Show();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::sComboBox_DeviceChange(TObject *Sender)
{
   if(sComboBox_Device->ItemIndex == 0)
        sComboBox_Baud->ItemIndex = 1;
   else
        sComboBox_Baud->ItemIndex = 0;
}
//---------------------------------------------------------------------------

