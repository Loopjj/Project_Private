//---------------------------------------------------------------------------

#ifndef TMainH
#define TMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <ComCtrls.hpp>
#include "AdPort.hpp"
#include "OoMisc.hpp"
#include <Dialogs.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <fcntl.h>
#include <io.h>
#include <Graphics.hpp>
#include "sSkinManager.hpp"
#include "sPanel.hpp"
#include "sLabel.hpp"
#include "sButton.hpp"
#include "sComboBox.hpp"
#include "AdStatLt.hpp"
#include <sys\stat.h>
#include <winbase.h>
//---------------------------------------------------------------------------
#define CHMODE    0x40  //0(Off),1(On)  Test on/off

#define RLYCMP    0x41  //0,1
#define SOLAIR    0x42  //1

#define RLYIGN    0x43  // 1 
#define PWMIGN    0x44  //0-255 0~100% -> 0~255

#define RLYFP     0x45  //1
#define SOLFP     0x46  // 1 
#define PWMFP     0x47  //0-255 0~100% -> 0~255

#define RVALUE    0x48  //0, NULL

#define SRIAL     0x50  //Serial N0 4byte

#pragma pack(push, before_pack, 1)
typedef struct {
  unsigned char	 enable;
  unsigned short AdcHigh;
  unsigned short AdcLow;
  unsigned short RangeHigh;
  unsigned short RangeLow;
}stCalib;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef union {
  unsigned short W;
  struct {
    unsigned short Beta         :1;
    unsigned short Reduction    :1;
    unsigned short ScrFlowStart :1;
    unsigned short SupplyStart  :1;
    unsigned short DosingStart  :1;
    unsigned short TimeRequest  :1;
    unsigned short WriteScrMap  :1;
    unsigned short SupplyT1Cut  :1;
    unsigned short ModuleT1Cut  :1;
    unsigned short P1Cut        :1;
    unsigned short MAFCut       :1;
    unsigned short Flushing     :1;
    unsigned short res          :4;
  };
}stFLAG;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef union {
  unsigned char B;
  struct {
    unsigned char Noxin         :1;   // Nox UP 센서 동작 에러
    unsigned char Noxout        :1;   // Nox DN 센서 동작 에러
    unsigned char T1            :1;   // 온도1 동작 에러
    unsigned char T2            :1;   // 온도2 동작 에러
    unsigned char ULevel        :1;   //
    unsigned char Nozzle        :1;   // 노즉 막힘
    unsigned char UTemp         :1;   // 요소수 온도
    unsigned char Supply        :1;   // 에어압력 불량
  };
}stERROR;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef union{
  unsigned char W;
  struct {
    unsigned char Noxin        :1;
    unsigned char Noxout       :1;
    unsigned char BatAlarm     :1;
    unsigned char MAF          :1;
    unsigned char MAFTemp      :1;
    unsigned char res          :11;
  };
}stCHECK;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef union{
  unsigned short W;
  struct {
    unsigned short Noxin        :1;
    unsigned short Noxout       :1;
    unsigned short KeyOn        :1;
    unsigned short Operation    :1;
    unsigned short Purge        :1;
    unsigned short Flushing     :1;
    unsigned short Motor        :1;
    unsigned short Dosing       :1;
    unsigned short Supply       :1;
    unsigned short res          :7;
  };
}stSIGNAL;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef struct {
  unsigned short Version  ;//2
  short T1_temp	          ;//4
  short T2_temp	          ;//6
  short Tavg_temp	  ;//8
  short AMB_temp          ;//10
  unsigned short P1_bar	  ;//12
  short Noxppm[2]	  ;//16
  unsigned short DosingDuty;//18
  unsigned short MafKg_H  ;//20
  short Maf_temp 	  ;//22
  short TankTemp	  ;//24
  unsigned short TankLevelP;//26
  unsigned short BattVoltage;//28
  unsigned char Map_X     ;//29
  unsigned char Map_Y     ;//30
  unsigned char	StatusAlpha;//31
  unsigned char	SCRMode	  ;//32
  unsigned short NoxReduction;//34
  stERROR SystemError     ;//35
  stCHECK SystemCheck	  ;//37
  stSIGNAL SystemSignal	  ;//39
  unsigned char	MDosingStart;//40
  double DosingRatehouer  ;//48
  double TotalDosingRate  ;//56
  double curXk            ;//64
  double Xc               ;//72
  double H1k              ;//80
  double H2k              ;//88
  double Yk               ;//96
  double Kl               ;//104
  double Kp               ;//112
  double curXh            ;//120
  double Vk		  ;//128
  double Gamma		  ;//136
  double Formula1         ;//144
  double Formula2         ;//152
  double Formula3         ;//160
  double NH3Up            ;//168
  double NH3Dn            ;//176
}stSysValue;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef struct {
  unsigned short BaseTemp[24];
  unsigned short BaseMaf[24];
}stLabel;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef struct{
  unsigned int Ka;
  unsigned int Kd;
  unsigned int Kr;
  unsigned short OFA;
  unsigned int S1;
  unsigned short S2;
  unsigned short L;	    //SCR Length(m)
  unsigned int ScrArea;     //SCR Filter의 면적
  unsigned char	 Xsp;
  unsigned char	 Coeff;
  unsigned int Kl;
  unsigned int Kp;
}stFactor;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef struct {
  unsigned char	sys_firstboot;
  unsigned char	Valid_Str[10];

  unsigned short CutMaf;        // PAR 1
  unsigned short LowTankLevel;
  short CutTankTemp;
  unsigned short TankType;

  unsigned short LowT1;         // PAR 2
  unsigned short HighT1;
  unsigned short LowP1;
  unsigned short HighP1;

  unsigned short BatVoltageLow; // PAR 3
  unsigned char	 Temp_Ratio;
  unsigned char	 Scr_Mode;
  unsigned char	 Scr_Period;
  unsigned char  Reserved0;
  unsigned short Reserved1;

  unsigned short ScrWaitTime;   // PAR 4
  unsigned char  ScrPurge;
  unsigned char  Reserved2;
  unsigned short ScrPurgeTime;
  unsigned short Reserved3;

  unsigned short MafOffset;	// PAR 5
  unsigned short MafDiameter;
  unsigned short MapOffset;
  unsigned char	 Nozzle;
  unsigned char	 Reserved4;

  unsigned short Reserved[4];   // PAR 6
}stSysControl;
#pragma pack(pop, before_pack)

#pragma pack(push, before_pack, 1)
typedef struct {
  unsigned char	sys_firstboot;
  unsigned char	Valid_Str[10];

  unsigned short Adc[10];
  unsigned short Value[10];
}stScrMaf;
#pragma pack(pop, before_pack)


//---------------------------------------------------------------------------
const unsigned char BitTime[9][2] =
                          {{0x03, 0x34}
        	  	  ,{0x07, 0x34}
	          	  ,{0x0F, 0x34}
	        	  ,{0x13, 0x34}
	  	          ,{0x1F, 0x34}
	  	          ,{0x27, 0x34}
        	  	  ,{0x27, 0x7A}
	          	  ,{0x31, 0x7A}
	  	          ,{0x3F, 0x7F}
                          };
                          
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TStatusBar *StatusBar1;
    TApdComPort *ApdComPort1;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    TTimer *DispTimer;
        TsSkinManager *sSkinManager1;
        TsLabelFX *sLabelFX1;
        TsButton *sButton_Settings;
        TsButton *sButton_Graph;
        TsButton *sButton_Exit;
        TsButton *sButton_TMap;
        TsButton *sButton_Param;
        TsButton *sButton_Factor;
        TApdStatusLight *ApdStatusLight_Link;
        TApdStatusLight *ApdStatusLight_Link1;
        TPanel *Panel16;
        TsButton *sButton_ECU;
        TsButton *sButton_RTC;
        TLabel *Label5;
        TLabel *Label60;
        TLabel *Label61;
        TApdStatusLight *ApdStatusLight1;
        TApdStatusLight *ApdStatusLight2;
        TApdStatusLight *ApdStatusLight3;
        TsPanel *sPanel2;
        TsButton *sButton_ComClose;
        TsButton *sButton_ComOpen;
        TsButton *sButton_CANConfig;
        TsPanel *sPanel9;
        TsPanel *sPanel10;
        TEdit *Edit_T1;
        TEdit *Edit_T2;
        TEdit *Edit_Tavg;
        TsPanel *sPanel12;
        TsPanel *sPanel13;
        TEdit *Edit_P1;
        TsPanel *sPanel14;
        TEdit *Edit_MAF;
        TsPanel *sPanel15;
        TEdit *Edit_NOxIn;
        TEdit *Edit_NOxOut;
        TsPanel *sPanel16;
        TsPanel *sPanel17;
        TsPanel *sPanel18;
        TsPanel *sPanel19;
        TEdit *Edit_Maf_T;
        TsPanel *sPanel20;
        TEdit *Edit_TankLevelP;
        TEdit *Edit_TankTemp;
        TsPanel *sPanel21;
        TsPanel *sPanel22;
        TEdit *Edit_Dosing;
        TsPanel *sPanel23;
        TEdit *Edit_DosingDuty;
        TsPanel *sPanel24;
        TEdit *Edit_Sum;
        TsPanel *sPanel25;
        TsPanel *sPanel26;
        TsPanel *sPanel27;
        TsPanel *sPanel28;
        TEdit *Edit_Pulse;
        TsPanel *sPanel29;
        TsPanel *sPanel30;
        TsPanel *sPanel31;
        TsPanel *sPanel32;
        TsPanel *sPanel33;
        TEdit *Edit_NOxReduce;
        TEdit *Edit_NOxRealReduce;
        TEdit *Edit_B0;
        TEdit *Edit_Xk;
        TEdit *Edit_Xc;
        TEdit *Edit_Xh;
        TEdit *Edit_H1;
        TEdit *Edit_H2;
        TsPanel *sPanel35;
        TsPanel *sPanel36;
        TEdit *Edit_KI;
        TEdit *Edit_KP;
        TsPanel *sPanel37;
        TsPanel *sPanel38;
        TsPanel *sPanel1;
        TsPanel *sPanel3;
        TEdit *Edit_Velocity;
        TEdit *Edit_Gamma;
        TsPanel *sPanel7;
        TsPanel *sPanel8;
        TPanel *PER0;
        TsPanel *sPanel11;
        TPanel *PER1;
        TsPanel *sPanel39;
        TsPanel *sPanel40;
        TsPanel *sPanel41;
        TsPanel *sPanel42;
        TsPanel *sPanel43;
        TsPanel *sPanel44;
        TPanel *PER2;
        TPanel *PER3;
        TPanel *PER4;
        TPanel *PER5;
        TPanel *PER6;
        TPanel *PER7;
        TsPanel *sPanel5;
        TsPanel *sPanel45;
        TsPanel *sPanel46;
        TsPanel *sPanel47;
        TsPanel *sPanel48;
        TsPanel *sPanel49;
        TsPanel *sPanel50;
        TsPanel *sPanel51;
        TsPanel *sPanel52;
        TsPanel *sPanel53;
        TsPanel *sPanel54;
        TsPanel *sPanel55;
        TsPanel *sPanel56;
        TsPanel *sPanel57;
        TsPanel *sPanel58;
        TsPanel *sPanel59;
        TsPanel *sPanel60;
        TsPanel *sPanel61;
        TsPanel *sPanel62;
        TsPanel *sPanel63;
        TPanel *PCK0;
        TPanel *PCK1;
        TPanel *PCK2;
        TPanel *PCK3;
        TPanel *PCK4;
        TPanel *PCK5;
        TPanel *PCK6;
        TPanel *PCK7;
        TPanel *PCK8;
        TPanel *PCK9;
        TPanel *PSG0;
        TPanel *PSG1;
        TPanel *PSG2;
        TPanel *PSG3;
        TPanel *PSG4;
        TPanel *PSG5;
        TPanel *PSG6;
        TPanel *PSG7;
        TsPanel *sPanel64;
        TPanel *Panel1;
        TsButton *sButton_AlarmClear;
        TsButton *sButton_InitGegenCnt;
        TsComboBox *sComboBox_Mode;
        TsButton *sButton_MODE;
        TEdit *Edit_mAlpha;
        TsButton *sButton_Alpha;
        TsButton *sButton_Inject;
        TEdit *Edit_mDosing;
        TLabel *Label_Mode;
        TsPanel *sPanel65;
        TsPanel *sPanel66;
        TsPanel *sPanel67;
        TEdit *Edit_2PPM;
        TEdit *Edit_3PPM;
        TEdit *Edit_1PPM;
        TsPanel *sPanel68;
        TsPanel *sPanel69;
        TEdit *Edit_NH3Up;
        TEdit *Edit_NH3Dn;
        TsPanel *sPanel70;
        TsPanel *sPanel71;
        TPanel *PSG9;
        TPanel *PSG8;
        TEdit *Edit_mTavg;
        TsPanel *sPanel_m1;
        TEdit *Edit_mMaf;
        TsPanel *sPanel_m2;
        TEdit *Edit_mNoxUp;
        TsPanel *sPanel_m3;
        TEdit *Edit_mNoxDn;
        TsPanel *sPanel_m4;
        TPanel *Panel5;
        TsButton *sButton_ManualSet;
        TPanel *Panel4;
        TPanel *Panel6;
        TsPanel *sPanel34;
        TEdit *Edit_Y;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TsButton *sButton_AirPressure;
        TsButton *sButton1;
        TRadioButton *RadioButton_gh;
        TRadioButton *RadioButton_duty;
        TsButton *sButton_SCRDsiable;
        TsButton *sButton_VauleSet;
        TPanel *Panel3;
        TsLabel *sLabel1;
        TsLabel *sLabel2;
        TsLabel *sLabel3;
        TsComboBox *sComboBox_ComPort;
        TsComboBox *sComboBox_Baud;
        TsComboBox *sComboBox_Device;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ApdComPort1TriggerAvail(TObject *CP, WORD Count);
        void __fastcall AppException(TObject *Sender, Exception *E);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall sButton_SettingsClick(TObject *Sender);
        void __fastcall sButton_ExitClick(TObject *Sender);
        void __fastcall sButton_GraphClick(TObject *Sender);
        void __fastcall sButton_ComOpenClick(TObject *Sender);
        void __fastcall sButton_ComCloseClick(TObject *Sender);
        void __fastcall sButton_InitGegenCntClick(TObject *Sender);
        void __fastcall sButton_ManualSetClick(TObject *Sender);
        void __fastcall sButton_ACOffClick(TObject *Sender);
        void __fastcall sButton_MODEClick(TObject *Sender);
        void __fastcall sButton_MANUALClick(TObject *Sender);
        void __fastcall sButton_AlarmClearClick(TObject *Sender);
        void __fastcall sButton_TMapClick(TObject *Sender);
        void __fastcall sButton_InjectClick(TObject *Sender);
        void __fastcall sButton_InjectOffClick(TObject *Sender);
        void __fastcall sButton_ParamClick(TObject *Sender);
        void __fastcall sButton_FactorClick(TObject *Sender);
        void __fastcall sButton_AirPressureClick(TObject *Sender);
        void __fastcall sButton_AlphaClick(TObject *Sender);
        void __fastcall sButton_RTCClick(TObject *Sender);
        void __fastcall sButton_ECUClick(TObject *Sender);
        void __fastcall DispTimerTimer(TObject *Sender);
        void __fastcall sButton_SCRDsiableClick(TObject *Sender);
        void __fastcall sButton_CANConfigClick(TObject *Sender);
        void __fastcall sButton1Click(TObject *Sender);
        void __fastcall sButton_VauleSetClick(TObject *Sender);
        void __fastcall sComboBox_DeviceChange(TObject *Sender);
private:	// User declarations
    unsigned char m_ucTxBuf[1024], m_ucRxData[1024],m_ucRxCommand, m_ucRxID;
    unsigned char m_ucRxStep,m_ucSyncCnt, m_ucLenCnt;
    unsigned short m_ucRxLength, m_ucRxDataCnt, ucLength;
    TPanel *RLED[7][8];
    unsigned int m_nRecCount;
    AnsiString m_asRecFileName;

public:		// User declarations
    TEdit *SData[4][4];
    TLabel *SLabel[4][4];
    TEdit *Map[72][8];
    TEdit *MapT[6][4];
    TEdit *MapMAF[6][4];
    short m_uiTVal[3], m_uiNOxIn, m_uiNOxOut, m_uiMAF, m_uiAirPChk, m_UreaTemp, m_State, m_Maf_T;
    unsigned short m_uiPVal, m_UreaLevelP, m_Duty;
    double PIDVal[3], ABCVal[3], m_dDosing, m_dSumDosing, m_dKI, m_dKP, m_dVelocity, m_dGamma, m_dBattery, m_Factor, m_UreaLevel, m_Xk, m_Xc, m_Xh;
    unsigned char ErrFlag, ChkFlag, SigFlag, m_bManual, m_bAscii[2], m_bBitTime[2];
    AnsiString StrLTime, StrITime;
    unsigned int m_uiTRangeL, m_uiTRangeH, LTime, m_uLat, m_uLon, m_uAlt, m_uLat1, m_uLon1, m_uAlt1, m_uSpd, m_Reduce;
    unsigned int m_uiPRangeL, m_uiPRangeH;
    unsigned int m_uiPADCVal,m_uiPADCValLow,m_uiPADCValHigh;
    unsigned int m_ufPQFact;
    //unsigned char m_bProgFlag;
    unsigned char m_flag, m_ScrFlag;
    unsigned char m_MapX, m_MapY, m_PreMapX, m_PreMapY;
    double m_ufPeakFact, m_ufPeakDecFact;

    unsigned short BaseTemp[24];
    unsigned int BaseMaf[24];

    unsigned char PreDay, MapRead;
    FILE *fout, *fchk;
    Word Year, Month, Day, Hour, Min, Sec, MSec;

    unsigned short Thour,Tmin,Tsec;
    unsigned char Cnt;
    AnsiString m_asFName, dir;
    unsigned char RecFileName[12];
    __fastcall TMainForm(TComponent* Owner);
    void __fastcall Fire_TxMap(unsigned char id);
    void __fastcall Target_TxMap(unsigned char id);
    void __fastcall ProgRxData(unsigned char rdata);
    void __fastcall CAN_RxData(unsigned char rdata);
    void __fastcall RS232_RxData(unsigned char rdata);
    void __fastcall CAN_DecodeData(void);
    void __fastcall RS232_DecodeData(void);
    void __fastcall UpdateVal(void);
    void __fastcall AlarmWindow(AnsiString str);
    void __fastcall Save_data(void);
    unsigned char __fastcall TMainForm::asciiHexToNibble(unsigned char  character);
    unsigned char __fastcall TMainForm::asciiHexToByte(unsigned char * string);
    unsigned char __fastcall TMainForm::nibbleHexToAscii(unsigned char  nibble);
    void __fastcall TMainForm::byteToAsciiHex(unsigned char character, unsigned char * string);
//-------------------------------------------------------------------------------------------
    void __fastcall TMainForm::ReceiveDataProcess(unsigned short id, unsigned char length, unsigned char *data);
    void __fastcall TMainForm::TranceiveDataProcess(unsigned short id, unsigned char length, unsigned char *data);
    void __fastcall TMainForm::TxCANData(unsigned char command, unsigned char length, unsigned char *data);
    void __fastcall TxData(unsigned char sync, unsigned short len, unsigned char command, unsigned char id, unsigned char *data);
    void __fastcall TxCmd(unsigned char sync, unsigned char command, unsigned char sig);
    void __fastcall TMainForm::Monitor_ON(void);
    void __fastcall TMainForm::Monitor_OFF(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
