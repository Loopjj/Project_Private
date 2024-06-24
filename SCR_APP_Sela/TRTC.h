//---------------------------------------------------------------------------

#ifndef TRTCH
#define TRTCH
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
//---------------------------------------------------------------------------
class TRTCForm : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox6;
        TLabel *Label24;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label31;
        TLabel *Label32;
        TEdit *Edit_Year;
        TUpDown *UpDown_Year;
        TUpDown *UpDown_Month;
        TEdit *Edit_Month;
        TUpDown *UpDown_Day;
        TEdit *Edit_Day;
        TUpDown *UpDown_Hour;
        TEdit *Edit_Hour;
        TUpDown *UpDown_Min;
        TEdit *Edit_Min;
        TEdit *Edit_Sec;
        TUpDown *UpDown_Sec;
        TCheckBox *CheckBox_PCTime;
        TPanel *Panel_RTC;
        TsButton *sButton_RTCRead;
        TsButton *sButton_RTCSet;
        TTimer *Timer2;
        TsButton *sButton_Close;
        void __fastcall sButton_RTCReadClick(TObject *Sender);
        void __fastcall sButton_RTCSetClick(TObject *Sender);
        void __fastcall CheckBox_PCTimeClick(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall sButton_CloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TRTCForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRTCForm *RTCForm;
//---------------------------------------------------------------------------
#endif
