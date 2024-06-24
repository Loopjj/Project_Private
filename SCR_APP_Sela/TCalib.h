//---------------------------------------------------------------------------

#ifndef TCalibH
#define TCalibH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sButton.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "sBitBtn.hpp"
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TCalibForm : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox5;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label9;
        TLabel *Label3;
        TLabel *Label7;
        TLabel *Label11;
        TLabel *Label13;
        TLabel *Label4;
        TLabel *Label6;
        TChart *Chart1;
        TButton *Button_Draw;
        TLineSeries *Series_Maf;
        TEdit *Edit_CalOffset;
        TEdit *Edit_CalADC;
        TsBitBtn *sBitBtn_MAFADCRead;
        TEdit *Edit_ADC1;
        TEdit *Edit_Offset1;
        TsButton *sButton_MAFRead;
        TsButton *sButton_MAFWrite;
        TsButton *sButton_MAFDefault;
        TEdit *Edit_ADC2;
        TEdit *Edit_Offset2;
        TEdit *Edit_ADC3;
        TEdit *Edit_Offset3;
        TEdit *Edit_ADC4;
        TEdit *Edit_Offset4;
        TEdit *Edit_ADC5;
        TEdit *Edit_Offset5;
        TsButton *sButton_CRead;
        TsButton *sButton_CSet;
        TsButton *sButton_CDefault;
        TPanel *Panel1;
        TsButton *sButton_PCRead;
        TsButton *sButton_PCWrite;
        TEdit *Edit_MAF00;
        TEdit *Edit_MAF01;
        TEdit *Edit_MAF02;
        TEdit *Edit_MAF03;
        TEdit *Edit_MAF04;
        TEdit *Edit_MAF05;
        TEdit *Edit_MAF06;
        TEdit *Edit_MAF07;
        TEdit *Edit_MAF08;
        TEdit *Edit_MAF09;
        TEdit *Edit_MAF10;
        TEdit *Edit_MAF11;
        TEdit *Edit_MAF12;
        TEdit *Edit_MAF13;
        TEdit *Edit_MAF14;
        TEdit *Edit_MAF15;
        TEdit *Edit_MAF16;
        TEdit *Edit_MAF17;
        TEdit *Edit_MAF18;
        TEdit *Edit_MAF19;
        TGroupBox *GroupBox4;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TEdit *Edit_TRangeL;
        TEdit *Edit_TRangeH;
        TBitBtn *BitBtn_TCalLow;
        TBitBtn *BitBtn_TCalHigh;
        TEdit *Edit_TCalLow;
        TEdit *Edit_TCalHigh;
        TComboBox *CB_TNo;
        TsButton *sButton_TRead;
        TsButton *sButton_TWrite;
        TsButton *sButton_dTress;
        TEdit *Edit_TCalADC;
        TsBitBtn *sBitBtn_TADCRead;
        TCheckBox *CheckBox_TNo;
        TGroupBox *GroupBox1;
        TLabel *Label16;
        TLabel *Label25;
        TLabel *Label17;
        TEdit *Edit_PRangeL;
        TEdit *Edit_PRangeH;
        TBitBtn *BitBtn_PCalLow;
        TBitBtn *BitBtn_PCalHigh;
        TEdit *Edit_PCalLow;
        TEdit *Edit_PCalHigh;
        TComboBox *CB_PNo;
        TsButton *sButton_PRead;
        TsButton *sButton_PWrite;
        TsButton *sButton_dPress;
        TEdit *Edit_PCalADC;
        TsBitBtn *sBitBtn_PADCRead;
        TCheckBox *CheckBox_PNo;
        TGroupBox *GroupBox2;
        TLabel *Label1;
        TComboBox *CB_NOx;
        TCheckBox *CheckBox_NOx;
        TsButton *sButton_NOxRead;
        TsButton *sButton_NOxSet;
        TOpenDialog *OpenDialog;
        TSaveDialog *SaveDialog;
        TMemo *Memo;
        TTimer *Timer1;
        TsBitBtn *sBitBtn_MAF1V;
        TsBitBtn *sBitBtn_MAF2V;
        TsBitBtn *sBitBtn_MAF3V;
        TsBitBtn *sBitBtn_MAF4V;
        TsBitBtn *sBitBtn_MAF45V;
        void __fastcall sButton_TReadClick(TObject *Sender);
        void __fastcall sButton_TWriteClick(TObject *Sender);
        void __fastcall sButton_MAFReadClick(TObject *Sender);
        void __fastcall sButton_MAFDefaultClick(TObject *Sender);
        void __fastcall sBitBtn_MAFADCReadClick(TObject *Sender);
        void __fastcall sButton_MAFWriteClick(TObject *Sender);
        void __fastcall sButton_dPressClick(TObject *Sender);
        void __fastcall sButton_PReadClick(TObject *Sender);
        void __fastcall BitBtn_PCalHighClick(TObject *Sender);
        void __fastcall BitBtn_PCalLowClick(TObject *Sender);
        void __fastcall sButton_PWriteClick(TObject *Sender);
        void __fastcall sBitBtn_PADCReadClick(TObject *Sender);
        void __fastcall sBitBtn_TADCReadClick(TObject *Sender);
        void __fastcall BitBtn_TCalHighClick(TObject *Sender);
        void __fastcall BitBtn_TCalLowClick(TObject *Sender);
        void __fastcall sButton_dTressClick(TObject *Sender);
        void __fastcall sButton_NOxSetClick(TObject *Sender);
        void __fastcall sButton_NOxReadClick(TObject *Sender);
        void __fastcall sButton_CDefaultClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall sButton_CReadClick(TObject *Sender);
        void __fastcall sButton_CSetClick(TObject *Sender);
        void __fastcall sButton_PCReadClick(TObject *Sender);
        void __fastcall sButton_PCWriteClick(TObject *Sender);
        void __fastcall Button_DrawClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall sBitBtn_MAF1VClick(TObject *Sender);
        void __fastcall sBitBtn_MAF2VClick(TObject *Sender);
        void __fastcall sBitBtn_MAF3VClick(TObject *Sender);
        void __fastcall sBitBtn_MAF4VClick(TObject *Sender);
        void __fastcall sBitBtn_MAF45VClick(TObject *Sender);
        void __fastcall Edit_MAF00DblClick(TObject *Sender);
        void __fastcall Edit_MAF01DblClick(TObject *Sender);
        void __fastcall Edit_MAF02DblClick(TObject *Sender);
        void __fastcall Edit_MAF03DblClick(TObject *Sender);
        void __fastcall Edit_MAF04DblClick(TObject *Sender);
        void __fastcall Edit_MAF05DblClick(TObject *Sender);
        void __fastcall Edit_MAF06DblClick(TObject *Sender);
        void __fastcall Edit_MAF07DblClick(TObject *Sender);
        void __fastcall Edit_MAF08DblClick(TObject *Sender);
        void __fastcall Edit_MAF09DblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TEdit *CMAF[20];
        __fastcall TCalibForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCalibForm *CalibForm;
//---------------------------------------------------------------------------
#endif

