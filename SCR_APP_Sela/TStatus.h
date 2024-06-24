//---------------------------------------------------------------------------

#ifndef TStatusH
#define TStatusH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "sLabel.hpp"
//---------------------------------------------------------------------------
class TStatusForm : public TForm
{
__published:	// IDE-managed Components
        TButton *StatusBtn;
        TsLabel *sLabel_Msg;
        void __fastcall StatusBtnClick(TObject *Sender);
        void __fastcall SetAlarmMsg(AnsiString str);
private:	// User declarations
public:		// User declarations
        __fastcall TStatusForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStatusForm *StatusForm;
//---------------------------------------------------------------------------
#endif
