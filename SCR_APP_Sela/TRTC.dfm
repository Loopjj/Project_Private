object RTCForm: TRTCForm
  Left = 311
  Top = 223
  Width = 354
  Height = 213
  Caption = 'RTC Settings'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox6: TGroupBox
    Left = 4
    Top = 5
    Width = 325
    Height = 166
    Caption = 'RTC SETTINGS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object Label24: TLabel
      Left = 75
      Top = 69
      Width = 22
      Height = 13
      Caption = 'Year'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label19: TLabel
      Left = 177
      Top = 69
      Width = 30
      Height = 13
      Caption = 'Month'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label20: TLabel
      Left = 287
      Top = 69
      Width = 19
      Height = 13
      Caption = 'Day'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label21: TLabel
      Left = 75
      Top = 94
      Width = 23
      Height = 13
      Caption = 'Hour'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label22: TLabel
      Left = 177
      Top = 94
      Width = 32
      Height = 13
      Caption = 'Minute'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label23: TLabel
      Left = 287
      Top = 94
      Width = 19
      Height = 13
      Caption = 'Sec'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label31: TLabel
      Left = 15
      Top = 46
      Width = 81
      Height = 13
      AutoSize = False
      Caption = 'SETTING TIME'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label32: TLabel
      Left = 10
      Top = 18
      Width = 119
      Height = 13
      AutoSize = False
      Caption = 'CURRENT RTC TIME'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Edit_Year: TEdit
      Left = 15
      Top = 66
      Width = 41
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 1
      Text = '2000'
    end
    object UpDown_Year: TUpDown
      Left = 56
      Top = 66
      Width = 16
      Height = 21
      Associate = Edit_Year
      Min = 2000
      Max = 2099
      Position = 2000
      TabOrder = 2
      TabStop = True
      Thousands = False
      Wrap = False
    end
    object UpDown_Month: TUpDown
      Left = 158
      Top = 66
      Width = 16
      Height = 21
      Associate = Edit_Month
      Min = 1
      Max = 12
      Position = 1
      TabOrder = 4
      TabStop = True
      Wrap = False
    end
    object Edit_Month: TEdit
      Left = 117
      Top = 66
      Width = 41
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 3
      Text = '1'
    end
    object UpDown_Day: TUpDown
      Left = 268
      Top = 66
      Width = 16
      Height = 21
      Associate = Edit_Day
      Min = 1
      Max = 31
      Position = 1
      TabOrder = 6
      TabStop = True
      Wrap = False
    end
    object Edit_Day: TEdit
      Left = 227
      Top = 66
      Width = 41
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 5
      Text = '1'
    end
    object UpDown_Hour: TUpDown
      Left = 56
      Top = 91
      Width = 16
      Height = 21
      Associate = Edit_Hour
      Min = 0
      Max = 23
      Position = 0
      TabOrder = 8
      TabStop = True
      Wrap = False
    end
    object Edit_Hour: TEdit
      Left = 15
      Top = 91
      Width = 41
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 7
      Text = '0'
    end
    object UpDown_Min: TUpDown
      Left = 158
      Top = 91
      Width = 16
      Height = 21
      Associate = Edit_Min
      Min = 0
      Max = 59
      Position = 0
      TabOrder = 11
      TabStop = True
      Wrap = False
    end
    object Edit_Min: TEdit
      Left = 117
      Top = 91
      Width = 41
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 10
      Text = '0'
    end
    object Edit_Sec: TEdit
      Left = 227
      Top = 91
      Width = 41
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 12
      Text = '0'
    end
    object UpDown_Sec: TUpDown
      Left = 268
      Top = 91
      Width = 16
      Height = 21
      Associate = Edit_Sec
      Min = 0
      Max = 59
      Position = 0
      TabOrder = 13
      TabStop = True
      Wrap = False
    end
    object CheckBox_PCTime: TCheckBox
      Left = 146
      Top = 46
      Width = 130
      Height = 17
      Caption = 'APPLY PC TIME'
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 0
      OnClick = CheckBox_PCTimeClick
    end
    object Panel_RTC: TPanel
      Left = 128
      Top = 12
      Width = 169
      Height = 27
      Caption = '2000-01-01  00:00:00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
    end
    object sButton_RTCRead: TsButton
      Left = 24
      Top = 128
      Width = 89
      Height = 25
      Caption = 'Read'
      TabOrder = 14
      OnClick = sButton_RTCReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_RTCSet: TsButton
      Left = 120
      Top = 128
      Width = 89
      Height = 25
      Caption = 'Set'
      TabOrder = 15
      OnClick = sButton_RTCSetClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_Close: TsButton
      Left = 216
      Top = 128
      Width = 89
      Height = 25
      Caption = 'Close'
      TabOrder = 16
      OnClick = sButton_CloseClick
      SkinData.SkinSection = 'BUTTON'
    end
  end
  object Timer2: TTimer
    OnTimer = Timer2Timer
  end
end
