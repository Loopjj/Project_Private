//---------------------------------------------------------------------------

#ifndef TSettingsH
#define TSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <stdio.h>
#include "sButton.hpp"
#include "sCheckBox.hpp"
#include "sLabel.hpp"
//---------------------------------------------------------------------------
class TParmForm : public TForm
{
__published:	// IDE-managed Components
        TsLabel *sLabel1;
        TsLabel *sLabel2;
        TsLabel *sLabel9;
        TsLabel *sLabel12;
        TsLabel *sLabel15;
        TsLabel *sLabel19;
        TPanel *Panel4;
        TPanel *Panel5;
        TEdit *Edit_P10;
        TEdit *Edit_P11;
        TEdit *Edit_P12;
        TEdit *Edit_P13;
        TPanel *Panel13;
        TPanel *Panel14;
        TPanel *Panel15;
        TPanel *Panel16;
        TPanel *Panel6;
        TPanel *Panel7;
        TEdit *Edit_P23;
        TEdit *Edit_P22;
        TEdit *Edit_P21;
        TEdit *Edit_P20;
        TPanel *Panel17;
        TPanel *Panel18;
        TPanel *Panel19;
        TPanel *Panel20;
        TPanel *Panel8;
        TPanel *Panel9;
        TEdit *Edit_P30;
        TEdit *Edit_P31;
        TEdit *Edit_P32;
        TEdit *Edit_P33;
        TPanel *Panel21;
        TPanel *Panel22;
        TPanel *Panel23;
        TPanel *Panel24;
        TsCheckBox *sCheckBox_P0;
        TsCheckBox *sCheckBox_P1;
        TsCheckBox *sCheckBox_P2;
        TsCheckBox *sCheckBox_P3;
        TPanel *Panel1;
        TPanel *Panel10;
        TEdit *Edit_P00;
        TEdit *Edit_P01;
        TEdit *Edit_P02;
        TEdit *Edit_P03;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel11;
        TPanel *Panel12;
        TsButton *sButton_Default;
        TsButton *sButton_SelectAll;
        TsButton *sButton_ParRead;
        TsButton *sButton_ParWrite;
        TPanel *Panel25;
        TPanel *Panel26;
        TEdit *Edit_P40;
        TEdit *Edit_P41;
        TEdit *Edit_P42;
        TEdit *Edit_P43;
        TPanel *Panel27;
        TPanel *Panel28;
        TPanel *Panel29;
        TPanel *Panel31;
        TPanel *Panel30;
        TsCheckBox *sCheckBox_P4;
        TsButton *sButton_End;
        TComboBox *ComboBox_MAF;
        TComboBox *ComboBox_Tank;
        TComboBox *ComboBox_Period;
        TComboBox *ComboBox_Mode;
        TComboBox *ComboBox_Purge;
        TComboBox *ComboBox_Nozzle;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall sButton_DefaultClick(TObject *Sender);
        void __fastcall sButton_SelectAllClick(TObject *Sender);
        void __fastcall sButton_ParReadClick(TObject *Sender);
        void __fastcall sButton_ParWriteClick(TObject *Sender);
        void __fastcall sCheckBox_P0Click(TObject *Sender);
        void __fastcall sCheckBox_P1Click(TObject *Sender);
        void __fastcall sCheckBox_P2Click(TObject *Sender);
        void __fastcall sCheckBox_P3Click(TObject *Sender);
        void __fastcall sCheckBox_P4Click(TObject *Sender);
        void __fastcall sButton_EndClick(TObject *Sender);
        void __fastcall ComboBox_TankChange(TObject *Sender);
        void __fastcall ComboBox_PeriodChange(TObject *Sender);
        void __fastcall ComboBox_ModeChange(TObject *Sender);
        void __fastcall ComboBox_PurgeChange(TObject *Sender);
        void __fastcall ComboBox_NozzleChange(TObject *Sender);
        void __fastcall ComboBox_MAFChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    bool m_bPWriteEnable[5];
    __fastcall TParmForm(TComponent* Owner);
    void __fastcall TxParm(unsigned char id);
};
//---------------------------------------------------------------------------
extern PACKAGE TParmForm *ParmForm;
//---------------------------------------------------------------------------
#endif
