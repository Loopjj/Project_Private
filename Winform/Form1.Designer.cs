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
            this.comboBox_port = new System.Windows.Forms.ComboBox();
            this.button_connect = new System.Windows.Forms.Button();
            this.button_disconnect = new System.Windows.Forms.Button();
            this.textBox_send = new System.Windows.Forms.TextBox();
            this.label_send = new System.Windows.Forms.Label();
            this.label_port = new System.Windows.Forms.Label();
            this.button_send = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.label_status = new System.Windows.Forms.Label();
            this.textBox_Size = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.metroTabControl1 = new MetroFramework.Controls.MetroTabControl();
            this.metroTabPage1 = new MetroFramework.Controls.MetroTabPage();
            this.textBox_Sts = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.textBox_Sig = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.textBox_n_REG = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.textBox_t_REG = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.textBox_Check = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.textBox_Error = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBox_IHC = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox_Rate = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_P = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_T4 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox_T3 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_T2 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBox_T1 = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox_f_REG = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBox_CarType = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox_FPD = new System.Windows.Forms.TextBox();
            this.metroTabPage2 = new System.Windows.Forms.TabPage();
            this.textBox_NOx2 = new System.Windows.Forms.TextBox();
            this.label24 = new System.Windows.Forms.Label();
            this.textBox_BV = new System.Windows.Forms.TextBox();
            this.label23 = new System.Windows.Forms.Label();
            this.textBox_NOx1 = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.metroTabPage3 = new System.Windows.Forms.TabPage();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label20 = new System.Windows.Forms.Label();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.metroTabControl1.SuspendLayout();
            this.metroTabPage1.SuspendLayout();
            this.metroTabPage2.SuspendLayout();
            this.metroTabPage3.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboBox_port
            // 
            this.comboBox_port.FormattingEnabled = true;
            this.comboBox_port.Location = new System.Drawing.Point(14, 107);
            this.comboBox_port.Name = "comboBox_port";
            this.comboBox_port.Size = new System.Drawing.Size(205, 20);
            this.comboBox_port.TabIndex = 0;
            // 
            // button_connect
            // 
            this.button_connect.Location = new System.Drawing.Point(13, 170);
            this.button_connect.Name = "button_connect";
            this.button_connect.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.button_connect.Size = new System.Drawing.Size(97, 44);
            this.button_connect.TabIndex = 1;
            this.button_connect.Text = "연결하기";
            this.button_connect.UseVisualStyleBackColor = true;
            this.button_connect.Click += new System.EventHandler(this.Button_connect_Click);
            // 
            // button_disconnect
            // 
            this.button_disconnect.Location = new System.Drawing.Point(122, 170);
            this.button_disconnect.Name = "button_disconnect";
            this.button_disconnect.Size = new System.Drawing.Size(97, 44);
            this.button_disconnect.TabIndex = 1;
            this.button_disconnect.Text = "연결끊기";
            this.button_disconnect.UseVisualStyleBackColor = true;
            // 
            // textBox_send
            // 
            this.textBox_send.Location = new System.Drawing.Point(8, 281);
            this.textBox_send.Name = "textBox_send";
            this.textBox_send.Size = new System.Drawing.Size(243, 21);
            this.textBox_send.TabIndex = 2;
            // 
            // label_send
            // 
            this.label_send.AutoSize = true;
            this.label_send.Location = new System.Drawing.Point(8, 263);
            this.label_send.Name = "label_send";
            this.label_send.Size = new System.Drawing.Size(29, 12);
            this.label_send.TabIndex = 4;
            this.label_send.Text = "송신";
            // 
            // label_port
            // 
            this.label_port.AutoSize = true;
            this.label_port.Location = new System.Drawing.Point(20, 73);
            this.label_port.Name = "label_port";
            this.label_port.Size = new System.Drawing.Size(90, 12);
            this.label_port.TabIndex = 5;
            this.label_port.Text = "COM 포트 설정";
            // 
            // button_send
            // 
            this.button_send.Location = new System.Drawing.Point(275, 279);
            this.button_send.Name = "button_send";
            this.button_send.Size = new System.Drawing.Size(91, 23);
            this.button_send.TabIndex = 6;
            this.button_send.Text = "보내기";
            this.button_send.UseVisualStyleBackColor = true;
            // 
            // label_status
            // 
            this.label_status.AutoSize = true;
            this.label_status.Location = new System.Drawing.Point(15, 232);
            this.label_status.Name = "label_status";
            this.label_status.Size = new System.Drawing.Size(53, 12);
            this.label_status.TabIndex = 7;
            this.label_status.Text = "연결상태";
            // 
            // textBox_Size
            // 
            this.textBox_Size.Location = new System.Drawing.Point(322, 52);
            this.textBox_Size.Name = "textBox_Size";
            this.textBox_Size.Size = new System.Drawing.Size(66, 21);
            this.textBox_Size.TabIndex = 38;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Consolas", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(220, 52);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(96, 18);
            this.label15.TabIndex = 39;
            this.label15.Text = "Data Length";
            // 
            // metroTabControl1
            // 
            this.metroTabControl1.Controls.Add(this.metroTabPage1);
            this.metroTabControl1.Controls.Add(this.metroTabPage2);
            this.metroTabControl1.Controls.Add(this.metroTabPage3);
            this.metroTabControl1.Location = new System.Drawing.Point(405, 38);
            this.metroTabControl1.Name = "metroTabControl1";
            this.metroTabControl1.SelectedIndex = 1;
            this.metroTabControl1.Size = new System.Drawing.Size(1013, 544);
            this.metroTabControl1.TabIndex = 78;
            this.metroTabControl1.UseSelectable = true;
            // 
            // metroTabPage1
            // 
            this.metroTabPage1.Controls.Add(this.textBox_Sts);
            this.metroTabPage1.Controls.Add(this.label17);
            this.metroTabPage1.Controls.Add(this.textBox_Sig);
            this.metroTabPage1.Controls.Add(this.label16);
            this.metroTabPage1.Controls.Add(this.textBox_n_REG);
            this.metroTabPage1.Controls.Add(this.label14);
            this.metroTabPage1.Controls.Add(this.textBox_t_REG);
            this.metroTabPage1.Controls.Add(this.label13);
            this.metroTabPage1.Controls.Add(this.textBox_Check);
            this.metroTabPage1.Controls.Add(this.label12);
            this.metroTabPage1.Controls.Add(this.textBox_Error);
            this.metroTabPage1.Controls.Add(this.label10);
            this.metroTabPage1.Controls.Add(this.textBox_IHC);
            this.metroTabPage1.Controls.Add(this.label6);
            this.metroTabPage1.Controls.Add(this.textBox_Rate);
            this.metroTabPage1.Controls.Add(this.label5);
            this.metroTabPage1.Controls.Add(this.textBox_P);
            this.metroTabPage1.Controls.Add(this.label4);
            this.metroTabPage1.Controls.Add(this.textBox_T4);
            this.metroTabPage1.Controls.Add(this.label3);
            this.metroTabPage1.Controls.Add(this.textBox_T3);
            this.metroTabPage1.Controls.Add(this.label2);
            this.metroTabPage1.Controls.Add(this.textBox_T2);
            this.metroTabPage1.Controls.Add(this.label1);
            this.metroTabPage1.Controls.Add(this.textBox_T1);
            this.metroTabPage1.Controls.Add(this.label11);
            this.metroTabPage1.Controls.Add(this.label7);
            this.metroTabPage1.Controls.Add(this.textBox_f_REG);
            this.metroTabPage1.Controls.Add(this.label8);
            this.metroTabPage1.Controls.Add(this.textBox_CarType);
            this.metroTabPage1.Controls.Add(this.label9);
            this.metroTabPage1.Controls.Add(this.textBox_FPD);
            this.metroTabPage1.HorizontalScrollbarBarColor = true;
            this.metroTabPage1.HorizontalScrollbarHighlightOnWheel = false;
            this.metroTabPage1.HorizontalScrollbarSize = 10;
            this.metroTabPage1.Location = new System.Drawing.Point(4, 38);
            this.metroTabPage1.Name = "metroTabPage1";
            this.metroTabPage1.Size = new System.Drawing.Size(1005, 502);
            this.metroTabPage1.TabIndex = 0;
            this.metroTabPage1.Text = "DPF";
            this.metroTabPage1.VerticalScrollbarBarColor = true;
            this.metroTabPage1.VerticalScrollbarHighlightOnWheel = false;
            this.metroTabPage1.VerticalScrollbarSize = 10;
            // 
            // textBox_Sts
            // 
            this.textBox_Sts.Location = new System.Drawing.Point(330, 381);
            this.textBox_Sts.Name = "textBox_Sts";
            this.textBox_Sts.Size = new System.Drawing.Size(120, 21);
            this.textBox_Sts.TabIndex = 60;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label17.Location = new System.Drawing.Point(235, 370);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(104, 32);
            this.label17.TabIndex = 61;
            this.label17.Text = "STATUS";
            // 
            // textBox_Sig
            // 
            this.textBox_Sig.Location = new System.Drawing.Point(330, 325);
            this.textBox_Sig.Name = "textBox_Sig";
            this.textBox_Sig.Size = new System.Drawing.Size(120, 21);
            this.textBox_Sig.TabIndex = 58;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label16.Location = new System.Drawing.Point(235, 314);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(104, 32);
            this.label16.TabIndex = 59;
            this.label16.Text = "SIGNAL";
            // 
            // textBox_n_REG
            // 
            this.textBox_n_REG.Location = new System.Drawing.Point(315, 280);
            this.textBox_n_REG.Name = "textBox_n_REG";
            this.textBox_n_REG.Size = new System.Drawing.Size(120, 21);
            this.textBox_n_REG.TabIndex = 56;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.Location = new System.Drawing.Point(220, 269);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(89, 32);
            this.label14.TabIndex = 57;
            this.label14.Text = "N_REG";
            // 
            // textBox_t_REG
            // 
            this.textBox_t_REG.Location = new System.Drawing.Point(315, 222);
            this.textBox_t_REG.Name = "textBox_t_REG";
            this.textBox_t_REG.Size = new System.Drawing.Size(120, 21);
            this.textBox_t_REG.TabIndex = 54;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(220, 211);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(89, 32);
            this.label13.TabIndex = 55;
            this.label13.Text = "T_REG";
            // 
            // textBox_Check
            // 
            this.textBox_Check.Location = new System.Drawing.Point(303, 179);
            this.textBox_Check.Name = "textBox_Check";
            this.textBox_Check.Size = new System.Drawing.Size(120, 21);
            this.textBox_Check.TabIndex = 52;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(208, 168);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(89, 32);
            this.label12.TabIndex = 53;
            this.label12.Text = "CHECK";
            // 
            // textBox_Error
            // 
            this.textBox_Error.Location = new System.Drawing.Point(303, 126);
            this.textBox_Error.Name = "textBox_Error";
            this.textBox_Error.Size = new System.Drawing.Size(120, 21);
            this.textBox_Error.TabIndex = 50;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(208, 116);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(89, 32);
            this.label10.TabIndex = 51;
            this.label10.Text = "ERROR";
            // 
            // textBox_IHC
            // 
            this.textBox_IHC.Location = new System.Drawing.Point(87, 359);
            this.textBox_IHC.Name = "textBox_IHC";
            this.textBox_IHC.Size = new System.Drawing.Size(120, 21);
            this.textBox_IHC.TabIndex = 48;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(22, 348);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(59, 32);
            this.label6.TabIndex = 49;
            this.label6.Text = "IHC";
            // 
            // textBox_Rate
            // 
            this.textBox_Rate.Location = new System.Drawing.Point(96, 300);
            this.textBox_Rate.Name = "textBox_Rate";
            this.textBox_Rate.Size = new System.Drawing.Size(120, 21);
            this.textBox_Rate.TabIndex = 46;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(22, 289);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(74, 32);
            this.label5.TabIndex = 47;
            this.label5.Text = "Rate";
            // 
            // textBox_P
            // 
            this.textBox_P.Location = new System.Drawing.Point(72, 244);
            this.textBox_P.Name = "textBox_P";
            this.textBox_P.Size = new System.Drawing.Size(120, 21);
            this.textBox_P.TabIndex = 44;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(22, 233);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 32);
            this.label4.TabIndex = 45;
            this.label4.Text = "P";
            // 
            // textBox_T4
            // 
            this.textBox_T4.Location = new System.Drawing.Point(72, 186);
            this.textBox_T4.Name = "textBox_T4";
            this.textBox_T4.Size = new System.Drawing.Size(120, 21);
            this.textBox_T4.TabIndex = 42;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(22, 175);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 32);
            this.label3.TabIndex = 43;
            this.label3.Text = "T4";
            // 
            // textBox_T3
            // 
            this.textBox_T3.Location = new System.Drawing.Point(72, 128);
            this.textBox_T3.Name = "textBox_T3";
            this.textBox_T3.Size = new System.Drawing.Size(120, 21);
            this.textBox_T3.TabIndex = 40;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(22, 117);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 32);
            this.label2.TabIndex = 41;
            this.label2.Text = "T3";
            // 
            // textBox_T2
            // 
            this.textBox_T2.Location = new System.Drawing.Point(71, 82);
            this.textBox_T2.Name = "textBox_T2";
            this.textBox_T2.Size = new System.Drawing.Size(120, 21);
            this.textBox_T2.TabIndex = 32;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(21, 71);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 32);
            this.label1.TabIndex = 33;
            this.label1.Text = "T2";
            // 
            // textBox_T1
            // 
            this.textBox_T1.Location = new System.Drawing.Point(71, 30);
            this.textBox_T1.Name = "textBox_T1";
            this.textBox_T1.Size = new System.Drawing.Size(120, 21);
            this.textBox_T1.TabIndex = 30;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(21, 19);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(44, 32);
            this.label11.TabIndex = 31;
            this.label11.Text = "T1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(22, 390);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(89, 32);
            this.label7.TabIndex = 41;
            this.label7.Text = "F_REG";
            // 
            // textBox_f_REG
            // 
            this.textBox_f_REG.Location = new System.Drawing.Point(105, 400);
            this.textBox_f_REG.Name = "textBox_f_REG";
            this.textBox_f_REG.Size = new System.Drawing.Size(120, 21);
            this.textBox_f_REG.TabIndex = 40;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(208, 14);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(59, 32);
            this.label8.TabIndex = 43;
            this.label8.Text = "CAR";
            // 
            // textBox_CarType
            // 
            this.textBox_CarType.Location = new System.Drawing.Point(282, 25);
            this.textBox_CarType.Name = "textBox_CarType";
            this.textBox_CarType.Size = new System.Drawing.Size(120, 21);
            this.textBox_CarType.TabIndex = 42;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(208, 68);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(59, 32);
            this.label9.TabIndex = 45;
            this.label9.Text = "FPD";
            // 
            // textBox_FPD
            // 
            this.textBox_FPD.Location = new System.Drawing.Point(273, 79);
            this.textBox_FPD.Name = "textBox_FPD";
            this.textBox_FPD.Size = new System.Drawing.Size(120, 21);
            this.textBox_FPD.TabIndex = 44;
            // 
            // metroTabPage2
            // 
            this.metroTabPage2.AllowDrop = true;
            this.metroTabPage2.Controls.Add(this.textBox3);
            this.metroTabPage2.Controls.Add(this.textBox_NOx2);
            this.metroTabPage2.Controls.Add(this.label24);
            this.metroTabPage2.Controls.Add(this.textBox_BV);
            this.metroTabPage2.Controls.Add(this.label23);
            this.metroTabPage2.Controls.Add(this.textBox_NOx1);
            this.metroTabPage2.Controls.Add(this.label18);
            this.metroTabPage2.Location = new System.Drawing.Point(4, 38);
            this.metroTabPage2.Name = "metroTabPage2";
            this.metroTabPage2.Size = new System.Drawing.Size(1005, 502);
            this.metroTabPage2.TabIndex = 1;
            this.metroTabPage2.Text = "SCR";
            // 
            // textBox_NOx2
            // 
            this.textBox_NOx2.Location = new System.Drawing.Point(114, 104);
            this.textBox_NOx2.Name = "textBox_NOx2";
            this.textBox_NOx2.Size = new System.Drawing.Size(120, 21);
            this.textBox_NOx2.TabIndex = 42;
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label24.Location = new System.Drawing.Point(12, 93);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(104, 32);
            this.label24.TabIndex = 43;
            this.label24.Text = "NOx Dn";
            // 
            // textBox_BV
            // 
            this.textBox_BV.Location = new System.Drawing.Point(61, 149);
            this.textBox_BV.Name = "textBox_BV";
            this.textBox_BV.Size = new System.Drawing.Size(120, 21);
            this.textBox_BV.TabIndex = 40;
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label23.Location = new System.Drawing.Point(11, 138);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(44, 32);
            this.label23.TabIndex = 41;
            this.label23.Text = "BV";
            // 
            // textBox_NOx1
            // 
            this.textBox_NOx1.Location = new System.Drawing.Point(116, 56);
            this.textBox_NOx1.Name = "textBox_NOx1";
            this.textBox_NOx1.Size = new System.Drawing.Size(120, 21);
            this.textBox_NOx1.TabIndex = 34;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label18.Location = new System.Drawing.Point(9, 49);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(104, 32);
            this.label18.TabIndex = 35;
            this.label18.Text = "NOx UP";
            // 
            // metroTabPage3
            // 
            this.metroTabPage3.Controls.Add(this.richTextBox1);
            this.metroTabPage3.Location = new System.Drawing.Point(4, 38);
            this.metroTabPage3.Name = "metroTabPage3";
            this.metroTabPage3.Size = new System.Drawing.Size(1005, 502);
            this.metroTabPage3.TabIndex = 2;
            this.metroTabPage3.Text = "Low Data";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(19, 68);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(541, 344);
            this.richTextBox1.TabIndex = 28;
            this.richTextBox1.Text = "";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(87, 350);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(257, 181);
            this.tabControl1.TabIndex = 79;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.textBox1);
            this.tabPage1.Controls.Add(this.label19);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(249, 155);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(56, 29);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(120, 21);
            this.textBox1.TabIndex = 32;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label19.Location = new System.Drawing.Point(6, 18);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(44, 32);
            this.label19.TabIndex = 33;
            this.label19.Text = "T1";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.textBox2);
            this.tabPage2.Controls.Add(this.label20);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(249, 155);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(90, 63);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(120, 21);
            this.textBox2.TabIndex = 32;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Font = new System.Drawing.Font("Consolas", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label20.Location = new System.Drawing.Point(40, 52);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(44, 32);
            this.label20.TabIndex = 33;
            this.label20.Text = "T1";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(442, 241);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(120, 21);
            this.textBox3.TabIndex = 44;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1469, 713);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.metroTabControl1);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.textBox_Size);
            this.Controls.Add(this.label_status);
            this.Controls.Add(this.button_send);
            this.Controls.Add(this.label_port);
            this.Controls.Add(this.label_send);
            this.Controls.Add(this.textBox_send);
            this.Controls.Add(this.button_disconnect);
            this.Controls.Add(this.button_connect);
            this.Controls.Add(this.comboBox_port);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "SDPF Viewer V1.0";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.metroTabControl1.ResumeLayout(false);
            this.metroTabPage1.ResumeLayout(false);
            this.metroTabPage1.PerformLayout();
            this.metroTabPage2.ResumeLayout(false);
            this.metroTabPage2.PerformLayout();
            this.metroTabPage3.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox_port;
        private System.Windows.Forms.Button button_connect;
        private System.Windows.Forms.Button button_disconnect;
        private System.Windows.Forms.TextBox textBox_send;
        private System.Windows.Forms.Label label_send;
        private System.Windows.Forms.Label label_port;
        private System.Windows.Forms.Button button_send;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Label label_status;
        private System.Windows.Forms.TextBox textBox_Size;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox textBox_FPD;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBox_CarType;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBox_f_REG;
        private System.Windows.Forms.Label label7;
        private MetroFramework.Controls.MetroTabControl metroTabControl1;
        private MetroFramework.Controls.MetroTabPage metroTabPage1;
        private System.Windows.Forms.TabPage metroTabPage2;
        private System.Windows.Forms.TextBox textBox_T2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_T1;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TabPage metroTabPage3;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.TextBox textBox_IHC;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_Rate;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_P;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_T4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox_T3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox_Sts;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox textBox_Sig;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox textBox_n_REG;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBox_t_REG;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textBox_Check;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textBox_Error;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.TextBox textBox_NOx2;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.TextBox textBox_BV;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.TextBox textBox_NOx1;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox textBox3;
    }
}

