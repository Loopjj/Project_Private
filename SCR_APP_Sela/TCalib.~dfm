object CalibForm: TCalibForm
  Left = 26
  Top = 113
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Calibration'
  ClientHeight = 725
  ClientWidth = 929
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
  object GroupBox5: TGroupBox
    Left = 16
    Top = 15
    Width = 897
    Height = 450
    Caption = 'MAF'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object Label14: TLabel
      Left = 122
      Top = 336
      Width = 55
      Height = 13
      Caption = 'MAF ADC'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label15: TLabel
      Left = 122
      Top = 360
      Width = 52
      Height = 13
      Caption = 'MAF Volt'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label9: TLabel
      Left = 162
      Top = 50
      Width = 28
      Height = 13
      Caption = 'Offset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 162
      Top = 98
      Width = 28
      Height = 13
      Caption = 'Offset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 162
      Top = 146
      Width = 28
      Height = 13
      Caption = 'Offset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label11: TLabel
      Left = 162
      Top = 194
      Width = 28
      Height = 13
      Caption = 'Offset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label13: TLabel
      Left = 162
      Top = 242
      Width = 28
      Height = 13
      Caption = 'Offset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 312
      Top = 344
      Width = 62
      Height = 13
      Caption = 'ADC Value'
    end
    object Label6: TLabel
      Left = 312
      Top = 368
      Width = 62
      Height = 13
      Caption = 'MAF Value'
    end
    object Chart1: TChart
      Left = 320
      Top = 16
      Width = 545
      Height = 297
      BackWall.Brush.Color = clWhite
      BackWall.Brush.Style = bsClear
      Foot.Alignment = taRightJustify
      Foot.Text.Strings = (
        'Voltage')
      Foot.Visible = False
      MarginRight = 6
      Title.Text.Strings = (
        'MAF')
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Title.Caption = 'Air Mass Flow(kg/h)'
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.ExactDateTime = False
      LeftAxis.Increment = 0.1
      LeftAxis.Maximum = 4096
      LeftAxis.Minimum = 9
      LeftAxis.Title.Caption = 'ADC'
      Legend.Inverted = True
      Legend.ShadowColor = clSilver
      Legend.Visible = False
      View3D = False
      TabOrder = 0
      object Button_Draw: TButton
        Left = 0
        Top = 272
        Width = 65
        Height = 25
        Caption = 'Draw'
        TabOrder = 0
        OnClick = Button_DrawClick
      end
      object Series_Maf: TLineSeries
        Cursor = crCross
        Marks.ArrowLength = 8
        Marks.Style = smsValue
        Marks.Transparent = True
        Marks.Visible = True
        SeriesColor = clRed
        Pointer.HorizSize = 2
        Pointer.InflateMargins = True
        Pointer.Style = psCircle
        Pointer.VertSize = 2
        Pointer.Visible = True
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
    object Edit_CalOffset: TEdit
      Left = 190
      Top = 356
      Width = 51
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 1
      Text = '0'
    end
    object Edit_CalADC: TEdit
      Left = 190
      Top = 332
      Width = 51
      Height = 21
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 2
      Text = '0'
    end
    object sBitBtn_MAFADCRead: TsBitBtn
      Left = 32
      Top = 352
      Width = 67
      Height = 25
      Caption = 'ADC'
      TabOrder = 3
      OnClick = sBitBtn_MAFADCReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object Edit_ADC1: TEdit
      Left = 98
      Top = 46
      Width = 47
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 4
      Text = '0'
    end
    object Edit_Offset1: TEdit
      Left = 198
      Top = 46
      Width = 47
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 5
      Text = '0'
    end
    object sButton_MAFRead: TsButton
      Left = 106
      Top = 400
      Width = 71
      Height = 25
      Caption = 'Read'
      TabOrder = 6
      OnClick = sButton_MAFReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_MAFWrite: TsButton
      Left = 185
      Top = 400
      Width = 71
      Height = 25
      Caption = 'Set'
      TabOrder = 7
      OnClick = sButton_MAFWriteClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_MAFDefault: TsButton
      Left = 32
      Top = 400
      Width = 67
      Height = 25
      Caption = 'Default'
      TabOrder = 8
      OnClick = sButton_MAFDefaultClick
      SkinData.SkinSection = 'BUTTON'
    end
    object Edit_ADC2: TEdit
      Left = 98
      Top = 94
      Width = 47
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 9
      Text = '0'
    end
    object Edit_Offset2: TEdit
      Left = 198
      Top = 94
      Width = 47
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
    object Edit_ADC3: TEdit
      Left = 98
      Top = 142
      Width = 47
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 11
      Text = '0'
    end
    object Edit_Offset3: TEdit
      Left = 198
      Top = 142
      Width = 47
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
    object Edit_ADC4: TEdit
      Left = 98
      Top = 190
      Width = 47
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 13
      Text = '0'
    end
    object Edit_Offset4: TEdit
      Left = 198
      Top = 190
      Width = 47
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 14
      Text = '0'
    end
    object Edit_ADC5: TEdit
      Left = 98
      Top = 238
      Width = 47
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 15
      Text = '0'
    end
    object Edit_Offset5: TEdit
      Left = 198
      Top = 238
      Width = 47
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 16
      Text = '0'
    end
    object sButton_CRead: TsButton
      Left = 450
      Top = 400
      Width = 71
      Height = 25
      Caption = 'Read'
      TabOrder = 17
      OnClick = sButton_CReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_CSet: TsButton
      Left = 529
      Top = 400
      Width = 71
      Height = 25
      Caption = 'Set'
      TabOrder = 18
      OnClick = sButton_CSetClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_CDefault: TsButton
      Left = 376
      Top = 400
      Width = 67
      Height = 25
      Caption = 'Default'
      TabOrder = 19
      OnClick = sButton_CDefaultClick
      SkinData.SkinSection = 'BUTTON'
    end
    object Panel1: TPanel
      Left = 292
      Top = 20
      Width = 3
      Height = 405
      TabOrder = 20
    end
    object sButton_PCRead: TsButton
      Left = 714
      Top = 400
      Width = 71
      Height = 25
      Caption = 'PC Read'
      TabOrder = 21
      OnClick = sButton_PCReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_PCWrite: TsButton
      Left = 793
      Top = 400
      Width = 71
      Height = 25
      Caption = 'PC Write'
      TabOrder = 22
      OnClick = sButton_PCWriteClick
      SkinData.SkinSection = 'BUTTON'
    end
    object Edit_MAF00: TEdit
      Left = 386
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 23
      Text = '0'
      OnDblClick = Edit_MAF00DblClick
    end
    object Edit_MAF01: TEdit
      Left = 434
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 24
      Text = '0'
      OnDblClick = Edit_MAF01DblClick
    end
    object Edit_MAF02: TEdit
      Left = 482
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 25
      Text = '0'
      OnDblClick = Edit_MAF02DblClick
    end
    object Edit_MAF03: TEdit
      Left = 530
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 26
      Text = '0'
      OnDblClick = Edit_MAF03DblClick
    end
    object Edit_MAF04: TEdit
      Left = 578
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 27
      Text = '0'
      OnDblClick = Edit_MAF04DblClick
    end
    object Edit_MAF05: TEdit
      Left = 626
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 28
      Text = '0'
      OnDblClick = Edit_MAF05DblClick
    end
    object Edit_MAF06: TEdit
      Left = 674
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 29
      Text = '0'
      OnDblClick = Edit_MAF06DblClick
    end
    object Edit_MAF07: TEdit
      Left = 722
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 30
      Text = '0'
      OnDblClick = Edit_MAF07DblClick
    end
    object Edit_MAF08: TEdit
      Left = 770
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 31
      Text = '0'
      OnDblClick = Edit_MAF08DblClick
    end
    object Edit_MAF09: TEdit
      Left = 818
      Top = 337
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 32
      Text = '0'
      OnDblClick = Edit_MAF09DblClick
    end
    object Edit_MAF10: TEdit
      Left = 386
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 33
      Text = '0'
    end
    object Edit_MAF11: TEdit
      Left = 434
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 34
      Text = '0'
    end
    object Edit_MAF12: TEdit
      Left = 482
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 35
      Text = '0'
    end
    object Edit_MAF13: TEdit
      Left = 530
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 36
      Text = '0'
    end
    object Edit_MAF14: TEdit
      Left = 578
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 37
      Text = '0'
    end
    object Edit_MAF15: TEdit
      Left = 626
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 38
      Text = '0'
    end
    object Edit_MAF16: TEdit
      Left = 674
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 39
      Text = '0'
    end
    object Edit_MAF17: TEdit
      Left = 722
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 40
      Text = '0'
    end
    object Edit_MAF18: TEdit
      Left = 770
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 41
      Text = '0'
    end
    object Edit_MAF19: TEdit
      Left = 818
      Top = 361
      Width = 47
      Height = 21
      TabStop = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 42
      Text = '0'
    end
    object sBitBtn_MAF1V: TsBitBtn
      Left = 24
      Top = 44
      Width = 65
      Height = 25
      Caption = 'MAF 1V'
      TabOrder = 43
      OnClick = sBitBtn_MAF1VClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sBitBtn_MAF2V: TsBitBtn
      Left = 24
      Top = 92
      Width = 65
      Height = 25
      Caption = 'MAF 2V'
      TabOrder = 44
      OnClick = sBitBtn_MAF2VClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sBitBtn_MAF3V: TsBitBtn
      Left = 24
      Top = 140
      Width = 65
      Height = 25
      Caption = 'MAF 3V'
      TabOrder = 45
      OnClick = sBitBtn_MAF3VClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sBitBtn_MAF4V: TsBitBtn
      Left = 24
      Top = 188
      Width = 65
      Height = 25
      Caption = 'MAF 4V'
      TabOrder = 46
      OnClick = sBitBtn_MAF4VClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sBitBtn_MAF45V: TsBitBtn
      Left = 24
      Top = 236
      Width = 65
      Height = 25
      Caption = 'MAF 4.5V'
      TabOrder = 47
      OnClick = sBitBtn_MAF45VClick
      SkinData.SkinSection = 'BUTTON'
    end
  end
  object GroupBox4: TGroupBox
    Left = 16
    Top = 469
    Width = 297
    Height = 241
    Caption = 'TEMPERATURE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    object Label18: TLabel
      Left = 19
      Top = 75
      Width = 58
      Height = 13
      Caption = 'RANGE ('#8451')'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label19: TLabel
      Left = 19
      Top = 37
      Width = 19
      Height = 13
      Caption = 'NO.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label20: TLabel
      Left = 195
      Top = 75
      Width = 7
      Height = 13
      Caption = '~'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Edit_TRangeL: TEdit
      Left = 114
      Top = 70
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 0
      Text = '0'
    end
    object Edit_TRangeH: TEdit
      Left = 210
      Top = 70
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 1
      Text = '0'
    end
    object BitBtn_TCalLow: TBitBtn
      Left = 110
      Top = 154
      Width = 75
      Height = 23
      Caption = 'Cal. Low'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = BitBtn_TCalLowClick
    end
    object BitBtn_TCalHigh: TBitBtn
      Left = 109
      Top = 114
      Width = 76
      Height = 23
      Caption = 'Cal. High'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = BitBtn_TCalHighClick
    end
    object Edit_TCalLow: TEdit
      Left = 206
      Top = 156
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 4
      Text = '0'
    end
    object Edit_TCalHigh: TEdit
      Left = 206
      Top = 116
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 5
      Text = '0'
    end
    object CB_TNo: TComboBox
      Left = 114
      Top = 31
      Width = 42
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ItemHeight = 13
      ParentFont = False
      TabOrder = 6
      Text = '1'
      Items.Strings = (
        '1'
        '2')
    end
    object sButton_TRead: TsButton
      Left = 112
      Top = 200
      Width = 71
      Height = 25
      Caption = 'Read'
      TabOrder = 7
      OnClick = sButton_TReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_TWrite: TsButton
      Left = 190
      Top = 200
      Width = 71
      Height = 25
      Caption = 'Set'
      TabOrder = 8
      OnClick = sButton_TWriteClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_dTress: TsButton
      Left = 34
      Top = 200
      Width = 71
      Height = 25
      Caption = 'Default'
      TabOrder = 9
      OnClick = sButton_dTressClick
      SkinData.SkinSection = 'BUTTON'
    end
    object Edit_TCalADC: TEdit
      Left = 28
      Top = 156
      Width = 59
      Height = 21
      Enabled = False
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
    object sBitBtn_TADCRead: TsBitBtn
      Left = 20
      Top = 112
      Width = 75
      Height = 25
      Caption = 'ADC'
      TabOrder = 11
      OnClick = sBitBtn_TADCReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object CheckBox_TNo: TCheckBox
      Left = 168
      Top = 32
      Width = 73
      Height = 17
      Caption = 'Enable'
      TabOrder = 12
    end
  end
  object GroupBox1: TGroupBox
    Left = 320
    Top = 469
    Width = 305
    Height = 241
    Caption = 'SUPPLY PRESSURE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    object Label16: TLabel
      Left = 19
      Top = 75
      Width = 70
      Height = 13
      Caption = 'RANGE (mbar)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label25: TLabel
      Left = 19
      Top = 37
      Width = 19
      Height = 13
      Caption = 'NO.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label17: TLabel
      Left = 195
      Top = 75
      Width = 7
      Height = 13
      Caption = '~'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Edit_PRangeL: TEdit
      Left = 114
      Top = 70
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 0
      Text = '0'
    end
    object Edit_PRangeH: TEdit
      Left = 210
      Top = 70
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 1
      Text = '0'
    end
    object BitBtn_PCalLow: TBitBtn
      Left = 110
      Top = 154
      Width = 75
      Height = 23
      Caption = 'Cal. Low'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = BitBtn_PCalLowClick
    end
    object BitBtn_PCalHigh: TBitBtn
      Left = 109
      Top = 114
      Width = 76
      Height = 23
      Caption = 'Cal. High'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = BitBtn_PCalHighClick
    end
    object Edit_PCalLow: TEdit
      Left = 206
      Top = 156
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 4
      Text = '0'
    end
    object Edit_PCalHigh: TEdit
      Left = 206
      Top = 116
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ParentFont = False
      TabOrder = 5
      Text = '0'
    end
    object CB_PNo: TComboBox
      Left = 114
      Top = 31
      Width = 42
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ItemHeight = 13
      ParentFont = False
      TabOrder = 6
      Text = '1'
      Items.Strings = (
        '1')
    end
    object sButton_PRead: TsButton
      Left = 112
      Top = 200
      Width = 71
      Height = 25
      Caption = 'Read'
      TabOrder = 7
      OnClick = sButton_PReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_PWrite: TsButton
      Left = 190
      Top = 200
      Width = 71
      Height = 25
      Caption = 'Set'
      TabOrder = 8
      OnClick = sButton_PWriteClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_dPress: TsButton
      Left = 34
      Top = 200
      Width = 71
      Height = 25
      Caption = 'Default'
      TabOrder = 9
      OnClick = sButton_dPressClick
      SkinData.SkinSection = 'BUTTON'
    end
    object Edit_PCalADC: TEdit
      Left = 28
      Top = 156
      Width = 59
      Height = 21
      Enabled = False
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
    object sBitBtn_PADCRead: TsBitBtn
      Left = 20
      Top = 112
      Width = 75
      Height = 25
      Caption = 'ADC'
      TabOrder = 11
      OnClick = sBitBtn_PADCReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object CheckBox_PNo: TCheckBox
      Left = 176
      Top = 32
      Width = 73
      Height = 17
      Caption = 'Enable'
      TabOrder = 12
    end
  end
  object GroupBox2: TGroupBox
    Left = 632
    Top = 469
    Width = 281
    Height = 241
    Caption = 'NOx SENSOR'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    object Label1: TLabel
      Left = 19
      Top = 37
      Width = 19
      Height = 13
      Caption = 'NO.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object CB_NOx: TComboBox
      Left = 79
      Top = 31
      Width = 42
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
      ItemHeight = 13
      ParentFont = False
      TabOrder = 0
      Text = '1'
      Items.Strings = (
        '1'
        '2')
    end
    object CheckBox_NOx: TCheckBox
      Left = 136
      Top = 32
      Width = 73
      Height = 17
      Caption = 'Enable'
      TabOrder = 1
    end
    object sButton_NOxRead: TsButton
      Left = 40
      Top = 200
      Width = 71
      Height = 25
      Caption = 'Read'
      TabOrder = 2
      OnClick = sButton_NOxReadClick
      SkinData.SkinSection = 'BUTTON'
    end
    object sButton_NOxSet: TsButton
      Left = 122
      Top = 200
      Width = 71
      Height = 25
      Caption = 'Set'
      TabOrder = 3
      OnClick = sButton_NOxSetClick
      SkinData.SkinSection = 'BUTTON'
    end
  end
  object Memo: TMemo
    Left = 1032
    Top = 176
    Width = 185
    Height = 89
    ScrollBars = ssBoth
    TabOrder = 4
  end
  object OpenDialog: TOpenDialog
    Left = 64
    Top = 8
  end
  object SaveDialog: TSaveDialog
    Left = 104
    Top = 8
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 144
    Top = 8
  end
end
