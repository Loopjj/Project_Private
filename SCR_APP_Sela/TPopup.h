//---------------------------------------------------------------------------

#ifndef TPopupH
#define TPopupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TPopup : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
private:	// User declarations
public:		// User declarations
        __fastcall TPopup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPopup *Popup;
//---------------------------------------------------------------------------
#endif
