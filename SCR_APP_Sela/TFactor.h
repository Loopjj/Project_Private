//---------------------------------------------------------------------------

#ifndef TFactorH
#define TFactorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sButton.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sLabel.hpp"
//---------------------------------------------------------------------------
class TFactorForm : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TsPanel *sPanel1;
        TEdit *Edit_Ka;
        TEdit *Edit_Kd;
        TEdit *Edit_Kr;
        TsButton *sButton_PidRead;
        TsButton *sButton_PidWrite;
        TsButton *sButton_PidDefault;
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel4;
        TEdit *Edit_OmegaS1;
        TPanel *Panel6;
        TEdit *Edit_OFA;
        TPanel *Panel7;
        TEdit *Edit_ScrArea;
        TEdit *Edit_Length;
        TPanel *Panel5;
        TPanel *Panel8;
        TEdit *Edit_OmegaS2;
        TLabel *Label8;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label5;
        TLabel *Label6;
        TsButton *sButton_End;
        TPanel *Panel11;
        TEdit *Edit_Kl;
        TPanel *Panel12;
        TEdit *Edit_Kp;
        TPanel *Panel9;
        TEdit *Edit_Xsp;
        TPanel *Panel10;
        TEdit *Edit_Coeff;
        TsLabel *sLabel1;
        TsLabel *sLabel2;
        TsLabel *sLabel3;
        TsLabel *sLabel4;
        TsLabel *sLabel5;
        TsLabel *sLabel6;
        TsLabel *sLabel7;
        TsLabel *sLabel8;
        TsLabel *sLabel9;
        TsLabel *sLabel10;
        TsLabel *sLabel11;
        TLabel *Label3;
        void __fastcall sButton_PidReadClick(TObject *Sender);
        void __fastcall sButton_PidWriteClick(TObject *Sender);
        void __fastcall sButton_PidDefaultClick(TObject *Sender);
        void __fastcall sButton_EndClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFactorForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFactorForm *FactorForm;
//---------------------------------------------------------------------------
#endif
