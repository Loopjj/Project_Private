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



namespace Serial_Communication
{
    public partial class Form1 : MetroFramework.Forms.MetroForm
    {
        byte  m_ucRxDataCnt, m_ucRxCommand, m_ucRxID, ucLength, rdata;
        byte[] m_ucRxData = new byte[1024];
        byte m_ucRxStep, m_ucSyncCnt, m_ucLenCnt;
        short[] m_uiTVal = new short[4];
        short[] TVal = new short[4];
        short[] PVal = new short[2];
        int m_ucRxLength;


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
    public byte SystemCheck;
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



        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)  //폼이 로드되면
        {
            comboBox_port1.DataSource = SerialPort.GetPortNames(); //연결 가능한 시리얼포트 이름을 콤보박스에 가져오기 
        }

        private void label50_Click(object sender, EventArgs e)
        {

        }

        private void label74_Click(object sender, EventArgs e)
        {

        }

        private void Button_connect_Click(object sender, EventArgs e)  //통신 연결하기 버튼
        {
            if (!serialPort1.IsOpen)  //시리얼포트가 열려 있지 않으면
            {
                serialPort1.PortName = comboBox_port1.Text;  //콤보박스의 선택된 COM포트명을 시리얼포트명으로 지정
                serialPort1.BaudRate = 115200;  //보레이트 변경이 필요하면 숫자 변경하기
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
                    if (rdata == 0xc6)
                    {
                        m_ucSyncCnt++;
                    }
                    else
                    {
                        m_ucSyncCnt = 0;
                    }
                    if (m_ucSyncCnt == 2)
                    {
                        m_ucRxStep = 1;
                        m_ucSyncCnt = 0;
                    }
                    break;
                case 1:
                    if (rdata == 0x7e)
                    {
                        m_ucRxStep = 2;
                    }
                    else
                        m_ucRxStep = 0;
                    break;
                case 2:
                    if (m_ucLenCnt == 0)
                    {
                        ucLength = rdata;
                        m_ucLenCnt++;
                    }
                    else
                    {
                        ucLength += (byte)(rdata << 8);
                        m_ucRxLength = ucLength - 2;
                        m_ucLenCnt = 0;
                        m_ucRxStep = 3;
                    }
                    break;
                case 3:
                    m_ucRxCommand = rdata; // 0x21 or 0x20
                    m_ucRxStep = 4;
                    break;
                case 4:
                    m_ucRxID = rdata;
                    m_ucRxDataCnt = 0;
                    m_ucRxStep = 5;
                    break;
                case 5:
                    m_ucRxData[m_ucRxDataCnt] = rdata;
                    m_ucRxDataCnt++;
                    if (m_ucRxDataCnt == m_ucRxLength)
                        m_ucRxStep = 6;
                    break;
                case 6:
                    checksum = (byte)((byte)(ucLength & 0xff) ^ (byte)((ucLength >> 8) & 0xff) ^ m_ucRxCommand ^ m_ucRxID);
                    for (int i = 0; i < m_ucRxLength; i++)
                        checksum ^= m_ucRxData[i];
                    checksum++;
                    if (checksum == rdata)
                    {
                        m_ucRxStep = 7;
                    }
                    else
                    {
                        m_ucRxStep = 0;
                        m_ucSyncCnt = 0;
                        m_ucLenCnt = 0;
                        m_ucRxDataCnt = 0;
                    }
                    break;
                case 7:
                    m_ucRxStep = 0;
                    m_ucSyncCnt = 0;
                    m_ucLenCnt = 0;
                    m_ucRxDataCnt = 0;
                    if (rdata == 0x7D)
                    {
                        DecodeData();
                    }
                        break;
            }
        }
        private void DecodeData()
        {



            switch (m_ucRxCommand)
            {
                case 0x48:

                    READ_VALUE ReadValue = new READ_VALUE();

                    // 바이트 배열에 데이터 직접 복사
                    ReadValue.T1 = BitConverter.ToUInt16(m_ucRxData, 0);
                    ReadValue.T2 = BitConverter.ToUInt16(m_ucRxData, 2);
                    ReadValue.T3 = BitConverter.ToUInt16(m_ucRxData, 4);
                    ReadValue.T4 = BitConverter.ToUInt16(m_ucRxData, 6);
                    ReadValue.P = BitConverter.ToUInt16(m_ucRxData, 8);
                    ReadValue.Rate = BitConverter.ToUInt16(m_ucRxData, 10);
                    ReadValue.IHC = BitConverter.ToUInt16(m_ucRxData, 12);
                    ReadValue.f_REG = BitConverter.ToUInt16(m_ucRxData, 14);
                    ReadValue.CarType = (byte)BitConverter.ToUInt16(m_ucRxData, 16);
                    ReadValue.FPD = (byte)BitConverter.ToUInt16(m_ucRxData, 17);
                    ReadValue.Error = (byte)BitConverter.ToUInt16(m_ucRxData, 18);
                    ReadValue.Check = (byte)BitConverter.ToUInt16(m_ucRxData, 19);
                    ReadValue.t_REG = BitConverter.ToUInt16(m_ucRxData, 20);
                    ReadValue.n_REG = BitConverter.ToUInt16(m_ucRxData, 22);
                    ReadValue.Sig = (byte)BitConverter.ToUInt16(m_ucRxData, 24);
                    ReadValue.Sts = (byte)BitConverter.ToUInt16(m_ucRxData, 25);
                    ReadValue.Lock = (byte)BitConverter.ToUInt16(m_ucRxData, 26);
                    ReadValue.DrvTime = BitConverter.ToUInt32(m_ucRxData, 28);
                    ReadValue.baseP1 = BitConverter.ToUInt16(m_ucRxData, 32);
                    ReadValue.f_SCR = BitConverter.ToUInt16(m_ucRxData, 34);
                    ReadValue.t_SCR = BitConverter.ToUInt16(m_ucRxData, 36);
                    ReadValue.RegenStartKey = (byte)BitConverter.ToUInt16(m_ucRxData, 38);
                    ReadValue.t_OFFSCR = BitConverter.ToUInt16(m_ucRxData, 40);
                    ReadValue.Speed = (byte)BitConverter.ToUInt16(m_ucRxData, 42);
                    ReadValue.Valid = (byte)BitConverter.ToUInt16(m_ucRxData, 43);

                    textBox_Size.Text = m_ucRxLength.ToString();
                    textBox_T1.Text = ReadValue.T1.ToString();
                    textBox_T2.Text = ReadValue.T2.ToString();
                    textBox_T3.Text = ReadValue.T3.ToString();
                    textBox_T4.Text = ReadValue.T4.ToString();
                    textBox_P.Text = ReadValue.P.ToString();
                    textBox_Rate.Text = ReadValue.Rate.ToString();
                    textBox_IHC.Text = ReadValue.IHC.ToString();
                    textBox_f_REG.Text = ReadValue.f_REG.ToString();
                    textBox_CarType.Text = ReadValue.CarType.ToString();
                    textBox_FPD.Text = ReadValue.FPD.ToString();
                    textBox_Error.Text = ReadValue.Error.ToString();
                    textBox_Check.Text = ReadValue.Check.ToString();
                    textBox_t_REG.Text = ReadValue.t_REG.ToString();
                    textBox_n_REG.Text = ReadValue.n_REG.ToString();
                    textBox_Sig.Text = ReadValue.Sig.ToString();
                    textBox_Sts.Text = ReadValue.Sts.ToString();
                    textBox_Lock.Text = ReadValue.Lock.ToString();
                    textBox_DrvTime.Text = ReadValue.DrvTime.ToString();
                    textBox_baseP1.Text = ReadValue.baseP1.ToString();
                    textBox_f_SCR.Text = ReadValue.f_SCR.ToString();
                    textBox_t_SCR.Text = ReadValue.t_SCR.ToString();
                    textBox_n_SCR.Text = ReadValue.n_SCR.ToString();
                    textBox_RegenStartKey.Text = ReadValue.RegenStartKey.ToString();
                    textBox_t_OFFSCR.Text = ReadValue.t_OFFSCR.ToString();
                    textBox_Speed.Text = ReadValue.Speed.ToString();
                    textBox_Valid.Text = ReadValue.Valid.ToString();

                    break;
                    //UpdateVal();

                    /* 이하 SCR DATA */
                case 0x28:

                    stSysValue ScrValue = new stSysValue();

                    ScrValue.Version = BitConverter.ToUInt16(m_ucRxData, 0);
                    ScrValue.T1_temp = (short)BitConverter.ToUInt16(m_ucRxData, 2);
                    ScrValue.T2_temp = (short)BitConverter.ToUInt16(m_ucRxData, 4);
                    ScrValue.Tavg_temp = (short)BitConverter.ToUInt16(m_ucRxData, 6);
                    ScrValue.T3_temp = (short)BitConverter.ToUInt16(m_ucRxData, 8);
                    ScrValue.P1_bar = BitConverter.ToUInt16(m_ucRxData, 10);
                     ScrValue.Noxppm1 = (short)BitConverter.ToUInt16(m_ucRxData, 12);
                     ScrValue.Noxppm2 = (short)BitConverter.ToUInt16(m_ucRxData, 14);
                     ScrValue.DosingDuty = BitConverter.ToUInt16(m_ucRxData, 16);
                     ScrValue.MafKg_H = BitConverter.ToUInt16(m_ucRxData, 18);
                     ScrValue.Maf_temp = (short)BitConverter.ToUInt16(m_ucRxData, 20);
                     ScrValue.TankTemp = (short)BitConverter.ToUInt16(m_ucRxData, 22);
                     ScrValue.TankLevelP = BitConverter.ToUInt16(m_ucRxData, 24);
                     ScrValue.BattVoltage = BitConverter.ToUInt16(m_ucRxData, 26); 
                     ScrValue.Map_X = (byte)BitConverter.ToUInt16(m_ucRxData, 28);
                     ScrValue.Map_Y = (byte)BitConverter.ToUInt16(m_ucRxData, 29);
                     ScrValue.StatusAlpha = (byte)BitConverter.ToUInt16(m_ucRxData, 30);
                     ScrValue.SCRMode = (byte)BitConverter.ToUInt16(m_ucRxData, 31);
                     ScrValue.UreaQuality = BitConverter.ToUInt16(m_ucRxData, 32);
                     ScrValue.NoxReduction = BitConverter.ToUInt16(m_ucRxData, 34);
                     ScrValue.SystemError = (byte)BitConverter.ToUInt16(m_ucRxData, 36);
                     ScrValue.SystemCheck = (byte)BitConverter.ToUInt16(m_ucRxData, 37);
                     ScrValue.SystemSignal = (short)BitConverter.ToUInt16(m_ucRxData, 38);
                     ScrValue.ScrFlag = (short)BitConverter.ToUInt16(m_ucRxData, 40);
                     ScrValue.DosingRatehouer = BitConverter.ToUInt32(m_ucRxData, 42);
                     ScrValue.TotalDosingRate = BitConverter.ToUInt32(m_ucRxData, 50);
                     ScrValue.curXk = BitConverter.ToUInt32(m_ucRxData, 58);
                     ScrValue.Xc = BitConverter.ToUInt32(m_ucRxData, 66);
                     ScrValue.H1k = BitConverter.ToUInt32(m_ucRxData, 74);
                     ScrValue.H2k = BitConverter.ToUInt32(m_ucRxData, 82);
                     ScrValue.Yk = BitConverter.ToUInt32(m_ucRxData, 90);
                     ScrValue.Kl = BitConverter.ToUInt32(m_ucRxData, 98);
                     ScrValue.Kp = BitConverter.ToUInt32(m_ucRxData, 106);
                     ScrValue.curXh = BitConverter.ToUInt32(m_ucRxData, 114);
                     ScrValue.Vk = BitConverter.ToUInt32(m_ucRxData, 122);
                     ScrValue.Gamma = BitConverter.ToUInt32(m_ucRxData, 130);
                     ScrValue.Formula1 = BitConverter.ToUInt32(m_ucRxData, 138);
                     ScrValue.Formula2 = BitConverter.ToUInt32(m_ucRxData, 146);
                     ScrValue.Formula3 = BitConverter.ToUInt32(m_ucRxData, 154);
                     ScrValue.NH3Up = BitConverter.ToUInt32(m_ucRxData, 162);
                     ScrValue.NH3Dn = BitConverter.ToUInt32(m_ucRxData, 170); 
                     ScrValue.PM_Senser1 = BitConverter.ToUInt16(m_ucRxData, 178);
                     ScrValue.PM_Senser2 = BitConverter.ToUInt16(m_ucRxData, 180);
                     ScrValue.PM_Senser3 = BitConverter.ToUInt16(m_ucRxData, 182);
                     ScrValue.PM_Senser4 = BitConverter.ToUInt16(m_ucRxData, 184);
                     ScrValue.NoxO2_1 = (short)BitConverter.ToUInt16(m_ucRxData,186);
                     ScrValue.NoxO2_2 = (short)BitConverter.ToUInt16(m_ucRxData, 188);


                    textBox_Version.Text = ScrValue.Version.ToString();
                    textBox_T1_temp.Text = ScrValue.T1_temp.ToString();
                    textBox_T2_temp.Text = ScrValue.T2_temp.ToString();
                    textBox_Tavg_temp.Text = ScrValue.Tavg_temp.ToString();
                    textBox_T3_temp.Text = ScrValue.T3_temp.ToString();
                    textBox_P1_bar.Text = ScrValue.P1_bar.ToString();
                    textBox_Noxppm1.Text = ScrValue.Noxppm1.ToString();
                    textBox_Noxppm2.Text = ScrValue.Noxppm2.ToString();
                    textBox_DosingDuty.Text = ScrValue.DosingDuty.ToString();
                    textBox_MafKg_H.Text = ScrValue.MafKg_H.ToString();
                    textBox_Maf_temp.Text = ScrValue.Maf_temp.ToString();
                    textBox_TankTemp.Text = ScrValue.TankTemp.ToString();
                    textBox_TankLevelP.Text = ScrValue.TankLevelP.ToString();

                    textBox_BattVoltage.Text = ScrValue.BattVoltage.ToString();
                    textBox_Map_X.Text = ScrValue.Map_X.ToString();
                    textBox_Map_Y.Text = ScrValue.Map_Y.ToString();
                    textBox_StatusAlpha.Text = ScrValue.StatusAlpha.ToString();
                    textBox_SCRMode.Text = ScrValue.SCRMode.ToString();
                    textBox_UreaQuality.Text = ScrValue.UreaQuality.ToString();
                    textBox_NoxReduction.Text = ScrValue.NoxReduction.ToString();
                    textBox_SystemError.Text = ScrValue.SystemError.ToString();
                    break;
            }
        }   


    }
}


//            }

//PRIVATE VOID BUTTON_SEND_CLICK(OBJECT SENDER, EVENTARGS E)  //보내기 버튼을 클릭하면
//{
//    SERIALPORT1.WRITE(TEXTBOX_SEND.TEXT);  //텍스트박스의 텍스트를 시리얼통신으로 송신
//}








