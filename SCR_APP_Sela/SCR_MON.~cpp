//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("TMain.cpp", MainForm);
USEFORM("TSettings.cpp", ParmForm);
USEFORM("TProg.cpp", ProgForm);
USEFORM("TGraph.cpp", GraphForm);
USEFORM("TStatus.cpp", StatusForm);
USEFORM("TConvert.cpp", ConvertForm);
USEFORM("TMap.cpp", TMapForm);
USEFORM("TCalib.cpp", CalibForm);
USEFORM("TFactor.cpp", FactorForm);
USEFORM("TRTC.cpp", RTCForm);
USEFORM("TSaveAs.cpp", SaveAsForm);
USEFORM("TCan.cpp", CanForm);
USEFORM("TIP.cpp", IPForm);
USEFORM("TPopup.cpp", Popup);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "SCR MONITOR";
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TParmForm), &ParmForm);
                 Application->CreateForm(__classid(TProgForm), &ProgForm);
                 Application->CreateForm(__classid(TGraphForm), &GraphForm);
                 Application->CreateForm(__classid(TStatusForm), &StatusForm);
                 Application->CreateForm(__classid(TConvertForm), &ConvertForm);
                 Application->CreateForm(__classid(TTMapForm), &TMapForm);
                 Application->CreateForm(__classid(TCalibForm), &CalibForm);
                 Application->CreateForm(__classid(TFactorForm), &FactorForm);
                 Application->CreateForm(__classid(TRTCForm), &RTCForm);
                 Application->CreateForm(__classid(TSaveAsForm), &SaveAsForm);
                 Application->CreateForm(__classid(TCanForm), &CanForm);
                 Application->CreateForm(__classid(TIPForm), &IPForm);
                 Application->CreateForm(__classid(TPopup), &Popup);
                 Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
