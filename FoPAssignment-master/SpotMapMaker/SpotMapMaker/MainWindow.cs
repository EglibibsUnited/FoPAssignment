using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace SpotMapMaker
{
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
        }
  
        private void Form1_Load(object sender, EventArgs e)
        {
            for (int length = 8; length <= 38; length++)
            {
                xHeightBox.Items.Add(length);
                yHeightBox.Items.Add(length);
            }
            xHeightBox.SelectedIndex = 0;
            yHeightBox.SelectedIndex = 0;

            xHeightBox.SelectedIndexChanged += delegate { generateButtons(); };
            yHeightBox.SelectedIndexChanged += delegate { generateButtons(); };
        }

        public List<List<string>> Level = new List<List<string>>();

        void generateButtons()
        {
            this.Level.Clear();
            for (int yLength = 0; yLength < Convert.ToInt32(yHeightBox.Text); yLength++)
            {
                this.Level.Add(new List<string>());
                for (int xLength = 0; xLength < Convert.ToInt32(xHeightBox.Text); xLength++)
                {
                    if(yLength == 0 || yLength == Convert.ToInt32(yHeightBox.Text)-1 || xLength == 0 || xLength == Convert.ToInt32(xHeightBox.Text)-1)
                    {
                        this.Level[yLength].Add("#");
                    } else
                    {
                        this.Level[yLength].Add(" ");
                    }
                }
            }

            redrawGrid();
        }

        private void redrawGrid()
        {
            editPanel.Controls.Clear();
            for (int yLength = 0; yLength < Convert.ToInt32(yHeightBox.Text); yLength++)
            {
                for (int xLength = 0; xLength < Convert.ToInt32(xHeightBox.Text); xLength++)
                {
                    Button levelElementButton = new Button();
                    levelElementButton.AutoSize = false;
                    levelElementButton.FlatStyle = FlatStyle.Popup;
                    levelElementButton.Size = new Size(20, 20);
                    levelElementButton.BackColor = Color.Black;
                    levelElementButton.Text = this.Level[yLength][xLength];
                    levelElementButton.Location = new Point(xLength * 20, yLength * 20);

                    switch (levelElementButton.Text)
                    {
                        case "#":
                            levelElementButton.BackColor = Color.Black;
                            levelElementButton.ForeColor = Color.White;
                            break;
                    }

                    levelElementButton.Tag = yLength.ToString() + " " + xLength.ToString();

                    if(yLength > 0 && yLength < Convert.ToInt32(yHeightBox.Text)-1 && xLength > 0 && xLength < Convert.ToInt32(xHeightBox.Text)-1)
                    {
                        levelElementButton.Click += delegate {
                            if (this.Level[Convert.ToInt32(levelElementButton.Tag.ToString().Split(' ')[0])][Convert.ToInt32(levelElementButton.Tag.ToString().Split(' ')[1])] == " ")
                            {
                                this.Level[Convert.ToInt32(levelElementButton.Tag.ToString().Split(' ')[0])][Convert.ToInt32(levelElementButton.Tag.ToString().Split(' ')[1])] = "#";
                                levelElementButton.Text = "#";
                            }
                            else
                            {
                                this.Level[Convert.ToInt32(levelElementButton.Tag.ToString().Split(' ')[0])][Convert.ToInt32(levelElementButton.Tag.ToString().Split(' ')[1])] = " ";
                                levelElementButton.Text = " ";
                            }

                            switch (levelElementButton.Text)
                            {
                                case "#":
                                    levelElementButton.ForeColor = Color.White;
                                    break;
                                default:
                                    levelElementButton.ForeColor = Color.Black;
                                    break;
                            }
                        };
                    }
                    
                    editPanel.Controls.Add(levelElementButton);
                }
            }
        }

        private void outputTxt_TextChanged(object sender, EventArgs e)
        {

        }

        private void saveBtn_Click(object sender, EventArgs e)
        {
            SaveFileDialog save = new SaveFileDialog();
            save.InitialDirectory = Directory.GetCurrentDirectory();
            save.Filter = "Spot & Zombie Level Files (*.spot)|*.spot";
            save.ShowDialog();

            try
            {
                File.OpenWrite(save.FileName).Close(); // Open new file //
                foreach (List<string> levelRow in this.Level)
                {
                    foreach (string levelPart in levelRow)
                    {
                        File.AppendAllText(save.FileName, levelPart);
                    }
                }
            } catch (Exception)
            {

            }
            
        }
    }
}
