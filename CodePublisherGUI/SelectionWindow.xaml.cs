using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace CodePublisherGUI
{
    /// <summary>
    /// Interaction logic for SelectionWindow.xaml
    /// </summary>
    public partial class SelectionWindow : Window
    {
        MainWindow mWin = null;

        public SelectionWindow()
        {
            InitializeComponent();
        }
        //----< called by MainWindow to give this window her ref >-------

        public void setMainWindow(MainWindow mwin)
        {
            mWin = mwin;
        }
        //----< called by MainWindow to add a selected file >------------

        public void Add(string file)
        {
            Selections.Text = file;
        }
        //----< unselect files and set mWin.swin to null >---------------

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            mWin.swin = null;
        }
        //----< may need this when SelectionWindow starts up >-----------

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            //this.Topmost = true;
        }
    }
}
