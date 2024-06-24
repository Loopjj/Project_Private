//---------------------------------------------------------------------------

#ifndef ManualH
#define ManualH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sButton.hpp"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TManualForm : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
        __fastcall TManualForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TManualForm *ManualForm;
//---------------------------------------------------------------------------
#endif
