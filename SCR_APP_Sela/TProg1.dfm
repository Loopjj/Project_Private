object Prog1Form: TProg1Form
  Left = 222
  Top = 119
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'OBD Program'
  ClientHeight = 410
  ClientWidth = 536
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
  object Label1: TLabel
    Left = 416
    Top = 181
    Width = 67
    Height = 13
    Caption = 'Download File'
  end
  object sGauge: TsGauge
    Left = 8
    Top = 336
    Width = 521
    Height = 17
    SkinData.SkinSection = 'GAUGE'
    ForeColor = clBlack
    Suffix = '%'
  end
  object Edit_Fname: TEdit
    Left = 8
    Top = 301
    Width = 521
    Height = 21
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    TabOrder = 5
  end
  object Memo: TMemo
    Left = 368
    Top = 7
    Width = 161
    Height = 282
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    ParentFont = False
    TabOrder = 4
    WantReturns = False
  end
  object DriveComboBox: TDriveComboBox
    Left = 8
    Top = 8
    Width = 177
    Height = 22
    Ctl3D = True
    DirList = DirectoryListBox
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 0
  end
  object DirectoryListBox: TDirectoryListBox
    Left = 7
    Top = 36
    Width = 178
    Height = 253
    FileList = FileListBox
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    ItemHeight = 16
    ParentFont = False
    TabOrder = 1
  end
  object FilterComboBox: TFilterComboBox
    Left = 192
    Top = 8
    Width = 169
    Height = 24
    FileList = FileListBox
    Filter = 'HEX files (*.HEX)|*.HEX|all files (*.*)|*.*'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    ParentFont = False
    TabOrder = 2
  end
  object FileListBox: TFileListBox
    Left = 192
    Top = 36
    Width = 169
    Height = 253
    FileType = [ftReadOnly, ftHidden, ftNormal]
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    ItemHeight = 14
    Mask = '*.HEX'
    MultiSelect = True
    ParentFont = False
    TabOrder = 3
    OnClick = FileListBoxClick
  end
  object sButton_Program: TsButton
    Left = 112
    Top = 368
    Width = 97
    Height = 25
    Caption = 'Program'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 6
    OnClick = sButton_ProgramClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sButton_Cancel: TsButton
    Left = 224
    Top = 368
    Width = 97
    Height = 25
    Caption = 'Cancel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 7
    OnClick = sButton_CancelClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sButton_Exit: TsButton
    Left = 336
    Top = 368
    Width = 97
    Height = 25
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 8
    OnClick = sButton_ExitClick
    SkinData.SkinSection = 'BUTTON'
  end
  object ProgTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = ProgTimerTimer
  end
end
