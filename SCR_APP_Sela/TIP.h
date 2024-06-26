//---------------------------------------------------------------------------

#ifndef TIPH
#define TIPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sButton.hpp"
//---------------------------------------------------------------------------
class TIPForm : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit_IP1;
        TEdit *Edit_IP2;
        TEdit *Edit_IP3;
        TEdit *Edit_IP4;
        TEdit *Edit_Port;
        TLabel *Label1;
        TLabel *Label2;
        TsButton *sButton_Read;
        TsButton *sButton_Set;
        TsButton *sButton_End;
        void __fastcall sButton_ReadClick(TObject *Sender);
        void __fastcall sButton_SetClick(TObject *Sender);
        void __fastcall sButton_EndClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TIPForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TIPForm *IPForm;
//---------------------------------------------------------------------------
#endif
