namespace Serial_Communication
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.metroTabControl1 = new MetroFramework.Controls.MetroTabControl();
            this.metroTabPage1 = new MetroFramework.Controls.MetroTabPage();
            this.metroButton2 = new MetroFramework.Controls.MetroButton();
            this.metroButton1 = new MetroFramework.Controls.MetroButton();
            this.comboBox_port1 = new MetroFramework.Controls.MetroComboBox();
            this.label_port = new System.Windows.Forms.Label();
            this.panel13 = new System.Windows.Forms.Panel();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.metroButton4 = new MetroFramework.Controls.MetroButton();
            this.Pedal_ADC_8 = new System.Windows.Forms.TextBox();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.Sync1 = new MetroFramework.Controls.MetroTextBox();
            this.Sync2 = new MetroFramework.Controls.MetroTextBox();
            this.StartBit = new MetroFramework.Controls.MetroTextBox();
            this.Length = new MetroFramework.Controls.MetroTextBox();
            this.Command = new MetroFramework.Controls.MetroTextBox();
            this.Checksum = new MetroFramework.Controls.MetroTextBox();
            this.Exitbit = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel55 = new MetroFramework.Controls.MetroLabel();
            this.Command1 = new MetroFramework.Controls.MetroLabel();
            this.sync1s = new MetroFramework.Controls.MetroLabel();
            this.metroLabel56 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel57 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel58 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel59 = new MetroFramework.Controls.MetroLabel();
            this.label60 = new System.Windows.Forms.Label();
            this.Flag = new System.Windows.Forms.TextBox();
            this.label61 = new System.Windows.Forms.Label();
            this.label62 = new System.Windows.Forms.Label();
            this.label63 = new System.Windows.Forms.Label();
            this.Limit = new System.Windows.Forms.TextBox();
            this.Level = new System.Windows.Forms.TextBox();
            this.Quality = new System.Windows.Forms.TextBox();
            this.panel8 = new System.Windows.Forms.Panel();
            this.label64 = new System.Windows.Forms.Label();
            this.label65 = new System.Windows.Forms.Label();
            this.label66 = new System.Windows.Forms.Label();
            this.TempIn = new System.Windows.Forms.TextBox();
            this.TempOut = new System.Windows.Forms.TextBox();
            this.DosingRate = new System.Windows.Forms.TextBox();
            this.label71 = new System.Windows.Forms.Label();
            this.label70 = new System.Windows.Forms.Label();
            this.label69 = new System.Windows.Forms.Label();
            this.NOxIn = new System.Windows.Forms.TextBox();
            this.NOxOut = new System.Windows.Forms.TextBox();
            this.MAF = new System.Windows.Forms.TextBox();
            this.panel9 = new System.Windows.Forms.Panel();
            this.label72 = new System.Windows.Forms.Label();
            this.label73 = new System.Windows.Forms.Label();
            this.label86 = new System.Windows.Forms.Label();
            this.label87 = new System.Windows.Forms.Label();
            this.UreaLvlHi = new System.Windows.Forms.TextBox();
            this.UreaLvlLo = new System.Windows.Forms.TextBox();
            this.UreaLvlAL = new System.Windows.Forms.TextBox();
            this.label90 = new System.Windows.Forms.Label();
            this.label89 = new System.Windows.Forms.Label();
            this.UreaConHi = new System.Windows.Forms.TextBox();
            this.UreaConLo = new System.Windows.Forms.TextBox();
            this.label92 = new System.Windows.Forms.Label();
            this.label91 = new System.Windows.Forms.Label();
            this.label88 = new System.Windows.Forms.Label();
            this.UreaAlHr1 = new System.Windows.Forms.TextBox();
            this.UreaAlHr2 = new System.Windows.Forms.TextBox();
            this.UreaBuzInt = new System.Windows.Forms.TextBox();
            this.Read = new MetroFramework.Controls.MetroButton();
            this.Write = new MetroFramework.Controls.MetroButton();
            this.Default = new MetroFramework.Controls.MetroButton();
            this.Set0 = new MetroFramework.Controls.MetroButton();
            this.panel10 = new System.Windows.Forms.Panel();
            this.Pedal_ADC_1 = new System.Windows.Forms.TextBox();
            this.Pedal_ADC_2 = new System.Windows.Forms.TextBox();
            this.Pedal_ADC_3 = new System.Windows.Forms.TextBox();
            this.Pedal_ADC_4 = new System.Windows.Forms.TextBox();
            this.Pedal_ADC_5 = new System.Windows.Forms.TextBox();
            this.Pedal_ADC_6 = new System.Windows.Forms.TextBox();
            this.Pedal_ADC_7 = new System.Windows.Forms.TextBox();
            this.metroButton5 = new MetroFramework.Controls.MetroButton();
            this.Test_0 = new System.Windows.Forms.Button();
            this.Test_1 = new System.Windows.Forms.Button();
            this.Test_2 = new System.Windows.Forms.Button();
            this.Test_Exit = new System.Windows.Forms.Button();
            this.metroTabControl1.SuspendLayout();
            this.metroTabPage1.SuspendLayout();
            this.panel8.SuspendLayout();
            this.panel9.SuspendLayout();
            this.panel10.SuspendLayout();
            this.SuspendLayout();
            // 
            // metroTabControl1
            // 
            this.metroTabControl1.Controls.Add(this.metroTabPage1);
            this.metroTabControl1.FontSize = MetroFramework.MetroTabControlSize.Tall;
            this.metroTabControl1.FontWeight = MetroFramework.MetroTabControlWeight.Regular;
            this.metroTabControl1.Location = new System.Drawing.Point(2, 94);
            this.metroTabControl1.Name = "metroTabControl1";
            this.metroTabControl1.SelectedIndex = 0;
            this.metroTabControl1.Size = new System.Drawing.Size(1102, 700);
            this.metroTabControl1.TabIndex = 81;
            this.metroTabControl1.UseSelectable = true;
            // 
            // metroTabPage1
            // 
            this.metroTabPage1.Controls.Add(this.metroButton5);
            this.metroTabPage1.Controls.Add(this.Pedal_ADC_7);
            this.metroTabPage1.Controls.Add(this.Pedal_ADC_8);
            this.metroTabPage1.Controls.Add(this.Pedal_ADC_6);
            this.metroTabPage1.Controls.Add(this.Pedal_ADC_5);
            this.metroTabPage1.Controls.Add(this.Pedal_ADC_4);
            this.metroTabPage1.Controls.Add(this.Pedal_ADC_3);
            this.metroTabPage1.Controls.Add(this.Pedal_ADC_2);
            this.metroTabPage1.Controls.Add(this.Pedal_ADC_1);
            this.metroTabPage1.Controls.Add(this.panel10);
            this.metroTabPage1.Controls.Add(this.panel9);
            this.metroTabPage1.Controls.Add(this.panel8);
            this.metroTabPage1.Controls.Add(this.metroLabel59);
            this.metroTabPage1.Controls.Add(this.metroLabel58);
            this.metroTabPage1.Controls.Add(this.metroLabel57);
            this.metroTabPage1.Controls.Add(this.metroLabel56);
            this.metroTabPage1.Controls.Add(this.sync1s);
            this.metroTabPage1.Controls.Add(this.Command1);
            this.metroTabPage1.Controls.Add(this.metroLabel55);
            this.metroTabPage1.Controls.Add(this.Exitbit);
            this.metroTabPage1.Controls.Add(this.Checksum);
            this.metroTabPage1.Controls.Add(this.Command);
            this.metroTabPage1.Controls.Add(this.Length);
            this.metroTabPage1.Controls.Add(this.StartBit);
            this.metroTabPage1.Controls.Add(this.Sync2);
            this.metroTabPage1.Controls.Add(this.Sync1);
            this.metroTabPage1.HorizontalScrollbarBarColor = true;
            this.metroTabPage1.HorizontalScrollbarHighlightOnWheel = false;
            this.metroTabPage1.HorizontalScrollbarSize = 10;
            this.metroTabPage1.Location = new System.Drawing.Point(4, 44);
            this.metroTabPage1.Name = "metroTabPage1";
            this.metroTabPage1.Size = new System.Drawing.Size(1094, 652);
            this.metroTabPage1.TabIndex = 0;
            this.metroTabPage1.Text = "Viewer";
            this.metroTabPage1.VerticalScrollbarBarColor = true;
            this.metroTabPage1.VerticalScrollbarHighlightOnWheel = false;
            this.metroTabPage1.VerticalScrollbarSize = 10;
            // 
            // metroButton2
            // 
            this.metroButton2.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.metroButton2.Location = new System.Drawing.Point(660, 51);
            this.metroButton2.Name = "metroButton2";
            this.metroButton2.Size = new System.Drawing.Size(94, 37);
            this.metroButton2.TabIndex = 56;
            this.metroButton2.Text = "Com Close";
            this.metroButton2.UseSelectable = true;
            this.metroButton2.Click += new System.EventHandler(this.button_disconnect_click);
            // 
            // metroButton1
            // 
            this.metroButton1.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.metroButton1.Location = new System.Drawing.Point(560, 51);
            this.metroButton1.Name = "metroButton1";
            this.metroButton1.Size = new System.Drawing.Size(94, 37);
            this.metroButton1.TabIndex = 55;
            this.metroButton1.Text = "Com Open";
            this.metroButton1.UseSelectable = true;
            this.metroButton1.Click += new System.EventHandler(this.Button_connect_Click);
            // 
            // comboBox_port1
            // 
            this.comboBox_port1.FormattingEnabled = true;
            this.comboBox_port1.ItemHeight = 23;
            this.comboBox_port1.Location = new System.Drawing.Point(433, 54);
            this.comboBox_port1.Name = "comboBox_port1";
            this.comboBox_port1.Size = new System.Drawing.Size(110, 29);
            this.comboBox_port1.TabIndex = 89;
            this.comboBox_port1.UseSelectable = true;
            // 
            // label_port
            // 
            this.label_port.AutoSize = true;
            this.label_port.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label_port.Location = new System.Drawing.Point(469, 36);
            this.label_port.Name = "label_port";
            this.label_port.Size = new System.Drawing.Size(29, 15);
            this.label_port.TabIndex = 88;
            this.label_port.Text = "Port";
            // 
            // panel13
            // 
            this.panel13.BackColor = System.Drawing.Color.DeepSkyBlue;
            this.panel13.Location = new System.Drawing.Point(22, 61);
            this.panel13.Name = "panel13";
            this.panel13.Size = new System.Drawing.Size(160, 3);
            this.panel13.TabIndex = 93;
            // 
            // timer1
            // 
            this.timer1.Interval = 1000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // metroButton4
            // 
            this.metroButton4.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.metroButton4.Location = new System.Drawing.Point(760, 51);
            this.metroButton4.Name = "metroButton4";
            this.metroButton4.Size = new System.Drawing.Size(94, 37);
            this.metroButton4.TabIndex = 94;
            this.metroButton4.Text = "Save As";
            this.metroButton4.UseSelectable = true;
            this.metroButton4.Click += new System.EventHandler(this.button1_Click);
            // 
            // Pedal_ADC_8
            // 
            this.Pedal_ADC_8.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Pedal_ADC_8.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Pedal_ADC_8.Location = new System.Drawing.Point(3, 594);
            this.Pedal_ADC_8.Name = "Pedal_ADC_8";
            this.Pedal_ADC_8.Size = new System.Drawing.Size(70, 26);
            this.Pedal_ADC_8.TabIndex = 88;
            this.Pedal_ADC_8.Text = "0";
            // 
            // timer2
            // 
            this.timer2.Interval = 500;
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // Sync1
            // 
            // 
            // 
            // 
            this.Sync1.CustomButton.Image = null;
            this.Sync1.CustomButton.Location = new System.Drawing.Point(17, 2);
            this.Sync1.CustomButton.Name = "";
            this.Sync1.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.Sync1.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.Sync1.CustomButton.TabIndex = 1;
            this.Sync1.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.Sync1.CustomButton.UseSelectable = true;
            this.Sync1.CustomButton.Visible = false;
            this.Sync1.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.Sync1.Lines = new string[0];
            this.Sync1.Location = new System.Drawing.Point(121, 12);
            this.Sync1.MaxLength = 32767;
            this.Sync1.Name = "Sync1";
            this.Sync1.PasswordChar = '\0';
            this.Sync1.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.Sync1.SelectedText = "";
            this.Sync1.SelectionLength = 0;
            this.Sync1.SelectionStart = 0;
            this.Sync1.ShortcutsEnabled = true;
            this.Sync1.Size = new System.Drawing.Size(55, 40);
            this.Sync1.TabIndex = 18;
            this.Sync1.UseSelectable = true;
            this.Sync1.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.Sync1.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // Sync2
            // 
            // 
            // 
            // 
            this.Sync2.CustomButton.Image = null;
            this.Sync2.CustomButton.Location = new System.Drawing.Point(17, 2);
            this.Sync2.CustomButton.Name = "";
            this.Sync2.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.Sync2.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.Sync2.CustomButton.TabIndex = 1;
            this.Sync2.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.Sync2.CustomButton.UseSelectable = true;
            this.Sync2.CustomButton.Visible = false;
            this.Sync2.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.Sync2.Lines = new string[0];
            this.Sync2.Location = new System.Drawing.Point(121, 65);
            this.Sync2.MaxLength = 32767;
            this.Sync2.Name = "Sync2";
            this.Sync2.PasswordChar = '\0';
            this.Sync2.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.Sync2.SelectedText = "";
            this.Sync2.SelectionLength = 0;
            this.Sync2.SelectionStart = 0;
            this.Sync2.ShortcutsEnabled = true;
            this.Sync2.Size = new System.Drawing.Size(55, 40);
            this.Sync2.TabIndex = 19;
            this.Sync2.UseSelectable = true;
            this.Sync2.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.Sync2.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // StartBit
            // 
            // 
            // 
            // 
            this.StartBit.CustomButton.Image = null;
            this.StartBit.CustomButton.Location = new System.Drawing.Point(17, 2);
            this.StartBit.CustomButton.Name = "";
            this.StartBit.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.StartBit.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.StartBit.CustomButton.TabIndex = 1;
            this.StartBit.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.StartBit.CustomButton.UseSelectable = true;
            this.StartBit.CustomButton.Visible = false;
            this.StartBit.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.StartBit.Lines = new string[0];
            this.StartBit.Location = new System.Drawing.Point(121, 111);
            this.StartBit.MaxLength = 32767;
            this.StartBit.Name = "StartBit";
            this.StartBit.PasswordChar = '\0';
            this.StartBit.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.StartBit.SelectedText = "";
            this.StartBit.SelectionLength = 0;
            this.StartBit.SelectionStart = 0;
            this.StartBit.ShortcutsEnabled = true;
            this.StartBit.Size = new System.Drawing.Size(55, 40);
            this.StartBit.TabIndex = 20;
            this.StartBit.UseSelectable = true;
            this.StartBit.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.StartBit.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // Length
            // 
            // 
            // 
            // 
            this.Length.CustomButton.Image = null;
            this.Length.CustomButton.Location = new System.Drawing.Point(17, 2);
            this.Length.CustomButton.Name = "";
            this.Length.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.Length.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.Length.CustomButton.TabIndex = 1;
            this.Length.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.Length.CustomButton.UseSelectable = true;
            this.Length.CustomButton.Visible = false;
            this.Length.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.Length.Lines = new string[0];
            this.Length.Location = new System.Drawing.Point(121, 164);
            this.Length.MaxLength = 32767;
            this.Length.Name = "Length";
            this.Length.PasswordChar = '\0';
            this.Length.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.Length.SelectedText = "";
            this.Length.SelectionLength = 0;
            this.Length.SelectionStart = 0;
            this.Length.ShortcutsEnabled = true;
            this.Length.Size = new System.Drawing.Size(55, 40);
            this.Length.TabIndex = 22;
            this.Length.UseSelectable = true;
            this.Length.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.Length.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // Command
            // 
            // 
            // 
            // 
            this.Command.CustomButton.Image = null;
            this.Command.CustomButton.Location = new System.Drawing.Point(17, 2);
            this.Command.CustomButton.Name = "";
            this.Command.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.Command.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.Command.CustomButton.TabIndex = 1;
            this.Command.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.Command.CustomButton.UseSelectable = true;
            this.Command.CustomButton.Visible = false;
            this.Command.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.Command.Lines = new string[0];
            this.Command.Location = new System.Drawing.Point(121, 225);
            this.Command.MaxLength = 32767;
            this.Command.Name = "Command";
            this.Command.PasswordChar = '\0';
            this.Command.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.Command.SelectedText = "";
            this.Command.SelectionLength = 0;
            this.Command.SelectionStart = 0;
            this.Command.ShortcutsEnabled = true;
            this.Command.Size = new System.Drawing.Size(55, 40);
            this.Command.TabIndex = 23;
            this.Command.UseSelectable = true;
            this.Command.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.Command.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // Checksum
            // 
            // 
            // 
            // 
            this.Checksum.CustomButton.Image = null;
            this.Checksum.CustomButton.Location = new System.Drawing.Point(17, 2);
            this.Checksum.CustomButton.Name = "";
            this.Checksum.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.Checksum.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.Checksum.CustomButton.TabIndex = 1;
            this.Checksum.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.Checksum.CustomButton.UseSelectable = true;
            this.Checksum.CustomButton.Visible = false;
            this.Checksum.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.Checksum.Lines = new string[0];
            this.Checksum.Location = new System.Drawing.Point(121, 285);
            this.Checksum.MaxLength = 32767;
            this.Checksum.Name = "Checksum";
            this.Checksum.PasswordChar = '\0';
            this.Checksum.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.Checksum.SelectedText = "";
            this.Checksum.SelectionLength = 0;
            this.Checksum.SelectionStart = 0;
            this.Checksum.ShortcutsEnabled = true;
            this.Checksum.Size = new System.Drawing.Size(55, 40);
            this.Checksum.TabIndex = 24;
            this.Checksum.UseSelectable = true;
            this.Checksum.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.Checksum.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // Exitbit
            // 
            // 
            // 
            // 
            this.Exitbit.CustomButton.Image = null;
            this.Exitbit.CustomButton.Location = new System.Drawing.Point(17, 2);
            this.Exitbit.CustomButton.Name = "";
            this.Exitbit.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.Exitbit.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.Exitbit.CustomButton.TabIndex = 1;
            this.Exitbit.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.Exitbit.CustomButton.UseSelectable = true;
            this.Exitbit.CustomButton.Visible = false;
            this.Exitbit.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.Exitbit.Lines = new string[0];
            this.Exitbit.Location = new System.Drawing.Point(121, 343);
            this.Exitbit.MaxLength = 32767;
            this.Exitbit.Name = "Exitbit";
            this.Exitbit.PasswordChar = '\0';
            this.Exitbit.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.Exitbit.SelectedText = "";
            this.Exitbit.SelectionLength = 0;
            this.Exitbit.SelectionStart = 0;
            this.Exitbit.ShortcutsEnabled = true;
            this.Exitbit.Size = new System.Drawing.Size(55, 40);
            this.Exitbit.TabIndex = 25;
            this.Exitbit.UseSelectable = true;
            this.Exitbit.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.Exitbit.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel55
            // 
            this.metroLabel55.AutoSize = true;
            this.metroLabel55.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel55.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel55.Location = new System.Drawing.Point(25, 174);
            this.metroLabel55.Name = "metroLabel55";
            this.metroLabel55.Size = new System.Drawing.Size(66, 25);
            this.metroLabel55.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel55.TabIndex = 63;
            this.metroLabel55.Text = "Length";
            // 
            // Command1
            // 
            this.Command1.AutoSize = true;
            this.Command1.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.Command1.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.Command1.Location = new System.Drawing.Point(14, 228);
            this.Command1.Name = "Command1";
            this.Command1.Size = new System.Drawing.Size(96, 25);
            this.Command1.Style = MetroFramework.MetroColorStyle.Black;
            this.Command1.TabIndex = 65;
            this.Command1.Text = "Command";
            // 
            // sync1s
            // 
            this.sync1s.AutoSize = true;
            this.sync1s.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.sync1s.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.sync1s.Location = new System.Drawing.Point(33, 22);
            this.sync1s.Name = "sync1s";
            this.sync1s.Size = new System.Drawing.Size(58, 25);
            this.sync1s.Style = MetroFramework.MetroColorStyle.Black;
            this.sync1s.TabIndex = 67;
            this.sync1s.Text = "Sync1";
            // 
            // metroLabel56
            // 
            this.metroLabel56.AutoSize = true;
            this.metroLabel56.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel56.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel56.Location = new System.Drawing.Point(33, 73);
            this.metroLabel56.Name = "metroLabel56";
            this.metroLabel56.Size = new System.Drawing.Size(58, 25);
            this.metroLabel56.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel56.TabIndex = 68;
            this.metroLabel56.Text = "Sync2";
            // 
            // metroLabel57
            // 
            this.metroLabel57.AutoSize = true;
            this.metroLabel57.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel57.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel57.Location = new System.Drawing.Point(25, 122);
            this.metroLabel57.Name = "metroLabel57";
            this.metroLabel57.Size = new System.Drawing.Size(74, 25);
            this.metroLabel57.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel57.TabIndex = 69;
            this.metroLabel57.Text = "Start bit";
            // 
            // metroLabel58
            // 
            this.metroLabel58.AutoSize = true;
            this.metroLabel58.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel58.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel58.Location = new System.Drawing.Point(14, 285);
            this.metroLabel58.Name = "metroLabel58";
            this.metroLabel58.Size = new System.Drawing.Size(93, 25);
            this.metroLabel58.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel58.TabIndex = 70;
            this.metroLabel58.Text = "Checksum";
            // 
            // metroLabel59
            // 
            this.metroLabel59.AutoSize = true;
            this.metroLabel59.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel59.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel59.Location = new System.Drawing.Point(26, 337);
            this.metroLabel59.Name = "metroLabel59";
            this.metroLabel59.Size = new System.Drawing.Size(65, 25);
            this.metroLabel59.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel59.TabIndex = 71;
            this.metroLabel59.Text = "Exit bit";
            // 
            // label60
            // 
            this.label60.AutoSize = true;
            this.label60.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label60.Font = new System.Drawing.Font("휴먼둥근헤드라인", 26.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label60.Location = new System.Drawing.Point(4, 19);
            this.label60.Name = "label60";
            this.label60.Size = new System.Drawing.Size(173, 36);
            this.label60.TabIndex = 0;
            this.label60.Text = "제한 단계";
            // 
            // Flag
            // 
            this.Flag.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Flag.Font = new System.Drawing.Font("Impact", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Flag.Location = new System.Drawing.Point(26, 70);
            this.Flag.Name = "Flag";
            this.Flag.Size = new System.Drawing.Size(129, 86);
            this.Flag.TabIndex = 1;
            this.Flag.Text = "0";
            // 
            // label61
            // 
            this.label61.AutoSize = true;
            this.label61.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label61.Font = new System.Drawing.Font("휴먼둥근헤드라인", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label61.Location = new System.Drawing.Point(216, 42);
            this.label61.Name = "label61";
            this.label61.Size = new System.Drawing.Size(129, 30);
            this.label61.TabIndex = 87;
            this.label61.Text = "출력감소";
            // 
            // label62
            // 
            this.label62.AutoSize = true;
            this.label62.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label62.Font = new System.Drawing.Font("Impact", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label62.Location = new System.Drawing.Point(385, 38);
            this.label62.Name = "label62";
            this.label62.Size = new System.Drawing.Size(151, 39);
            this.label62.TabIndex = 88;
            this.label62.Text = "Urea Level";
            // 
            // label63
            // 
            this.label63.AutoSize = true;
            this.label63.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label63.Font = new System.Drawing.Font("Impact", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label63.Location = new System.Drawing.Point(583, 38);
            this.label63.Name = "label63";
            this.label63.Size = new System.Drawing.Size(108, 39);
            this.label63.TabIndex = 89;
            this.label63.Text = "Quality";
            // 
            // Limit
            // 
            this.Limit.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Limit.Font = new System.Drawing.Font("Impact", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Limit.Location = new System.Drawing.Point(210, 83);
            this.Limit.Name = "Limit";
            this.Limit.Size = new System.Drawing.Size(135, 66);
            this.Limit.TabIndex = 90;
            this.Limit.Text = "0%";
            // 
            // Level
            // 
            this.Level.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Level.Font = new System.Drawing.Font("Impact", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Level.Location = new System.Drawing.Point(403, 83);
            this.Level.Name = "Level";
            this.Level.Size = new System.Drawing.Size(128, 66);
            this.Level.TabIndex = 91;
            this.Level.Text = "0";
            // 
            // Quality
            // 
            this.Quality.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Quality.Font = new System.Drawing.Font("Impact", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Quality.Location = new System.Drawing.Point(572, 83);
            this.Quality.Name = "Quality";
            this.Quality.Size = new System.Drawing.Size(132, 66);
            this.Quality.TabIndex = 92;
            this.Quality.Text = "0";
            // 
            // panel8
            // 
            this.panel8.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panel8.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.panel8.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel8.Controls.Add(this.Test_Exit);
            this.panel8.Controls.Add(this.Test_2);
            this.panel8.Controls.Add(this.Test_1);
            this.panel8.Controls.Add(this.Test_0);
            this.panel8.Controls.Add(this.Quality);
            this.panel8.Controls.Add(this.Level);
            this.panel8.Controls.Add(this.Limit);
            this.panel8.Controls.Add(this.label63);
            this.panel8.Controls.Add(this.label62);
            this.panel8.Controls.Add(this.label61);
            this.panel8.Controls.Add(this.Flag);
            this.panel8.Controls.Add(this.label60);
            this.panel8.Location = new System.Drawing.Point(197, 22);
            this.panel8.Name = "panel8";
            this.panel8.Size = new System.Drawing.Size(894, 211);
            this.panel8.TabIndex = 79;
            // 
            // label64
            // 
            this.label64.AutoSize = true;
            this.label64.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label64.Font = new System.Drawing.Font("Impact", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label64.Location = new System.Drawing.Point(46, 55);
            this.label64.Name = "label64";
            this.label64.Size = new System.Drawing.Size(121, 39);
            this.label64.TabIndex = 1;
            this.label64.Text = "Temp In";
            // 
            // label65
            // 
            this.label65.AutoSize = true;
            this.label65.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label65.Font = new System.Drawing.Font("Impact", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label65.Location = new System.Drawing.Point(224, 55);
            this.label65.Name = "label65";
            this.label65.Size = new System.Drawing.Size(139, 39);
            this.label65.TabIndex = 2;
            this.label65.Text = "Temp Out";
            // 
            // label66
            // 
            this.label66.AutoSize = true;
            this.label66.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label66.Font = new System.Drawing.Font("Impact", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label66.Location = new System.Drawing.Point(404, 55);
            this.label66.Name = "label66";
            this.label66.Size = new System.Drawing.Size(173, 39);
            this.label66.TabIndex = 3;
            this.label66.Text = "Dosing Rate";
            // 
            // TempIn
            // 
            this.TempIn.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.TempIn.Font = new System.Drawing.Font("Impact", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TempIn.Location = new System.Drawing.Point(51, 97);
            this.TempIn.Name = "TempIn";
            this.TempIn.Size = new System.Drawing.Size(141, 53);
            this.TempIn.TabIndex = 4;
            this.TempIn.Text = "0";
            // 
            // TempOut
            // 
            this.TempOut.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.TempOut.Font = new System.Drawing.Font("Impact", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TempOut.Location = new System.Drawing.Point(229, 97);
            this.TempOut.Name = "TempOut";
            this.TempOut.Size = new System.Drawing.Size(141, 53);
            this.TempOut.TabIndex = 5;
            this.TempOut.Text = "0";
            // 
            // DosingRate
            // 
            this.DosingRate.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.DosingRate.Font = new System.Drawing.Font("Impact", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DosingRate.Location = new System.Drawing.Point(409, 97);
            this.DosingRate.Name = "DosingRate";
            this.DosingRate.Size = new System.Drawing.Size(141, 53);
            this.DosingRate.TabIndex = 6;
            this.DosingRate.Text = "0";
            // 
            // label71
            // 
            this.label71.AutoSize = true;
            this.label71.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label71.Font = new System.Drawing.Font("Impact", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label71.Location = new System.Drawing.Point(46, 170);
            this.label71.Name = "label71";
            this.label71.Size = new System.Drawing.Size(96, 39);
            this.label71.TabIndex = 7;
            this.label71.Text = "NOx In";
            // 
            // label70
            // 
            this.label70.AutoSize = true;
            this.label70.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label70.Font = new System.Drawing.Font("Impact", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label70.Location = new System.Drawing.Point(224, 170);
            this.label70.Name = "label70";
            this.label70.Size = new System.Drawing.Size(114, 39);
            this.label70.TabIndex = 8;
            this.label70.Text = "NOx Out";
            // 
            // label69
            // 
            this.label69.AutoSize = true;
            this.label69.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label69.Font = new System.Drawing.Font("Impact", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label69.Location = new System.Drawing.Point(404, 170);
            this.label69.Name = "label69";
            this.label69.Size = new System.Drawing.Size(69, 39);
            this.label69.TabIndex = 9;
            this.label69.Text = "MAF";
            // 
            // NOxIn
            // 
            this.NOxIn.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.NOxIn.Font = new System.Drawing.Font("Impact", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.NOxIn.Location = new System.Drawing.Point(51, 212);
            this.NOxIn.Name = "NOxIn";
            this.NOxIn.Size = new System.Drawing.Size(141, 53);
            this.NOxIn.TabIndex = 10;
            this.NOxIn.Text = "0";
            // 
            // NOxOut
            // 
            this.NOxOut.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.NOxOut.Font = new System.Drawing.Font("Impact", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.NOxOut.Location = new System.Drawing.Point(229, 212);
            this.NOxOut.Name = "NOxOut";
            this.NOxOut.Size = new System.Drawing.Size(141, 53);
            this.NOxOut.TabIndex = 11;
            this.NOxOut.Text = "0";
            // 
            // MAF
            // 
            this.MAF.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.MAF.Font = new System.Drawing.Font("Impact", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MAF.Location = new System.Drawing.Point(409, 212);
            this.MAF.Name = "MAF";
            this.MAF.Size = new System.Drawing.Size(141, 53);
            this.MAF.TabIndex = 12;
            this.MAF.Text = "0";
            // 
            // panel9
            // 
            this.panel9.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.panel9.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel9.Controls.Add(this.MAF);
            this.panel9.Controls.Add(this.NOxOut);
            this.panel9.Controls.Add(this.NOxIn);
            this.panel9.Controls.Add(this.label69);
            this.panel9.Controls.Add(this.label70);
            this.panel9.Controls.Add(this.label71);
            this.panel9.Controls.Add(this.DosingRate);
            this.panel9.Controls.Add(this.TempOut);
            this.panel9.Controls.Add(this.TempIn);
            this.panel9.Controls.Add(this.label66);
            this.panel9.Controls.Add(this.label65);
            this.panel9.Controls.Add(this.label64);
            this.panel9.Location = new System.Drawing.Point(197, 239);
            this.panel9.Name = "panel9";
            this.panel9.Size = new System.Drawing.Size(609, 339);
            this.panel9.TabIndex = 80;
            // 
            // label72
            // 
            this.label72.AutoSize = true;
            this.label72.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label72.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label72.Location = new System.Drawing.Point(3, 3);
            this.label72.Name = "label72";
            this.label72.Size = new System.Drawing.Size(104, 26);
            this.label72.TabIndex = 4;
            this.label72.Text = "Parameter";
            // 
            // label73
            // 
            this.label73.AutoSize = true;
            this.label73.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label73.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label73.Location = new System.Drawing.Point(3, 42);
            this.label73.Name = "label73";
            this.label73.Size = new System.Drawing.Size(68, 26);
            this.label73.TabIndex = 5;
            this.label73.Text = "LEVEL 1";
            // 
            // label86
            // 
            this.label86.AutoSize = true;
            this.label86.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label86.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label86.Location = new System.Drawing.Point(97, 42);
            this.label86.Name = "label86";
            this.label86.Size = new System.Drawing.Size(68, 26);
            this.label86.TabIndex = 6;
            this.label86.Text = "LEVEL 1";
            // 
            // label87
            // 
            this.label87.AutoSize = true;
            this.label87.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label87.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label87.Location = new System.Drawing.Point(188, 42);
            this.label87.Name = "label87";
            this.label87.Size = new System.Drawing.Size(68, 26);
            this.label87.TabIndex = 7;
            this.label87.Text = "LEVEL 1";
            // 
            // UreaLvlHi
            // 
            this.UreaLvlHi.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.UreaLvlHi.Font = new System.Drawing.Font("Impact", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UreaLvlHi.Location = new System.Drawing.Point(3, 77);
            this.UreaLvlHi.Name = "UreaLvlHi";
            this.UreaLvlHi.Size = new System.Drawing.Size(70, 31);
            this.UreaLvlHi.TabIndex = 8;
            this.UreaLvlHi.Text = "0";
            // 
            // UreaLvlLo
            // 
            this.UreaLvlLo.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.UreaLvlLo.Font = new System.Drawing.Font("Impact", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UreaLvlLo.Location = new System.Drawing.Point(101, 77);
            this.UreaLvlLo.Name = "UreaLvlLo";
            this.UreaLvlLo.Size = new System.Drawing.Size(70, 31);
            this.UreaLvlLo.TabIndex = 9;
            this.UreaLvlLo.Text = "0";
            // 
            // UreaLvlAL
            // 
            this.UreaLvlAL.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.UreaLvlAL.Font = new System.Drawing.Font("Impact", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UreaLvlAL.Location = new System.Drawing.Point(192, 77);
            this.UreaLvlAL.Name = "UreaLvlAL";
            this.UreaLvlAL.Size = new System.Drawing.Size(70, 31);
            this.UreaLvlAL.TabIndex = 10;
            this.UreaLvlAL.Text = "0";
            // 
            // label90
            // 
            this.label90.AutoSize = true;
            this.label90.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label90.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label90.Location = new System.Drawing.Point(3, 130);
            this.label90.Name = "label90";
            this.label90.Size = new System.Drawing.Size(66, 26);
            this.label90.TabIndex = 11;
            this.label90.Text = "Q High";
            // 
            // label89
            // 
            this.label89.AutoSize = true;
            this.label89.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label89.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label89.Location = new System.Drawing.Point(97, 130);
            this.label89.Name = "label89";
            this.label89.Size = new System.Drawing.Size(59, 26);
            this.label89.TabIndex = 12;
            this.label89.Text = "Q Low";
            // 
            // UreaConHi
            // 
            this.UreaConHi.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.UreaConHi.Font = new System.Drawing.Font("Impact", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UreaConHi.Location = new System.Drawing.Point(3, 165);
            this.UreaConHi.Name = "UreaConHi";
            this.UreaConHi.Size = new System.Drawing.Size(70, 31);
            this.UreaConHi.TabIndex = 14;
            this.UreaConHi.Text = "0";
            // 
            // UreaConLo
            // 
            this.UreaConLo.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.UreaConLo.Font = new System.Drawing.Font("Impact", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UreaConLo.Location = new System.Drawing.Point(101, 165);
            this.UreaConLo.Name = "UreaConLo";
            this.UreaConLo.Size = new System.Drawing.Size(70, 31);
            this.UreaConLo.TabIndex = 15;
            this.UreaConLo.Text = "0";
            // 
            // label92
            // 
            this.label92.AutoSize = true;
            this.label92.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label92.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label92.Location = new System.Drawing.Point(3, 207);
            this.label92.Name = "label92";
            this.label92.Size = new System.Drawing.Size(73, 26);
            this.label92.TabIndex = 16;
            this.label92.Text = "Alarm 1";
            // 
            // label91
            // 
            this.label91.AutoSize = true;
            this.label91.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label91.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label91.Location = new System.Drawing.Point(97, 207);
            this.label91.Name = "label91";
            this.label91.Size = new System.Drawing.Size(76, 26);
            this.label91.TabIndex = 17;
            this.label91.Text = "Alarm 2";
            // 
            // label88
            // 
            this.label88.AutoSize = true;
            this.label88.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label88.Font = new System.Drawing.Font("Impact", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label88.Location = new System.Drawing.Point(188, 207);
            this.label88.Name = "label88";
            this.label88.Size = new System.Drawing.Size(68, 26);
            this.label88.TabIndex = 18;
            this.label88.Text = "Buzzer";
            // 
            // UreaAlHr1
            // 
            this.UreaAlHr1.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.UreaAlHr1.Font = new System.Drawing.Font("Impact", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UreaAlHr1.Location = new System.Drawing.Point(3, 242);
            this.UreaAlHr1.Name = "UreaAlHr1";
            this.UreaAlHr1.Size = new System.Drawing.Size(70, 31);
            this.UreaAlHr1.TabIndex = 19;
            this.UreaAlHr1.Text = "0";
            // 
            // UreaAlHr2
            // 
            this.UreaAlHr2.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.UreaAlHr2.Font = new System.Drawing.Font("Impact", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UreaAlHr2.Location = new System.Drawing.Point(101, 242);
            this.UreaAlHr2.Name = "UreaAlHr2";
            this.UreaAlHr2.Size = new System.Drawing.Size(70, 31);
            this.UreaAlHr2.TabIndex = 20;
            this.UreaAlHr2.Text = "0";
            // 
            // UreaBuzInt
            // 
            this.UreaBuzInt.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.UreaBuzInt.Font = new System.Drawing.Font("Impact", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UreaBuzInt.Location = new System.Drawing.Point(192, 242);
            this.UreaBuzInt.Name = "UreaBuzInt";
            this.UreaBuzInt.Size = new System.Drawing.Size(70, 31);
            this.UreaBuzInt.TabIndex = 21;
            this.UreaBuzInt.Text = "0";
            // 
            // Read
            // 
            this.Read.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Read.Location = new System.Drawing.Point(111, 305);
            this.Read.Name = "Read";
            this.Read.Size = new System.Drawing.Size(75, 29);
            this.Read.TabIndex = 148;
            this.Read.Text = "Read";
            this.Read.UseSelectable = true;
            this.Read.Click += new System.EventHandler(this.Read_Click);
            // 
            // Write
            // 
            this.Write.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Write.Location = new System.Drawing.Point(192, 305);
            this.Write.Name = "Write";
            this.Write.Size = new System.Drawing.Size(75, 29);
            this.Write.TabIndex = 149;
            this.Write.Text = "Write";
            this.Write.UseSelectable = true;
            this.Write.Click += new System.EventHandler(this.Write_Click);
            // 
            // Default
            // 
            this.Default.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Default.Location = new System.Drawing.Point(30, 305);
            this.Default.Name = "Default";
            this.Default.Size = new System.Drawing.Size(75, 29);
            this.Default.TabIndex = 150;
            this.Default.Text = "Default";
            this.Default.UseSelectable = true;
            this.Default.Click += new System.EventHandler(this.Default_Click);
            // 
            // Set0
            // 
            this.Set0.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Set0.Location = new System.Drawing.Point(-2, 305);
            this.Set0.Name = "Set0";
            this.Set0.Size = new System.Drawing.Size(26, 29);
            this.Set0.TabIndex = 151;
            this.Set0.Text = "0";
            this.Set0.UseSelectable = true;
            this.Set0.Click += new System.EventHandler(this.Set0_Click);
            // 
            // panel10
            // 
            this.panel10.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.panel10.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel10.Controls.Add(this.Set0);
            this.panel10.Controls.Add(this.Default);
            this.panel10.Controls.Add(this.Write);
            this.panel10.Controls.Add(this.Read);
            this.panel10.Controls.Add(this.UreaBuzInt);
            this.panel10.Controls.Add(this.UreaAlHr2);
            this.panel10.Controls.Add(this.UreaAlHr1);
            this.panel10.Controls.Add(this.label88);
            this.panel10.Controls.Add(this.label91);
            this.panel10.Controls.Add(this.label92);
            this.panel10.Controls.Add(this.UreaConLo);
            this.panel10.Controls.Add(this.UreaConHi);
            this.panel10.Controls.Add(this.label89);
            this.panel10.Controls.Add(this.label90);
            this.panel10.Controls.Add(this.UreaLvlAL);
            this.panel10.Controls.Add(this.UreaLvlLo);
            this.panel10.Controls.Add(this.UreaLvlHi);
            this.panel10.Controls.Add(this.label87);
            this.panel10.Controls.Add(this.label86);
            this.panel10.Controls.Add(this.label73);
            this.panel10.Controls.Add(this.label72);
            this.panel10.Location = new System.Drawing.Point(812, 239);
            this.panel10.Name = "panel10";
            this.panel10.Size = new System.Drawing.Size(279, 339);
            this.panel10.TabIndex = 81;
            // 
            // Pedal_ADC_1
            // 
            this.Pedal_ADC_1.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Pedal_ADC_1.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Pedal_ADC_1.Location = new System.Drawing.Point(3, 370);
            this.Pedal_ADC_1.Name = "Pedal_ADC_1";
            this.Pedal_ADC_1.Size = new System.Drawing.Size(70, 26);
            this.Pedal_ADC_1.TabIndex = 82;
            this.Pedal_ADC_1.Text = "0";
            // 
            // Pedal_ADC_2
            // 
            this.Pedal_ADC_2.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Pedal_ADC_2.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Pedal_ADC_2.Location = new System.Drawing.Point(3, 402);
            this.Pedal_ADC_2.Name = "Pedal_ADC_2";
            this.Pedal_ADC_2.Size = new System.Drawing.Size(70, 26);
            this.Pedal_ADC_2.TabIndex = 83;
            this.Pedal_ADC_2.Text = "0";
            // 
            // Pedal_ADC_3
            // 
            this.Pedal_ADC_3.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Pedal_ADC_3.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Pedal_ADC_3.Location = new System.Drawing.Point(3, 434);
            this.Pedal_ADC_3.Name = "Pedal_ADC_3";
            this.Pedal_ADC_3.Size = new System.Drawing.Size(70, 26);
            this.Pedal_ADC_3.TabIndex = 84;
            this.Pedal_ADC_3.Text = "0";
            // 
            // Pedal_ADC_4
            // 
            this.Pedal_ADC_4.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Pedal_ADC_4.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Pedal_ADC_4.Location = new System.Drawing.Point(3, 466);
            this.Pedal_ADC_4.Name = "Pedal_ADC_4";
            this.Pedal_ADC_4.Size = new System.Drawing.Size(70, 26);
            this.Pedal_ADC_4.TabIndex = 85;
            this.Pedal_ADC_4.Text = "0";
            // 
            // Pedal_ADC_5
            // 
            this.Pedal_ADC_5.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Pedal_ADC_5.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Pedal_ADC_5.Location = new System.Drawing.Point(3, 498);
            this.Pedal_ADC_5.Name = "Pedal_ADC_5";
            this.Pedal_ADC_5.Size = new System.Drawing.Size(70, 26);
            this.Pedal_ADC_5.TabIndex = 86;
            this.Pedal_ADC_5.Text = "0";
            // 
            // Pedal_ADC_6
            // 
            this.Pedal_ADC_6.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Pedal_ADC_6.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Pedal_ADC_6.Location = new System.Drawing.Point(3, 530);
            this.Pedal_ADC_6.Name = "Pedal_ADC_6";
            this.Pedal_ADC_6.Size = new System.Drawing.Size(70, 26);
            this.Pedal_ADC_6.TabIndex = 87;
            this.Pedal_ADC_6.Text = "0";
            // 
            // Pedal_ADC_7
            // 
            this.Pedal_ADC_7.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Pedal_ADC_7.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Pedal_ADC_7.Location = new System.Drawing.Point(3, 562);
            this.Pedal_ADC_7.Name = "Pedal_ADC_7";
            this.Pedal_ADC_7.Size = new System.Drawing.Size(70, 26);
            this.Pedal_ADC_7.TabIndex = 89;
            this.Pedal_ADC_7.Text = "0";
            // 
            // metroButton5
            // 
            this.metroButton5.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.metroButton5.Location = new System.Drawing.Point(89, 498);
            this.metroButton5.Name = "metroButton5";
            this.metroButton5.Size = new System.Drawing.Size(75, 29);
            this.metroButton5.TabIndex = 150;
            this.metroButton5.Text = "metroButton5";
            this.metroButton5.UseSelectable = true;
            this.metroButton5.Click += new System.EventHandler(this.metroButton5_Click);
            // 
            // Test_0
            // 
            this.Test_0.BackColor = System.Drawing.Color.Silver;
            this.Test_0.FlatAppearance.BorderColor = System.Drawing.SystemColors.ControlLightLight;
            this.Test_0.FlatAppearance.BorderSize = 0;
            this.Test_0.Font = new System.Drawing.Font("휴먼둥근헤드라인", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Test_0.Location = new System.Drawing.Point(23, 168);
            this.Test_0.Name = "Test_0";
            this.Test_0.Size = new System.Drawing.Size(92, 38);
            this.Test_0.TabIndex = 93;
            this.Test_0.Text = "0단계";
            this.Test_0.UseVisualStyleBackColor = false;
            this.Test_0.Click += new System.EventHandler(this.Test_0_Click);
            // 
            // Test_1
            // 
            this.Test_1.BackColor = System.Drawing.Color.Silver;
            this.Test_1.FlatAppearance.BorderColor = System.Drawing.SystemColors.ControlLightLight;
            this.Test_1.FlatAppearance.BorderSize = 0;
            this.Test_1.Font = new System.Drawing.Font("휴먼둥근헤드라인", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Test_1.Location = new System.Drawing.Point(121, 168);
            this.Test_1.Name = "Test_1";
            this.Test_1.Size = new System.Drawing.Size(92, 38);
            this.Test_1.TabIndex = 94;
            this.Test_1.Text = "1단계";
            this.Test_1.UseVisualStyleBackColor = false;
            this.Test_1.Click += new System.EventHandler(this.Test_1_Click);
            // 
            // Test_2
            // 
            this.Test_2.BackColor = System.Drawing.Color.Silver;
            this.Test_2.FlatAppearance.BorderColor = System.Drawing.SystemColors.ControlLightLight;
            this.Test_2.FlatAppearance.BorderSize = 0;
            this.Test_2.Font = new System.Drawing.Font("휴먼둥근헤드라인", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Test_2.Location = new System.Drawing.Point(221, 168);
            this.Test_2.Name = "Test_2";
            this.Test_2.Size = new System.Drawing.Size(92, 38);
            this.Test_2.TabIndex = 95;
            this.Test_2.Text = "2단계";
            this.Test_2.UseVisualStyleBackColor = false;
            this.Test_2.Click += new System.EventHandler(this.Test_2_Click);
            // 
            // Test_Exit
            // 
            this.Test_Exit.BackColor = System.Drawing.Color.Silver;
            this.Test_Exit.FlatAppearance.BorderColor = System.Drawing.SystemColors.ControlLightLight;
            this.Test_Exit.FlatAppearance.BorderSize = 0;
            this.Test_Exit.Font = new System.Drawing.Font("휴먼둥근헤드라인", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Test_Exit.Location = new System.Drawing.Point(319, 168);
            this.Test_Exit.Name = "Test_Exit";
            this.Test_Exit.Size = new System.Drawing.Size(106, 38);
            this.Test_Exit.TabIndex = 96;
            this.Test_Exit.Text = "테스트 종료";
            this.Test_Exit.UseVisualStyleBackColor = false;
            this.Test_Exit.Click += new System.EventHandler(this.Test_Exit_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BorderStyle = MetroFramework.Forms.MetroFormBorderStyle.FixedSingle;
            this.ClientSize = new System.Drawing.Size(1107, 727);
            this.Controls.Add(this.metroButton4);
            this.Controls.Add(this.panel13);
            this.Controls.Add(this.metroButton2);
            this.Controls.Add(this.metroTabControl1);
            this.Controls.Add(this.comboBox_port1);
            this.Controls.Add(this.label_port);
            this.Controls.Add(this.metroButton1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "Limiter Control";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.metroTabControl1.ResumeLayout(false);
            this.metroTabPage1.ResumeLayout(false);
            this.metroTabPage1.PerformLayout();
            this.panel8.ResumeLayout(false);
            this.panel8.PerformLayout();
            this.panel9.ResumeLayout(false);
            this.panel9.PerformLayout();
            this.panel10.ResumeLayout(false);
            this.panel10.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.IO.Ports.SerialPort serialPort1;
        private MetroFramework.Controls.MetroTabControl metroTabControl1;
        private MetroFramework.Controls.MetroTabPage metroTabPage1;
        private MetroFramework.Controls.MetroComboBox comboBox_port1;
        private System.Windows.Forms.Label label_port;
        private MetroFramework.Controls.MetroButton metroButton2;
        private MetroFramework.Controls.MetroButton metroButton1;
        private System.Windows.Forms.Panel panel13;
        public System.Windows.Forms.Timer timer1;
        private MetroFramework.Controls.MetroButton metroButton4;
        private System.Windows.Forms.TextBox Pedal_ADC_8;
        private System.Windows.Forms.Timer timer2;
        private MetroFramework.Controls.MetroButton metroButton5;
        private System.Windows.Forms.TextBox Pedal_ADC_7;
        private System.Windows.Forms.TextBox Pedal_ADC_6;
        private System.Windows.Forms.TextBox Pedal_ADC_5;
        private System.Windows.Forms.TextBox Pedal_ADC_4;
        private System.Windows.Forms.TextBox Pedal_ADC_3;
        private System.Windows.Forms.TextBox Pedal_ADC_2;
        private System.Windows.Forms.TextBox Pedal_ADC_1;
        private System.Windows.Forms.Panel panel10;
        private MetroFramework.Controls.MetroButton Set0;
        private MetroFramework.Controls.MetroButton Default;
        private MetroFramework.Controls.MetroButton Write;
        private MetroFramework.Controls.MetroButton Read;
        public System.Windows.Forms.TextBox UreaBuzInt;
        private System.Windows.Forms.TextBox UreaAlHr2;
        private System.Windows.Forms.TextBox UreaAlHr1;
        private System.Windows.Forms.Label label88;
        private System.Windows.Forms.Label label91;
        private System.Windows.Forms.Label label92;
        private System.Windows.Forms.TextBox UreaConLo;
        private System.Windows.Forms.TextBox UreaConHi;
        private System.Windows.Forms.Label label89;
        private System.Windows.Forms.Label label90;
        private System.Windows.Forms.TextBox UreaLvlAL;
        private System.Windows.Forms.TextBox UreaLvlLo;
        private System.Windows.Forms.TextBox UreaLvlHi;
        private System.Windows.Forms.Label label87;
        private System.Windows.Forms.Label label86;
        private System.Windows.Forms.Label label73;
        private System.Windows.Forms.Label label72;
        private System.Windows.Forms.Panel panel9;
        private System.Windows.Forms.TextBox MAF;
        private System.Windows.Forms.TextBox NOxOut;
        private System.Windows.Forms.TextBox NOxIn;
        private System.Windows.Forms.Label label69;
        private System.Windows.Forms.Label label70;
        private System.Windows.Forms.Label label71;
        private System.Windows.Forms.TextBox DosingRate;
        private System.Windows.Forms.TextBox TempOut;
        private System.Windows.Forms.TextBox TempIn;
        private System.Windows.Forms.Label label66;
        private System.Windows.Forms.Label label65;
        private System.Windows.Forms.Label label64;
        private System.Windows.Forms.Panel panel8;
        private System.Windows.Forms.TextBox Quality;
        private System.Windows.Forms.TextBox Level;
        private System.Windows.Forms.TextBox Limit;
        private System.Windows.Forms.Label label63;
        private System.Windows.Forms.Label label62;
        private System.Windows.Forms.Label label61;
        private System.Windows.Forms.TextBox Flag;
        private System.Windows.Forms.Label label60;
        private MetroFramework.Controls.MetroLabel metroLabel59;
        private MetroFramework.Controls.MetroLabel metroLabel58;
        private MetroFramework.Controls.MetroLabel metroLabel57;
        private MetroFramework.Controls.MetroLabel metroLabel56;
        private MetroFramework.Controls.MetroLabel sync1s;
        private MetroFramework.Controls.MetroLabel Command1;
        private MetroFramework.Controls.MetroLabel metroLabel55;
        private MetroFramework.Controls.MetroTextBox Exitbit;
        private MetroFramework.Controls.MetroTextBox Checksum;
        private MetroFramework.Controls.MetroTextBox Command;
        private MetroFramework.Controls.MetroTextBox Length;
        private MetroFramework.Controls.MetroTextBox StartBit;
        private MetroFramework.Controls.MetroTextBox Sync2;
        private MetroFramework.Controls.MetroTextBox Sync1;
        private System.Windows.Forms.Button Test_0;
        private System.Windows.Forms.Button Test_Exit;
        private System.Windows.Forms.Button Test_2;
        private System.Windows.Forms.Button Test_1;
    }
}

