//---------------------------------------------------------------------------

#ifndef TPasswordH
#define TPasswordH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TPassForm : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label1;
        TButton *BitBtn_OK;
        TButton *BitBtn_Exit;
        TMaskEdit *EditPasswrod;
        TMemo *Memo1;
        TMaskEdit *MaskEdit_CPass1;
        TMaskEdit *MaskEdit_CPass2;
        TButton *Button_CPass;
        void __fastcall BitBtn_OKClick(TObject *Sender);
        void __fastcall BitBtn_ExitClick(TObject *Sender);
        void __fastcall Button_CPassClick(TObject *Sender);
        void __fastcall EditPasswrodKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        AnsiString mPassword;
        __fastcall TPassForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPassForm *PassForm;
//---------------------------------------------------------------------------
#endif
