object ConvertForm: TConvertForm
  Left = 319
  Top = 152
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Backup data conversion'
  ClientHeight = 409
  ClientWidth = 511
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label_SelCount: TLabel
    Left = 367
    Top = 16
    Width = 87
    Height = 16
    Caption = '0 files selected'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label_Total: TLabel
    Left = 17
    Top = 321
    Width = 84
    Height = 14
    Caption = 'Total Progress '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label_One: TLabel
    Left = 19
    Top = 281
    Width = 77
    Height = 14
    Caption = 'File Progress '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object sGauge_One: TsGauge
    Left = 16
    Top = 296
    Width = 481
    Height = 17
    SkinData.SkinSection = 'GAUGE'
    ForeColor = clBlack
    Suffix = '%'
  end
  object sGauge_Total: TsGauge
    Left = 16
    Top = 336
    Width = 481
    Height = 17
    SkinData.SkinSection = 'GAUGE'
    ForeColor = clBlack
    Suffix = '%'
  end
  object DriveComboBox: TDriveComboBox
    Left = 8
    Top = 8
    Width = 162
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
    Width = 163
    Height = 181
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
    Left = 176
    Top = 8
    Width = 138
    Height = 24
    FileList = FileListBox
    Filter = 'lop files (*.lop)|*.lop|all files (*.*)|*.*'
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
    Left = 176
    Top = 36
    Width = 135
    Height = 182
    FileType = [ftReadOnly, ftHidden, ftNormal]
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    ItemHeight = 16
    Mask = '*.lop'
    MultiSelect = True
    ParentFont = False
    TabOrder = 3
    OnDblClick = FileListBoxDblClick
  end
  object BitBtn_SelectOne: TBitBtn
    Left = 324
    Top = 56
    Width = 25
    Height = 25
    Caption = '>'
    TabOrder = 4
    OnClick = BitBtn_SelectOneClick
  end
  object BitBtn_DeSelOne: TBitBtn
    Left = 324
    Top = 86
    Width = 25
    Height = 25
    Caption = '<'
    TabOrder = 5
    OnClick = BitBtn_DeSelOneClick
  end
  object BitBtn_SellectAll: TBitBtn
    Left = 324
    Top = 128
    Width = 25
    Height = 25
    Caption = '>>'
    TabOrder = 6
    OnClick = BitBtn_SellectAllClick
  end
  object BitBtn_DSelAll: TBitBtn
    Left = 324
    Top = 158
    Width = 25
    Height = 25
    Caption = '<<'
    TabOrder = 7
    OnClick = BitBtn_DSelAllClick
  end
  object ListBox: TListBox
    Left = 361
    Top = 36
    Width = 136
    Height = 181
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    ItemHeight = 16
    MultiSelect = True
    ParentFont = False
    Sorted = True
    TabOrder = 8
    OnDblClick = ListBoxDblClick
  end
  object sButton_OK: TsButton
    Left = 96
    Top = 368
    Width = 97
    Height = 25
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 9
    OnClick = sButton_OKClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sButton_Cancel: TsButton
    Left = 208
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
    TabOrder = 10
    OnClick = sButton_CancelClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sButton_Exit: TsButton
    Left = 320
    Top = 368
    Width = 97
    Height = 25
    Caption = 'Close'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 11
    OnClick = sButton_ExitClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sPanel1: TsPanel
    Left = 9
    Top = 232
    Width = 493
    Height = 41
    TabOrder = 12
    SkinData.SkinSection = 'PANEL'
    object Label1: TLabel
      Left = 11
      Top = 15
      Width = 94
      Height = 14
      Caption = 'File Extension    : '
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object RadioButton_Txt: TRadioButton
      Left = 128
      Top = 12
      Width = 41
      Height = 17
      Caption = 'txt'
      Checked = True
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      TabStop = True
    end
    object RadioButton_Xcl: TRadioButton
      Left = 200
      Top = 12
      Width = 41
      Height = 17
      Caption = 'xls'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 1
    end
  end
end