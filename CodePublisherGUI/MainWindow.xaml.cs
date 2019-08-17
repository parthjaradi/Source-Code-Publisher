using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Text.RegularExpressions;
using mshtml;
using MsgPassingCommunication;


namespace CodePublisherGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string path { get; set; }
        private string patterns { get; set; }
        private string regex { get; set; }

        private List<String> convertedFiles { get; set; }

        public SelectionWindow swin { get; set; } = null; // Pop Up Window for Source Code Display

        private Stack<string> pathStack_ = new Stack<string>();

        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();



        public MainWindow()
        {
            InitializeComponent();
        }
     
        //----< process incoming messages on child thread >----------------
        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }


        //----< Clears Directory List Box >-------
        private void clearDirs()
        {
            lstBoxDirs.Items.Clear();
        }
        //----< Adds directory Directory List Box >-------

        private void addDir(string dir)
        {
            lstBoxDirs.Items.Add(dir);
        }
        //----< Adds Parent Directory List Box >-------
        private void insertParent()
        {
            lstBoxDirs.Items.Insert(0, "..");
        }
        //----< Clears Files List Box >-------
        private void clearFiles()
        {
            lstBoxFiles.Items.Clear();
        }
        //----< Clears Converted Files List Box >-------
        private void clearConvertedFiles()
        {
            lstBoxConvertedPages.Items.Clear();
        }


        //----< Adds file to Files List Box >-------
        private void addFile(string file)
        {
            lstBoxFiles.Items.Add(file);
        }


        //----< Adds converted file to Converted Files List Box >-------
        private void addConvertedFile(string file)
        {
            string sourceFile = "..\\..\\" + file;
            lstBoxConvertedPages.Items.Add(sourceFile);
          
        }


        //----< add client processing for message with key >---------------
        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }
        //----< load getDirs processing into dispatcher dictionary >-------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }

        //----< Calls Dispatcher for Loading Directories and Files >------
        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
        }

        //----< load getFiles processing into dispatcher dictionary >------
        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }

        //----< load converted files processing into dispatcher dictionary >------
        private void DispatcherLoadGetConvertedFiles()
        {
            Action<CsMessage> getConvertFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearConvertedFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addConvertedFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("convert", getConvertFiles);
        }

        private void DispatcherTransferFiles()
        {
            // do nothing
        }


        // -----< Window loaded event handler - Add your initialization here >-------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // start Comm
            string[] portCmdArgs = Environment.GetCommandLineArgs();
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = portCmdArgs[3];
            endPoint_.port = Int32.Parse(portCmdArgs[4]);
            translater = new Translater();
            translater.listen(endPoint_);

            // start processing messages
            processMessages();

            // load dispatcher
            loadDispatcher();

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = portCmdArgs[1];
            serverEndPoint.port = Int32.Parse(portCmdArgs[2]);

            txtPath.Text = "Storage";
            pathStack_.Push("../Storage");
            path = "../Storage";
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
            Console.Write("\n");

            patterns = "*.h *.cpp";
            regex = "[A-C](.*)";

            txtPatterns.Text = patterns;
            txtRegex.Text = regex;
        }

        //----< find parent paths >--------------------------------------
        string getAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }


        //----< strip off name of first part of path >---------------------
        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }


        //-----------< Directory List Box Double Click Event >-----------------
        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)lstBoxDirs.SelectedItem;
            if (selectedDir == null)
                return;
            if (selectedDir == "..")
            {
                if (pathStack_.Count > 1)  // don't pop off "Storage"
                    pathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = pathStack_.Peek() + "/" + selectedDir;
                pathStack_.Push(path);
            }
            // display path in Dir TextBlcok
            txtPath.Text = removeFirstDir(pathStack_.Peek());

            // build message to get dirs and post it
            string[] portCmdArgs = Environment.GetCommandLineArgs();
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = portCmdArgs[1];
            serverEndPoint.port = Int32.Parse(portCmdArgs[2]);
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            Console.Write("\n");

            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
            Console.Write("\n");
        }


        //-----------< Sub-Directory CheckBox Click Event >-----------------
        private void chkRecurse_OnClick(object sender, RoutedEventArgs e)
        {
            //path = txtPath.Text;
        }

        //-----------< Directory Path Change Event >-----------------
        private void txtPath_TextChanged(object sender, TextChangedEventArgs e)
        {
            
        }

        //-----------< Patterns Change Event >-----------------
        private void txtPattern_TextChanged(object sender, TextChangedEventArgs e)
        {
            patterns = txtPatterns.Text;
        }

        //-----------< Regex Change Event >-----------------
        private void txtRegex_TextChanged(object sender, TextChangedEventArgs e)
        {
            regex = txtRegex.Text;
        }

        //-----------< Display Files Btn Click Event >-----------------
        private async void btnDisplay_OnClick(object sender, RoutedEventArgs e)
        {
            try
            {
                foreach (var cwf in lstBoxConvertedPages.Items)
                {
                    string file = cwf.ToString();
                    var proc1 = System.Diagnostics.Process.Start(file);
                    statusBarTxt.Text = "Displaying Web Pages...";
                    while (!proc1.HasExited)
                        await Task.Delay(750);
                    statusBarTxt.Text = "Ready";
                }

                foreach (string cFile in convertedFiles)
                {
                    var proc1 = System.Diagnostics.Process.Start(cFile);
                    statusBarTxt.Text = "Displaying Web Pages...";
                    while (!proc1.HasExited)
                        //wait Task.Delay(500);
                        statusBarTxt.Text = "Ready";
                }
            }
            catch { 
            }
        }


        //-----------< Publish Btn Click Event >-----------------
        private void btnPublish_OnClick(object sender, RoutedEventArgs e)
        {
            lstBoxConvertedPages.Items.Clear();
            webBrwsrPreview.Navigate("about:blank");
            List<string> args = new List<string>();
            args.Add("CodePublisher.exe");
            args.Add(path);
            if (chkRecurse.IsChecked == true)
                args.Add("/s");
            else
                args.Add(" ");
            args.AddRange(patterns.Split(' '));
            args.AddRange(regex.Split(' '));


            string strArgs="";
            for(int i = 0;i< args.Count();i++)
            {
                strArgs +=  args[i];
                if (i != args.Count() - 1)
                    strArgs += "$";
            }

            CsEndPoint serverEndPoint = new CsEndPoint();
            string[] portCmdArgs = Environment.GetCommandLineArgs();
            serverEndPoint.machineAddress = portCmdArgs[1];
            serverEndPoint.port = Int32.Parse(portCmdArgs[2]);
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("strArgs", strArgs);
            msg.add("command", "convert");
            translater.postMessage(msg);
            Console.Write("\n");
            DispatcherLoadGetConvertedFiles();
            Console.Write("\n");
        }

        //-----------< Change in Selection of COnverted Pages List Box Event >--------------
        private void ConvertedPages_SelChng(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                string file = lstBoxConvertedPages.SelectedItem.ToString();
                string fullPath = System.IO.Path.GetFullPath(file);
                webBrwsrPreview.Navigate(fullPath);
            }
            catch
            {
                // just return
            }
        }

        //-----------< Source Code Of The Selected Web Page Event >--------------
        private void menu_ViewSourceCode(object sender, RoutedEventArgs e)
        {
            swin = new SelectionWindow();
            swin.setMainWindow(this);
            swin.Show();
            mshtml.HTMLDocument document = webBrwsrPreview.Document as HTMLDocument;
            if (document != null)
            {
                var x = document.doctype;
                string source = document.documentElement.innerHTML;
                swin.Add(source);
            }
        }

        //-----------< Display Web Page Event in Web Browser Event >--------------
        private async void menu_ViewWebPage(object sender, RoutedEventArgs e)
        {
            string file = lstBoxConvertedPages.SelectedItem.ToString();
            var proc1 = System.Diagnostics.Process.Start(file);
            statusBarTxt.Text = "Displaying Web Pages...";
            while (!proc1.HasExited)
              await Task.Delay(750);
            statusBarTxt.Text = "Ready";
        }

        //-----------< Exit Application Event >--------------
        private void menuExit_OnClick(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        //-----------< Help about the Application Event >--------------
        private void menuHelp_OnClick(object sender, RoutedEventArgs e)
        {
            string file = "..\\..\\..\\ReadMe.txt";
            var proc1 = System.Diagnostics.Process.Start(file);
        }

        //-----------< Transfer Files when Converted Web Page is Selected >-------------------------------------
        private void btnTransferFiles_OnClick(object sender, MouseButtonEventArgs e)
        {
            
            string[] portCmdArgs = Environment.GetCommandLineArgs();
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = portCmdArgs[1];
            serverEndPoint.port = Int32.Parse(portCmdArgs[2]);
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            var saveFilePath = System.IO.Path.GetFullPath("..\\..\\..\\SaveFiles") + lstBoxConvertedPages.Items[lstBoxConvertedPages.SelectedIndex].ToString().Substring(lstBoxConvertedPages.Items[lstBoxConvertedPages.SelectedIndex].ToString().LastIndexOf("\\"));
            msg.add("saveFilePath", saveFilePath);
            msg.add("file", lstBoxConvertedPages.Items[lstBoxConvertedPages.SelectedIndex].ToString());
            msg.add("command", "xfile");
            translater.postMessage(msg);
            Console.Write("\n");
            DispatcherTransferFiles();

        }
    }

}
