using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WINFORM_STUDY
{
    public partial class Form1 : MetroFramework.Forms.MetroForm
    {
        public Form1()
        {
            InitializeComponent();
            this.StyleManager = metroStyleManager1;
        }

        private void mbTheme_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (mbTheme.SelectedIndex)
            {
                case 0:
                    metroStyleManager1.Theme = MetroFramework.MetroThemeStyle.Dark; 
                    break;
                case 1:
                    metroStyleManager1.Theme = MetroFramework.MetroThemeStyle.Light;
                    break;
            }
        }

        private void mbColor_SelectedIndexChanged(object sender, EventArgs e)
        {
            metroStyleManager1.Style = (MetroFramework.MetroColorStyle)Convert.ToInt32(mbColor.SelectedIndex);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            mbColor.SelectedIndex = 0;
            mbTheme.SelectedIndex = 0;
        }

        private void metroButton1_Click(object sender, EventArgs e)
        {

        }
    }
}
