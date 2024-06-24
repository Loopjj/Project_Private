//---------------------------------------------------------------------------

#ifndef TCanH
#define TCanH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sButton.hpp"
#include "sComboBox.hpp"
//---------------------------------------------------------------------------
class TCanForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label31;
        TsComboBox *sComboBox_CAN_Protocol;
        TLabel *Label32;
        TsComboBox *sComboBox_CAN_BSP;
        TsButton *sButton_CAN_Reset;
        TsButton *sButton_CAN_Read;
        TsButton *sButton_CAN_Set;
        TGroupBox *Waring;
        TCheckBox *CheckBox_BUSOff;
        TCheckBox *CheckBox_CANError;
        TCheckBox *CheckBox_CANWaring;
        TCheckBox *CheckBox_AutoRest;
        TsButton *sButton_Ver;
        TsButton *sButton_End;
        void __fastcall sButton_CAN_ReadClick(TObject *Sender);
        void __fastcall sButton_CAN_SetClick(TObject *Sender);
        void __fastcall sButton_CAN_ResetClick(TObject *Sender);
        void __fastcall sButton_VerClick(TObject *Sender);
        void __fastcall sButton_EndClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TCanForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCanForm *CanForm;
//---------------------------------------------------------------------------
#endif
