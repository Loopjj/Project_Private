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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.metroTabControl1 = new MetroFramework.Controls.MetroTabControl();
            this.metroTabPage1 = new MetroFramework.Controls.MetroTabPage();
            this.textBox_Valid = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel18 = new MetroFramework.Controls.MetroLabel();
            this.textBox_Speed = new MetroFramework.Controls.MetroTextBox();
            this.textBox_t_OFFSCR = new MetroFramework.Controls.MetroTextBox();
            this.textBox_RegenStartKey = new MetroFramework.Controls.MetroTextBox();
            this.textBox_n_SCR = new MetroFramework.Controls.MetroTextBox();
            this.textBox_t_SCR = new MetroFramework.Controls.MetroTextBox();
            this.textBox_f_SCR = new MetroFramework.Controls.MetroTextBox();
            this.textBox_baseP1 = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel19 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel20 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel21 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel22 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel23 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel24 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel25 = new MetroFramework.Controls.MetroLabel();
            this.textBox_DrvTime = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Lock = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel16 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel15 = new MetroFramework.Controls.MetroLabel();
            this.textBox_Sts = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Sig = new MetroFramework.Controls.MetroTextBox();
            this.textBox_n_REG = new MetroFramework.Controls.MetroTextBox();
            this.textBox_t_REG = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Check = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Error = new MetroFramework.Controls.MetroTextBox();
            this.textBox_FPD = new MetroFramework.Controls.MetroTextBox();
            this.textBox_CarType = new MetroFramework.Controls.MetroTextBox();
            this.textBox_f_REG = new MetroFramework.Controls.MetroTextBox();
            this.textBox_IHC = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Rate = new MetroFramework.Controls.MetroTextBox();
            this.textBox_P = new MetroFramework.Controls.MetroTextBox();
            this.textBox_T4 = new MetroFramework.Controls.MetroTextBox();
            this.textBox_T3 = new MetroFramework.Controls.MetroTextBox();
            this.textBox_T2 = new MetroFramework.Controls.MetroTextBox();
            this.textBox_T1 = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel14 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel13 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel12 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel11 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel10 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel2 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel1 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel9 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel8 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel7 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel6 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel5 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel4 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel3 = new MetroFramework.Controls.MetroLabel();
            this.T2 = new MetroFramework.Controls.MetroLabel();
            this.T1 = new MetroFramework.Controls.MetroLabel();
            this.metroTabPage2 = new MetroFramework.Controls.MetroTabPage();
            this.textBox_H2k = new MetroFramework.Controls.MetroTextBox();
            this.textBox_H1k = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Xc = new MetroFramework.Controls.MetroTextBox();
            this.textBox_curXk = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel45 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel46 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel47 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel48 = new MetroFramework.Controls.MetroLabel();
            this.textBox_TotalDosingRate = new MetroFramework.Controls.MetroTextBox();
            this.textBox_DosingRatehouer = new MetroFramework.Controls.MetroTextBox();
            this.textBox_ScrFlag = new MetroFramework.Controls.MetroTextBox();
            this.textBox_SystemSignal = new MetroFramework.Controls.MetroTextBox();
            this.textBox_SystemCheck = new MetroFramework.Controls.MetroTextBox();
            this.textBox_SystemError = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel49 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel50 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel51 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel52 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel53 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel54 = new MetroFramework.Controls.MetroLabel();
            this.textBox_NoxReduction = new MetroFramework.Controls.MetroTextBox();
            this.textBox_UreaQuality = new MetroFramework.Controls.MetroTextBox();
            this.textBox_SCRMode = new MetroFramework.Controls.MetroTextBox();
            this.textBox_StatusAlpha = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel35 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel36 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel37 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel38 = new MetroFramework.Controls.MetroLabel();
            this.textBox_Map_Y = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Map_X = new MetroFramework.Controls.MetroTextBox();
            this.textBox_BattVoltage = new MetroFramework.Controls.MetroTextBox();
            this.textBox_TankLevelP = new MetroFramework.Controls.MetroTextBox();
            this.textBox_TankTemp = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Maf_temp = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel39 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel40 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel41 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel42 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel43 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel44 = new MetroFramework.Controls.MetroLabel();
            this.textBox_MafKg_H = new MetroFramework.Controls.MetroTextBox();
            this.textBox_DosingDuty = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Noxppm2 = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Noxppm1 = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel17 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel32 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel33 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel34 = new MetroFramework.Controls.MetroLabel();
            this.textBox_P1_bar = new MetroFramework.Controls.MetroTextBox();
            this.textBox_T3_temp = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Tavg_temp = new MetroFramework.Controls.MetroTextBox();
            this.textBox_T2_temp = new MetroFramework.Controls.MetroTextBox();
            this.textBox_T1_temp = new MetroFramework.Controls.MetroTextBox();
            this.textBox_Version = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel26 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel27 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel28 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel29 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel30 = new MetroFramework.Controls.MetroLabel();
            this.metroLabel31 = new MetroFramework.Controls.MetroLabel();
            this.metroTabPage3 = new MetroFramework.Controls.MetroTabPage();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.metroTabPage4 = new MetroFramework.Controls.MetroTabPage();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.metroPanel1 = new MetroFramework.Controls.MetroPanel();
            this.comboBox_port1 = new MetroFramework.Controls.MetroComboBox();
            this.label_port = new System.Windows.Forms.Label();
            this.button_connect = new System.Windows.Forms.Button();
            this.button_disconnect = new System.Windows.Forms.Button();
            this.label15 = new System.Windows.Forms.Label();
            this.textBox_Size = new System.Windows.Forms.TextBox();
            this.label_status = new System.Windows.Forms.Label();
            this.button_send = new System.Windows.Forms.Button();
            this.label_send = new System.Windows.Forms.Label();
            this.textBox_send = new System.Windows.Forms.TextBox();
            this.metroTabPage5 = new MetroFramework.Controls.MetroTabPage();
            this.metroTabControl1.SuspendLayout();
            this.metroTabPage1.SuspendLayout();
            this.metroTabPage2.SuspendLayout();
            this.metroTabPage3.SuspendLayout();
            this.metroTabPage4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.metroPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // metroTabControl1
            // 
            this.metroTabControl1.Controls.Add(this.metroTabPage5);
            this.metroTabControl1.Controls.Add(this.metroTabPage1);
            this.metroTabControl1.Controls.Add(this.metroTabPage2);
            this.metroTabControl1.Controls.Add(this.metroTabPage3);
            this.metroTabControl1.Controls.Add(this.metroTabPage4);
            this.metroTabControl1.FontSize = MetroFramework.MetroTabControlSize.Tall;
            this.metroTabControl1.FontWeight = MetroFramework.MetroTabControlWeight.Regular;
            this.metroTabControl1.Location = new System.Drawing.Point(12, 86);
            this.metroTabControl1.Name = "metroTabControl1";
            this.metroTabControl1.SelectedIndex = 0;
            this.metroTabControl1.Size = new System.Drawing.Size(1130, 712);
            this.metroTabControl1.TabIndex = 81;
            this.metroTabControl1.UseSelectable = true;
            // 
            // metroTabPage1
            // 
            this.metroTabPage1.Controls.Add(this.textBox_Valid);
            this.metroTabPage1.Controls.Add(this.metroLabel18);
            this.metroTabPage1.Controls.Add(this.textBox_Speed);
            this.metroTabPage1.Controls.Add(this.textBox_t_OFFSCR);
            this.metroTabPage1.Controls.Add(this.textBox_RegenStartKey);
            this.metroTabPage1.Controls.Add(this.textBox_n_SCR);
            this.metroTabPage1.Controls.Add(this.textBox_t_SCR);
            this.metroTabPage1.Controls.Add(this.textBox_f_SCR);
            this.metroTabPage1.Controls.Add(this.textBox_baseP1);
            this.metroTabPage1.Controls.Add(this.metroLabel19);
            this.metroTabPage1.Controls.Add(this.metroLabel20);
            this.metroTabPage1.Controls.Add(this.metroLabel21);
            this.metroTabPage1.Controls.Add(this.metroLabel22);
            this.metroTabPage1.Controls.Add(this.metroLabel23);
            this.metroTabPage1.Controls.Add(this.metroLabel24);
            this.metroTabPage1.Controls.Add(this.metroLabel25);
            this.metroTabPage1.Controls.Add(this.textBox_DrvTime);
            this.metroTabPage1.Controls.Add(this.textBox_Lock);
            this.metroTabPage1.Controls.Add(this.metroLabel16);
            this.metroTabPage1.Controls.Add(this.metroLabel15);
            this.metroTabPage1.Controls.Add(this.textBox_Sts);
            this.metroTabPage1.Controls.Add(this.textBox_Sig);
            this.metroTabPage1.Controls.Add(this.textBox_n_REG);
            this.metroTabPage1.Controls.Add(this.textBox_t_REG);
            this.metroTabPage1.Controls.Add(this.textBox_Check);
            this.metroTabPage1.Controls.Add(this.textBox_Error);
            this.metroTabPage1.Controls.Add(this.textBox_FPD);
            this.metroTabPage1.Controls.Add(this.textBox_CarType);
            this.metroTabPage1.Controls.Add(this.textBox_f_REG);
            this.metroTabPage1.Controls.Add(this.textBox_IHC);
            this.metroTabPage1.Controls.Add(this.textBox_Rate);
            this.metroTabPage1.Controls.Add(this.textBox_P);
            this.metroTabPage1.Controls.Add(this.textBox_T4);
            this.metroTabPage1.Controls.Add(this.textBox_T3);
            this.metroTabPage1.Controls.Add(this.textBox_T2);
            this.metroTabPage1.Controls.Add(this.textBox_T1);
            this.metroTabPage1.Controls.Add(this.metroLabel14);
            this.metroTabPage1.Controls.Add(this.metroLabel13);
            this.metroTabPage1.Controls.Add(this.metroLabel12);
            this.metroTabPage1.Controls.Add(this.metroLabel11);
            this.metroTabPage1.Controls.Add(this.metroLabel10);
            this.metroTabPage1.Controls.Add(this.metroLabel2);
            this.metroTabPage1.Controls.Add(this.metroLabel1);
            this.metroTabPage1.Controls.Add(this.metroLabel9);
            this.metroTabPage1.Controls.Add(this.metroLabel8);
            this.metroTabPage1.Controls.Add(this.metroLabel7);
            this.metroTabPage1.Controls.Add(this.metroLabel6);
            this.metroTabPage1.Controls.Add(this.metroLabel5);
            this.metroTabPage1.Controls.Add(this.metroLabel4);
            this.metroTabPage1.Controls.Add(this.metroLabel3);
            this.metroTabPage1.Controls.Add(this.T2);
            this.metroTabPage1.Controls.Add(this.T1);
            this.metroTabPage1.HorizontalScrollbarBarColor = true;
            this.metroTabPage1.HorizontalScrollbarHighlightOnWheel = false;
            this.metroTabPage1.HorizontalScrollbarSize = 10;
            this.metroTabPage1.Location = new System.Drawing.Point(4, 44);
            this.metroTabPage1.Name = "metroTabPage1";
            this.metroTabPage1.Size = new System.Drawing.Size(1122, 664);
            this.metroTabPage1.TabIndex = 0;
            this.metroTabPage1.Text = "DPF";
            this.metroTabPage1.VerticalScrollbarBarColor = true;
            this.metroTabPage1.VerticalScrollbarHighlightOnWheel = false;
            this.metroTabPage1.VerticalScrollbarSize = 10;
            // 
            // textBox_Valid
            // 
            // 
            // 
            // 
            this.textBox_Valid.CustomButton.Image = null;
            this.textBox_Valid.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Valid.CustomButton.Name = "";
            this.textBox_Valid.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Valid.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Valid.CustomButton.TabIndex = 1;
            this.textBox_Valid.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Valid.CustomButton.UseSelectable = true;
            this.textBox_Valid.CustomButton.Visible = false;
            this.textBox_Valid.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Valid.Lines = new string[0];
            this.textBox_Valid.Location = new System.Drawing.Point(607, 364);
            this.textBox_Valid.MaxLength = 32767;
            this.textBox_Valid.Name = "textBox_Valid";
            this.textBox_Valid.PasswordChar = '\0';
            this.textBox_Valid.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Valid.SelectedText = "";
            this.textBox_Valid.SelectionLength = 0;
            this.textBox_Valid.SelectionStart = 0;
            this.textBox_Valid.ShortcutsEnabled = true;
            this.textBox_Valid.Size = new System.Drawing.Size(130, 40);
            this.textBox_Valid.TabIndex = 54;
            this.textBox_Valid.UseSelectable = true;
            this.textBox_Valid.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Valid.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel18
            // 
            this.metroLabel18.AutoSize = true;
            this.metroLabel18.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel18.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel18.Location = new System.Drawing.Point(528, 364);
            this.metroLabel18.Name = "metroLabel18";
            this.metroLabel18.Size = new System.Drawing.Size(50, 25);
            this.metroLabel18.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel18.TabIndex = 52;
            this.metroLabel18.Text = "Valid";
            // 
            // textBox_Speed
            // 
            // 
            // 
            // 
            this.textBox_Speed.CustomButton.Image = null;
            this.textBox_Speed.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Speed.CustomButton.Name = "";
            this.textBox_Speed.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Speed.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Speed.CustomButton.TabIndex = 1;
            this.textBox_Speed.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Speed.CustomButton.UseSelectable = true;
            this.textBox_Speed.CustomButton.Visible = false;
            this.textBox_Speed.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Speed.Lines = new string[0];
            this.textBox_Speed.Location = new System.Drawing.Point(607, 311);
            this.textBox_Speed.MaxLength = 32767;
            this.textBox_Speed.Name = "textBox_Speed";
            this.textBox_Speed.PasswordChar = '\0';
            this.textBox_Speed.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Speed.SelectedText = "";
            this.textBox_Speed.SelectionLength = 0;
            this.textBox_Speed.SelectionStart = 0;
            this.textBox_Speed.ShortcutsEnabled = true;
            this.textBox_Speed.Size = new System.Drawing.Size(130, 40);
            this.textBox_Speed.TabIndex = 51;
            this.textBox_Speed.UseSelectable = true;
            this.textBox_Speed.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Speed.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_t_OFFSCR
            // 
            // 
            // 
            // 
            this.textBox_t_OFFSCR.CustomButton.Image = null;
            this.textBox_t_OFFSCR.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_t_OFFSCR.CustomButton.Name = "";
            this.textBox_t_OFFSCR.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_t_OFFSCR.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_t_OFFSCR.CustomButton.TabIndex = 1;
            this.textBox_t_OFFSCR.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_t_OFFSCR.CustomButton.UseSelectable = true;
            this.textBox_t_OFFSCR.CustomButton.Visible = false;
            this.textBox_t_OFFSCR.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_t_OFFSCR.Lines = new string[0];
            this.textBox_t_OFFSCR.Location = new System.Drawing.Point(607, 262);
            this.textBox_t_OFFSCR.MaxLength = 32767;
            this.textBox_t_OFFSCR.Name = "textBox_t_OFFSCR";
            this.textBox_t_OFFSCR.PasswordChar = '\0';
            this.textBox_t_OFFSCR.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_t_OFFSCR.SelectedText = "";
            this.textBox_t_OFFSCR.SelectionLength = 0;
            this.textBox_t_OFFSCR.SelectionStart = 0;
            this.textBox_t_OFFSCR.ShortcutsEnabled = true;
            this.textBox_t_OFFSCR.Size = new System.Drawing.Size(130, 40);
            this.textBox_t_OFFSCR.TabIndex = 50;
            this.textBox_t_OFFSCR.UseSelectable = true;
            this.textBox_t_OFFSCR.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_t_OFFSCR.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_RegenStartKey
            // 
            // 
            // 
            // 
            this.textBox_RegenStartKey.CustomButton.Image = null;
            this.textBox_RegenStartKey.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_RegenStartKey.CustomButton.Name = "";
            this.textBox_RegenStartKey.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_RegenStartKey.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_RegenStartKey.CustomButton.TabIndex = 1;
            this.textBox_RegenStartKey.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_RegenStartKey.CustomButton.UseSelectable = true;
            this.textBox_RegenStartKey.CustomButton.Visible = false;
            this.textBox_RegenStartKey.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_RegenStartKey.Lines = new string[0];
            this.textBox_RegenStartKey.Location = new System.Drawing.Point(607, 216);
            this.textBox_RegenStartKey.MaxLength = 32767;
            this.textBox_RegenStartKey.Name = "textBox_RegenStartKey";
            this.textBox_RegenStartKey.PasswordChar = '\0';
            this.textBox_RegenStartKey.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_RegenStartKey.SelectedText = "";
            this.textBox_RegenStartKey.SelectionLength = 0;
            this.textBox_RegenStartKey.SelectionStart = 0;
            this.textBox_RegenStartKey.ShortcutsEnabled = true;
            this.textBox_RegenStartKey.Size = new System.Drawing.Size(130, 40);
            this.textBox_RegenStartKey.TabIndex = 49;
            this.textBox_RegenStartKey.UseSelectable = true;
            this.textBox_RegenStartKey.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_RegenStartKey.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_n_SCR
            // 
            // 
            // 
            // 
            this.textBox_n_SCR.CustomButton.Image = null;
            this.textBox_n_SCR.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_n_SCR.CustomButton.Name = "";
            this.textBox_n_SCR.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_n_SCR.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_n_SCR.CustomButton.TabIndex = 1;
            this.textBox_n_SCR.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_n_SCR.CustomButton.UseSelectable = true;
            this.textBox_n_SCR.CustomButton.Visible = false;
            this.textBox_n_SCR.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_n_SCR.Lines = new string[0];
            this.textBox_n_SCR.Location = new System.Drawing.Point(607, 170);
            this.textBox_n_SCR.MaxLength = 32767;
            this.textBox_n_SCR.Name = "textBox_n_SCR";
            this.textBox_n_SCR.PasswordChar = '\0';
            this.textBox_n_SCR.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_n_SCR.SelectedText = "";
            this.textBox_n_SCR.SelectionLength = 0;
            this.textBox_n_SCR.SelectionStart = 0;
            this.textBox_n_SCR.ShortcutsEnabled = true;
            this.textBox_n_SCR.Size = new System.Drawing.Size(130, 40);
            this.textBox_n_SCR.TabIndex = 48;
            this.textBox_n_SCR.UseSelectable = true;
            this.textBox_n_SCR.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_n_SCR.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_t_SCR
            // 
            // 
            // 
            // 
            this.textBox_t_SCR.CustomButton.Image = null;
            this.textBox_t_SCR.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_t_SCR.CustomButton.Name = "";
            this.textBox_t_SCR.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_t_SCR.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_t_SCR.CustomButton.TabIndex = 1;
            this.textBox_t_SCR.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_t_SCR.CustomButton.UseSelectable = true;
            this.textBox_t_SCR.CustomButton.Visible = false;
            this.textBox_t_SCR.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_t_SCR.Lines = new string[0];
            this.textBox_t_SCR.Location = new System.Drawing.Point(607, 125);
            this.textBox_t_SCR.MaxLength = 32767;
            this.textBox_t_SCR.Name = "textBox_t_SCR";
            this.textBox_t_SCR.PasswordChar = '\0';
            this.textBox_t_SCR.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_t_SCR.SelectedText = "";
            this.textBox_t_SCR.SelectionLength = 0;
            this.textBox_t_SCR.SelectionStart = 0;
            this.textBox_t_SCR.ShortcutsEnabled = true;
            this.textBox_t_SCR.Size = new System.Drawing.Size(130, 40);
            this.textBox_t_SCR.TabIndex = 47;
            this.textBox_t_SCR.UseSelectable = true;
            this.textBox_t_SCR.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_t_SCR.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_f_SCR
            // 
            // 
            // 
            // 
            this.textBox_f_SCR.CustomButton.Image = null;
            this.textBox_f_SCR.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_f_SCR.CustomButton.Name = "";
            this.textBox_f_SCR.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_f_SCR.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_f_SCR.CustomButton.TabIndex = 1;
            this.textBox_f_SCR.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_f_SCR.CustomButton.UseSelectable = true;
            this.textBox_f_SCR.CustomButton.Visible = false;
            this.textBox_f_SCR.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_f_SCR.Lines = new string[0];
            this.textBox_f_SCR.Location = new System.Drawing.Point(607, 79);
            this.textBox_f_SCR.MaxLength = 32767;
            this.textBox_f_SCR.Name = "textBox_f_SCR";
            this.textBox_f_SCR.PasswordChar = '\0';
            this.textBox_f_SCR.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_f_SCR.SelectedText = "";
            this.textBox_f_SCR.SelectionLength = 0;
            this.textBox_f_SCR.SelectionStart = 0;
            this.textBox_f_SCR.ShortcutsEnabled = true;
            this.textBox_f_SCR.Size = new System.Drawing.Size(130, 40);
            this.textBox_f_SCR.TabIndex = 46;
            this.textBox_f_SCR.UseSelectable = true;
            this.textBox_f_SCR.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_f_SCR.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_baseP1
            // 
            // 
            // 
            // 
            this.textBox_baseP1.CustomButton.Image = null;
            this.textBox_baseP1.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_baseP1.CustomButton.Name = "";
            this.textBox_baseP1.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_baseP1.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_baseP1.CustomButton.TabIndex = 1;
            this.textBox_baseP1.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_baseP1.CustomButton.UseSelectable = true;
            this.textBox_baseP1.CustomButton.Visible = false;
            this.textBox_baseP1.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_baseP1.Lines = new string[0];
            this.textBox_baseP1.Location = new System.Drawing.Point(607, 24);
            this.textBox_baseP1.MaxLength = 32767;
            this.textBox_baseP1.Name = "textBox_baseP1";
            this.textBox_baseP1.PasswordChar = '\0';
            this.textBox_baseP1.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_baseP1.SelectedText = "";
            this.textBox_baseP1.SelectionLength = 0;
            this.textBox_baseP1.SelectionStart = 0;
            this.textBox_baseP1.ShortcutsEnabled = true;
            this.textBox_baseP1.Size = new System.Drawing.Size(130, 40);
            this.textBox_baseP1.TabIndex = 45;
            this.textBox_baseP1.UseSelectable = true;
            this.textBox_baseP1.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_baseP1.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel19
            // 
            this.metroLabel19.AutoSize = true;
            this.metroLabel19.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel19.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel19.Location = new System.Drawing.Point(528, 311);
            this.metroLabel19.Name = "metroLabel19";
            this.metroLabel19.Size = new System.Drawing.Size(62, 25);
            this.metroLabel19.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel19.TabIndex = 44;
            this.metroLabel19.Text = "Speed";
            // 
            // metroLabel20
            // 
            this.metroLabel20.AutoSize = true;
            this.metroLabel20.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel20.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel20.Location = new System.Drawing.Point(496, 262);
            this.metroLabel20.Name = "metroLabel20";
            this.metroLabel20.Size = new System.Drawing.Size(89, 25);
            this.metroLabel20.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel20.TabIndex = 43;
            this.metroLabel20.Text = "t_OFFSCR";
            // 
            // metroLabel21
            // 
            this.metroLabel21.AutoSize = true;
            this.metroLabel21.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel21.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel21.Location = new System.Drawing.Point(471, 216);
            this.metroLabel21.Name = "metroLabel21";
            this.metroLabel21.Size = new System.Drawing.Size(130, 25);
            this.metroLabel21.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel21.TabIndex = 42;
            this.metroLabel21.Text = "RegenStart Key";
            // 
            // metroLabel22
            // 
            this.metroLabel22.AutoSize = true;
            this.metroLabel22.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel22.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel22.Location = new System.Drawing.Point(528, 170);
            this.metroLabel22.Name = "metroLabel22";
            this.metroLabel22.Size = new System.Drawing.Size(61, 25);
            this.metroLabel22.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel22.TabIndex = 41;
            this.metroLabel22.Text = "n_SCR";
            // 
            // metroLabel23
            // 
            this.metroLabel23.AutoSize = true;
            this.metroLabel23.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel23.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel23.Location = new System.Drawing.Point(528, 121);
            this.metroLabel23.Name = "metroLabel23";
            this.metroLabel23.Size = new System.Drawing.Size(57, 25);
            this.metroLabel23.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel23.TabIndex = 40;
            this.metroLabel23.Text = "t_SCR";
            // 
            // metroLabel24
            // 
            this.metroLabel24.AutoSize = true;
            this.metroLabel24.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel24.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel24.Location = new System.Drawing.Point(528, 76);
            this.metroLabel24.Name = "metroLabel24";
            this.metroLabel24.Size = new System.Drawing.Size(57, 25);
            this.metroLabel24.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel24.TabIndex = 39;
            this.metroLabel24.Text = "f_SCR";
            // 
            // metroLabel25
            // 
            this.metroLabel25.AutoSize = true;
            this.metroLabel25.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel25.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel25.Location = new System.Drawing.Point(528, 33);
            this.metroLabel25.Name = "metroLabel25";
            this.metroLabel25.Size = new System.Drawing.Size(73, 25);
            this.metroLabel25.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel25.TabIndex = 38;
            this.metroLabel25.Text = "Base P1";
            // 
            // textBox_DrvTime
            // 
            // 
            // 
            // 
            this.textBox_DrvTime.CustomButton.Image = null;
            this.textBox_DrvTime.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_DrvTime.CustomButton.Name = "";
            this.textBox_DrvTime.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_DrvTime.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_DrvTime.CustomButton.TabIndex = 1;
            this.textBox_DrvTime.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_DrvTime.CustomButton.UseSelectable = true;
            this.textBox_DrvTime.CustomButton.Visible = false;
            this.textBox_DrvTime.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_DrvTime.Lines = new string[0];
            this.textBox_DrvTime.Location = new System.Drawing.Point(344, 417);
            this.textBox_DrvTime.MaxLength = 32767;
            this.textBox_DrvTime.Name = "textBox_DrvTime";
            this.textBox_DrvTime.PasswordChar = '\0';
            this.textBox_DrvTime.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_DrvTime.SelectedText = "";
            this.textBox_DrvTime.SelectionLength = 0;
            this.textBox_DrvTime.SelectionStart = 0;
            this.textBox_DrvTime.ShortcutsEnabled = true;
            this.textBox_DrvTime.Size = new System.Drawing.Size(130, 40);
            this.textBox_DrvTime.TabIndex = 37;
            this.textBox_DrvTime.UseSelectable = true;
            this.textBox_DrvTime.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_DrvTime.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Lock
            // 
            // 
            // 
            // 
            this.textBox_Lock.CustomButton.Image = null;
            this.textBox_Lock.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Lock.CustomButton.Name = "";
            this.textBox_Lock.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Lock.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Lock.CustomButton.TabIndex = 1;
            this.textBox_Lock.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Lock.CustomButton.UseSelectable = true;
            this.textBox_Lock.CustomButton.Visible = false;
            this.textBox_Lock.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Lock.Lines = new string[0];
            this.textBox_Lock.Location = new System.Drawing.Point(344, 364);
            this.textBox_Lock.MaxLength = 32767;
            this.textBox_Lock.Name = "textBox_Lock";
            this.textBox_Lock.PasswordChar = '\0';
            this.textBox_Lock.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Lock.SelectedText = "";
            this.textBox_Lock.SelectionLength = 0;
            this.textBox_Lock.SelectionStart = 0;
            this.textBox_Lock.ShortcutsEnabled = true;
            this.textBox_Lock.Size = new System.Drawing.Size(130, 40);
            this.textBox_Lock.TabIndex = 36;
            this.textBox_Lock.UseSelectable = true;
            this.textBox_Lock.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Lock.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel16
            // 
            this.metroLabel16.AutoSize = true;
            this.metroLabel16.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel16.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel16.Location = new System.Drawing.Point(260, 417);
            this.metroLabel16.Name = "metroLabel16";
            this.metroLabel16.Size = new System.Drawing.Size(78, 25);
            this.metroLabel16.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel16.TabIndex = 35;
            this.metroLabel16.Text = "DrvTime";
            // 
            // metroLabel15
            // 
            this.metroLabel15.AutoSize = true;
            this.metroLabel15.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel15.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel15.Location = new System.Drawing.Point(277, 364);
            this.metroLabel15.Name = "metroLabel15";
            this.metroLabel15.Size = new System.Drawing.Size(48, 25);
            this.metroLabel15.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel15.TabIndex = 34;
            this.metroLabel15.Text = "Lock";
            // 
            // textBox_Sts
            // 
            // 
            // 
            // 
            this.textBox_Sts.CustomButton.Image = null;
            this.textBox_Sts.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Sts.CustomButton.Name = "";
            this.textBox_Sts.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Sts.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Sts.CustomButton.TabIndex = 1;
            this.textBox_Sts.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Sts.CustomButton.UseSelectable = true;
            this.textBox_Sts.CustomButton.Visible = false;
            this.textBox_Sts.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Sts.Lines = new string[0];
            this.textBox_Sts.Location = new System.Drawing.Point(344, 311);
            this.textBox_Sts.MaxLength = 32767;
            this.textBox_Sts.Name = "textBox_Sts";
            this.textBox_Sts.PasswordChar = '\0';
            this.textBox_Sts.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Sts.SelectedText = "";
            this.textBox_Sts.SelectionLength = 0;
            this.textBox_Sts.SelectionStart = 0;
            this.textBox_Sts.ShortcutsEnabled = true;
            this.textBox_Sts.Size = new System.Drawing.Size(130, 40);
            this.textBox_Sts.TabIndex = 33;
            this.textBox_Sts.UseSelectable = true;
            this.textBox_Sts.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Sts.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Sig
            // 
            // 
            // 
            // 
            this.textBox_Sig.CustomButton.Image = null;
            this.textBox_Sig.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Sig.CustomButton.Name = "";
            this.textBox_Sig.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Sig.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Sig.CustomButton.TabIndex = 1;
            this.textBox_Sig.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Sig.CustomButton.UseSelectable = true;
            this.textBox_Sig.CustomButton.Visible = false;
            this.textBox_Sig.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Sig.Lines = new string[0];
            this.textBox_Sig.Location = new System.Drawing.Point(344, 262);
            this.textBox_Sig.MaxLength = 32767;
            this.textBox_Sig.Name = "textBox_Sig";
            this.textBox_Sig.PasswordChar = '\0';
            this.textBox_Sig.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Sig.SelectedText = "";
            this.textBox_Sig.SelectionLength = 0;
            this.textBox_Sig.SelectionStart = 0;
            this.textBox_Sig.ShortcutsEnabled = true;
            this.textBox_Sig.Size = new System.Drawing.Size(130, 40);
            this.textBox_Sig.TabIndex = 32;
            this.textBox_Sig.UseSelectable = true;
            this.textBox_Sig.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Sig.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_n_REG
            // 
            // 
            // 
            // 
            this.textBox_n_REG.CustomButton.Image = null;
            this.textBox_n_REG.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_n_REG.CustomButton.Name = "";
            this.textBox_n_REG.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_n_REG.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_n_REG.CustomButton.TabIndex = 1;
            this.textBox_n_REG.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_n_REG.CustomButton.UseSelectable = true;
            this.textBox_n_REG.CustomButton.Visible = false;
            this.textBox_n_REG.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_n_REG.Lines = new string[0];
            this.textBox_n_REG.Location = new System.Drawing.Point(344, 216);
            this.textBox_n_REG.MaxLength = 32767;
            this.textBox_n_REG.Name = "textBox_n_REG";
            this.textBox_n_REG.PasswordChar = '\0';
            this.textBox_n_REG.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_n_REG.SelectedText = "";
            this.textBox_n_REG.SelectionLength = 0;
            this.textBox_n_REG.SelectionStart = 0;
            this.textBox_n_REG.ShortcutsEnabled = true;
            this.textBox_n_REG.Size = new System.Drawing.Size(130, 40);
            this.textBox_n_REG.TabIndex = 31;
            this.textBox_n_REG.UseSelectable = true;
            this.textBox_n_REG.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_n_REG.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_t_REG
            // 
            // 
            // 
            // 
            this.textBox_t_REG.CustomButton.Image = null;
            this.textBox_t_REG.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_t_REG.CustomButton.Name = "";
            this.textBox_t_REG.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_t_REG.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_t_REG.CustomButton.TabIndex = 1;
            this.textBox_t_REG.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_t_REG.CustomButton.UseSelectable = true;
            this.textBox_t_REG.CustomButton.Visible = false;
            this.textBox_t_REG.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_t_REG.Lines = new string[0];
            this.textBox_t_REG.Location = new System.Drawing.Point(342, 170);
            this.textBox_t_REG.MaxLength = 32767;
            this.textBox_t_REG.Name = "textBox_t_REG";
            this.textBox_t_REG.PasswordChar = '\0';
            this.textBox_t_REG.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_t_REG.SelectedText = "";
            this.textBox_t_REG.SelectionLength = 0;
            this.textBox_t_REG.SelectionStart = 0;
            this.textBox_t_REG.ShortcutsEnabled = true;
            this.textBox_t_REG.Size = new System.Drawing.Size(130, 40);
            this.textBox_t_REG.TabIndex = 30;
            this.textBox_t_REG.UseSelectable = true;
            this.textBox_t_REG.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_t_REG.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Check
            // 
            // 
            // 
            // 
            this.textBox_Check.CustomButton.Image = null;
            this.textBox_Check.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Check.CustomButton.Name = "";
            this.textBox_Check.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Check.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Check.CustomButton.TabIndex = 1;
            this.textBox_Check.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Check.CustomButton.UseSelectable = true;
            this.textBox_Check.CustomButton.Visible = false;
            this.textBox_Check.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Check.Lines = new string[0];
            this.textBox_Check.Location = new System.Drawing.Point(341, 125);
            this.textBox_Check.MaxLength = 32767;
            this.textBox_Check.Name = "textBox_Check";
            this.textBox_Check.PasswordChar = '\0';
            this.textBox_Check.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Check.SelectedText = "";
            this.textBox_Check.SelectionLength = 0;
            this.textBox_Check.SelectionStart = 0;
            this.textBox_Check.ShortcutsEnabled = true;
            this.textBox_Check.Size = new System.Drawing.Size(130, 40);
            this.textBox_Check.TabIndex = 29;
            this.textBox_Check.UseSelectable = true;
            this.textBox_Check.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Check.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Error
            // 
            // 
            // 
            // 
            this.textBox_Error.CustomButton.Image = null;
            this.textBox_Error.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Error.CustomButton.Name = "";
            this.textBox_Error.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Error.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Error.CustomButton.TabIndex = 1;
            this.textBox_Error.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Error.CustomButton.UseSelectable = true;
            this.textBox_Error.CustomButton.Visible = false;
            this.textBox_Error.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Error.Lines = new string[0];
            this.textBox_Error.Location = new System.Drawing.Point(342, 76);
            this.textBox_Error.MaxLength = 32767;
            this.textBox_Error.Name = "textBox_Error";
            this.textBox_Error.PasswordChar = '\0';
            this.textBox_Error.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Error.SelectedText = "";
            this.textBox_Error.SelectionLength = 0;
            this.textBox_Error.SelectionStart = 0;
            this.textBox_Error.ShortcutsEnabled = true;
            this.textBox_Error.Size = new System.Drawing.Size(130, 40);
            this.textBox_Error.TabIndex = 28;
            this.textBox_Error.UseSelectable = true;
            this.textBox_Error.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Error.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_FPD
            // 
            // 
            // 
            // 
            this.textBox_FPD.CustomButton.Image = null;
            this.textBox_FPD.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_FPD.CustomButton.Name = "";
            this.textBox_FPD.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_FPD.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_FPD.CustomButton.TabIndex = 1;
            this.textBox_FPD.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_FPD.CustomButton.UseSelectable = true;
            this.textBox_FPD.CustomButton.Visible = false;
            this.textBox_FPD.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_FPD.Lines = new string[0];
            this.textBox_FPD.Location = new System.Drawing.Point(341, 24);
            this.textBox_FPD.MaxLength = 32767;
            this.textBox_FPD.Name = "textBox_FPD";
            this.textBox_FPD.PasswordChar = '\0';
            this.textBox_FPD.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_FPD.SelectedText = "";
            this.textBox_FPD.SelectionLength = 0;
            this.textBox_FPD.SelectionStart = 0;
            this.textBox_FPD.ShortcutsEnabled = true;
            this.textBox_FPD.Size = new System.Drawing.Size(130, 40);
            this.textBox_FPD.TabIndex = 27;
            this.textBox_FPD.UseSelectable = true;
            this.textBox_FPD.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_FPD.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_CarType
            // 
            // 
            // 
            // 
            this.textBox_CarType.CustomButton.Image = null;
            this.textBox_CarType.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_CarType.CustomButton.Name = "";
            this.textBox_CarType.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_CarType.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_CarType.CustomButton.TabIndex = 1;
            this.textBox_CarType.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_CarType.CustomButton.UseSelectable = true;
            this.textBox_CarType.CustomButton.Visible = false;
            this.textBox_CarType.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_CarType.Lines = new string[0];
            this.textBox_CarType.Location = new System.Drawing.Point(90, 417);
            this.textBox_CarType.MaxLength = 32767;
            this.textBox_CarType.Name = "textBox_CarType";
            this.textBox_CarType.PasswordChar = '\0';
            this.textBox_CarType.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_CarType.SelectedText = "";
            this.textBox_CarType.SelectionLength = 0;
            this.textBox_CarType.SelectionStart = 0;
            this.textBox_CarType.ShortcutsEnabled = true;
            this.textBox_CarType.Size = new System.Drawing.Size(130, 40);
            this.textBox_CarType.TabIndex = 26;
            this.textBox_CarType.UseSelectable = true;
            this.textBox_CarType.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_CarType.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_f_REG
            // 
            // 
            // 
            // 
            this.textBox_f_REG.CustomButton.Image = null;
            this.textBox_f_REG.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_f_REG.CustomButton.Name = "";
            this.textBox_f_REG.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_f_REG.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_f_REG.CustomButton.TabIndex = 1;
            this.textBox_f_REG.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_f_REG.CustomButton.UseSelectable = true;
            this.textBox_f_REG.CustomButton.Visible = false;
            this.textBox_f_REG.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_f_REG.Lines = new string[0];
            this.textBox_f_REG.Location = new System.Drawing.Point(90, 364);
            this.textBox_f_REG.MaxLength = 32767;
            this.textBox_f_REG.Name = "textBox_f_REG";
            this.textBox_f_REG.PasswordChar = '\0';
            this.textBox_f_REG.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_f_REG.SelectedText = "";
            this.textBox_f_REG.SelectionLength = 0;
            this.textBox_f_REG.SelectionStart = 0;
            this.textBox_f_REG.ShortcutsEnabled = true;
            this.textBox_f_REG.Size = new System.Drawing.Size(130, 40);
            this.textBox_f_REG.TabIndex = 25;
            this.textBox_f_REG.UseSelectable = true;
            this.textBox_f_REG.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_f_REG.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_IHC
            // 
            // 
            // 
            // 
            this.textBox_IHC.CustomButton.Image = null;
            this.textBox_IHC.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_IHC.CustomButton.Name = "";
            this.textBox_IHC.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_IHC.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_IHC.CustomButton.TabIndex = 1;
            this.textBox_IHC.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_IHC.CustomButton.UseSelectable = true;
            this.textBox_IHC.CustomButton.Visible = false;
            this.textBox_IHC.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_IHC.Lines = new string[0];
            this.textBox_IHC.Location = new System.Drawing.Point(90, 311);
            this.textBox_IHC.MaxLength = 32767;
            this.textBox_IHC.Name = "textBox_IHC";
            this.textBox_IHC.PasswordChar = '\0';
            this.textBox_IHC.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_IHC.SelectedText = "";
            this.textBox_IHC.SelectionLength = 0;
            this.textBox_IHC.SelectionStart = 0;
            this.textBox_IHC.ShortcutsEnabled = true;
            this.textBox_IHC.Size = new System.Drawing.Size(130, 40);
            this.textBox_IHC.TabIndex = 24;
            this.textBox_IHC.UseSelectable = true;
            this.textBox_IHC.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_IHC.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Rate
            // 
            // 
            // 
            // 
            this.textBox_Rate.CustomButton.Image = null;
            this.textBox_Rate.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Rate.CustomButton.Name = "";
            this.textBox_Rate.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Rate.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Rate.CustomButton.TabIndex = 1;
            this.textBox_Rate.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Rate.CustomButton.UseSelectable = true;
            this.textBox_Rate.CustomButton.Visible = false;
            this.textBox_Rate.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Rate.Lines = new string[0];
            this.textBox_Rate.Location = new System.Drawing.Point(90, 259);
            this.textBox_Rate.MaxLength = 32767;
            this.textBox_Rate.Name = "textBox_Rate";
            this.textBox_Rate.PasswordChar = '\0';
            this.textBox_Rate.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Rate.SelectedText = "";
            this.textBox_Rate.SelectionLength = 0;
            this.textBox_Rate.SelectionStart = 0;
            this.textBox_Rate.ShortcutsEnabled = true;
            this.textBox_Rate.Size = new System.Drawing.Size(130, 40);
            this.textBox_Rate.TabIndex = 23;
            this.textBox_Rate.UseSelectable = true;
            this.textBox_Rate.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Rate.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_P
            // 
            // 
            // 
            // 
            this.textBox_P.CustomButton.Image = null;
            this.textBox_P.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_P.CustomButton.Name = "";
            this.textBox_P.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_P.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_P.CustomButton.TabIndex = 1;
            this.textBox_P.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_P.CustomButton.UseSelectable = true;
            this.textBox_P.CustomButton.Visible = false;
            this.textBox_P.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_P.Lines = new string[0];
            this.textBox_P.Location = new System.Drawing.Point(90, 216);
            this.textBox_P.MaxLength = 32767;
            this.textBox_P.Name = "textBox_P";
            this.textBox_P.PasswordChar = '\0';
            this.textBox_P.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_P.SelectedText = "";
            this.textBox_P.SelectionLength = 0;
            this.textBox_P.SelectionStart = 0;
            this.textBox_P.ShortcutsEnabled = true;
            this.textBox_P.Size = new System.Drawing.Size(130, 40);
            this.textBox_P.TabIndex = 22;
            this.textBox_P.UseSelectable = true;
            this.textBox_P.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_P.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_T4
            // 
            // 
            // 
            // 
            this.textBox_T4.CustomButton.Image = null;
            this.textBox_T4.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_T4.CustomButton.Name = "";
            this.textBox_T4.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_T4.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_T4.CustomButton.TabIndex = 1;
            this.textBox_T4.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_T4.CustomButton.UseSelectable = true;
            this.textBox_T4.CustomButton.Visible = false;
            this.textBox_T4.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_T4.Lines = new string[0];
            this.textBox_T4.Location = new System.Drawing.Point(90, 170);
            this.textBox_T4.MaxLength = 32767;
            this.textBox_T4.Name = "textBox_T4";
            this.textBox_T4.PasswordChar = '\0';
            this.textBox_T4.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_T4.SelectedText = "";
            this.textBox_T4.SelectionLength = 0;
            this.textBox_T4.SelectionStart = 0;
            this.textBox_T4.ShortcutsEnabled = true;
            this.textBox_T4.Size = new System.Drawing.Size(130, 40);
            this.textBox_T4.TabIndex = 21;
            this.textBox_T4.UseSelectable = true;
            this.textBox_T4.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_T4.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_T3
            // 
            // 
            // 
            // 
            this.textBox_T3.CustomButton.Image = null;
            this.textBox_T3.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_T3.CustomButton.Name = "";
            this.textBox_T3.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_T3.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_T3.CustomButton.TabIndex = 1;
            this.textBox_T3.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_T3.CustomButton.UseSelectable = true;
            this.textBox_T3.CustomButton.Visible = false;
            this.textBox_T3.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_T3.Lines = new string[0];
            this.textBox_T3.Location = new System.Drawing.Point(90, 121);
            this.textBox_T3.MaxLength = 32767;
            this.textBox_T3.Name = "textBox_T3";
            this.textBox_T3.PasswordChar = '\0';
            this.textBox_T3.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_T3.SelectedText = "";
            this.textBox_T3.SelectionLength = 0;
            this.textBox_T3.SelectionStart = 0;
            this.textBox_T3.ShortcutsEnabled = true;
            this.textBox_T3.Size = new System.Drawing.Size(130, 40);
            this.textBox_T3.TabIndex = 20;
            this.textBox_T3.UseSelectable = true;
            this.textBox_T3.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_T3.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_T2
            // 
            // 
            // 
            // 
            this.textBox_T2.CustomButton.Image = null;
            this.textBox_T2.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_T2.CustomButton.Name = "";
            this.textBox_T2.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_T2.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_T2.CustomButton.TabIndex = 1;
            this.textBox_T2.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_T2.CustomButton.UseSelectable = true;
            this.textBox_T2.CustomButton.Visible = false;
            this.textBox_T2.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_T2.Lines = new string[0];
            this.textBox_T2.Location = new System.Drawing.Point(90, 70);
            this.textBox_T2.MaxLength = 32767;
            this.textBox_T2.Name = "textBox_T2";
            this.textBox_T2.PasswordChar = '\0';
            this.textBox_T2.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_T2.SelectedText = "";
            this.textBox_T2.SelectionLength = 0;
            this.textBox_T2.SelectionStart = 0;
            this.textBox_T2.ShortcutsEnabled = true;
            this.textBox_T2.Size = new System.Drawing.Size(130, 40);
            this.textBox_T2.TabIndex = 19;
            this.textBox_T2.UseSelectable = true;
            this.textBox_T2.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_T2.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_T1
            // 
            // 
            // 
            // 
            this.textBox_T1.CustomButton.Image = null;
            this.textBox_T1.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_T1.CustomButton.Name = "";
            this.textBox_T1.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_T1.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_T1.CustomButton.TabIndex = 1;
            this.textBox_T1.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_T1.CustomButton.UseSelectable = true;
            this.textBox_T1.CustomButton.Visible = false;
            this.textBox_T1.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_T1.Lines = new string[0];
            this.textBox_T1.Location = new System.Drawing.Point(90, 24);
            this.textBox_T1.MaxLength = 32767;
            this.textBox_T1.Name = "textBox_T1";
            this.textBox_T1.PasswordChar = '\0';
            this.textBox_T1.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_T1.SelectedText = "";
            this.textBox_T1.SelectionLength = 0;
            this.textBox_T1.SelectionStart = 0;
            this.textBox_T1.ShortcutsEnabled = true;
            this.textBox_T1.Size = new System.Drawing.Size(130, 40);
            this.textBox_T1.TabIndex = 18;
            this.textBox_T1.UseSelectable = true;
            this.textBox_T1.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_T1.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel14
            // 
            this.metroLabel14.AutoSize = true;
            this.metroLabel14.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel14.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel14.Location = new System.Drawing.Point(277, 311);
            this.metroLabel14.Name = "metroLabel14";
            this.metroLabel14.Size = new System.Drawing.Size(35, 25);
            this.metroLabel14.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel14.TabIndex = 17;
            this.metroLabel14.Text = "Sts";
            // 
            // metroLabel13
            // 
            this.metroLabel13.AutoSize = true;
            this.metroLabel13.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel13.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel13.Location = new System.Drawing.Point(277, 259);
            this.metroLabel13.Name = "metroLabel13";
            this.metroLabel13.Size = new System.Drawing.Size(37, 25);
            this.metroLabel13.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel13.TabIndex = 16;
            this.metroLabel13.Text = "Sig";
            // 
            // metroLabel12
            // 
            this.metroLabel12.AutoSize = true;
            this.metroLabel12.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel12.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel12.Location = new System.Drawing.Point(277, 214);
            this.metroLabel12.Name = "metroLabel12";
            this.metroLabel12.Size = new System.Drawing.Size(61, 25);
            this.metroLabel12.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel12.TabIndex = 15;
            this.metroLabel12.Text = "n_REG";
            // 
            // metroLabel11
            // 
            this.metroLabel11.AutoSize = true;
            this.metroLabel11.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel11.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel11.Location = new System.Drawing.Point(277, 170);
            this.metroLabel11.Name = "metroLabel11";
            this.metroLabel11.Size = new System.Drawing.Size(57, 25);
            this.metroLabel11.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel11.TabIndex = 14;
            this.metroLabel11.Text = "t_REG";
            // 
            // metroLabel10
            // 
            this.metroLabel10.AutoSize = true;
            this.metroLabel10.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel10.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel10.Location = new System.Drawing.Point(277, 121);
            this.metroLabel10.Name = "metroLabel10";
            this.metroLabel10.Size = new System.Drawing.Size(59, 25);
            this.metroLabel10.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel10.TabIndex = 13;
            this.metroLabel10.Text = "Check";
            // 
            // metroLabel2
            // 
            this.metroLabel2.AutoSize = true;
            this.metroLabel2.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel2.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel2.Location = new System.Drawing.Point(277, 76);
            this.metroLabel2.Name = "metroLabel2";
            this.metroLabel2.Size = new System.Drawing.Size(50, 25);
            this.metroLabel2.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel2.TabIndex = 12;
            this.metroLabel2.Text = "Error";
            // 
            // metroLabel1
            // 
            this.metroLabel1.AutoSize = true;
            this.metroLabel1.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel1.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel1.Location = new System.Drawing.Point(277, 33);
            this.metroLabel1.Name = "metroLabel1";
            this.metroLabel1.Size = new System.Drawing.Size(44, 25);
            this.metroLabel1.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel1.TabIndex = 11;
            this.metroLabel1.Text = "FPD";
            // 
            // metroLabel9
            // 
            this.metroLabel9.AutoSize = true;
            this.metroLabel9.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel9.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel9.Location = new System.Drawing.Point(-4, 417);
            this.metroLabel9.Name = "metroLabel9";
            this.metroLabel9.Size = new System.Drawing.Size(88, 25);
            this.metroLabel9.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel9.TabIndex = 10;
            this.metroLabel9.Text = "CAR Type";
            // 
            // metroLabel8
            // 
            this.metroLabel8.AutoSize = true;
            this.metroLabel8.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel8.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel8.Location = new System.Drawing.Point(-4, 364);
            this.metroLabel8.Name = "metroLabel8";
            this.metroLabel8.Size = new System.Drawing.Size(57, 25);
            this.metroLabel8.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel8.TabIndex = 9;
            this.metroLabel8.Text = "F_REF";
            // 
            // metroLabel7
            // 
            this.metroLabel7.AutoSize = true;
            this.metroLabel7.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel7.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel7.Location = new System.Drawing.Point(-4, 311);
            this.metroLabel7.Name = "metroLabel7";
            this.metroLabel7.Size = new System.Drawing.Size(41, 25);
            this.metroLabel7.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel7.TabIndex = 8;
            this.metroLabel7.Text = "IHC";
            // 
            // metroLabel6
            // 
            this.metroLabel6.AutoSize = true;
            this.metroLabel6.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel6.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel6.Location = new System.Drawing.Point(0, 259);
            this.metroLabel6.Name = "metroLabel6";
            this.metroLabel6.Size = new System.Drawing.Size(47, 25);
            this.metroLabel6.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel6.TabIndex = 7;
            this.metroLabel6.Text = "Rate";
            // 
            // metroLabel5
            // 
            this.metroLabel5.AutoSize = true;
            this.metroLabel5.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel5.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel5.Location = new System.Drawing.Point(-4, 214);
            this.metroLabel5.Name = "metroLabel5";
            this.metroLabel5.Size = new System.Drawing.Size(22, 25);
            this.metroLabel5.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel5.TabIndex = 6;
            this.metroLabel5.Text = "P";
            // 
            // metroLabel4
            // 
            this.metroLabel4.AutoSize = true;
            this.metroLabel4.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel4.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel4.Location = new System.Drawing.Point(-4, 170);
            this.metroLabel4.Name = "metroLabel4";
            this.metroLabel4.Size = new System.Drawing.Size(31, 25);
            this.metroLabel4.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel4.TabIndex = 5;
            this.metroLabel4.Text = "T4";
            // 
            // metroLabel3
            // 
            this.metroLabel3.AutoSize = true;
            this.metroLabel3.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel3.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel3.Location = new System.Drawing.Point(-4, 121);
            this.metroLabel3.Name = "metroLabel3";
            this.metroLabel3.Size = new System.Drawing.Size(31, 25);
            this.metroLabel3.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel3.TabIndex = 4;
            this.metroLabel3.Text = "T3";
            // 
            // T2
            // 
            this.T2.AutoSize = true;
            this.T2.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.T2.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.T2.Location = new System.Drawing.Point(-4, 76);
            this.T2.Name = "T2";
            this.T2.Size = new System.Drawing.Size(31, 25);
            this.T2.Style = MetroFramework.MetroColorStyle.Black;
            this.T2.TabIndex = 3;
            this.T2.Text = "T2";
            // 
            // T1
            // 
            this.T1.AutoSize = true;
            this.T1.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.T1.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.T1.Location = new System.Drawing.Point(-4, 33);
            this.T1.Name = "T1";
            this.T1.Size = new System.Drawing.Size(31, 25);
            this.T1.Style = MetroFramework.MetroColorStyle.Black;
            this.T1.TabIndex = 2;
            this.T1.Text = "T1";
            // 
            // metroTabPage2
            // 
            this.metroTabPage2.Controls.Add(this.textBox_H2k);
            this.metroTabPage2.Controls.Add(this.textBox_H1k);
            this.metroTabPage2.Controls.Add(this.textBox_Xc);
            this.metroTabPage2.Controls.Add(this.textBox_curXk);
            this.metroTabPage2.Controls.Add(this.metroLabel45);
            this.metroTabPage2.Controls.Add(this.metroLabel46);
            this.metroTabPage2.Controls.Add(this.metroLabel47);
            this.metroTabPage2.Controls.Add(this.metroLabel48);
            this.metroTabPage2.Controls.Add(this.textBox_TotalDosingRate);
            this.metroTabPage2.Controls.Add(this.textBox_DosingRatehouer);
            this.metroTabPage2.Controls.Add(this.textBox_ScrFlag);
            this.metroTabPage2.Controls.Add(this.textBox_SystemSignal);
            this.metroTabPage2.Controls.Add(this.textBox_SystemCheck);
            this.metroTabPage2.Controls.Add(this.textBox_SystemError);
            this.metroTabPage2.Controls.Add(this.metroLabel49);
            this.metroTabPage2.Controls.Add(this.metroLabel50);
            this.metroTabPage2.Controls.Add(this.metroLabel51);
            this.metroTabPage2.Controls.Add(this.metroLabel52);
            this.metroTabPage2.Controls.Add(this.metroLabel53);
            this.metroTabPage2.Controls.Add(this.metroLabel54);
            this.metroTabPage2.Controls.Add(this.textBox_NoxReduction);
            this.metroTabPage2.Controls.Add(this.textBox_UreaQuality);
            this.metroTabPage2.Controls.Add(this.textBox_SCRMode);
            this.metroTabPage2.Controls.Add(this.textBox_StatusAlpha);
            this.metroTabPage2.Controls.Add(this.metroLabel35);
            this.metroTabPage2.Controls.Add(this.metroLabel36);
            this.metroTabPage2.Controls.Add(this.metroLabel37);
            this.metroTabPage2.Controls.Add(this.metroLabel38);
            this.metroTabPage2.Controls.Add(this.textBox_Map_Y);
            this.metroTabPage2.Controls.Add(this.textBox_Map_X);
            this.metroTabPage2.Controls.Add(this.textBox_BattVoltage);
            this.metroTabPage2.Controls.Add(this.textBox_TankLevelP);
            this.metroTabPage2.Controls.Add(this.textBox_TankTemp);
            this.metroTabPage2.Controls.Add(this.textBox_Maf_temp);
            this.metroTabPage2.Controls.Add(this.metroLabel39);
            this.metroTabPage2.Controls.Add(this.metroLabel40);
            this.metroTabPage2.Controls.Add(this.metroLabel41);
            this.metroTabPage2.Controls.Add(this.metroLabel42);
            this.metroTabPage2.Controls.Add(this.metroLabel43);
            this.metroTabPage2.Controls.Add(this.metroLabel44);
            this.metroTabPage2.Controls.Add(this.textBox_MafKg_H);
            this.metroTabPage2.Controls.Add(this.textBox_DosingDuty);
            this.metroTabPage2.Controls.Add(this.textBox_Noxppm2);
            this.metroTabPage2.Controls.Add(this.textBox_Noxppm1);
            this.metroTabPage2.Controls.Add(this.metroLabel17);
            this.metroTabPage2.Controls.Add(this.metroLabel32);
            this.metroTabPage2.Controls.Add(this.metroLabel33);
            this.metroTabPage2.Controls.Add(this.metroLabel34);
            this.metroTabPage2.Controls.Add(this.textBox_P1_bar);
            this.metroTabPage2.Controls.Add(this.textBox_T3_temp);
            this.metroTabPage2.Controls.Add(this.textBox_Tavg_temp);
            this.metroTabPage2.Controls.Add(this.textBox_T2_temp);
            this.metroTabPage2.Controls.Add(this.textBox_T1_temp);
            this.metroTabPage2.Controls.Add(this.textBox_Version);
            this.metroTabPage2.Controls.Add(this.metroLabel26);
            this.metroTabPage2.Controls.Add(this.metroLabel27);
            this.metroTabPage2.Controls.Add(this.metroLabel28);
            this.metroTabPage2.Controls.Add(this.metroLabel29);
            this.metroTabPage2.Controls.Add(this.metroLabel30);
            this.metroTabPage2.Controls.Add(this.metroLabel31);
            this.metroTabPage2.HorizontalScrollbarBarColor = true;
            this.metroTabPage2.HorizontalScrollbarHighlightOnWheel = false;
            this.metroTabPage2.HorizontalScrollbarSize = 10;
            this.metroTabPage2.Location = new System.Drawing.Point(4, 44);
            this.metroTabPage2.Name = "metroTabPage2";
            this.metroTabPage2.Size = new System.Drawing.Size(1122, 664);
            this.metroTabPage2.TabIndex = 1;
            this.metroTabPage2.Text = "SCR";
            this.metroTabPage2.VerticalScrollbarBarColor = true;
            this.metroTabPage2.VerticalScrollbarHighlightOnWheel = false;
            this.metroTabPage2.VerticalScrollbarSize = 10;
            // 
            // textBox_H2k
            // 
            // 
            // 
            // 
            this.textBox_H2k.CustomButton.Image = null;
            this.textBox_H2k.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_H2k.CustomButton.Name = "";
            this.textBox_H2k.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_H2k.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_H2k.CustomButton.TabIndex = 1;
            this.textBox_H2k.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_H2k.CustomButton.UseSelectable = true;
            this.textBox_H2k.CustomButton.Visible = false;
            this.textBox_H2k.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_H2k.Lines = new string[0];
            this.textBox_H2k.Location = new System.Drawing.Point(583, 448);
            this.textBox_H2k.MaxLength = 32767;
            this.textBox_H2k.Name = "textBox_H2k";
            this.textBox_H2k.PasswordChar = '\0';
            this.textBox_H2k.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_H2k.SelectedText = "";
            this.textBox_H2k.SelectionLength = 0;
            this.textBox_H2k.SelectionStart = 0;
            this.textBox_H2k.ShortcutsEnabled = true;
            this.textBox_H2k.Size = new System.Drawing.Size(130, 40);
            this.textBox_H2k.TabIndex = 85;
            this.textBox_H2k.UseSelectable = true;
            this.textBox_H2k.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_H2k.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_H1k
            // 
            // 
            // 
            // 
            this.textBox_H1k.CustomButton.Image = null;
            this.textBox_H1k.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_H1k.CustomButton.Name = "";
            this.textBox_H1k.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_H1k.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_H1k.CustomButton.TabIndex = 1;
            this.textBox_H1k.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_H1k.CustomButton.UseSelectable = true;
            this.textBox_H1k.CustomButton.Visible = false;
            this.textBox_H1k.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_H1k.Lines = new string[0];
            this.textBox_H1k.Location = new System.Drawing.Point(583, 405);
            this.textBox_H1k.MaxLength = 32767;
            this.textBox_H1k.Name = "textBox_H1k";
            this.textBox_H1k.PasswordChar = '\0';
            this.textBox_H1k.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_H1k.SelectedText = "";
            this.textBox_H1k.SelectionLength = 0;
            this.textBox_H1k.SelectionStart = 0;
            this.textBox_H1k.ShortcutsEnabled = true;
            this.textBox_H1k.Size = new System.Drawing.Size(130, 40);
            this.textBox_H1k.TabIndex = 84;
            this.textBox_H1k.UseSelectable = true;
            this.textBox_H1k.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_H1k.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Xc
            // 
            // 
            // 
            // 
            this.textBox_Xc.CustomButton.Image = null;
            this.textBox_Xc.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Xc.CustomButton.Name = "";
            this.textBox_Xc.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Xc.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Xc.CustomButton.TabIndex = 1;
            this.textBox_Xc.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Xc.CustomButton.UseSelectable = true;
            this.textBox_Xc.CustomButton.Visible = false;
            this.textBox_Xc.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Xc.Lines = new string[0];
            this.textBox_Xc.Location = new System.Drawing.Point(583, 359);
            this.textBox_Xc.MaxLength = 32767;
            this.textBox_Xc.Name = "textBox_Xc";
            this.textBox_Xc.PasswordChar = '\0';
            this.textBox_Xc.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Xc.SelectedText = "";
            this.textBox_Xc.SelectionLength = 0;
            this.textBox_Xc.SelectionStart = 0;
            this.textBox_Xc.ShortcutsEnabled = true;
            this.textBox_Xc.Size = new System.Drawing.Size(130, 40);
            this.textBox_Xc.TabIndex = 83;
            this.textBox_Xc.UseSelectable = true;
            this.textBox_Xc.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Xc.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_curXk
            // 
            // 
            // 
            // 
            this.textBox_curXk.CustomButton.Image = null;
            this.textBox_curXk.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_curXk.CustomButton.Name = "";
            this.textBox_curXk.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_curXk.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_curXk.CustomButton.TabIndex = 1;
            this.textBox_curXk.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_curXk.CustomButton.UseSelectable = true;
            this.textBox_curXk.CustomButton.Visible = false;
            this.textBox_curXk.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_curXk.Lines = new string[0];
            this.textBox_curXk.Location = new System.Drawing.Point(583, 310);
            this.textBox_curXk.MaxLength = 32767;
            this.textBox_curXk.Name = "textBox_curXk";
            this.textBox_curXk.PasswordChar = '\0';
            this.textBox_curXk.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_curXk.SelectedText = "";
            this.textBox_curXk.SelectionLength = 0;
            this.textBox_curXk.SelectionStart = 0;
            this.textBox_curXk.ShortcutsEnabled = true;
            this.textBox_curXk.Size = new System.Drawing.Size(130, 40);
            this.textBox_curXk.TabIndex = 82;
            this.textBox_curXk.UseSelectable = true;
            this.textBox_curXk.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_curXk.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel45
            // 
            this.metroLabel45.AutoSize = true;
            this.metroLabel45.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel45.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel45.Location = new System.Drawing.Point(493, 448);
            this.metroLabel45.Name = "metroLabel45";
            this.metroLabel45.Size = new System.Drawing.Size(44, 25);
            this.metroLabel45.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel45.TabIndex = 81;
            this.metroLabel45.Text = "H2k";
            // 
            // metroLabel46
            // 
            this.metroLabel46.AutoSize = true;
            this.metroLabel46.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel46.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel46.Location = new System.Drawing.Point(493, 405);
            this.metroLabel46.Name = "metroLabel46";
            this.metroLabel46.Size = new System.Drawing.Size(44, 25);
            this.metroLabel46.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel46.TabIndex = 80;
            this.metroLabel46.Text = "H1k";
            // 
            // metroLabel47
            // 
            this.metroLabel47.AutoSize = true;
            this.metroLabel47.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel47.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel47.Location = new System.Drawing.Point(489, 359);
            this.metroLabel47.Name = "metroLabel47";
            this.metroLabel47.Size = new System.Drawing.Size(68, 25);
            this.metroLabel47.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel47.TabIndex = 79;
            this.metroLabel47.Text = "SCR Xc";
            // 
            // metroLabel48
            // 
            this.metroLabel48.AutoSize = true;
            this.metroLabel48.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel48.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel48.Location = new System.Drawing.Point(489, 310);
            this.metroLabel48.Name = "metroLabel48";
            this.metroLabel48.Size = new System.Drawing.Size(59, 25);
            this.metroLabel48.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel48.TabIndex = 78;
            this.metroLabel48.Text = "CurXk";
            // 
            // textBox_TotalDosingRate
            // 
            // 
            // 
            // 
            this.textBox_TotalDosingRate.CustomButton.Image = null;
            this.textBox_TotalDosingRate.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_TotalDosingRate.CustomButton.Name = "";
            this.textBox_TotalDosingRate.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_TotalDosingRate.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_TotalDosingRate.CustomButton.TabIndex = 1;
            this.textBox_TotalDosingRate.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_TotalDosingRate.CustomButton.UseSelectable = true;
            this.textBox_TotalDosingRate.CustomButton.Visible = false;
            this.textBox_TotalDosingRate.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_TotalDosingRate.Lines = new string[0];
            this.textBox_TotalDosingRate.Location = new System.Drawing.Point(583, 260);
            this.textBox_TotalDosingRate.MaxLength = 32767;
            this.textBox_TotalDosingRate.Name = "textBox_TotalDosingRate";
            this.textBox_TotalDosingRate.PasswordChar = '\0';
            this.textBox_TotalDosingRate.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_TotalDosingRate.SelectedText = "";
            this.textBox_TotalDosingRate.SelectionLength = 0;
            this.textBox_TotalDosingRate.SelectionStart = 0;
            this.textBox_TotalDosingRate.ShortcutsEnabled = true;
            this.textBox_TotalDosingRate.Size = new System.Drawing.Size(130, 40);
            this.textBox_TotalDosingRate.TabIndex = 77;
            this.textBox_TotalDosingRate.UseSelectable = true;
            this.textBox_TotalDosingRate.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_TotalDosingRate.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_DosingRatehouer
            // 
            // 
            // 
            // 
            this.textBox_DosingRatehouer.CustomButton.Image = null;
            this.textBox_DosingRatehouer.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_DosingRatehouer.CustomButton.Name = "";
            this.textBox_DosingRatehouer.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_DosingRatehouer.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_DosingRatehouer.CustomButton.TabIndex = 1;
            this.textBox_DosingRatehouer.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_DosingRatehouer.CustomButton.UseSelectable = true;
            this.textBox_DosingRatehouer.CustomButton.Visible = false;
            this.textBox_DosingRatehouer.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_DosingRatehouer.Lines = new string[0];
            this.textBox_DosingRatehouer.Location = new System.Drawing.Point(583, 217);
            this.textBox_DosingRatehouer.MaxLength = 32767;
            this.textBox_DosingRatehouer.Name = "textBox_DosingRatehouer";
            this.textBox_DosingRatehouer.PasswordChar = '\0';
            this.textBox_DosingRatehouer.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_DosingRatehouer.SelectedText = "";
            this.textBox_DosingRatehouer.SelectionLength = 0;
            this.textBox_DosingRatehouer.SelectionStart = 0;
            this.textBox_DosingRatehouer.ShortcutsEnabled = true;
            this.textBox_DosingRatehouer.Size = new System.Drawing.Size(130, 40);
            this.textBox_DosingRatehouer.TabIndex = 76;
            this.textBox_DosingRatehouer.UseSelectable = true;
            this.textBox_DosingRatehouer.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_DosingRatehouer.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_ScrFlag
            // 
            // 
            // 
            // 
            this.textBox_ScrFlag.CustomButton.Image = null;
            this.textBox_ScrFlag.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_ScrFlag.CustomButton.Name = "";
            this.textBox_ScrFlag.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_ScrFlag.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_ScrFlag.CustomButton.TabIndex = 1;
            this.textBox_ScrFlag.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_ScrFlag.CustomButton.UseSelectable = true;
            this.textBox_ScrFlag.CustomButton.Visible = false;
            this.textBox_ScrFlag.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_ScrFlag.Lines = new string[0];
            this.textBox_ScrFlag.Location = new System.Drawing.Point(583, 171);
            this.textBox_ScrFlag.MaxLength = 32767;
            this.textBox_ScrFlag.Name = "textBox_ScrFlag";
            this.textBox_ScrFlag.PasswordChar = '\0';
            this.textBox_ScrFlag.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_ScrFlag.SelectedText = "";
            this.textBox_ScrFlag.SelectionLength = 0;
            this.textBox_ScrFlag.SelectionStart = 0;
            this.textBox_ScrFlag.ShortcutsEnabled = true;
            this.textBox_ScrFlag.Size = new System.Drawing.Size(130, 40);
            this.textBox_ScrFlag.TabIndex = 75;
            this.textBox_ScrFlag.UseSelectable = true;
            this.textBox_ScrFlag.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_ScrFlag.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_SystemSignal
            // 
            // 
            // 
            // 
            this.textBox_SystemSignal.CustomButton.Image = null;
            this.textBox_SystemSignal.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_SystemSignal.CustomButton.Name = "";
            this.textBox_SystemSignal.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_SystemSignal.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_SystemSignal.CustomButton.TabIndex = 1;
            this.textBox_SystemSignal.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_SystemSignal.CustomButton.UseSelectable = true;
            this.textBox_SystemSignal.CustomButton.Visible = false;
            this.textBox_SystemSignal.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_SystemSignal.Lines = new string[0];
            this.textBox_SystemSignal.Location = new System.Drawing.Point(583, 122);
            this.textBox_SystemSignal.MaxLength = 32767;
            this.textBox_SystemSignal.Name = "textBox_SystemSignal";
            this.textBox_SystemSignal.PasswordChar = '\0';
            this.textBox_SystemSignal.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_SystemSignal.SelectedText = "";
            this.textBox_SystemSignal.SelectionLength = 0;
            this.textBox_SystemSignal.SelectionStart = 0;
            this.textBox_SystemSignal.ShortcutsEnabled = true;
            this.textBox_SystemSignal.Size = new System.Drawing.Size(130, 40);
            this.textBox_SystemSignal.TabIndex = 74;
            this.textBox_SystemSignal.UseSelectable = true;
            this.textBox_SystemSignal.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_SystemSignal.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_SystemCheck
            // 
            // 
            // 
            // 
            this.textBox_SystemCheck.CustomButton.Image = null;
            this.textBox_SystemCheck.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_SystemCheck.CustomButton.Name = "";
            this.textBox_SystemCheck.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_SystemCheck.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_SystemCheck.CustomButton.TabIndex = 1;
            this.textBox_SystemCheck.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_SystemCheck.CustomButton.UseSelectable = true;
            this.textBox_SystemCheck.CustomButton.Visible = false;
            this.textBox_SystemCheck.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_SystemCheck.Lines = new string[0];
            this.textBox_SystemCheck.Location = new System.Drawing.Point(583, 71);
            this.textBox_SystemCheck.MaxLength = 32767;
            this.textBox_SystemCheck.Name = "textBox_SystemCheck";
            this.textBox_SystemCheck.PasswordChar = '\0';
            this.textBox_SystemCheck.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_SystemCheck.SelectedText = "";
            this.textBox_SystemCheck.SelectionLength = 0;
            this.textBox_SystemCheck.SelectionStart = 0;
            this.textBox_SystemCheck.ShortcutsEnabled = true;
            this.textBox_SystemCheck.Size = new System.Drawing.Size(130, 40);
            this.textBox_SystemCheck.TabIndex = 73;
            this.textBox_SystemCheck.UseSelectable = true;
            this.textBox_SystemCheck.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_SystemCheck.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_SystemError
            // 
            // 
            // 
            // 
            this.textBox_SystemError.CustomButton.Image = null;
            this.textBox_SystemError.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_SystemError.CustomButton.Name = "";
            this.textBox_SystemError.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_SystemError.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_SystemError.CustomButton.TabIndex = 1;
            this.textBox_SystemError.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_SystemError.CustomButton.UseSelectable = true;
            this.textBox_SystemError.CustomButton.Visible = false;
            this.textBox_SystemError.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_SystemError.Lines = new string[0];
            this.textBox_SystemError.Location = new System.Drawing.Point(583, 25);
            this.textBox_SystemError.MaxLength = 32767;
            this.textBox_SystemError.Name = "textBox_SystemError";
            this.textBox_SystemError.PasswordChar = '\0';
            this.textBox_SystemError.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_SystemError.SelectedText = "";
            this.textBox_SystemError.SelectionLength = 0;
            this.textBox_SystemError.SelectionStart = 0;
            this.textBox_SystemError.ShortcutsEnabled = true;
            this.textBox_SystemError.Size = new System.Drawing.Size(130, 40);
            this.textBox_SystemError.TabIndex = 72;
            this.textBox_SystemError.UseSelectable = true;
            this.textBox_SystemError.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_SystemError.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel49
            // 
            this.metroLabel49.AutoSize = true;
            this.metroLabel49.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel49.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel49.Location = new System.Drawing.Point(475, 260);
            this.metroLabel49.Name = "metroLabel49";
            this.metroLabel49.Size = new System.Drawing.Size(106, 25);
            this.metroLabel49.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel49.TabIndex = 71;
            this.metroLabel49.Text = "TotalDosing";
            // 
            // metroLabel50
            // 
            this.metroLabel50.AutoSize = true;
            this.metroLabel50.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel50.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel50.Location = new System.Drawing.Point(468, 217);
            this.metroLabel50.Name = "metroLabel50";
            this.metroLabel50.Size = new System.Drawing.Size(117, 25);
            this.metroLabel50.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel50.TabIndex = 70;
            this.metroLabel50.Text = "DosingRateH";
            // 
            // metroLabel51
            // 
            this.metroLabel51.AutoSize = true;
            this.metroLabel51.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel51.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel51.Location = new System.Drawing.Point(489, 171);
            this.metroLabel51.Name = "metroLabel51";
            this.metroLabel51.Size = new System.Drawing.Size(82, 25);
            this.metroLabel51.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel51.TabIndex = 69;
            this.metroLabel51.Text = "SCR Flag";
            // 
            // metroLabel52
            // 
            this.metroLabel52.AutoSize = true;
            this.metroLabel52.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel52.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel52.Location = new System.Drawing.Point(489, 122);
            this.metroLabel52.Name = "metroLabel52";
            this.metroLabel52.Size = new System.Drawing.Size(91, 25);
            this.metroLabel52.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel52.TabIndex = 68;
            this.metroLabel52.Text = "Sys Signal";
            // 
            // metroLabel53
            // 
            this.metroLabel53.AutoSize = true;
            this.metroLabel53.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel53.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel53.Location = new System.Drawing.Point(489, 77);
            this.metroLabel53.Name = "metroLabel53";
            this.metroLabel53.Size = new System.Drawing.Size(90, 25);
            this.metroLabel53.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel53.TabIndex = 67;
            this.metroLabel53.Text = "Sys Check";
            // 
            // metroLabel54
            // 
            this.metroLabel54.AutoSize = true;
            this.metroLabel54.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel54.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel54.Location = new System.Drawing.Point(489, 34);
            this.metroLabel54.Name = "metroLabel54";
            this.metroLabel54.Size = new System.Drawing.Size(81, 25);
            this.metroLabel54.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel54.TabIndex = 66;
            this.metroLabel54.Text = "Sys Error";
            // 
            // textBox_NoxReduction
            // 
            // 
            // 
            // 
            this.textBox_NoxReduction.CustomButton.Image = null;
            this.textBox_NoxReduction.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_NoxReduction.CustomButton.Name = "";
            this.textBox_NoxReduction.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_NoxReduction.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_NoxReduction.CustomButton.TabIndex = 1;
            this.textBox_NoxReduction.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_NoxReduction.CustomButton.UseSelectable = true;
            this.textBox_NoxReduction.CustomButton.Visible = false;
            this.textBox_NoxReduction.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_NoxReduction.Lines = new string[0];
            this.textBox_NoxReduction.Location = new System.Drawing.Point(344, 448);
            this.textBox_NoxReduction.MaxLength = 32767;
            this.textBox_NoxReduction.Name = "textBox_NoxReduction";
            this.textBox_NoxReduction.PasswordChar = '\0';
            this.textBox_NoxReduction.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_NoxReduction.SelectedText = "";
            this.textBox_NoxReduction.SelectionLength = 0;
            this.textBox_NoxReduction.SelectionStart = 0;
            this.textBox_NoxReduction.ShortcutsEnabled = true;
            this.textBox_NoxReduction.Size = new System.Drawing.Size(130, 40);
            this.textBox_NoxReduction.TabIndex = 65;
            this.textBox_NoxReduction.UseSelectable = true;
            this.textBox_NoxReduction.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_NoxReduction.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_UreaQuality
            // 
            // 
            // 
            // 
            this.textBox_UreaQuality.CustomButton.Image = null;
            this.textBox_UreaQuality.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_UreaQuality.CustomButton.Name = "";
            this.textBox_UreaQuality.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_UreaQuality.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_UreaQuality.CustomButton.TabIndex = 1;
            this.textBox_UreaQuality.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_UreaQuality.CustomButton.UseSelectable = true;
            this.textBox_UreaQuality.CustomButton.Visible = false;
            this.textBox_UreaQuality.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_UreaQuality.Lines = new string[0];
            this.textBox_UreaQuality.Location = new System.Drawing.Point(344, 405);
            this.textBox_UreaQuality.MaxLength = 32767;
            this.textBox_UreaQuality.Name = "textBox_UreaQuality";
            this.textBox_UreaQuality.PasswordChar = '\0';
            this.textBox_UreaQuality.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_UreaQuality.SelectedText = "";
            this.textBox_UreaQuality.SelectionLength = 0;
            this.textBox_UreaQuality.SelectionStart = 0;
            this.textBox_UreaQuality.ShortcutsEnabled = true;
            this.textBox_UreaQuality.Size = new System.Drawing.Size(130, 40);
            this.textBox_UreaQuality.TabIndex = 64;
            this.textBox_UreaQuality.UseSelectable = true;
            this.textBox_UreaQuality.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_UreaQuality.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_SCRMode
            // 
            // 
            // 
            // 
            this.textBox_SCRMode.CustomButton.Image = null;
            this.textBox_SCRMode.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_SCRMode.CustomButton.Name = "";
            this.textBox_SCRMode.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_SCRMode.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_SCRMode.CustomButton.TabIndex = 1;
            this.textBox_SCRMode.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_SCRMode.CustomButton.UseSelectable = true;
            this.textBox_SCRMode.CustomButton.Visible = false;
            this.textBox_SCRMode.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_SCRMode.Lines = new string[0];
            this.textBox_SCRMode.Location = new System.Drawing.Point(344, 359);
            this.textBox_SCRMode.MaxLength = 32767;
            this.textBox_SCRMode.Name = "textBox_SCRMode";
            this.textBox_SCRMode.PasswordChar = '\0';
            this.textBox_SCRMode.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_SCRMode.SelectedText = "";
            this.textBox_SCRMode.SelectionLength = 0;
            this.textBox_SCRMode.SelectionStart = 0;
            this.textBox_SCRMode.ShortcutsEnabled = true;
            this.textBox_SCRMode.Size = new System.Drawing.Size(130, 40);
            this.textBox_SCRMode.TabIndex = 63;
            this.textBox_SCRMode.UseSelectable = true;
            this.textBox_SCRMode.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_SCRMode.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_StatusAlpha
            // 
            // 
            // 
            // 
            this.textBox_StatusAlpha.CustomButton.Image = null;
            this.textBox_StatusAlpha.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_StatusAlpha.CustomButton.Name = "";
            this.textBox_StatusAlpha.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_StatusAlpha.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_StatusAlpha.CustomButton.TabIndex = 1;
            this.textBox_StatusAlpha.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_StatusAlpha.CustomButton.UseSelectable = true;
            this.textBox_StatusAlpha.CustomButton.Visible = false;
            this.textBox_StatusAlpha.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_StatusAlpha.Lines = new string[0];
            this.textBox_StatusAlpha.Location = new System.Drawing.Point(344, 310);
            this.textBox_StatusAlpha.MaxLength = 32767;
            this.textBox_StatusAlpha.Name = "textBox_StatusAlpha";
            this.textBox_StatusAlpha.PasswordChar = '\0';
            this.textBox_StatusAlpha.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_StatusAlpha.SelectedText = "";
            this.textBox_StatusAlpha.SelectionLength = 0;
            this.textBox_StatusAlpha.SelectionStart = 0;
            this.textBox_StatusAlpha.ShortcutsEnabled = true;
            this.textBox_StatusAlpha.Size = new System.Drawing.Size(130, 40);
            this.textBox_StatusAlpha.TabIndex = 62;
            this.textBox_StatusAlpha.UseSelectable = true;
            this.textBox_StatusAlpha.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_StatusAlpha.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel35
            // 
            this.metroLabel35.AutoSize = true;
            this.metroLabel35.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel35.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel35.Location = new System.Drawing.Point(254, 448);
            this.metroLabel35.Name = "metroLabel35";
            this.metroLabel35.Size = new System.Drawing.Size(91, 25);
            this.metroLabel35.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel35.TabIndex = 61;
            this.metroLabel35.Text = "Reduction";
            // 
            // metroLabel36
            // 
            this.metroLabel36.AutoSize = true;
            this.metroLabel36.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel36.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel36.Location = new System.Drawing.Point(236, 405);
            this.metroLabel36.Name = "metroLabel36";
            this.metroLabel36.Size = new System.Drawing.Size(87, 25);
            this.metroLabel36.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel36.TabIndex = 60;
            this.metroLabel36.Text = "U_Quality";
            // 
            // metroLabel37
            // 
            this.metroLabel37.AutoSize = true;
            this.metroLabel37.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel37.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel37.Location = new System.Drawing.Point(250, 359);
            this.metroLabel37.Name = "metroLabel37";
            this.metroLabel37.Size = new System.Drawing.Size(96, 25);
            this.metroLabel37.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel37.TabIndex = 59;
            this.metroLabel37.Text = "SCR Mode";
            // 
            // metroLabel38
            // 
            this.metroLabel38.AutoSize = true;
            this.metroLabel38.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel38.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel38.Location = new System.Drawing.Point(250, 310);
            this.metroLabel38.Name = "metroLabel38";
            this.metroLabel38.Size = new System.Drawing.Size(72, 25);
            this.metroLabel38.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel38.TabIndex = 58;
            this.metroLabel38.Text = "StatusA";
            // 
            // textBox_Map_Y
            // 
            // 
            // 
            // 
            this.textBox_Map_Y.CustomButton.Image = null;
            this.textBox_Map_Y.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Map_Y.CustomButton.Name = "";
            this.textBox_Map_Y.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Map_Y.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Map_Y.CustomButton.TabIndex = 1;
            this.textBox_Map_Y.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Map_Y.CustomButton.UseSelectable = true;
            this.textBox_Map_Y.CustomButton.Visible = false;
            this.textBox_Map_Y.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Map_Y.Lines = new string[0];
            this.textBox_Map_Y.Location = new System.Drawing.Point(344, 260);
            this.textBox_Map_Y.MaxLength = 32767;
            this.textBox_Map_Y.Name = "textBox_Map_Y";
            this.textBox_Map_Y.PasswordChar = '\0';
            this.textBox_Map_Y.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Map_Y.SelectedText = "";
            this.textBox_Map_Y.SelectionLength = 0;
            this.textBox_Map_Y.SelectionStart = 0;
            this.textBox_Map_Y.ShortcutsEnabled = true;
            this.textBox_Map_Y.Size = new System.Drawing.Size(130, 40);
            this.textBox_Map_Y.TabIndex = 57;
            this.textBox_Map_Y.UseSelectable = true;
            this.textBox_Map_Y.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Map_Y.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Map_X
            // 
            // 
            // 
            // 
            this.textBox_Map_X.CustomButton.Image = null;
            this.textBox_Map_X.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Map_X.CustomButton.Name = "";
            this.textBox_Map_X.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Map_X.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Map_X.CustomButton.TabIndex = 1;
            this.textBox_Map_X.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Map_X.CustomButton.UseSelectable = true;
            this.textBox_Map_X.CustomButton.Visible = false;
            this.textBox_Map_X.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Map_X.Lines = new string[0];
            this.textBox_Map_X.Location = new System.Drawing.Point(344, 217);
            this.textBox_Map_X.MaxLength = 32767;
            this.textBox_Map_X.Name = "textBox_Map_X";
            this.textBox_Map_X.PasswordChar = '\0';
            this.textBox_Map_X.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Map_X.SelectedText = "";
            this.textBox_Map_X.SelectionLength = 0;
            this.textBox_Map_X.SelectionStart = 0;
            this.textBox_Map_X.ShortcutsEnabled = true;
            this.textBox_Map_X.Size = new System.Drawing.Size(130, 40);
            this.textBox_Map_X.TabIndex = 56;
            this.textBox_Map_X.UseSelectable = true;
            this.textBox_Map_X.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Map_X.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_BattVoltage
            // 
            // 
            // 
            // 
            this.textBox_BattVoltage.CustomButton.Image = null;
            this.textBox_BattVoltage.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_BattVoltage.CustomButton.Name = "";
            this.textBox_BattVoltage.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_BattVoltage.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_BattVoltage.CustomButton.TabIndex = 1;
            this.textBox_BattVoltage.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_BattVoltage.CustomButton.UseSelectable = true;
            this.textBox_BattVoltage.CustomButton.Visible = false;
            this.textBox_BattVoltage.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_BattVoltage.Lines = new string[0];
            this.textBox_BattVoltage.Location = new System.Drawing.Point(344, 171);
            this.textBox_BattVoltage.MaxLength = 32767;
            this.textBox_BattVoltage.Name = "textBox_BattVoltage";
            this.textBox_BattVoltage.PasswordChar = '\0';
            this.textBox_BattVoltage.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_BattVoltage.SelectedText = "";
            this.textBox_BattVoltage.SelectionLength = 0;
            this.textBox_BattVoltage.SelectionStart = 0;
            this.textBox_BattVoltage.ShortcutsEnabled = true;
            this.textBox_BattVoltage.Size = new System.Drawing.Size(130, 40);
            this.textBox_BattVoltage.TabIndex = 55;
            this.textBox_BattVoltage.UseSelectable = true;
            this.textBox_BattVoltage.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_BattVoltage.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_TankLevelP
            // 
            // 
            // 
            // 
            this.textBox_TankLevelP.CustomButton.Image = null;
            this.textBox_TankLevelP.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_TankLevelP.CustomButton.Name = "";
            this.textBox_TankLevelP.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_TankLevelP.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_TankLevelP.CustomButton.TabIndex = 1;
            this.textBox_TankLevelP.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_TankLevelP.CustomButton.UseSelectable = true;
            this.textBox_TankLevelP.CustomButton.Visible = false;
            this.textBox_TankLevelP.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_TankLevelP.Lines = new string[0];
            this.textBox_TankLevelP.Location = new System.Drawing.Point(344, 122);
            this.textBox_TankLevelP.MaxLength = 32767;
            this.textBox_TankLevelP.Name = "textBox_TankLevelP";
            this.textBox_TankLevelP.PasswordChar = '\0';
            this.textBox_TankLevelP.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_TankLevelP.SelectedText = "";
            this.textBox_TankLevelP.SelectionLength = 0;
            this.textBox_TankLevelP.SelectionStart = 0;
            this.textBox_TankLevelP.ShortcutsEnabled = true;
            this.textBox_TankLevelP.Size = new System.Drawing.Size(130, 40);
            this.textBox_TankLevelP.TabIndex = 54;
            this.textBox_TankLevelP.UseSelectable = true;
            this.textBox_TankLevelP.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_TankLevelP.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_TankTemp
            // 
            // 
            // 
            // 
            this.textBox_TankTemp.CustomButton.Image = null;
            this.textBox_TankTemp.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_TankTemp.CustomButton.Name = "";
            this.textBox_TankTemp.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_TankTemp.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_TankTemp.CustomButton.TabIndex = 1;
            this.textBox_TankTemp.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_TankTemp.CustomButton.UseSelectable = true;
            this.textBox_TankTemp.CustomButton.Visible = false;
            this.textBox_TankTemp.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_TankTemp.Lines = new string[0];
            this.textBox_TankTemp.Location = new System.Drawing.Point(344, 71);
            this.textBox_TankTemp.MaxLength = 32767;
            this.textBox_TankTemp.Name = "textBox_TankTemp";
            this.textBox_TankTemp.PasswordChar = '\0';
            this.textBox_TankTemp.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_TankTemp.SelectedText = "";
            this.textBox_TankTemp.SelectionLength = 0;
            this.textBox_TankTemp.SelectionStart = 0;
            this.textBox_TankTemp.ShortcutsEnabled = true;
            this.textBox_TankTemp.Size = new System.Drawing.Size(130, 40);
            this.textBox_TankTemp.TabIndex = 53;
            this.textBox_TankTemp.UseSelectable = true;
            this.textBox_TankTemp.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_TankTemp.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Maf_temp
            // 
            // 
            // 
            // 
            this.textBox_Maf_temp.CustomButton.Image = null;
            this.textBox_Maf_temp.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Maf_temp.CustomButton.Name = "";
            this.textBox_Maf_temp.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Maf_temp.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Maf_temp.CustomButton.TabIndex = 1;
            this.textBox_Maf_temp.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Maf_temp.CustomButton.UseSelectable = true;
            this.textBox_Maf_temp.CustomButton.Visible = false;
            this.textBox_Maf_temp.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Maf_temp.Lines = new string[0];
            this.textBox_Maf_temp.Location = new System.Drawing.Point(344, 25);
            this.textBox_Maf_temp.MaxLength = 32767;
            this.textBox_Maf_temp.Name = "textBox_Maf_temp";
            this.textBox_Maf_temp.PasswordChar = '\0';
            this.textBox_Maf_temp.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Maf_temp.SelectedText = "";
            this.textBox_Maf_temp.SelectionLength = 0;
            this.textBox_Maf_temp.SelectionStart = 0;
            this.textBox_Maf_temp.ShortcutsEnabled = true;
            this.textBox_Maf_temp.Size = new System.Drawing.Size(130, 40);
            this.textBox_Maf_temp.TabIndex = 52;
            this.textBox_Maf_temp.UseSelectable = true;
            this.textBox_Maf_temp.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Maf_temp.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel39
            // 
            this.metroLabel39.AutoSize = true;
            this.metroLabel39.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel39.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel39.Location = new System.Drawing.Point(254, 260);
            this.metroLabel39.Name = "metroLabel39";
            this.metroLabel39.Size = new System.Drawing.Size(68, 25);
            this.metroLabel39.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel39.TabIndex = 51;
            this.metroLabel39.Text = "MAp_Y";
            // 
            // metroLabel40
            // 
            this.metroLabel40.AutoSize = true;
            this.metroLabel40.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel40.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel40.Location = new System.Drawing.Point(250, 215);
            this.metroLabel40.Name = "metroLabel40";
            this.metroLabel40.Size = new System.Drawing.Size(66, 25);
            this.metroLabel40.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel40.TabIndex = 50;
            this.metroLabel40.Text = "Map_X";
            // 
            // metroLabel41
            // 
            this.metroLabel41.AutoSize = true;
            this.metroLabel41.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel41.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel41.Location = new System.Drawing.Point(250, 171);
            this.metroLabel41.Name = "metroLabel41";
            this.metroLabel41.Size = new System.Drawing.Size(33, 25);
            this.metroLabel41.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel41.TabIndex = 49;
            this.metroLabel41.Text = "BV";
            // 
            // metroLabel42
            // 
            this.metroLabel42.AutoSize = true;
            this.metroLabel42.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel42.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel42.Location = new System.Drawing.Point(250, 122);
            this.metroLabel42.Name = "metroLabel42";
            this.metroLabel42.Size = new System.Drawing.Size(86, 25);
            this.metroLabel42.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel42.TabIndex = 48;
            this.metroLabel42.Text = "TankLevel";
            // 
            // metroLabel43
            // 
            this.metroLabel43.AutoSize = true;
            this.metroLabel43.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel43.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel43.Location = new System.Drawing.Point(250, 77);
            this.metroLabel43.Name = "metroLabel43";
            this.metroLabel43.Size = new System.Drawing.Size(90, 25);
            this.metroLabel43.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel43.TabIndex = 47;
            this.metroLabel43.Text = "TankTemp";
            // 
            // metroLabel44
            // 
            this.metroLabel44.AutoSize = true;
            this.metroLabel44.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel44.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel44.Location = new System.Drawing.Point(250, 34);
            this.metroLabel44.Name = "metroLabel44";
            this.metroLabel44.Size = new System.Drawing.Size(96, 25);
            this.metroLabel44.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel44.TabIndex = 46;
            this.metroLabel44.Text = "MAf_Temp";
            // 
            // textBox_MafKg_H
            // 
            // 
            // 
            // 
            this.textBox_MafKg_H.CustomButton.Image = null;
            this.textBox_MafKg_H.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_MafKg_H.CustomButton.Name = "";
            this.textBox_MafKg_H.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_MafKg_H.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_MafKg_H.CustomButton.TabIndex = 1;
            this.textBox_MafKg_H.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_MafKg_H.CustomButton.UseSelectable = true;
            this.textBox_MafKg_H.CustomButton.Visible = false;
            this.textBox_MafKg_H.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_MafKg_H.Lines = new string[0];
            this.textBox_MafKg_H.Location = new System.Drawing.Point(104, 448);
            this.textBox_MafKg_H.MaxLength = 32767;
            this.textBox_MafKg_H.Name = "textBox_MafKg_H";
            this.textBox_MafKg_H.PasswordChar = '\0';
            this.textBox_MafKg_H.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_MafKg_H.SelectedText = "";
            this.textBox_MafKg_H.SelectionLength = 0;
            this.textBox_MafKg_H.SelectionStart = 0;
            this.textBox_MafKg_H.ShortcutsEnabled = true;
            this.textBox_MafKg_H.Size = new System.Drawing.Size(130, 40);
            this.textBox_MafKg_H.TabIndex = 45;
            this.textBox_MafKg_H.UseSelectable = true;
            this.textBox_MafKg_H.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_MafKg_H.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_DosingDuty
            // 
            // 
            // 
            // 
            this.textBox_DosingDuty.CustomButton.Image = null;
            this.textBox_DosingDuty.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_DosingDuty.CustomButton.Name = "";
            this.textBox_DosingDuty.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_DosingDuty.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_DosingDuty.CustomButton.TabIndex = 1;
            this.textBox_DosingDuty.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_DosingDuty.CustomButton.UseSelectable = true;
            this.textBox_DosingDuty.CustomButton.Visible = false;
            this.textBox_DosingDuty.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_DosingDuty.Lines = new string[0];
            this.textBox_DosingDuty.Location = new System.Drawing.Point(104, 405);
            this.textBox_DosingDuty.MaxLength = 32767;
            this.textBox_DosingDuty.Name = "textBox_DosingDuty";
            this.textBox_DosingDuty.PasswordChar = '\0';
            this.textBox_DosingDuty.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_DosingDuty.SelectedText = "";
            this.textBox_DosingDuty.SelectionLength = 0;
            this.textBox_DosingDuty.SelectionStart = 0;
            this.textBox_DosingDuty.ShortcutsEnabled = true;
            this.textBox_DosingDuty.Size = new System.Drawing.Size(130, 40);
            this.textBox_DosingDuty.TabIndex = 44;
            this.textBox_DosingDuty.UseSelectable = true;
            this.textBox_DosingDuty.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_DosingDuty.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Noxppm2
            // 
            // 
            // 
            // 
            this.textBox_Noxppm2.CustomButton.Image = null;
            this.textBox_Noxppm2.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Noxppm2.CustomButton.Name = "";
            this.textBox_Noxppm2.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Noxppm2.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Noxppm2.CustomButton.TabIndex = 1;
            this.textBox_Noxppm2.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Noxppm2.CustomButton.UseSelectable = true;
            this.textBox_Noxppm2.CustomButton.Visible = false;
            this.textBox_Noxppm2.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Noxppm2.Lines = new string[0];
            this.textBox_Noxppm2.Location = new System.Drawing.Point(104, 359);
            this.textBox_Noxppm2.MaxLength = 32767;
            this.textBox_Noxppm2.Name = "textBox_Noxppm2";
            this.textBox_Noxppm2.PasswordChar = '\0';
            this.textBox_Noxppm2.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Noxppm2.SelectedText = "";
            this.textBox_Noxppm2.SelectionLength = 0;
            this.textBox_Noxppm2.SelectionStart = 0;
            this.textBox_Noxppm2.ShortcutsEnabled = true;
            this.textBox_Noxppm2.Size = new System.Drawing.Size(130, 40);
            this.textBox_Noxppm2.TabIndex = 43;
            this.textBox_Noxppm2.UseSelectable = true;
            this.textBox_Noxppm2.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Noxppm2.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Noxppm1
            // 
            // 
            // 
            // 
            this.textBox_Noxppm1.CustomButton.Image = null;
            this.textBox_Noxppm1.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Noxppm1.CustomButton.Name = "";
            this.textBox_Noxppm1.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Noxppm1.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Noxppm1.CustomButton.TabIndex = 1;
            this.textBox_Noxppm1.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Noxppm1.CustomButton.UseSelectable = true;
            this.textBox_Noxppm1.CustomButton.Visible = false;
            this.textBox_Noxppm1.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Noxppm1.Lines = new string[0];
            this.textBox_Noxppm1.Location = new System.Drawing.Point(104, 310);
            this.textBox_Noxppm1.MaxLength = 32767;
            this.textBox_Noxppm1.Name = "textBox_Noxppm1";
            this.textBox_Noxppm1.PasswordChar = '\0';
            this.textBox_Noxppm1.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Noxppm1.SelectedText = "";
            this.textBox_Noxppm1.SelectionLength = 0;
            this.textBox_Noxppm1.SelectionStart = 0;
            this.textBox_Noxppm1.ShortcutsEnabled = true;
            this.textBox_Noxppm1.Size = new System.Drawing.Size(130, 40);
            this.textBox_Noxppm1.TabIndex = 42;
            this.textBox_Noxppm1.UseSelectable = true;
            this.textBox_Noxppm1.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Noxppm1.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel17
            // 
            this.metroLabel17.AutoSize = true;
            this.metroLabel17.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel17.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel17.Location = new System.Drawing.Point(14, 448);
            this.metroLabel17.Name = "metroLabel17";
            this.metroLabel17.Size = new System.Drawing.Size(49, 25);
            this.metroLabel17.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel17.TabIndex = 41;
            this.metroLabel17.Text = "MAF";
            // 
            // metroLabel32
            // 
            this.metroLabel32.AutoSize = true;
            this.metroLabel32.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel32.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel32.Location = new System.Drawing.Point(-4, 405);
            this.metroLabel32.Name = "metroLabel32";
            this.metroLabel32.Size = new System.Drawing.Size(107, 25);
            this.metroLabel32.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel32.TabIndex = 40;
            this.metroLabel32.Text = "DosingDuty";
            // 
            // metroLabel33
            // 
            this.metroLabel33.AutoSize = true;
            this.metroLabel33.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel33.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel33.Location = new System.Drawing.Point(10, 359);
            this.metroLabel33.Name = "metroLabel33";
            this.metroLabel33.Size = new System.Drawing.Size(72, 25);
            this.metroLabel33.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel33.TabIndex = 39;
            this.metroLabel33.Text = "Nox Dn";
            // 
            // metroLabel34
            // 
            this.metroLabel34.AutoSize = true;
            this.metroLabel34.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel34.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel34.Location = new System.Drawing.Point(10, 310);
            this.metroLabel34.Name = "metroLabel34";
            this.metroLabel34.Size = new System.Drawing.Size(70, 25);
            this.metroLabel34.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel34.TabIndex = 38;
            this.metroLabel34.Text = "Nox up";
            // 
            // textBox_P1_bar
            // 
            // 
            // 
            // 
            this.textBox_P1_bar.CustomButton.Image = null;
            this.textBox_P1_bar.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_P1_bar.CustomButton.Name = "";
            this.textBox_P1_bar.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_P1_bar.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_P1_bar.CustomButton.TabIndex = 1;
            this.textBox_P1_bar.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_P1_bar.CustomButton.UseSelectable = true;
            this.textBox_P1_bar.CustomButton.Visible = false;
            this.textBox_P1_bar.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_P1_bar.Lines = new string[0];
            this.textBox_P1_bar.Location = new System.Drawing.Point(104, 260);
            this.textBox_P1_bar.MaxLength = 32767;
            this.textBox_P1_bar.Name = "textBox_P1_bar";
            this.textBox_P1_bar.PasswordChar = '\0';
            this.textBox_P1_bar.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_P1_bar.SelectedText = "";
            this.textBox_P1_bar.SelectionLength = 0;
            this.textBox_P1_bar.SelectionStart = 0;
            this.textBox_P1_bar.ShortcutsEnabled = true;
            this.textBox_P1_bar.Size = new System.Drawing.Size(130, 40);
            this.textBox_P1_bar.TabIndex = 37;
            this.textBox_P1_bar.UseSelectable = true;
            this.textBox_P1_bar.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_P1_bar.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_T3_temp
            // 
            // 
            // 
            // 
            this.textBox_T3_temp.CustomButton.Image = null;
            this.textBox_T3_temp.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_T3_temp.CustomButton.Name = "";
            this.textBox_T3_temp.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_T3_temp.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_T3_temp.CustomButton.TabIndex = 1;
            this.textBox_T3_temp.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_T3_temp.CustomButton.UseSelectable = true;
            this.textBox_T3_temp.CustomButton.Visible = false;
            this.textBox_T3_temp.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_T3_temp.Lines = new string[0];
            this.textBox_T3_temp.Location = new System.Drawing.Point(104, 217);
            this.textBox_T3_temp.MaxLength = 32767;
            this.textBox_T3_temp.Name = "textBox_T3_temp";
            this.textBox_T3_temp.PasswordChar = '\0';
            this.textBox_T3_temp.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_T3_temp.SelectedText = "";
            this.textBox_T3_temp.SelectionLength = 0;
            this.textBox_T3_temp.SelectionStart = 0;
            this.textBox_T3_temp.ShortcutsEnabled = true;
            this.textBox_T3_temp.Size = new System.Drawing.Size(130, 40);
            this.textBox_T3_temp.TabIndex = 36;
            this.textBox_T3_temp.UseSelectable = true;
            this.textBox_T3_temp.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_T3_temp.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Tavg_temp
            // 
            // 
            // 
            // 
            this.textBox_Tavg_temp.CustomButton.Image = null;
            this.textBox_Tavg_temp.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Tavg_temp.CustomButton.Name = "";
            this.textBox_Tavg_temp.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Tavg_temp.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Tavg_temp.CustomButton.TabIndex = 1;
            this.textBox_Tavg_temp.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Tavg_temp.CustomButton.UseSelectable = true;
            this.textBox_Tavg_temp.CustomButton.Visible = false;
            this.textBox_Tavg_temp.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Tavg_temp.Lines = new string[0];
            this.textBox_Tavg_temp.Location = new System.Drawing.Point(104, 171);
            this.textBox_Tavg_temp.MaxLength = 32767;
            this.textBox_Tavg_temp.Name = "textBox_Tavg_temp";
            this.textBox_Tavg_temp.PasswordChar = '\0';
            this.textBox_Tavg_temp.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Tavg_temp.SelectedText = "";
            this.textBox_Tavg_temp.SelectionLength = 0;
            this.textBox_Tavg_temp.SelectionStart = 0;
            this.textBox_Tavg_temp.ShortcutsEnabled = true;
            this.textBox_Tavg_temp.Size = new System.Drawing.Size(130, 40);
            this.textBox_Tavg_temp.TabIndex = 35;
            this.textBox_Tavg_temp.UseSelectable = true;
            this.textBox_Tavg_temp.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Tavg_temp.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_T2_temp
            // 
            // 
            // 
            // 
            this.textBox_T2_temp.CustomButton.Image = null;
            this.textBox_T2_temp.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_T2_temp.CustomButton.Name = "";
            this.textBox_T2_temp.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_T2_temp.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_T2_temp.CustomButton.TabIndex = 1;
            this.textBox_T2_temp.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_T2_temp.CustomButton.UseSelectable = true;
            this.textBox_T2_temp.CustomButton.Visible = false;
            this.textBox_T2_temp.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_T2_temp.Lines = new string[0];
            this.textBox_T2_temp.Location = new System.Drawing.Point(104, 122);
            this.textBox_T2_temp.MaxLength = 32767;
            this.textBox_T2_temp.Name = "textBox_T2_temp";
            this.textBox_T2_temp.PasswordChar = '\0';
            this.textBox_T2_temp.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_T2_temp.SelectedText = "";
            this.textBox_T2_temp.SelectionLength = 0;
            this.textBox_T2_temp.SelectionStart = 0;
            this.textBox_T2_temp.ShortcutsEnabled = true;
            this.textBox_T2_temp.Size = new System.Drawing.Size(130, 40);
            this.textBox_T2_temp.TabIndex = 34;
            this.textBox_T2_temp.UseSelectable = true;
            this.textBox_T2_temp.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_T2_temp.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_T1_temp
            // 
            // 
            // 
            // 
            this.textBox_T1_temp.CustomButton.Image = null;
            this.textBox_T1_temp.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_T1_temp.CustomButton.Name = "";
            this.textBox_T1_temp.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_T1_temp.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_T1_temp.CustomButton.TabIndex = 1;
            this.textBox_T1_temp.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_T1_temp.CustomButton.UseSelectable = true;
            this.textBox_T1_temp.CustomButton.Visible = false;
            this.textBox_T1_temp.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_T1_temp.Lines = new string[0];
            this.textBox_T1_temp.Location = new System.Drawing.Point(104, 71);
            this.textBox_T1_temp.MaxLength = 32767;
            this.textBox_T1_temp.Name = "textBox_T1_temp";
            this.textBox_T1_temp.PasswordChar = '\0';
            this.textBox_T1_temp.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_T1_temp.SelectedText = "";
            this.textBox_T1_temp.SelectionLength = 0;
            this.textBox_T1_temp.SelectionStart = 0;
            this.textBox_T1_temp.ShortcutsEnabled = true;
            this.textBox_T1_temp.Size = new System.Drawing.Size(130, 40);
            this.textBox_T1_temp.TabIndex = 33;
            this.textBox_T1_temp.UseSelectable = true;
            this.textBox_T1_temp.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_T1_temp.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // textBox_Version
            // 
            // 
            // 
            // 
            this.textBox_Version.CustomButton.Image = null;
            this.textBox_Version.CustomButton.Location = new System.Drawing.Point(92, 2);
            this.textBox_Version.CustomButton.Name = "";
            this.textBox_Version.CustomButton.Size = new System.Drawing.Size(35, 35);
            this.textBox_Version.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.textBox_Version.CustomButton.TabIndex = 1;
            this.textBox_Version.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.textBox_Version.CustomButton.UseSelectable = true;
            this.textBox_Version.CustomButton.Visible = false;
            this.textBox_Version.FontSize = MetroFramework.MetroTextBoxSize.Tall;
            this.textBox_Version.Lines = new string[0];
            this.textBox_Version.Location = new System.Drawing.Point(104, 25);
            this.textBox_Version.MaxLength = 32767;
            this.textBox_Version.Name = "textBox_Version";
            this.textBox_Version.PasswordChar = '\0';
            this.textBox_Version.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.textBox_Version.SelectedText = "";
            this.textBox_Version.SelectionLength = 0;
            this.textBox_Version.SelectionStart = 0;
            this.textBox_Version.ShortcutsEnabled = true;
            this.textBox_Version.Size = new System.Drawing.Size(130, 40);
            this.textBox_Version.TabIndex = 32;
            this.textBox_Version.UseSelectable = true;
            this.textBox_Version.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.textBox_Version.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel26
            // 
            this.metroLabel26.AutoSize = true;
            this.metroLabel26.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel26.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel26.Location = new System.Drawing.Point(14, 260);
            this.metroLabel26.Name = "metroLabel26";
            this.metroLabel26.Size = new System.Drawing.Size(62, 25);
            this.metroLabel26.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel26.TabIndex = 30;
            this.metroLabel26.Text = "P1 Bar";
            // 
            // metroLabel27
            // 
            this.metroLabel27.AutoSize = true;
            this.metroLabel27.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel27.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel27.Location = new System.Drawing.Point(10, 215);
            this.metroLabel27.Name = "metroLabel27";
            this.metroLabel27.Size = new System.Drawing.Size(31, 25);
            this.metroLabel27.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel27.TabIndex = 29;
            this.metroLabel27.Text = "T3";
            // 
            // metroLabel28
            // 
            this.metroLabel28.AutoSize = true;
            this.metroLabel28.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel28.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel28.Location = new System.Drawing.Point(10, 171);
            this.metroLabel28.Name = "metroLabel28";
            this.metroLabel28.Size = new System.Drawing.Size(60, 25);
            this.metroLabel28.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel28.TabIndex = 28;
            this.metroLabel28.Text = "T AVG";
            // 
            // metroLabel29
            // 
            this.metroLabel29.AutoSize = true;
            this.metroLabel29.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel29.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel29.Location = new System.Drawing.Point(10, 122);
            this.metroLabel29.Name = "metroLabel29";
            this.metroLabel29.Size = new System.Drawing.Size(47, 25);
            this.metroLabel29.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel29.TabIndex = 27;
            this.metroLabel29.Text = "T dn";
            // 
            // metroLabel30
            // 
            this.metroLabel30.AutoSize = true;
            this.metroLabel30.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel30.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel30.Location = new System.Drawing.Point(10, 77);
            this.metroLabel30.Name = "metroLabel30";
            this.metroLabel30.Size = new System.Drawing.Size(47, 25);
            this.metroLabel30.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel30.TabIndex = 26;
            this.metroLabel30.Text = "T up";
            // 
            // metroLabel31
            // 
            this.metroLabel31.AutoSize = true;
            this.metroLabel31.FontSize = MetroFramework.MetroLabelSize.Tall;
            this.metroLabel31.FontWeight = MetroFramework.MetroLabelWeight.Regular;
            this.metroLabel31.Location = new System.Drawing.Point(10, 34);
            this.metroLabel31.Name = "metroLabel31";
            this.metroLabel31.Size = new System.Drawing.Size(70, 25);
            this.metroLabel31.Style = MetroFramework.MetroColorStyle.Black;
            this.metroLabel31.TabIndex = 25;
            this.metroLabel31.Text = "Version";
            // 
            // metroTabPage3
            // 
            this.metroTabPage3.Controls.Add(this.label15);
            this.metroTabPage3.Controls.Add(this.textBox_Size);
            this.metroTabPage3.Controls.Add(this.label_status);
            this.metroTabPage3.Controls.Add(this.button_send);
            this.metroTabPage3.Controls.Add(this.label_send);
            this.metroTabPage3.Controls.Add(this.textBox_send);
            this.metroTabPage3.Controls.Add(this.richTextBox1);
            this.metroTabPage3.HorizontalScrollbarBarColor = true;
            this.metroTabPage3.HorizontalScrollbarHighlightOnWheel = false;
            this.metroTabPage3.HorizontalScrollbarSize = 10;
            this.metroTabPage3.Location = new System.Drawing.Point(4, 44);
            this.metroTabPage3.Name = "metroTabPage3";
            this.metroTabPage3.Size = new System.Drawing.Size(1122, 664);
            this.metroTabPage3.TabIndex = 2;
            this.metroTabPage3.Text = "Low Data";
            this.metroTabPage3.VerticalScrollbarBarColor = true;
            this.metroTabPage3.VerticalScrollbarHighlightOnWheel = false;
            this.metroTabPage3.VerticalScrollbarSize = 10;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(33, 46);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(775, 401);
            this.richTextBox1.TabIndex = 81;
            this.richTextBox1.Text = "";
            // 
            // metroTabPage4
            // 
            this.metroTabPage4.Controls.Add(this.chart1);
            this.metroTabPage4.HorizontalScrollbarBarColor = true;
            this.metroTabPage4.HorizontalScrollbarHighlightOnWheel = false;
            this.metroTabPage4.HorizontalScrollbarSize = 10;
            this.metroTabPage4.Location = new System.Drawing.Point(4, 44);
            this.metroTabPage4.Name = "metroTabPage4";
            this.metroTabPage4.Size = new System.Drawing.Size(1122, 664);
            this.metroTabPage4.TabIndex = 3;
            this.metroTabPage4.Text = "Chart";
            this.metroTabPage4.VerticalScrollbarBarColor = true;
            this.metroTabPage4.VerticalScrollbarHighlightOnWheel = false;
            this.metroTabPage4.VerticalScrollbarSize = 10;
            // 
            // chart1
            // 
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            legend1.Title = "T1";
            this.chart1.Legends.Add(legend1);
            this.chart1.Location = new System.Drawing.Point(52, 55);
            this.chart1.Name = "chart1";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chart1.Series.Add(series1);
            this.chart1.Size = new System.Drawing.Size(566, 334);
            this.chart1.TabIndex = 2;
            this.chart1.Text = "chart1";
            // 
            // metroPanel1
            // 
            this.metroPanel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.metroPanel1.Controls.Add(this.comboBox_port1);
            this.metroPanel1.Controls.Add(this.label_port);
            this.metroPanel1.Controls.Add(this.button_connect);
            this.metroPanel1.Controls.Add(this.button_disconnect);
            this.metroPanel1.HorizontalScrollbarBarColor = true;
            this.metroPanel1.HorizontalScrollbarHighlightOnWheel = false;
            this.metroPanel1.HorizontalScrollbarSize = 10;
            this.metroPanel1.Location = new System.Drawing.Point(759, 12);
            this.metroPanel1.Name = "metroPanel1";
            this.metroPanel1.Size = new System.Drawing.Size(357, 64);
            this.metroPanel1.TabIndex = 85;
            this.metroPanel1.VerticalScrollbarBarColor = true;
            this.metroPanel1.VerticalScrollbarHighlightOnWheel = false;
            this.metroPanel1.VerticalScrollbarSize = 10;
            // 
            // comboBox_port1
            // 
            this.comboBox_port1.FormattingEnabled = true;
            this.comboBox_port1.ItemHeight = 23;
            this.comboBox_port1.Location = new System.Drawing.Point(15, 25);
            this.comboBox_port1.Name = "comboBox_port1";
            this.comboBox_port1.Size = new System.Drawing.Size(110, 29);
            this.comboBox_port1.TabIndex = 89;
            this.comboBox_port1.UseSelectable = true;
            // 
            // label_port
            // 
            this.label_port.AutoSize = true;
            this.label_port.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label_port.Location = new System.Drawing.Point(51, 7);
            this.label_port.Name = "label_port";
            this.label_port.Size = new System.Drawing.Size(29, 15);
            this.label_port.TabIndex = 88;
            this.label_port.Text = "Port";
            // 
            // button_connect
            // 
            this.button_connect.BackColor = System.Drawing.SystemColors.ControlLight;
            this.button_connect.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.button_connect.Location = new System.Drawing.Point(146, 24);
            this.button_connect.Name = "button_connect";
            this.button_connect.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.button_connect.Size = new System.Drawing.Size(90, 30);
            this.button_connect.TabIndex = 86;
            this.button_connect.Text = "Com Open";
            this.button_connect.UseVisualStyleBackColor = false;
            // 
            // button_disconnect
            // 
            this.button_disconnect.BackColor = System.Drawing.SystemColors.ControlLight;
            this.button_disconnect.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.button_disconnect.Location = new System.Drawing.Point(244, 24);
            this.button_disconnect.Name = "button_disconnect";
            this.button_disconnect.Size = new System.Drawing.Size(97, 30);
            this.button_disconnect.TabIndex = 87;
            this.button_disconnect.Text = "Com Close";
            this.button_disconnect.UseVisualStyleBackColor = false;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Consolas", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(175, 147);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(96, 18);
            this.label15.TabIndex = 87;
            this.label15.Text = "Data Length";
            // 
            // textBox_Size
            // 
            this.textBox_Size.Location = new System.Drawing.Point(277, 147);
            this.textBox_Size.Name = "textBox_Size";
            this.textBox_Size.Size = new System.Drawing.Size(66, 21);
            this.textBox_Size.TabIndex = 86;
            // 
            // label_status
            // 
            this.label_status.AutoSize = true;
            this.label_status.Location = new System.Drawing.Point(116, 136);
            this.label_status.Name = "label_status";
            this.label_status.Size = new System.Drawing.Size(53, 12);
            this.label_status.TabIndex = 85;
            this.label_status.Text = "연결상태";
            // 
            // button_send
            // 
            this.button_send.Location = new System.Drawing.Point(376, 183);
            this.button_send.Name = "button_send";
            this.button_send.Size = new System.Drawing.Size(91, 23);
            this.button_send.TabIndex = 84;
            this.button_send.Text = "보내기";
            this.button_send.UseVisualStyleBackColor = true;
            // 
            // label_send
            // 
            this.label_send.AutoSize = true;
            this.label_send.Location = new System.Drawing.Point(109, 167);
            this.label_send.Name = "label_send";
            this.label_send.Size = new System.Drawing.Size(29, 12);
            this.label_send.TabIndex = 83;
            this.label_send.Text = "송신";
            // 
            // textBox_send
            // 
            this.textBox_send.Location = new System.Drawing.Point(111, 188);
            this.textBox_send.Name = "textBox_send";
            this.textBox_send.Size = new System.Drawing.Size(243, 21);
            this.textBox_send.TabIndex = 82;
            // 
            // metroTabPage5
            // 
            this.metroTabPage5.HorizontalScrollbarBarColor = true;
            this.metroTabPage5.HorizontalScrollbarHighlightOnWheel = false;
            this.metroTabPage5.HorizontalScrollbarSize = 10;
            this.metroTabPage5.Location = new System.Drawing.Point(4, 44);
            this.metroTabPage5.Name = "metroTabPage5";
            this.metroTabPage5.Size = new System.Drawing.Size(1122, 664);
            this.metroTabPage5.TabIndex = 4;
            this.metroTabPage5.Text = "Main ";
            this.metroTabPage5.VerticalScrollbarBarColor = true;
            this.metroTabPage5.VerticalScrollbarHighlightOnWheel = false;
            this.metroTabPage5.VerticalScrollbarSize = 10;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1149, 808);
            this.Controls.Add(this.metroPanel1);
            this.Controls.Add(this.metroTabControl1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "SCR CONTROL";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.metroTabControl1.ResumeLayout(false);
            this.metroTabPage1.ResumeLayout(false);
            this.metroTabPage1.PerformLayout();
            this.metroTabPage2.ResumeLayout(false);
            this.metroTabPage2.PerformLayout();
            this.metroTabPage3.ResumeLayout(false);
            this.metroTabPage3.PerformLayout();
            this.metroTabPage4.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.metroPanel1.ResumeLayout(false);
            this.metroPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.IO.Ports.SerialPort serialPort1;
        private MetroFramework.Controls.MetroTabControl metroTabControl1;
        private MetroFramework.Controls.MetroTabPage metroTabPage1;
        private MetroFramework.Controls.MetroTabPage metroTabPage2;
        private MetroFramework.Controls.MetroTabPage metroTabPage3;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private MetroFramework.Controls.MetroTextBox textBox_Sts;
        private MetroFramework.Controls.MetroTextBox textBox_Sig;
        private MetroFramework.Controls.MetroTextBox textBox_n_REG;
        private MetroFramework.Controls.MetroTextBox textBox_t_REG;
        private MetroFramework.Controls.MetroTextBox textBox_Check;
        private MetroFramework.Controls.MetroTextBox textBox_Error;
        private MetroFramework.Controls.MetroTextBox textBox_FPD;
        private MetroFramework.Controls.MetroTextBox textBox_CarType;
        private MetroFramework.Controls.MetroTextBox textBox_f_REG;
        private MetroFramework.Controls.MetroTextBox textBox_IHC;
        private MetroFramework.Controls.MetroTextBox textBox_Rate;
        private MetroFramework.Controls.MetroTextBox textBox_P;
        private MetroFramework.Controls.MetroTextBox textBox_T4;
        private MetroFramework.Controls.MetroTextBox textBox_T3;
        private MetroFramework.Controls.MetroTextBox textBox_T2;
        private MetroFramework.Controls.MetroTextBox textBox_T1;
        private MetroFramework.Controls.MetroLabel metroLabel14;
        private MetroFramework.Controls.MetroLabel metroLabel13;
        private MetroFramework.Controls.MetroLabel metroLabel12;
        private MetroFramework.Controls.MetroLabel metroLabel11;
        private MetroFramework.Controls.MetroLabel metroLabel10;
        private MetroFramework.Controls.MetroLabel metroLabel2;
        private MetroFramework.Controls.MetroLabel metroLabel1;
        private MetroFramework.Controls.MetroLabel metroLabel9;
        private MetroFramework.Controls.MetroLabel metroLabel8;
        private MetroFramework.Controls.MetroLabel metroLabel7;
        private MetroFramework.Controls.MetroLabel metroLabel6;
        private MetroFramework.Controls.MetroLabel metroLabel5;
        private MetroFramework.Controls.MetroLabel metroLabel4;
        private MetroFramework.Controls.MetroLabel metroLabel3;
        private MetroFramework.Controls.MetroLabel T2;
        private MetroFramework.Controls.MetroLabel T1;
        private MetroFramework.Controls.MetroTextBox textBox_Valid;
        private MetroFramework.Controls.MetroLabel metroLabel18;
        private MetroFramework.Controls.MetroTextBox textBox_Speed;
        private MetroFramework.Controls.MetroTextBox textBox_t_OFFSCR;
        private MetroFramework.Controls.MetroTextBox textBox_RegenStartKey;
        private MetroFramework.Controls.MetroTextBox textBox_n_SCR;
        private MetroFramework.Controls.MetroTextBox textBox_t_SCR;
        private MetroFramework.Controls.MetroTextBox textBox_f_SCR;
        private MetroFramework.Controls.MetroTextBox textBox_baseP1;
        private MetroFramework.Controls.MetroLabel metroLabel19;
        private MetroFramework.Controls.MetroLabel metroLabel20;
        private MetroFramework.Controls.MetroLabel metroLabel21;
        private MetroFramework.Controls.MetroLabel metroLabel22;
        private MetroFramework.Controls.MetroLabel metroLabel23;
        private MetroFramework.Controls.MetroLabel metroLabel24;
        private MetroFramework.Controls.MetroLabel metroLabel25;
        private MetroFramework.Controls.MetroTextBox textBox_DrvTime;
        private MetroFramework.Controls.MetroTextBox textBox_Lock;
        private MetroFramework.Controls.MetroLabel metroLabel16;
        private MetroFramework.Controls.MetroLabel metroLabel15;
        private MetroFramework.Controls.MetroTextBox textBox_P1_bar;
        private MetroFramework.Controls.MetroTextBox textBox_T3_temp;
        private MetroFramework.Controls.MetroTextBox textBox_Tavg_temp;
        private MetroFramework.Controls.MetroTextBox textBox_T2_temp;
        private MetroFramework.Controls.MetroTextBox textBox_T1_temp;
        private MetroFramework.Controls.MetroTextBox textBox_Version;
        private MetroFramework.Controls.MetroLabel metroLabel26;
        private MetroFramework.Controls.MetroLabel metroLabel27;
        private MetroFramework.Controls.MetroLabel metroLabel28;
        private MetroFramework.Controls.MetroLabel metroLabel29;
        private MetroFramework.Controls.MetroLabel metroLabel30;
        private MetroFramework.Controls.MetroLabel metroLabel31;
        private MetroFramework.Controls.MetroTextBox textBox_H2k;
        private MetroFramework.Controls.MetroTextBox textBox_H1k;
        private MetroFramework.Controls.MetroTextBox textBox_Xc;
        private MetroFramework.Controls.MetroTextBox textBox_curXk;
        private MetroFramework.Controls.MetroLabel metroLabel45;
        private MetroFramework.Controls.MetroLabel metroLabel46;
        private MetroFramework.Controls.MetroLabel metroLabel47;
        private MetroFramework.Controls.MetroLabel metroLabel48;
        private MetroFramework.Controls.MetroTextBox textBox_TotalDosingRate;
        private MetroFramework.Controls.MetroTextBox textBox_DosingRatehouer;
        private MetroFramework.Controls.MetroTextBox textBox_ScrFlag;
        private MetroFramework.Controls.MetroTextBox textBox_SystemSignal;
        private MetroFramework.Controls.MetroTextBox textBox_SystemCheck;
        private MetroFramework.Controls.MetroTextBox textBox_SystemError;
        private MetroFramework.Controls.MetroLabel metroLabel49;
        private MetroFramework.Controls.MetroLabel metroLabel50;
        private MetroFramework.Controls.MetroLabel metroLabel51;
        private MetroFramework.Controls.MetroLabel metroLabel52;
        private MetroFramework.Controls.MetroLabel metroLabel53;
        private MetroFramework.Controls.MetroLabel metroLabel54;
        private MetroFramework.Controls.MetroTextBox textBox_NoxReduction;
        private MetroFramework.Controls.MetroTextBox textBox_UreaQuality;
        private MetroFramework.Controls.MetroTextBox textBox_SCRMode;
        private MetroFramework.Controls.MetroTextBox textBox_StatusAlpha;
        private MetroFramework.Controls.MetroLabel metroLabel35;
        private MetroFramework.Controls.MetroLabel metroLabel36;
        private MetroFramework.Controls.MetroLabel metroLabel37;
        private MetroFramework.Controls.MetroLabel metroLabel38;
        private MetroFramework.Controls.MetroTextBox textBox_Map_Y;
        private MetroFramework.Controls.MetroTextBox textBox_Map_X;
        private MetroFramework.Controls.MetroTextBox textBox_BattVoltage;
        private MetroFramework.Controls.MetroTextBox textBox_TankLevelP;
        private MetroFramework.Controls.MetroTextBox textBox_TankTemp;
        private MetroFramework.Controls.MetroTextBox textBox_Maf_temp;
        private MetroFramework.Controls.MetroLabel metroLabel39;
        private MetroFramework.Controls.MetroLabel metroLabel40;
        private MetroFramework.Controls.MetroLabel metroLabel41;
        private MetroFramework.Controls.MetroLabel metroLabel42;
        private MetroFramework.Controls.MetroLabel metroLabel43;
        private MetroFramework.Controls.MetroLabel metroLabel44;
        private MetroFramework.Controls.MetroTextBox textBox_MafKg_H;
        private MetroFramework.Controls.MetroTextBox textBox_DosingDuty;
        private MetroFramework.Controls.MetroTextBox textBox_Noxppm2;
        private MetroFramework.Controls.MetroTextBox textBox_Noxppm1;
        private MetroFramework.Controls.MetroLabel metroLabel17;
        private MetroFramework.Controls.MetroLabel metroLabel32;
        private MetroFramework.Controls.MetroLabel metroLabel33;
        private MetroFramework.Controls.MetroLabel metroLabel34;
        private MetroFramework.Controls.MetroTabPage metroTabPage4;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private MetroFramework.Controls.MetroPanel metroPanel1;
        private MetroFramework.Controls.MetroComboBox comboBox_port1;
        private System.Windows.Forms.Label label_port;
        private System.Windows.Forms.Button button_connect;
        private System.Windows.Forms.Button button_disconnect;
        private MetroFramework.Controls.MetroTabPage metroTabPage5;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox textBox_Size;
        private System.Windows.Forms.Label label_status;
        private System.Windows.Forms.Button button_send;
        private System.Windows.Forms.Label label_send;
        private System.Windows.Forms.TextBox textBox_send;
    }
}

