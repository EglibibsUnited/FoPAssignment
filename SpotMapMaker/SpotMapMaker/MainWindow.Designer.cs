namespace SpotMapMaker
{
    partial class MainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.editPanel = new System.Windows.Forms.Panel();
            this.xHeightBox = new System.Windows.Forms.ComboBox();
            this.yHeightBox = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.saveBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(442, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(306, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "Spot and Zombies Level Designer";
            // 
            // editPanel
            // 
            this.editPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.editPanel.Location = new System.Drawing.Point(13, 64);
            this.editPanel.Name = "editPanel";
            this.editPanel.Size = new System.Drawing.Size(1100, 750);
            this.editPanel.TabIndex = 1;
            // 
            // xHeightBox
            // 
            this.xHeightBox.FormattingEnabled = true;
            this.xHeightBox.Location = new System.Drawing.Point(41, 32);
            this.xHeightBox.Name = "xHeightBox";
            this.xHeightBox.Size = new System.Drawing.Size(75, 21);
            this.xHeightBox.TabIndex = 3;
            // 
            // yHeightBox
            // 
            this.yHeightBox.FormattingEnabled = true;
            this.yHeightBox.Location = new System.Drawing.Point(144, 32);
            this.yHeightBox.Name = "yHeightBox";
            this.yHeightBox.Size = new System.Drawing.Size(75, 21);
            this.yHeightBox.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(124, 31);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(15, 18);
            this.label2.TabIndex = 4;
            this.label2.Text = "x";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F);
            this.label3.Location = new System.Drawing.Point(94, 7);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 18);
            this.label3.TabIndex = 5;
            this.label3.Text = "Level Size";
            // 
            // saveBtn
            // 
            this.saveBtn.Location = new System.Drawing.Point(988, 16);
            this.saveBtn.Name = "saveBtn";
            this.saveBtn.Size = new System.Drawing.Size(81, 34);
            this.saveBtn.TabIndex = 6;
            this.saveBtn.Text = "Save Level";
            this.saveBtn.UseVisualStyleBackColor = true;
            this.saveBtn.Click += new System.EventHandler(this.saveBtn_Click);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1125, 851);
            this.Controls.Add(this.saveBtn);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.yHeightBox);
            this.Controls.Add(this.xHeightBox);
            this.Controls.Add(this.editPanel);
            this.Controls.Add(this.label1);
            this.DoubleBuffered = true;
            this.Name = "MainWindow";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel editPanel;
        private System.Windows.Forms.ComboBox xHeightBox;
        private System.Windows.Forms.ComboBox yHeightBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button saveBtn;
    }
}

