object SaveAsForm: TSaveAsForm
  Left = 242
  Top = 85
  BorderStyle = bsDialog
  Caption = 'Save As'
  ClientHeight = 294
  ClientWidth = 537
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
  object DriveComboBox: TDriveComboBox
    Left = 8
    Top = 8
    Width = 273
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
    Top = 40
    Width = 522
    Height = 217
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    ItemHeight = 16
    ParentFont = False
    TabOrder = 1
    OnChange = DirectoryListBoxChange
  end
  object sButton_SavePath: TsButton
    Left = 296
    Top = 8
    Width = 113
    Height = 25
    Caption = 'Save Path'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = sButton_SavePathClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sButton_Exit: TsButton
    Left = 416
    Top = 8
    Width = 113
    Height = 25
    Caption = 'Close'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    OnClick = sButton_ExitClick
    SkinData.SkinSection = 'BUTTON'
  end
  object Edit_Fname: TEdit
    Left = 8
    Top = 264
    Width = 521
    Height = 21
    ImeName = 'Microsoft IME 2003'
    TabOrder = 4
  end
end
