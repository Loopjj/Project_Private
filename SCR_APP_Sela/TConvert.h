//---------------------------------------------------------------------------

#ifndef TConvertH
#define TConvertH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <fcntl.h>
#include <io.h>
#include <ExtCtrls.hpp>
#include "sButton.hpp"
#include "sPanel.hpp"
#include "sGauge.hpp"
#include <sys\stat.h>
#include <winbase.h>
//---------------------------------------------------------------------------
class TConvertForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label_SelCount;
        TLabel *Label_Total;
        TLabel *Label_One;
        TDriveComboBox *DriveComboBox;
        TDirectoryListBox *DirectoryListBox;
        TFilterComboBox *FilterComboBox;
        TFileListBox *FileListBox;
        TBitBtn *BitBtn_SelectOne;
        TBitBtn *BitBtn_DeSelOne;
        TBitBtn *BitBtn_SellectAll;
        TBitBtn *BitBtn_DSelAll;
        TListBox *ListBox;
        TsButton *sButton_OK;
        TsButton *sButton_Cancel;
        TsButton *sButton_Exit;
        TsPanel *sPanel1;
        TLabel *Label1;
        TRadioButton *RadioButton_Txt;
        TRadioButton *RadioButton_Xcl;
        TsGauge *sGauge_One;
        TsGauge *sGauge_Total;
        void __fastcall BitBtn_SelectOneClick(TObject *Sender);
        void __fastcall BitBtn_DeSelOneClick(TObject *Sender);
        void __fastcall BitBtn_SellectAllClick(TObject *Sender);
        void __fastcall BitBtn_DSelAllClick(TObject *Sender);
        void __fastcall FileListBoxDblClick(TObject *Sender);
        void __fastcall ListBoxDblClick(TObject *Sender);
        void __fastcall AppException(TObject *Sender, Exception *E);
        void __fastcall sButton_OKClick(TObject *Sender);
        void __fastcall sButton_CancelClick(TObject *Sender);
        void __fastcall sButton_ExitClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        AnsiString  m_asFileName[1000];
        unsigned char PreDay;
        bool m_bOKFlag;
        FILE *fin, *fout, *fchk;

        typedef struct {
          unsigned char tm_sec;
          unsigned char tm_min;
          unsigned char tm_hour;
          unsigned char tm_mday;
          unsigned char tm_mon;
          unsigned char tm_year;
          unsigned char tm_wday;
          unsigned char tm_yday;
        }rtc_time;

        typedef struct {
          unsigned short  tm_hour;
          unsigned char tm_min;
          unsigned char tm_sec;
        }TIME ;

        typedef union {
          unsigned char B;
          struct {
            unsigned char T1        : 1;
            unsigned char T2        : 1;
            unsigned char T3        : 1;
            unsigned char T4        : 1;
            unsigned char P1        : 1;
            unsigned char P2        : 1;
            unsigned char BV        : 1;
            unsigned char Misfire   : 1;
          };
        }ALARM;

        typedef union{
          unsigned char B;
          struct {
            unsigned char Over1     : 1;
            unsigned char Over2     : 1;
            unsigned char Over3     : 1;
            unsigned char Over4     : 1;
            unsigned char IHC       : 1;
            unsigned char reserved  : 3;
          };
        }CHECK;

        typedef union {
          unsigned char B;
          struct {
            unsigned char Ready     : 1;
            unsigned char COMPRelay : 1;
            unsigned char IGNRelay  : 1;
            unsigned char FPRelay   : 1;
            unsigned char AIRSol    : 1;
            unsigned char FPSol     : 1;
            unsigned char reserved  : 2;
          };
        }SIGNAL;

        typedef union {
          unsigned char B;
          struct {
            unsigned char REGStart  : 1;
            unsigned char FSH1Sol   : 1;
            unsigned char FSH2Sol   : 1;
            unsigned char Mode      : 1;
            unsigned char reserved  : 4;
          };
        }STATUS;

        typedef union{
          unsigned int _Long;
          struct {
            unsigned short _High;
            unsigned short _Low;
          }_Word;
        }_ULONG;

        // GPS Data
        typedef struct{
          _ULONG Latitude;
          _ULONG Longitude;
          _ULONG Altitude;
          unsigned short   Speed;
        } GPSData;

        typedef struct {
          rtc_time Date;
          unsigned short  T1;
          unsigned short  T2;
          unsigned short  T3;
          unsigned short  T4;
          unsigned short  P1;
          unsigned short  P2;
          unsigned short  Rate;
          unsigned short  BV;
          unsigned short  IHC;
          unsigned short  f_REG;
          unsigned char   IGD;
          unsigned char   FPD;
          ALARM           Error;
          CHECK           Check;
          unsigned short  t_REG;
          unsigned short  n_REG;
          SIGNAL          Sig;
          STATUS          Sts;
          unsigned short  Ver;
          TIME            Time;
          unsigned short  peakP1;
          unsigned short  peakP2;
          GPSData         GPSdata;
        }data_t;

        rtc_time CRTC;

        __fastcall TConvertForm(TComponent* Owner);
   	void __fastcall Analyze(void);
        void __fastcall Save_data(unsigned char *ReadBuf);
        char __fastcall mem_check(unsigned char *s1, unsigned char ch, int len);
        void __fastcall mem_strcpy(unsigned char *s1, unsigned char *s2, int len);
};
//---------------------------------------------------------------------------
extern PACKAGE TConvertForm *ConvertForm;
//---------------------------------------------------------------------------
#endif
