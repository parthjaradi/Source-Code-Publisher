///////////////////////////////////////////////////////////////////////
// ServerPrototype.h - Console App that processes incoming messages  //
// ver 1.1                                                           //
//                                                                   //
// Application   : OOD-S19 Project 4                                 //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64  //
// Author        : Parth Jaradi                                      //
//                 Source : Jim Fawcett                              //
///////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem/FileSystem.h"
#include "../CodePublisher/CodePublisher.h"
#include <chrono>

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

Files Server::getFiles(const Repository::SearchPath& path)
{
  return Directory::getFiles(path);
}

Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n" << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n " << item.c_str();
  }
}

std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  return reply;
};

std::function<Msg(Msg)> getFiles = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files files = Server::getFiles(searchPath);
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n GetFiles message did not define a path attribute";
  }
  return reply;
};

std::function<Msg(Msg)> getDirs = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n GetDirs message did not define a path attribute";
  }
  return reply;
};

// ---------< Lambda funtion to convert source code files >-------------------
std::function<Msg(Msg)> convert = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("convert");
	
	std::string cmdArgv = msg.value("strArgs");
	std::vector<std::string> tokens;
	// stringstream class check1 
	std::stringstream check1(cmdArgv);
	std::string intermediate;
	// Tokenizing w.r.t. space '$' 
	while (std::getline(check1, intermediate, '$'))
		tokens.push_back(intermediate);
	int argc = tokens.size();
	char ** argv = new char*[argc];
	for (int i = 0; i < argc; i++)
	{
		std::string arg = tokens[i];
		argv[i] = new char[tokens[i].length()];
		strcpy_s(argv[i], tokens[i].length() + 1, arg.c_str());
	}
	CodePublisher cp;
	cp.processCommandLineArgs(argc, argv); // processing cmd line arguments
	if (!cp.extractFiles(argc, argv)) // if this fails, then there are no files to process at all
		std::cout << std::endl << "File Extraction Failed";
	else
	{
		std::cout << std::endl << "\nFollowing Files matched the Patterns and Regular Expressions";
		std::cout << std::endl << "---------------------------------------------------------------\n" << std::endl;
		std::vector<std::string> files = cp.extractedFiles();
		int count = files.size();
		for (int i = 0; i < count; i++)
			std::cout << files[i] << std::endl;
		std::cout << std::endl << "-------------------------------------------------------" << std::endl << std::endl;
	}
	std::vector<std::string> convertedFiles;
	convertedFiles = cp.publish();
	for(int i=0;i< convertedFiles.size();i++)
		std::cout << convertedFiles[i] << std::endl;
	size_t count = 0;
	for (auto item : convertedFiles)
	{
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		reply.attribute("file" + countStr, item);
	}
	delete[] argv;
	return reply;
};


// ---------< Lambda funtion to convert transfer files >-------------------
std::function<Msg(Msg)> xfile = [](Msg msg) {
	std::cout<<"\n\nRequirement 6: Sending of Converted File back to Local Client";
	std::cout<<"\n==============================================================\n";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("xfile");

	return reply;
};


void Repository::Server::displayReq()
{
	std::cout << "\n\t\t\t\t\tRemote Code Page Management";

	std::cout << "\n\nRequirement 3: Assembling of Projects 1, 2 and 3";
	std::cout << "\n===================================================";
	std::cout << "\nThe GUI acts as an Client and the Server has the ";
	std::cout << "Code Publisher Facility.\nThe Client has access to ";
	std::cout << "Remote Root Directory.\nThe Client can't change Root Directory";
	std::cout << "and has the list of files.Translater package acts as C++/CLI translater.";
	std::cout << "\nRead Me file helps to understand how to use the Client GUI";

	std::cout << "\n\nRequirement 4: Provide GUI for the Client to navigate, convert and display Converted Files";
	std::cout << "\n===============================================";
	std::cout << "===============================================";
	std::cout << "\nClient GUI is provided which opens in a different window as an WPF Application";
	std::cout << "\n PS : The dependency links of converted web page will not show the relevant web page";
	std::cout << "\n if the dependent file isn't matching the Regex and Patterns provided by the user.";
}

void Repository::Server::displayReqs()
{
	std::cout << "\n\nRequirement 7: Operations for Two Or more Concurrent Client";
	std::cout << "\n===============================================================";
	std::cout << "\nThe Run.bat file has two different command to start for different clients.";
	std::cout << "\nTwo Consoles will be displayed.";

	std::cout << "\n\nRequirement 8: Automated Test";
	std::cout << "\n=================================";
	std::cout << "\nThis was the automated test data to show all requirements are met.";
	
}


int main()
{
  Repository::Server::displayReq();
  
  Server server(serverEndPoint, "ServerPrototype");
  server.start();
  std::cout << "\n";

  std::cout << "\nRequirement 5: Testing message processing";
  std::cout << "\n=========================================";
  server.addMsgProc("echo", echo);
  server.addMsgProc("getFiles", getFiles);
  server.addMsgProc("getDirs", getDirs);
  server.addMsgProc("serverQuit", echo);
  server.addMsgProc("convert", convert); // msg proc to convert files
  server.addMsgProc("xfile", xfile); // msg proc to transfer files

  server.processMessages();

  Msg msg(serverEndPoint, serverEndPoint);  // send to self
  msg.name("msgToSelf");
  msg.command("echo");
  msg.attribute("verbose", "show me");
  server.postMessage(msg);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  msg.command("getFiles");
  msg.remove("verbose");
  msg.attributes()["path"] = storageRoot;
  server.postMessage(msg);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  msg.command("getDirs");
  msg.attributes()["path"] = storageRoot;
  server.postMessage(msg);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout << "\nPress Enter to EXIT";
  std::cin.get();
  std::cout << "\n";

  msg.command("serverQuit");
  server.postMessage(msg);
  server.stop();

  Repository::Server::displayReqs();
  
  std::getchar();
  return 0;
}



