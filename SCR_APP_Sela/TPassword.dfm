object PassForm: TPassForm
  Left = 299
  Top = 163
  BorderStyle = bsDialog
  Caption = 'Password'
  ClientHeight = 207
  ClientWidth = 330
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 330
    Height = 57
    Align = alTop
    Color = 10967041
    TabOrder = 3
    object Label1: TLabel
      Left = 95
      Top = 11
      Width = 144
      Height = 39
      Caption = 'PASSWORD'
      Color = 10967041
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -32
      Font.Name = 'Impact'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object BitBtn_OK: TButton
    Left = 48
    Top = 144
    Width = 113
    Height = 41
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = BitBtn_OKClick
  end
  object BitBtn_Exit: TButton
    Left = 168
    Top = 144
    Width = 113
    Height = 41
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = BitBtn_ExitClick
  end
  object EditPasswrod: TMaskEdit
    Left = 96
    Top = 80
    Width = 137
    Height = 33
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = 'Microsoft IME 2003'
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 0
    OnKeyPress = EditPasswrodKeyPress
  end
  object Memo1: TMemo
    Left = 80
    Top = 328
    Width = 185
    Height = 89
    TabStop = False
    ImeName = 'Microsoft IME 2003'
    TabOrder = 4
    Visible = False
  end
  object MaskEdit_CPass1: TMaskEdit
    Left = 88
    Top = 232
    Width = 137
    Height = 33
    TabStop = False
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = 'Microsoft IME 2003'
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 5
  end
  object MaskEdit_CPass2: TMaskEdit
    Left = 88
    Top = 272
    Width = 137
    Height = 33
    TabStop = False
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = 'Microsoft IME 2003'
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 6
  end
  object Button_CPass: TButton
    Left = 232
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Change'
    TabOrder = 7
    TabStop = False
    OnClick = Button_CPassClick
  end
end
