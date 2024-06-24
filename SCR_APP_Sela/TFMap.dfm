object FMapForm: TFMapForm
  Left = 163
  Top = 138
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Fire Map'
  ClientHeight = 456
  ClientWidth = 633
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
  object ETVMapBox: TGroupBox
    Left = 8
    Top = 8
    Width = 617
    Height = 377
    Caption = 'Map Info.'
    TabOrder = 0
    object RPMReadBtn: TBitBtn
      Left = 446
      Top = 18
      Width = 75
      Height = 25
      Caption = 'Read Colum'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = RPMReadBtnClick
    end
    object RPMWriteBtn: TBitBtn
      Left = 525
      Top = 18
      Width = 75
      Height = 25
      Caption = 'Write Colum'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = RPMWriteBtnClick
    end
    object T2ReadBtn: TBitBtn
      Left = 10
      Top = 336
      Width = 75
      Height = 25
      Caption = 'Read Row'
      TabOrder = 2
      OnClick = T2ReadBtnClick
    end
    object T2WriteBtn: TBitBtn
      Left = 88
      Top = 336
      Width = 75
      Height = 25
      Caption = 'Write Row'
      TabOrder = 3
      OnClick = T2WriteBtnClick
    end
    object StringGrid_Map: TStringGrid
      Left = 15
      Top = 50
      Width = 588
      Height = 280
      ColCount = 11
      DefaultColWidth = 52
      RowCount = 11
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goAlwaysShowEditor, goThumbTracking]
      ParentFont = False
      ScrollBars = ssNone
      TabOrder = 4
      OnExit = StringGrid_MapExit
      OnSetEditText = StringGrid_MapSetEditText
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 392
    Width = 305
    Height = 57
    Caption = 'To PC'
    TabOrder = 1
    object SaveBtn: TSpeedButton
      Left = 160
      Top = 16
      Width = 105
      Height = 25
      Caption = 'Map SAVE'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnClick = SaveBtnClick
    end
    object LoadBtn: TSpeedButton
      Left = 46
      Top = 16
      Width = 107
      Height = 25
      Caption = 'Map LOAD'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnClick = LoadBtnClick
    end
  end
  object GroupBox1: TGroupBox
    Left = 320
    Top = 392
    Width = 305
    Height = 57
    Caption = 'To MCU'
    TabOrder = 2
    object MapReadBtn: TBitBtn
      Left = 48
      Top = 16
      Width = 105
      Height = 25
      Caption = 'Read MAP'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = MapReadBtnClick
    end
    object MapWriteBtn: TBitBtn
      Left = 160
      Top = 16
      Width = 105
      Height = 25
      Caption = 'Write MAP'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = MapWriteBtnClick
    end
  end
  object Memo: TMemo
    Left = 408
    Top = 457
    Width = 225
    Height = 113
    ImeName = 'Microsoft IME 2010'
    ScrollBars = ssBoth
    TabOrder = 3
    Visible = False
    WantReturns = False
    WordWrap = False
  end
  object SaveDialog: TSaveDialog
    Filter = 'Map File(*.map)|*.map'
    Left = 16
    Top = 16
  end
  object OpenDialog: TOpenDialog
    Filter = 'Map File(*.map)|*.map'
    Left = 48
    Top = 16
  end
end
