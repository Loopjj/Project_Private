//---------------------------------------------------------------------------

#ifndef TFMapH
#define TFMapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
class TFMapForm : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *ETVMapBox;
        TBitBtn *RPMReadBtn;
        TBitBtn *RPMWriteBtn;
        TBitBtn *T2ReadBtn;
        TBitBtn *T2WriteBtn;
        TStringGrid *StringGrid_Map;
        TGroupBox *GroupBox2;
        TSpeedButton *SaveBtn;
        TSpeedButton *LoadBtn;
        TGroupBox *GroupBox1;
        TBitBtn *MapReadBtn;
        TBitBtn *MapWriteBtn;
        TMemo *Memo;
        TSaveDialog *SaveDialog;
        TOpenDialog *OpenDialog;
        void __fastcall RPMReadBtnClick(TObject *Sender);
        void __fastcall RPMWriteBtnClick(TObject *Sender);
        void __fastcall T2ReadBtnClick(TObject *Sender);
        void __fastcall T2WriteBtnClick(TObject *Sender);
        void __fastcall LoadBtnClick(TObject *Sender);
        void __fastcall SaveBtnClick(TObject *Sender);
        void __fastcall MapReadBtnClick(TObject *Sender);
        void __fastcall MapWriteBtnClick(TObject *Sender);
        void __fastcall StringGrid_MapSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
        void __fastcall StringGrid_MapExit(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFMapForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFMapForm *FMapForm;
//---------------------------------------------------------------------------
#endif
