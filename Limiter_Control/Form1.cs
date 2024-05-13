using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;  //시리얼통신을 위해 추가해줘야 함
using System.Security.Cryptography.X509Certificates; //추가
using System.Runtime.InteropServices; //추가 
using System.Threading;
using Excel = Microsoft.Office.Interop.Excel; // 엑셀 파일 생성을 위함 
using System.Timers;
using System.IO;
using static System.Collections.Specialized.BitVector32;
using MetroFramework.Components;
using System.Net.Sockets;
using MetroFramework.Design;
using MetroFramework;

namespace Serial_Communication
{
    public partial class Form1 : MetroFramework.Forms.MetroForm
    {
        String SavePath;
        string settingsFilePath = Path.Combine(Application.StartupPath, SettingsFileName);
        private const string SettingsFileName = "Setting.ini";
        byte m_ucRxDataCnt, m_ucRxCommand, m_ucRxID, ucLength, rdata;
        byte[] m_ucRxData = new byte[1024];
        byte m_ucRxStep, m_ucSyncCnt, m_ucLenCnt;
        short[] m_uiTVal = new short[4];
        short[] TVal = new short[4];
        short[] PVal = new short[2];
        int m_ucRxLength;
        double NOxRealReduce;
        private bool buttonClicked = false;
        private READ_VALUE ReadValue = new READ_VALUE();
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct READ_VALUE
        {
            public ushort T1;
            public ushort T2;
            public ushort T3;
            public ushort T4;
            public ushort P;
            public ushort Rate;
            public ushort IHC;
            public ushort f_REG;
            public byte CarType;
            public byte FPD;
            public byte Error;
            public byte Check;
            public ushort t_REG;
            public ushort n_REG;
            public byte Sig;
            public byte Sts;
            public byte Lock;
            public uint DrvTime;
            public ushort baseP1;
            public ushort f_SCR;
            public ushort t_SCR;
            public ushort n_SCR;
            public byte RegenStartKey;
            public ushort t_OFFSCR;
            public byte Speed;
            public byte Valid;
        }
        stSysValue ScrValue = new stSysValue();
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct stSysValue
        {
            public ushort Version;
            public short T1_temp;
            public short T2_temp;
            public short Tavg_temp;
            public short T3_temp;
            public ushort P1_bar;
            // [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public short Noxppm1;
            public short Noxppm2;
            public ushort DosingDuty;
            public ushort MafKg_H;
            public short Maf_temp;
            public short TankTemp;
            public ushort TankLevelP;
            public ushort BattVoltage;
            public byte Map_X;
            public byte Map_Y;
            public byte StatusAlpha;
            public byte SCRMode;
            public ushort UreaQuality;
            public ushort NoxReduction;
            public byte SystemError;
            public short SystemCheck;
            public short SystemSignal;
            public short ScrFlag;
            public double DosingRatehouer;
            public double TotalDosingRate;
            public double curXk;
            public double Xc;
            public double H1k;
            public double H2k;
            public double Yk;
            public double Kl;
            public double Kp;
            public double curXh;
            public double Vk;
            public double Gamma;
            public double Formula1;
            public double Formula2;
            public double Formula3;
            public double NH3Up;
            public double NH3Dn;
            public ushort PM_Senser1;
            public ushort PM_Senser2;
            public ushort PM_Senser3;
            public ushort PM_Senser4;
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public short NoxO2_1;
            public short NoxO2_2;
        }
        tsSecData SecData = new tsSecData();
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct tsSecData
        {
            public uint TM;             // 4 bytes
            public uint LT;                 // 4 bytes
            public uint LN;                 // 4 bytes
            public ushort basePress;        // 2 bytes
            public ushort CO;               // 2 bytes
            public ushort FW_Ver;           // 2 bytes
            public byte Speed;              // 1 byte
            public byte Error;             // 2 bytes
            public short SysCode;       // 2 bytes
            public byte User;               // 1 byte
            public ushort DpfTemp;          // 2 bytes
            public ushort Chk;             // 2 bytes
            public short NOxIn;            // 2 bytes
            public short NOxOut;           // 2 bytes
            public short TempIn;           // 2 bytes
            public short TempOut;          // 2 bytes
            public uint DosingRate;        // 4 bytes
            public ushort UreaLevel;       // 2 bytes
            public ushort MAF;             // 2 bytes
            public byte State;             // 1 byte
            public byte Concentration;     // 1 byte
            public byte SystemError_1;     // 8 bytes
            public byte SystemError_2;     // 8 bytes
            public byte SystemError_3;     // 8 bytes
            public byte SystemError_4;     // 8 bytes
            public byte SystemError_5;     // 8 bytes
            public byte SystemError_6;     // 8 bytes
            public byte SystemError_7;     // 8 bytes
            public byte SystemError_8;     // 8 bytes
            public byte DoserError;      // 8 bytes
            public byte DoserError_1;      // 8 bytes
            public byte DoserError_2;      // 8 bytes
            public byte DoserError_3;      // 8 bytes
            public byte DoserError_4;      // 8 bytes
            public byte DoserError_5;      // 8 bytes
            public byte DoserError_6;      // 8 bytes
            public byte DoserError_7;      // 8 bytes
            public byte DoserError_8;      // 8 bytes
            public ushort Vbat;            // 2 bytes
            public byte UreaAccFlag;       // 1 byte
        }

        st_Setting Setting = new st_Setting();
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct st_Setting
        {
            public uint Identity;
            public byte TM_1;
            public byte TM_2;
            public byte TM_3;
            public byte TM_4;
            public byte TM_5;
            public byte TM_6;
            public byte TM_7;

            public ulong Drv_Name_1;
            public ushort Drv_Name_2;

            public ulong Drv_Code_1;
            public ulong Drv_Code_2;
            public ushort Drv_Code_3;

            public ulong Car_Reg_No_1;
            public uint Car_Reg_No_2;

            public ulong Car_No_1;
            public ulong Car_No_2;
            public byte Car_No_3;

            public ulong Company_Name_1;
            public uint Company_Name_2;

            public ulong Company_Reg_No_1;
            public ushort Company_Reg_No_2;

            public ulong Form_Approval_No_1;
            public ushort Form_Approval_No_2;

            public ulong Serial_No_1;
            public uint Serial_No_2;
            public ushort Serial_No_3;

            public ulong Model_No_1;
            public ulong Model_No_2;
            public uint Model_No_3;

            public ushort K_Factor;
            public ushort RPM_Factor;

            public byte FW_Type;
            public byte FW_Ver_1;
            public byte FW_Ver_2;
            public byte FW_Ver_3;
            public byte FW_Ver_4;
            public byte FW_Ver_5;
            public byte FW_Ver_6;
            public uint FW_Date;
            public ushort t_DelayP;
            public ushort t_Buzzer;
            public ushort P1L_Set;
            public ushort P1H_Set;
            public ushort Prange;
            public ushort Prangehigh;
            public ushort Prangelow;
            public ushort P1CalHigh;
            public ushort P1CalLow;
            public ushort T1Offset;
            public ushort TLL_Set;
            public ushort TLH_Set;
            public ushort t_DelayTL;
            public ushort LED;
            public ushort IdlePress;
            public byte UserNumber;
            public byte CanBPS;
            public byte PAlarm1;
            public byte PAlarm2;
            public byte PAlarm3;

            public byte UreaLvlHi;
            public byte UreaLvlLo;
            public byte UreaLvlAL;
            public byte UreaConHi;
            public byte UreaConLo;
            public byte UreaAlHr1;
            public byte UreaAlHr2;
            public byte UreaBuzInt;

            public byte UreaAlEn;
            public byte FileOutput;
            public byte PinCode1;
            public byte PinCode2;
            public byte PinCode3;
            public byte PinCode4;
            public byte PinCode5;
            public byte PinCode6;
            public byte ExtModemMode;
            public byte ExtModemTelecom;
            public byte ExtModemModel;

            public uint Lat;
            public uint Lon;

            public ushort Azi;
            public uint UpdateCount;
            public byte FactoryNumber;
            public byte NandType;
            public ushort CRC16;

        }

        public Form1()
        {
            InitializeComponent();
            CheckAndCreateSettingsFile(); //최초 실행시 Setting.INI파일의 유무를 확인한다. 
            comboBox_Mode.SelectedItem = "AUTO";
            this.MinimumSize = new Size(1100, 720);
            this.MaximumSize = new Size(1100, 720);
        }

        private void Form1_Load(object sender, EventArgs e)  //폼이 로드되면
        {
            comboBox_port1.DataSource = SerialPort.GetPortNames(); //연결 가능한 시리얼포트 이름을 콤보박스에 가져오기
        }
        private void CheckAndCreateSettingsFile()
        {


            if (!File.Exists(settingsFilePath))
            {
                // 설정 파일이 없는 경우, 설정 파일 생성
                try
                {
                    File.Create(settingsFilePath).Close(); // 파일을 생성하고 즉시 닫습니다.
                    // 이후에 필요한 설정 데이터를 파일에 저장할 수도 있습니다.
                }
                catch (Exception ex)
                {
                    // 파일 생성 중 오류가 발생한 경우 예외 처리를 수행합니다.
                    // 예를 들어 로깅하거나 알림을 보여주는 등의 작업을 수행할 수 있습니다.
                    Console.WriteLine("Setting.ini 파일 생성 중 오류 발생: " + ex.Message);
                }
            }
        }
        public void button1_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();

            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                string SaveDataPath = folderBrowserDialog.SelectedPath;
                //MessageBox.Show("선택된 폴더 경로: " + SaveDataPath);           
                CheckAndCreateSettingsFile();
                WriteToIniFile("Setting", "Path", SaveDataPath);
                MetroFramework.MetroMessageBox.Show(this, "\n데이터 저장경로 설정완료.\n\n데이터 저장경로 수정시 Setting.INI 파일을 삭제하고 경로를 다시 설정하세요.", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);

            }
        }
        private Dictionary<string, string> ReadFromIniFile(string filePath)
        {
            Dictionary<string, string> data = new Dictionary<string, string>();
            string[] lines = File.ReadAllLines(filePath);

            foreach (string line in lines)
            {
                string[] parts = line.Split('=');
                if (parts.Length == 2)
                {
                    string key = parts[0].Trim();
                    string value = parts[1].Trim();
                    data[key] = value;
                }
            }

            return data;
        }
        private void WriteToIniFile(string section, string key, string value)
        {
            try
            {
                // 파일에 쓸 내용을 준비한다.
                string content = $"[{section}]\n{key}={value}\n";

                // 설정 파일에 내용을 추가 모드로 연다. 
                using (StreamWriter writer = new StreamWriter(settingsFilePath, true))
                {
                    // 파일에 내용을 쓴다.
                    writer.Write(content);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"오류 발생: {ex.Message}");
            }
        }
        private string ReadFromIniFile(string section, string key)
        {
            try
            {
                // 설정 파일에서 데이터를 읽어오기 위해 StreamReader를 사용합니다.
                using (StreamReader reader = new StreamReader(settingsFilePath))
                {
                    string line;
                    while ((line = reader.ReadLine()) != null)
                    {
                        // 섹션과 키에 해당하는 줄을 찾습니다.
                        if (line.StartsWith($"[{section}]"))
                        {
                            while ((line = reader.ReadLine()) != null)
                            {
                                // 해당 키를 찾았으면 값을 반환합니다.
                                if (line.StartsWith($"{key}="))
                                {
                                    return line.Substring(key.Length + 1); // 값을 리턴합니다.
                                }

                                // 만약 다음 섹션을 만나면 중지합니다.
                                if (line.StartsWith("["))
                                    break;
                            }
                        }
                    }
                }

                // 해당 섹션 또는 키를 찾지 못했을 경우 null을 반환합니다.
                return null;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"오류 발생: {ex.Message}");
                return null;
            }
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                DateTime now = DateTime.Now; // 현재 날짜와 시간을 가져온다.
                string fileName = now.ToString("yyMMdd") + ".xls";
                string sectionName = "Setting";
                string keyName = "Path";
                string directoryPath = ReadFromIniFile(sectionName, keyName);
                string filePath = Path.Combine(directoryPath, fileName);
                string formattedDate = now.ToString("yy.MM.dd");
                string formattedTime = now.ToString("HH:mm:ss");

                // 파일을 동시에 읽고 쓰기 위해 FileShare.ReadWrite 옵션을 사용하여 파일 열기
                // using (FileStream fileStream = new FileStream(filePath, FileMode.Append, FileAccess.Write, FileShare.ReadWrite))
                // {
                if (File.Exists(filePath))
                {
                    using (FileStream fileStream = new FileStream(filePath, FileMode.Append, FileAccess.Write, FileShare.ReadWrite))
                    using (StreamWriter writer = new StreamWriter(fileStream))
                    {
                        writer.WriteLine($"{formattedDate}\t{formattedTime}\t{ReadValue.f_REG}\t{ReadValue.t_REG}\t{ReadValue.f_SCR}\t{ReadValue.t_SCR}\t{ReadValue.T1}\t" +
                            $"{ReadValue.T2}\t{ReadValue.T3}\t{ReadValue.T4}\t{ReadValue.P}\t{ReadValue.baseP1}\t{ReadValue.FPD}\t{ReadValue.RegenStartKey}\t{ScrValue.T1_temp}\t" +
                            $"{ScrValue.T2_temp}\t{ScrValue.Tavg_temp}\t{ScrValue.Noxppm1}\t{ScrValue.Noxppm2}\t{ScrValue.NoxO2_1}\t{ScrValue.NoxO2_2}\t{ScrValue.MafKg_H}\t{ScrValue.DosingRatehouer}\t" +
                            $"{ScrValue.TotalDosingRate}\t{ScrValue.BattVoltage / 100}\t{ScrValue.TankLevelP}\t{ReadValue.IHC}\t{(ReadValue.Sig >> 1) & 0x01}\t{(ReadValue.Sig >> 2) & 0x01}\t{ScrValue.P1_bar}\t{ScrValue.StatusAlpha}\t" +
                            $"{ReadValue.Error}\t{ReadValue.Check}\t{ScrValue.SystemError}\t{ScrValue.SystemCheck}\t{ScrValue.SystemCheck}\t{ScrValue.SystemSignal}\t{ReadValue.DrvTime / 3600}\t{(ReadValue.Sig >> 5) & 0x03}\t" +
                            $"{ScrValue.PM_Senser1}\t{ScrValue.PM_Senser2}\t{ScrValue.PM_Senser3}\t{ScrValue.PM_Senser4}\t{ReadValue.RegenStartKey}\t{0}\t{ScrValue.UreaQuality / 10}{ScrValue.UreaQuality % 10}\t" +
                            $"{ScrValue.Formula1}\t{ScrValue.Formula2}\t{ScrValue.Formula3}\t{ScrValue.curXk}\t{ScrValue.Xc}\t{ScrValue.H1k}\t{ScrValue.Yk}\t{ScrValue.Kl}\t" +
                            $"{ScrValue.Kp}\t{ScrValue.Vk}\t{ScrValue.Gamma}\t{ScrValue.curXh}\t{ScrValue.StatusAlpha}");
                        //$"value15\tvalue16\tvalue17\tvalue18\tvalue19\tvalue20\tvalue21\tvalue22\tvalue23\tvalue24");
                    }
                    Console.WriteLine($"경로" + filePath);
                    Console.WriteLine($"A 경로에 {fileName} 파일이 존재합니다.");
                }
                else
                {
                    using (FileStream fileStream = new FileStream(filePath, FileMode.Append, FileAccess.Write, FileShare.ReadWrite))
                    using (StreamWriter writer = new StreamWriter(fileStream))
                    {
                        writer.WriteLine("Data\tTime\tStep\tRegTime\tscrSetp\tscrRegTime\tT1\tT2\tT3\tT4\tP1\tBaseP1\tFPD\tReady\t" +
                                         "NOxIn_T\tNOxOut_T\tTavg\tNOxIn\tNOxOut\tO2In\tO2Out\tMAF\tDosingRate\tTotalDosingRate\t" +
                                         "BV\tUreaLevel\tCurrent\tIgniter\tMotor\tSupplyP\tStatusAlpha\t" +
                                         "DpfError\tDpfCheck\tScrError\tScrCheck\tScrSignal\tdrvH\tRegenSts\t" +
                                         "PM_Senser1\tPM_Senser2\tPM_Senser3\tPM_Senser4\tRegenMode\tSpeed\tUreaQuality\t" +
                                         "Fomula1\tFomula2\tFomula3\tcurXk\tXc\tH1k\tH2k\tYk\tKl\tKp\tVk\tGamma\tXh\tAlpha");

                        //writer.WriteLine($"{formattedDate}\t{formattedTime}\tvalue1\tvalue2\tvalue3\tvalue4\tvalue5\tvalue6\tvalue7\tvalue8\tvalue9\tvalue10\tvalue11\tvalue12\tvalue13\tvalue14\tvalue15\tvalue16\tvalue17\tvalue18\tvalue19\tvalue20\tvalue21\tvalue22\tvalue23\tvalue24");
                    }
                    Console.WriteLine($"경로" + filePath);
                    Console.WriteLine($"A 경로에 {fileName} 파일이 존재하지 않습니다.");
                }
                // }
            }
            else
            {
                return;
            }
        }
        private void metroButton3_Click(object sender, EventArgs e)
        {
            //Setting form = new Setting();
            //form.Show();
            foreach (Form frm in Application.OpenForms)// Form 중복실행을 방지한다. 
            {
                if (frm.Name == "Setting")
                {
                    frm.Activate();
                    return;
                }
            }

            Setting sfrm = new Setting();
            sfrm.Show();
        }
        private void Button_connect_Click(object sender, EventArgs e)  //통신 연결하기 버튼
        {
            if (!serialPort1.IsOpen)  //시리얼포트가 열려 있지 않으면
            {
                serialPort1.PortName = comboBox_port1.Text;  //콤보박스의 선택된 COM포트명을 시리얼포트명으로 지정
                serialPort1.BaudRate = 9600;  //보레이트 변경이 필요하면 숫자 변경하기
                serialPort1.DataBits = 8;
                serialPort1.StopBits = StopBits.One;
                serialPort1.Parity = Parity.None;
                serialPort1.DataReceived += new SerialDataReceivedEventHandler(serialPort1_DataReceived); //이것이 꼭 필요하다

                serialPort1.Open();  //시리얼포트 열기
                label_status.Text = "포트가 열렸습니다.";
                comboBox_port1.Enabled = false;  //COM포트설정 콤보박스 비활성화
            }
            else  //시리얼포트가 열려 있으면
            {
                label_status.Text = "포트가 이미 열려 있습니다.";
            }
        }
        private void button_disconnect_click(object sender, EventArgs e)  //통신 연결끊기 버튼
        {
            if (serialPort1.IsOpen)  //시리얼포트가 열려 있으면
            {
                serialPort1.Close();  //시리얼포트 닫기

                label_status.Text = "포트가 닫혔습니다.";
                comboBox_port1.Enabled = true;  //com포트설정 콤보박스 활성화
            }
            else  //시리얼포트가 닫혀 있으면
            {
                label_status.Text = "포트가 이미 닫혀 있습니다.";
            }
        }
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)  //수신 이벤트가 발생하면 이 부분이 실행된다.
        {
            this.Invoke(new EventHandler(MySerialReceived));  //메인 쓰레드와 수신 쓰레드의 충돌 방지를 위해 Invoke 사용. MySerialReceived로 이동하여 추가 작업 실행.

            //byte[] buffer = new byte[1];
            //serialPort1.Read(buffer, 0, 1);
            //// buffer[0]에 1바이트 데이터가 저장됩니다.
            //textBox4.Text = string.Format(buffer[0]);
        }
        private void MySerialReceived(object s, EventArgs e)  //여기에서 수신 데이타를 사용자의 용도에 따라 처리한다.
        {
            try
            {
                int iRecivedSize = serialPort1.BytesToRead;

                if (iRecivedSize != 0) // 수신 데이터가 있으면
                {
                    byte[] buff = new byte[iRecivedSize];
                    try
                    {
                        //textBox3.Text = iRecivedSize.ToString(); ;
                        serialPort1.Read(buff, 0, iRecivedSize);
                        richTextBox1.AppendText(DateTime.Now.ToString("\r\n[HH:mm:ss] [RX] ") + BitConverter.ToString(buff).Replace("-", " "));
                        //debugText.AppendText(Encoding.ASCII.GetString(buff));
                        for (int i = 0; i < buff.Length; i++)
                        {
                            RxData(buff[i]);
                        }
                    }
                    catch (System.Exception ex)
                    {
                        // this.debugText.AppendText(ex.Message); //오류 메시지 출력 
                    }
                }
            }
            catch (System.Exception ex)
            {
                //this.debugText.AppendText(ex.Message);
            }
        }

        private void metroButton_ManualSet_Click(object sender, EventArgs e)
        {
            ushort stmp;
            byte[] sdata = new byte[8];

            if (comboBox_Mode.SelectedIndex < 0)
                comboBox_Mode.SelectedIndex = 0;
            stmp = ushort.Parse(textBox_mTavg.Text);
            sdata[0] = (byte)(stmp & 0xff);
            sdata[1] = (byte)((stmp >> 8) & 0xff);
            stmp = ushort.Parse(textBox_mMaf.Text);
            sdata[2] = (byte)(stmp & 0xff);
            sdata[3] = (byte)((stmp >> 8) & 0xff);
            stmp = ushort.Parse(textBox_mNoxUp.Text);
            sdata[4] = (byte)(stmp & 0xff);
            sdata[5] = (byte)((stmp >> 8) & 0xff);
            stmp = ushort.Parse(textBox_mNoxDn.Text);
            sdata[6] = (byte)(stmp & 0xff);
            sdata[7] = (byte)((stmp >> 8) & 0xff);

            TxData(0xC6, 8, 0x23, 0, sdata);
        }

        private void metroButton_Alpha_Click(object sender, EventArgs e)
        {
            double dtmp;
            ushort stmp;
            byte[] sdata = new byte[8];

            if (comboBox_Mode.SelectedIndex < 0)
                comboBox_Mode.SelectedIndex = 0;

            dtmp = double.Parse(textBox_mAlpha.Text) * 100.0;
            stmp = (ushort)dtmp;
            sdata[0] = (byte)(stmp & 0xff);
            sdata[1] = (byte)((stmp >> 8) & 0xff);
            sdata[2] = 0x00;
            sdata[3] = 0x00;
            sdata[4] = 0x00;
            sdata[5] = 0x00;
            sdata[6] = 0x00;
            sdata[7] = 0x00;

            TxData(0xC6, 8, 0x22, 0, sdata);
        }

        private void metroButton_Inject_Click(object sender, EventArgs e)
        {
            byte[] sdata = new byte[8];
            byte cmd;
            double dtmp;
            long ltmp;

            if (comboBox_Mode.SelectedIndex < 0)
                comboBox_Mode.SelectedIndex = 0;

            if (buttonClicked == false)
            {
                metroButton_VauleSet.Enabled = false;
                metroButton_Inject.Text = "Pump OFF";
                buttonClicked = false;

                sdata[0] = 0x00;
                sdata[1] = 0x00;
                sdata[2] = 0x00;
                sdata[3] = 0x00;
                sdata[4] = 0x00;
                sdata[5] = 0x00;
                sdata[6] = 0x00;
                sdata[7] = 0x00;
            }
            else
            {
                metroButton_VauleSet.Enabled = true;
                if (metroRadioButton_gh.Checked == true)
                    cmd = 0x01;
                else
                    cmd = 0x02;

                metroButton_Inject.Text = "Pump ON";
                buttonClicked = true;
                sdata[0] = cmd;
                sdata[1] = 0x00;
                sdata[2] = 0x00;
                sdata[3] = 0x00;
                dtmp = double.Parse(textBox_mDosing.Text) * 100.0;
                ltmp = (long)dtmp;
                sdata[4] = (byte)(ltmp & 0xff);
                sdata[5] = (byte)((ltmp >> 8) & 0xff);
                sdata[6] = (byte)((ltmp >> 16) & 0xff);
                sdata[7] = (byte)((ltmp >> 24) & 0xff);
            }
                TxData(0xC6, 8, 0x21, 0, sdata);
        }

        private void Read_Click(object sender, EventArgs e) //Parameter Read
        {
            TxCmd(0xc6, 0x70, 0x12);
        }

        private void Default_Click(object sender, EventArgs e)
        {
            UreaLvlHi.Text = 20.ToString();
            UreaLvlLo.Text = 10.ToString();
            UreaLvlAL.Text = 2.ToString();
            UreaConHi.Text = 40.ToString();
            UreaConLo.Text = 26.ToString();
            UreaAlHr1.Text = 10.ToString();
            UreaAlHr2.Text = 20.ToString();
            UreaBuzInt.Text = 10.ToString();
        }

        private void Write_Click(object sender, EventArgs e)
        {
            byte[] sdata = new byte[8];
            sdata[0] = Convert.ToByte(UreaLvlHi.Text);
            sdata[1] = Convert.ToByte(UreaLvlLo.Text);
            sdata[2] = Convert.ToByte(UreaLvlAL.Text);
            sdata[3] = Convert.ToByte(UreaConHi.Text);
            sdata[4] = Convert.ToByte(UreaConLo.Text);
            sdata[5] = Convert.ToByte(UreaAlHr1.Text);
            sdata[6] = Convert.ToByte(UreaAlHr2.Text);
            sdata[7] = Convert.ToByte(UreaBuzInt.Text);

            TxData(0xc6, 0x08, 0xff, 0x11, sdata);
        }

        private void Set0_Click(object sender, EventArgs e)
        {
            UreaLvlHi.Text = 0.ToString();
            UreaLvlLo.Text = 0.ToString();
            UreaLvlAL.Text = 0.ToString();
            UreaConHi.Text = 0.ToString();
            UreaConLo.Text = 0.ToString();
            UreaAlHr1.Text = 0.ToString();
            UreaAlHr2.Text = 0.ToString();
            UreaBuzInt.Text = 0.ToString();
        }

        private void metroButton5_Click(object sender, EventArgs e)
        {
            byte[] sdata = new byte[8];
            sdata[0] = Convert.ToByte(Pedal_ADC_1.Text);
            sdata[1] = Convert.ToByte(Pedal_ADC_2.Text);
            sdata[2] = Convert.ToByte(Pedal_ADC_3.Text);
            sdata[3] = Convert.ToByte(Pedal_ADC_4.Text);
            sdata[4] = Convert.ToByte(Pedal_ADC_5.Text);
            sdata[5] = Convert.ToByte(Pedal_ADC_6.Text);
            sdata[6] = Convert.ToByte(Pedal_ADC_7.Text);
            sdata[7] = Convert.ToByte(Pedal_ADC_8.Text);

            TxData(0xc6, 0x08, 0x48, 0x11, sdata);
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            byte[] sdata = new byte[8];
            if (serialPort1.IsOpen)
            {
                TxData(0xc6, 0x08, 0x48, 0x11, sdata);
            }

        }

        private void metroButton_MODE_Click(object sender, EventArgs e) // Select Mode
        {
            byte[] sdata = new byte[8];
            byte Mode = 0;
            if (comboBox_Mode.SelectedItem != null)
            {
                string selectedMode = comboBox_Mode.SelectedItem.ToString();

                if (selectedMode == "AUTO")
                {
                    Mode = 0;
                }
                else if (selectedMode == "MANUAL")
                {
                    Mode = 1;
                }
                else if (selectedMode == "ALPHA")
                {
                    Mode = 2;
                }
                else if (selectedMode == "DOSING")
                {
                    Mode = 3;
                }
            }
            else
            {
                Mode = 0;  // ComboBox에서 아무 항목도 선택되지 않은 경우에 대한 처리를 원하면 여기에 추가할 수 있습니다.
            }
            //Convert.ToByte(comboBox_Mode.Text);
            sdata[0] = Mode;
            sdata[1] = 0x00;
            sdata[2] = 0x00;
            sdata[3] = 0x00;
            sdata[4] = 0x00;
            sdata[5] = 0x00;
            sdata[6] = 0x00;
            sdata[7] = 0x00;

            TxData(0xC6, 8, 0x24, 0, sdata);
        }

        //string ReceiveData = serialPort1.ReadExisting();  //시리얼 버터에 수신된 데이타를 ReceiveData 읽어오기
        //    richTextBox_received.Text = richTextBox_received.Text + string.Format("0x{0:X2}", ReceiveData);  //int 형식을 string형식으로 변환하여 출력
        //rdata = byte.ReceiveData;
        //RxData(rdata);
        //textBox1.Text = string.Format("0x{0:X2}", ReceiveData);
        //textBox2.Text = "연결상태 양호";
        //textBox3.Text = string.Format("0x{0:X2}", ReceiveData);
        public void RxData(byte rdata)  // Data Protocol 
        {
            byte checksum;

            switch (m_ucRxStep)
            {
                case 0:
                    if (rdata == 0xc6) // Sync
                    {
                        Sync1.Text = rdata.ToString("X");
                        m_ucSyncCnt++;
                        
                    }
                    else
                    {
                        m_ucSyncCnt = 0;
                    }
                    if (m_ucSyncCnt == 2)
                    {
                        Sync2.Text = rdata.ToString("X");
                        m_ucRxStep = 1;
                        m_ucSyncCnt = 0;
                    }
                    break;
                case 1:
                    if (rdata == 0x7e) // Start Bit
                    {
                        StartBit.Text = rdata.ToString("X");
                        m_ucRxStep = 2;
                    }
                    else
                        m_ucRxStep = 0;
                    break;
                case 2:
                    m_ucRxLength = rdata; // Length
                    Length.Text = m_ucRxLength.ToString("X");
                    m_ucRxStep = 3;
                    break;
                case 3:
                    m_ucRxCommand = rdata; // Command
                    Command.Text = m_ucRxCommand.ToString("X");
                    m_ucRxStep = 4;
             
                    break;
                case 4:
                    m_ucRxData[m_ucRxDataCnt] = rdata;
                    m_ucRxDataCnt++;
                    if (m_ucRxDataCnt == m_ucRxLength)
                    {
                        m_ucRxStep = 5;
                    }
                    break;
                case 5:
                    checksum = (byte)((byte)m_ucRxLength ^ (byte)m_ucRxCommand);
                    for (int i = 0; i < m_ucRxLength; i++)
                        checksum ^= m_ucRxData[i];
                    checksum++;
                    if (checksum == rdata)
                    {
                        Checksum.Text = checksum.ToString("X");
                        m_ucRxStep = 6;
                    }
                    else
                    {
                        Checksum.Text = "이상해";
                        m_ucRxStep = 0;
                        m_ucSyncCnt = 0;
                        m_ucLenCnt = 0;
                        m_ucRxDataCnt = 0;
                    }
                    break;
                case 6:
                    m_ucRxStep = 0;
                    m_ucSyncCnt = 0;
                    m_ucLenCnt = 0;
                    m_ucRxDataCnt = 0;
                    if (rdata == 0x7D)
                    {
                        Exitbit.Text = rdata.ToString("X");
                        DecodeData();
                    }
                    else Exitbit.Text = "이상해";
                    break;
            }
        }
        private void DecodeData()
        {
            switch (m_ucRxCommand)
            {
                case 0x1A:
                    Flag.Text = SecData.UreaAccFlag.ToString();
                    Level.Text = SecData.UreaLevel.ToString();
                    Quality.Text = SecData.Concentration.ToString();
                    NOxIn.Text = SecData.NOxIn.ToString();
                    NOxOut.Text = SecData.NOxOut.ToString();
                    TempIn.Text = SecData.TempIn.ToString();
                    TempOut.Text = SecData.TempOut.ToString();
                    DosingRate.Text = SecData.DosingRate.ToString();
                    MAF.Text = SecData.MAF.ToString();

                    SecData.TM = BitConverter.ToUInt32(m_ucRxData, 0);
                    SecData.LT = BitConverter.ToUInt32(m_ucRxData, 4);
                    SecData.LN = BitConverter.ToUInt32(m_ucRxData, 8);
                    SecData.basePress = BitConverter.ToUInt16(m_ucRxData, 12);
                    SecData.CO = BitConverter.ToUInt16(m_ucRxData, 14);
                    SecData.FW_Ver = BitConverter.ToUInt16(m_ucRxData, 16);
                    SecData.Speed = (byte)BitConverter.ToUInt16(m_ucRxData, 18);
                    SecData.Error = (byte)BitConverter.ToUInt16(m_ucRxData, 19);
                    SecData.SysCode = (short)BitConverter.ToUInt16(m_ucRxData, 20);
                    SecData.User = (byte)BitConverter.ToUInt16(m_ucRxData, 22);
                    SecData.DpfTemp = BitConverter.ToUInt16(m_ucRxData, 23);
                    SecData.Chk = BitConverter.ToUInt16(m_ucRxData, 25);
                    SecData.NOxIn = BitConverter.ToInt16(m_ucRxData, 27);
                    SecData.NOxOut = BitConverter.ToInt16(m_ucRxData, 29);
                    SecData.TempIn = BitConverter.ToInt16(m_ucRxData, 31);
                    SecData.TempOut = BitConverter.ToInt16(m_ucRxData, 33);
                    SecData.DosingRate = BitConverter.ToUInt32(m_ucRxData, 35);
                    SecData.UreaLevel = BitConverter.ToUInt16(m_ucRxData, 39);
                    SecData.MAF = BitConverter.ToUInt16(m_ucRxData, 41);
                    SecData.State = (byte)BitConverter.ToUInt16(m_ucRxData, 43);
                    SecData.Concentration = (byte)BitConverter.ToUInt16(m_ucRxData, 44);
                    SecData.SystemError_1 = (byte)BitConverter.ToUInt16(m_ucRxData, 45);
                    SecData.SystemError_2 = (byte)BitConverter.ToUInt16(m_ucRxData, 46);
                    SecData.SystemError_3 = (byte)BitConverter.ToUInt16(m_ucRxData, 47);
                    SecData.SystemError_4 = (byte)BitConverter.ToUInt16(m_ucRxData, 48);
                    SecData.SystemError_5 = (byte)BitConverter.ToUInt16(m_ucRxData, 49);
                    SecData.SystemError_6 = (byte)BitConverter.ToUInt16(m_ucRxData, 50);
                    SecData.SystemError_7 = (byte)BitConverter.ToUInt16(m_ucRxData, 51);
                    SecData.SystemError_8 = (byte)BitConverter.ToUInt16(m_ucRxData, 52);

                    SecData.DoserError_1 = (byte)BitConverter.ToUInt16(m_ucRxData, 53);
                    SecData.DoserError_2 = (byte)BitConverter.ToUInt16(m_ucRxData, 54);
                    SecData.DoserError_3 = (byte)BitConverter.ToUInt16(m_ucRxData, 55);
                    SecData.DoserError_4 = (byte)BitConverter.ToUInt16(m_ucRxData, 56);
                    SecData.DoserError_5 = (byte)BitConverter.ToUInt16(m_ucRxData, 57);
                    SecData.DoserError_6 = (byte)BitConverter.ToUInt16(m_ucRxData, 58);
                    SecData.DoserError_7 = (byte)BitConverter.ToUInt16(m_ucRxData, 59);
                    SecData.DoserError_8 = (byte)BitConverter.ToUInt16(m_ucRxData, 60);

                    SecData.Vbat = BitConverter.ToUInt16(m_ucRxData, 61);
                    SecData.UreaAccFlag = (byte)BitConverter.ToUInt16(m_ucRxData, 63);
                    break;

                case 0x3B:
                    UreaLvlHi.Text = Setting.UreaLvlHi.ToString();
                    UreaLvlLo.Text = Setting.UreaLvlLo.ToString();
                    UreaLvlAL.Text = Setting.UreaLvlAL.ToString();
                    UreaConHi.Text = Setting.UreaConHi.ToString();
                    UreaConLo.Text = Setting.UreaConLo.ToString();
                    UreaAlHr1.Text = Setting.UreaAlHr1.ToString();
                    UreaAlHr2.Text = Setting.UreaAlHr2.ToString();
                    UreaBuzInt.Text = Setting.UreaBuzInt.ToString();

                    Setting.UreaLvlHi = (byte)BitConverter.ToUInt16(m_ucRxData, 184);
                    Setting.UreaLvlLo = (byte)BitConverter.ToUInt16(m_ucRxData, 185);
                    Setting.UreaLvlAL = (byte)BitConverter.ToUInt16(m_ucRxData, 186);
                    Setting.UreaConHi = (byte)BitConverter.ToUInt16(m_ucRxData, 187);
                    Setting.UreaConLo = (byte)BitConverter.ToUInt16(m_ucRxData, 188);
                    Setting.UreaAlHr1 = (byte)BitConverter.ToUInt16(m_ucRxData, 189);
                    Setting.UreaAlHr2 = (byte)BitConverter.ToUInt16(m_ucRxData, 190);
                    Setting.UreaBuzInt = (byte)BitConverter.ToUInt16(m_ucRxData, 191);
                    break;



            }
            UpdateVal();
        }
        private void UpdateVal()
        {   /* DPF Value */
            //Sync1.Text = SecData.TM.ToString();
            //Sync2.Text = SecData.Concentration.ToString();
            //StartBit.Text = SecData.UreaLevel.ToString();
            
            //UreaLevel.Text = SecData.UreaLevel.ToString();
            /* SCR Value */
        }
        private void TxData(byte sync, ushort len, byte command, byte id, byte[] data)
        {
            string str = "<-";
            byte checkSum ;

            if (!serialPort1.IsOpen)
            {
                MessageBox.Show("Please Open the COM Port First.");
                return;
            }

            byte[] sdata = new byte[100];
            byte check_sum;

            sdata[0] = sync;
            sdata[1] = sync;
            sdata[2] = 0x7e;
            sdata[3] = (byte)(len + 2);
            sdata[4] = command;
            check_sum = (byte)((len + 2) ^ command ^ id);

            for (int i = 0; i < len; i++)
            {
                sdata[i + 6] = data[i];
                check_sum ^= data[i];
            }

            check_sum++;
            sdata[len + 6] = check_sum;
            sdata[len + 7] = 0x7d;

            for (int i = 0; i < len + 8; i++)
            {
                serialPort1.Write(new byte[] { sdata[i] }, 0, 1);
            }

        }
        private void TxCmd(byte sync, byte command, byte sig)
        {
            byte[] data = { sig };
            TxData(sync, 1, command, 0, data);
        }
    }
}




