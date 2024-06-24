//---------------------------------------------------------------------------

#ifndef TGraphH
#define TGraphH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include "sPanel.hpp"
#include "sButton.hpp"
//---------------------------------------------------------------------------
class TGraphForm : public TForm
{
__published:	// IDE-managed Components
    TChart *Chart;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TGroupBox *GroupBox3;
    TGroupBox *GroupBox4;
        TCheckBox *CB_T1;
        TCheckBox *CB_T2;
        TCheckBox *CB_T3;
        TCheckBox *CB_NI;
        TCheckBox *CB_MAF;
    TCheckBox *CB_FPD;
    TGroupBox *GroupBox6;
    TEdit *Edit_MinLeft;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Edit_MaxLeft;
    TCheckBox *CB_AutoLeft;
    TGroupBox *GroupBox7;
    TButton *Button_ApplyLeft;
    TEdit *Edit_MinRight;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *Edit_MaxRight;
    TCheckBox *CB_AutoRight;
    TButton *Button_ApplyRight;
    TRadioGroup *RG_Left;
    TRadioGroup *RG_Right;
    TTimer *GraphTimer;
        TLineSeries *Series_T1;
        TLineSeries *Series_T2;
        TLineSeries *Series_T3;
        TLineSeries *Series_NOxIn;
        TLineSeries *Series_MAF;
    TSaveDialog *SaveDialog;
    TPrintDialog *PrintDialog;
    TGroupBox *GroupBox8;
    TCheckBox *CB_3D;
        TLineSeries *Series_DOSING;
        TCheckBox *CB_NO;
        TLineSeries *Series_NOxOut;
        TCheckBox *CB_SP;
        TLineSeries *Series_Supply;
        TPanel *Panel_T1;
        TPanel *Panel_T2;
        TPanel *Panel_T3;
        TPanel *Panel_NI;
        TPanel *Panel_NO;
        TPanel *Panel_FPD;
        TPanel *Panel_SP;
        TPanel *Panel_MAF;
        TColorDialog *ColorDialog1;
        TGroupBox *GroupBox5;
        TsButton *sButton_Start;
        TsButton *sButton_Stop;
        TsButton *sButton_Clear;
        TsButton *sButton_Save;
        TsButton *sButton_Print;
        TsButton *sButton_End;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall GraphTimerTimer(TObject *Sender);
    void __fastcall Button_ApplyLeftClick(TObject *Sender);
    void __fastcall Button_ApplyRightClick(TObject *Sender);
    void __fastcall CB_3DClick(TObject *Sender);
        void __fastcall CB_T1Click(TObject *Sender);
        void __fastcall CB_T2Click(TObject *Sender);
        void __fastcall CB_T3Click(TObject *Sender);
        void __fastcall CB_NIClick(TObject *Sender);
        void __fastcall CB_NOClick(TObject *Sender);
        void __fastcall CB_FPDClick(TObject *Sender);
        void __fastcall CB_SPClick(TObject *Sender);
        void __fastcall CB_MAFClick(TObject *Sender);
        void __fastcall sButton_StartClick(TObject *Sender);
        void __fastcall sButton_StopClick(TObject *Sender);
        void __fastcall sButton_ClearClick(TObject *Sender);
        void __fastcall sButton_SaveClick(TObject *Sender);
        void __fastcall sButton_PrintClick(TObject *Sender);
        void __fastcall sButton_EndClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TGraphForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGraphForm *GraphForm;
//---------------------------------------------------------------------------
#endif
