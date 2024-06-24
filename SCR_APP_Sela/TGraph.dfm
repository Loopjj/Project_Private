object GraphForm: TGraphForm
  Left = 54
  Top = 127
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Graph'
  ClientHeight = 566
  ClientWidth = 986
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Chart: TChart
    Left = 9
    Top = 136
    Width = 832
    Height = 417
    BackWall.Brush.Color = clWhite
    BackWall.Brush.Style = bsClear
    Title.Text.Strings = (
      '')
    Title.Visible = False
    Legend.LegendStyle = lsSeries
    MaxPointsPerPage = 10000
    View3D = False
    TabOrder = 0
    object Series_T1: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clGreen
      Title = 'T In'
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series_T2: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clFuchsia
      Title = 'T Out'
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series_T3: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clAqua
      Title = 'Tavg'
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series_NOxIn: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clRed
      Title = 'NOx In'
      VertAxis = aRightAxis
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series_NOxOut: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clBlue
      Title = 'NOx Out'
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series_MAF: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = 33023
      Title = 'MAF'
      VertAxis = aRightAxis
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series_DOSING: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clLime
      Title = 'Dosing'
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series_Supply: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clYellow
      Title = 'Supply'
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
  end
  object GroupBox1: TGroupBox
    Left = 850
    Top = 8
    Width = 127
    Height = 137
    Caption = 'Temperature'
    TabOrder = 1
    object CB_T1: TCheckBox
      Left = 20
      Top = 35
      Width = 41
      Height = 17
      Caption = 'T In'
      TabOrder = 0
      OnClick = CB_T1Click
    end
    object CB_T2: TCheckBox
      Left = 20
      Top = 67
      Width = 45
      Height = 17
      Caption = 'T Out'
      TabOrder = 1
      OnClick = CB_T2Click
    end
    object CB_T3: TCheckBox
      Left = 19
      Top = 99
      Width = 54
      Height = 17
      Caption = 'T Avg'
      TabOrder = 2
      OnClick = CB_T3Click
    end
    object Panel_T1: TPanel
      Left = 88
      Top = 32
      Width = 17
      Height = 17
      TabOrder = 3
    end
    object Panel_T2: TPanel
      Left = 88
      Top = 64
      Width = 17
      Height = 17
      TabOrder = 4
    end
    object Panel_T3: TPanel
      Left = 88
      Top = 96
      Width = 17
      Height = 17
      TabOrder = 5
    end
  end
  object GroupBox2: TGroupBox
    Left = 850
    Top = 152
    Width = 127
    Height = 105
    Caption = 'NOx'
    TabOrder = 2
    object CB_NI: TCheckBox
      Left = 17
      Top = 33
      Width = 56
      Height = 17
      Caption = 'IN(ppm)'
      TabOrder = 0
      OnClick = CB_NIClick
    end
    object CB_NO: TCheckBox
      Left = 18
      Top = 65
      Width = 71
      Height = 17
      Caption = 'OUT(ppm)'
      TabOrder = 1
      OnClick = CB_NOClick
    end
    object Panel_NI: TPanel
      Left = 88
      Top = 32
      Width = 17
      Height = 17
      TabOrder = 2
    end
    object Panel_NO: TPanel
      Left = 88
      Top = 64
      Width = 17
      Height = 17
      TabOrder = 3
    end
  end
  object GroupBox3: TGroupBox
    Left = 850
    Top = 376
    Width = 127
    Height = 73
    Caption = 'MAF'
    TabOrder = 3
    object CB_MAF: TCheckBox
      Left = 17
      Top = 34
      Width = 41
      Height = 17
      Caption = 'kg/h'
      TabOrder = 0
      OnClick = CB_MAFClick
    end
    object Panel_MAF: TPanel
      Left = 88
      Top = 32
      Width = 17
      Height = 17
      TabOrder = 1
    end
  end
  object GroupBox4: TGroupBox
    Left = 850
    Top = 265
    Width = 127
    Height = 104
    Caption = 'Dosing'
    TabOrder = 4
    object CB_FPD: TCheckBox
      Left = 15
      Top = 33
      Width = 46
      Height = 17
      Caption = 'g/h'
      TabOrder = 0
      OnClick = CB_FPDClick
    end
    object CB_SP: TCheckBox
      Left = 15
      Top = 65
      Width = 46
      Height = 17
      Caption = 'mbar'
      TabOrder = 1
      OnClick = CB_SPClick
    end
    object Panel_FPD: TPanel
      Left = 88
      Top = 32
      Width = 17
      Height = 17
      TabOrder = 2
    end
    object Panel_SP: TPanel
      Left = 88
      Top = 64
      Width = 17
      Height = 17
      TabOrder = 3
    end
  end
  object GroupBox6: TGroupBox
    Left = 11
    Top = 6
    Width = 254
    Height = 123
    Caption = 'Left Axis'
    TabOrder = 5
    object Label1: TLabel
      Left = 115
      Top = 41
      Width = 50
      Height = 13
      Caption = 'Min. Value'
    end
    object Label2: TLabel
      Left = 180
      Top = 41
      Width = 53
      Height = 13
      Caption = 'Max. Value'
    end
    object Edit_MinLeft: TEdit
      Left = 113
      Top = 57
      Width = 60
      Height = 21
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      TabOrder = 0
      Text = '0'
    end
    object Edit_MaxLeft: TEdit
      Left = 179
      Top = 58
      Width = 61
      Height = 21
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      TabOrder = 1
      Text = '1000'
    end
    object CB_AutoLeft: TCheckBox
      Left = 112
      Top = 22
      Width = 97
      Height = 17
      Caption = 'Auto-Range'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
    object Button_ApplyLeft: TButton
      Left = 114
      Top = 86
      Width = 128
      Height = 25
      Caption = 'Apply'
      TabOrder = 3
      OnClick = Button_ApplyLeftClick
    end
    object RG_Left: TRadioGroup
      Left = 7
      Top = 14
      Width = 98
      Height = 99
      Ctl3D = True
      ItemIndex = 0
      Items.Strings = (
        'None'
        'Temperature'
        'NOx'
        'Dosing'
        'MAF')
      ParentCtl3D = False
      TabOrder = 4
    end
  end
  object GroupBox7: TGroupBox
    Left = 272
    Top = 6
    Width = 257
    Height = 123
    Caption = 'Right Axis'
    TabOrder = 6
    object Label3: TLabel
      Left = 116
      Top = 41
      Width = 50
      Height = 13
      Caption = 'Min. Value'
    end
    object Label4: TLabel
      Left = 181
      Top = 41
      Width = 53
      Height = 13
      Caption = 'Max. Value'
    end
    object Edit_MinRight: TEdit
      Left = 114
      Top = 57
      Width = 60
      Height = 21
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      TabOrder = 0
      Text = '0'
    end
    object Edit_MaxRight: TEdit
      Left = 180
      Top = 58
      Width = 61
      Height = 21
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      TabOrder = 1
      Text = '1000'
    end
    object CB_AutoRight: TCheckBox
      Left = 113
      Top = 22
      Width = 97
      Height = 17
      Caption = 'Auto-Range'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
    object Button_ApplyRight: TButton
      Left = 115
      Top = 86
      Width = 128
      Height = 25
      Caption = 'Apply'
      TabOrder = 3
      OnClick = Button_ApplyRightClick
    end
    object RG_Right: TRadioGroup
      Left = 7
      Top = 14
      Width = 98
      Height = 99
      Ctl3D = True
      ItemIndex = 0
      Items.Strings = (
        'None'
        'Temperature'
        'NOx'
        'Dosing'
        'MAF')
      ParentCtl3D = False
      TabOrder = 4
    end
  end
  object GroupBox8: TGroupBox
    Left = 536
    Top = 6
    Width = 73
    Height = 123
    Caption = 'ETC'
    TabOrder = 7
    object CB_3D: TCheckBox
      Left = 16
      Top = 24
      Width = 41
      Height = 17
      Caption = '3D'
      TabOrder = 0
      OnClick = CB_3DClick
    end
  end
  object GroupBox5: TGroupBox
    Left = 616
    Top = 7
    Width = 225
    Height = 122
    TabOrder = 8
    object sButton_Start: TsButton
      Left = 8
      Top = 24
      Width = 65
      Height = 33
      Caption = 'Start'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = sButton_StartClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_Stop: TsButton
      Left = 8
      Top = 64
      Width = 65
      Height = 33
      Caption = 'Stop'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = sButton_StopClick
      SkinData.SkinSection = 'BUTTON'
      Down = True
    end
    object sButton_Clear: TsButton
      Left = 80
      Top = 24
      Width = 65
      Height = 33
      Caption = 'Clear'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = sButton_ClearClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_Save: TsButton
      Left = 80
      Top = 64
      Width = 65
      Height = 33
      Caption = 'Save'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = sButton_SaveClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_Print: TsButton
      Left = 152
      Top = 24
      Width = 65
      Height = 33
      Caption = 'Print'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      OnClick = sButton_PrintClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_End: TsButton
      Left = 152
      Top = 64
      Width = 65
      Height = 33
      Caption = 'Close'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 5
      OnClick = sButton_EndClick
      SkinData.SkinSection = 'BUTTON'
    end
  end
  object GraphTimer: TTimer
    Enabled = False
    OnTimer = GraphTimerTimer
    Left = 80
    Top = 160
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '*.bmp'
    Filter = 'Bitmap Files(*.bmp)|*.bmp'
    Left = 48
    Top = 160
  end
  object PrintDialog: TPrintDialog
    Left = 16
    Top = 160
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
    Left = 120
    Top = 160
  end
end
