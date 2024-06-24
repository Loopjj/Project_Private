object CanForm: TCanForm
  Left = 324
  Top = 284
  Width = 482
  Height = 244
  Caption = 'CAN Settings'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label31: TLabel
    Left = 15
    Top = 25
    Width = 74
    Height = 16
    Caption = 'PROTOCOL'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label32: TLabel
    Left = 32
    Top = 57
    Width = 58
    Height = 16
    Caption = 'CAN BPS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object sComboBox_CAN_Protocol: TsComboBox
    Left = 96
    Top = 20
    Width = 121
    Height = 24
    Alignment = taLeftJustify
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'MS Sans Serif'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = 'Microsoft IME 2003'
    ItemHeight = 16
    ItemIndex = -1
    ParentFont = False
    TabOrder = 0
    Text = 'Standard(2.0A)'
    Items.Strings = (
      'Standard(2.0A)'
      'Extended(2.0B)')
  end
  object sComboBox_CAN_BSP: TsComboBox
    Left = 96
    Top = 52
    Width = 121
    Height = 24
    Alignment = taLeftJustify
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'MS Sans Serif'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = 'Microsoft IME 2003'
    ItemHeight = 16
    ItemIndex = -1
    ParentFont = False
    TabOrder = 1
    Text = '500K'
    Items.Strings = (
      '1M'
      '500K'
      '250K'
      '200K'
      '125K'
      '100K'
      '50K'
      '40K'
      '25K')
  end
  object sButton_CAN_Reset: TsButton
    Left = 232
    Top = 160
    Width = 81
    Height = 24
    Caption = 'Reset'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = sButton_CAN_ResetClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sButton_CAN_Read: TsButton
    Left = 52
    Top = 160
    Width = 85
    Height = 24
    Caption = 'Read'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    OnClick = sButton_CAN_ReadClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sButton_CAN_Set: TsButton
    Left = 145
    Top = 160
    Width = 80
    Height = 24
    Caption = 'Set'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
    OnClick = sButton_CAN_SetClick
    SkinData.SkinSection = 'BUTTON'
  end
  object Waring: TGroupBox
    Left = 232
    Top = 16
    Width = 217
    Height = 129
    Caption = 'CAN '#50640#47084' '#48156#49373#49884' '#50508#47548#50668#48512' '#49444#51221
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    object CheckBox_BUSOff: TCheckBox
      Left = 16
      Top = 32
      Width = 169
      Height = 17
      Caption = 'CAN BUS-Off '#50640#47084' '#50508#47548
      TabOrder = 0
    end
    object CheckBox_CANError: TCheckBox
      Left = 16
      Top = 64
      Width = 185
      Height = 17
      Caption = 'CAN Error-Passive '#50640#47084' '#50508#47548
      TabOrder = 1
    end
    object CheckBox_CANWaring: TCheckBox
      Left = 16
      Top = 96
      Width = 169
      Height = 17
      Caption = 'CAN Warining '#50640#47084' '#50508#47548
      TabOrder = 2
    end
  end
  object CheckBox_AutoRest: TCheckBox
    Left = 24
    Top = 88
    Width = 185
    Height = 17
    Caption = 'CAN BUS-Off '#49884' '#51088#46041#47532#49483' '#49324#50857
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
  end
  object sButton_Ver: TsButton
    Left = 52
    Top = 128
    Width = 85
    Height = 24
    Caption = 'Version'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 7
    OnClick = sButton_VerClick
    SkinData.SkinSection = 'BUTTON'
  end
  object sButton_End: TsButton
    Left = 320
    Top = 160
    Width = 81
    Height = 24
    Caption = 'Close'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 8
    OnClick = sButton_EndClick
    SkinData.SkinSection = 'BUTTON'
  end
end
