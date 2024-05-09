using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Serial_Communication; 

namespace Serial_Communication
{
    public partial class Setting : MetroFramework.Forms.MetroForm
    {
        private Form1 form1Instance;
        public Setting()
        {
            InitializeComponent();
            //form1Instance = form1;
        }
        private void metroButton1_Click(object sender, EventArgs e)
        {
            Form1 form1Instance = new Form1();
            form1Instance.button1_Click(sender, e);
        }

    }
}
