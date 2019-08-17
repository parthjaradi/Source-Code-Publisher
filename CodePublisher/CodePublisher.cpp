///////////////////////////////////////////////////////////////////////////
// CodePublisher.cpp : defines facade/executive for OOD Pr3 S19          //
// ver 1.1                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 3                                     //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Parth Jaradi                                          //
//                 Source : Ammar Salman, Instructor Solution            //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////

#include "CodePublisher.h"
#include "../Logger/Logger.h"
#include "../Loader/Loader.h"
#include "IPublisher.h"

using namespace CodeUtilities;
using namespace Logging; 
using namespace FileSystem;

ProcessCmdLine::Usage customUsage();

CodePublisher::CodePublisher()
{
}

// -----< process cmd to get info >------------------------------------
bool CodePublisher::processCommandLineArgs(int argc, char ** argv)
{
  pcl_ = new ProcessCmdLine(argc, argv); 
  pcl_->usage(customUsage());

  preface("Command Line: ");
  pcl_->showCmdLine();

  if (pcl_->parseError())
  {
    pcl_->usage();
    std::cout << "\n\n";
    return false;
  }
  dirIn_ = pcl_->path();
  return true;
}

// -----< return input directory - from PCL >----------------------------
const std::string & CodePublisher::workingDirectory() const
{
  return dirIn_;
}

// -----< set output directory >-----------------------------------------
void CodePublisher::outputDirectory(const std::string & dir)
{
  dirOut_ = dir; 
  cconv_.outputDir(dir);
}

// -----< return output directory >--------------------------------------
const std::string & CodePublisher::outputDirectory() const
{
  return dirOut_;
}

// -----< extract files - using Loader Package >-------------------------
bool CodePublisher::extractFiles(int argc, char ** argv)
{
	DirExplorerN de(pcl_->path());

	for (auto patt : pcl_->patterns())
	{
		de.addPattern(patt);
	}

	if (pcl_->hasOption("s"))
	{
		de.recurse();
	}

	bool res = de.search() && de.match_regexes(pcl_->regexes());
	de.showStats();

	files_ = de.filesList();

  return res;
}


//------< Fetch extracted files >------------------
std::vector<std::string> CodePublisher::extractedFiles()
{
	return files_;
}

// -----< publish - files extracted from directory explorer >---------------
std::vector<std::string> CodePublisher::publish()
{
  return cconv_.convert(files_);
 
}

// -----< publish - single file >-------------------------------------------
void CodePublisher::publish(const std::string & file)
{
  display_.display(cconv_.convert(file));
}

// -----< publish - must provide list >-------------------------------------
void CodePublisher::publish(const std::vector<std::string>& files)
{
  display_.display(cconv_.convert(files));
}

// -----< gets display mode as set by PCL >---------------------------------
CodeUtilities::DisplayMode CodePublisher::displayMode() const
{
  return pcl_->displayMode();
}


// -----< command line usage >----------------------------------------------
ProcessCmdLine::Usage customUsage()
{
  std::string usage;
  usage += "\n  Command Line: path [/option]* [/pattern]* [/regex]*";
  usage += "\n    path is relative or absolute path where processing begins";
  usage += "\n    [/option]* are one or more options of the form:";
  usage += "\n      /s     - walk directory recursively";
  usage += "\n      /demo  - run in demonstration mode (cannot coexist with /debug)";
  usage += "\n      /debug - run in debug mode (cannot coexist with /demo)";
  //usage += "\n      /h - hide empty directories";
  //usage += "\n      /a - on stopping, show all files in current directory";
  usage += "\n    [pattern]* are one or more pattern strings of the form:";
  usage += "\n      *.h *.cpp *.cs *.txt or *.*";
  usage += "\n    [regex] regular expression(s), i.e. [A-B](.*)";
  usage += "\n";
  return usage;
}

// -----< initialize loggers in proper display mode >------------------------
void initializeLoggers(CodeUtilities::DisplayMode dm) {
  if (dm == CodeUtilities::DisplayMode::Debug) {
    LoggerDebug::attach(&std::cout);
    LoggerDebug::start();
  }

  else if (dm == CodeUtilities::DisplayMode::Demo) {
    LoggerDemo::attach(&std::cout);
    LoggerDemo::start();
  }
}

// -----< stop loggers >-----------------------------------------------------
void stopLoggers() {
  LoggerDemo::stop();
  LoggerDebug::stop();
}



#ifdef DEMO_CP

#include <iostream>

// -----< demonstrate requirements #3 #4 and #5 >---------------------------------------
int demoReq345(CodePublisher& cp, int argc, char ** argv) {
  LoggerDemo::write("\n");
  LoggerDemo::title("Demonstrating Requirement #3 - Executive Package - command line arguments");
  LoggerDemo::write(customUsage());

  if (argc < 2) return 1; // even before processing, if no path, just reject it

  if (!cp.processCommandLineArgs(argc, argv)) return 1; // just in case

  LoggerDemo::write("\n");
  LoggerDemo::title("Demonstrating Requirement #4 - Packages");
  LoggerDemo::write("\n  Loader, Converter, Dependencies and Display packages added in the project. ");
  
  LoggerDemo::title("\n Demonstrating Requirement #5 - Packages");
  LoggerDemo::write("\n Displaying Loader Package finds and loads file to be converted from path");
  LoggerDemo::write(" provided in command line argument\n");

  if (!cp.extractFiles(argc, argv)) { // if this fails, then there are no files to process at all
    return 2; // the demo will end after this
  }

  return 0;
}

// -----< demonstrate requirements 6 >------------------------------
void demoReq67(CodePublisher& cp) {
  LoggerDemo::write("\n");
  LoggerDemo::title("Demonstrating Requirements #6 - Convert Package");
  cp.publish();
}

// -----< demonstrate requirements 7 and 8 >------------------------------
void CodePublisher::demoReq78() {
	cconv_.showReq78();
}

// -----< demonstrate requirement 9 >-------------------------------------
void demoReq9() {
  LoggerDemo::write("\n");
  LoggerDemo::write("\n  Dependency Links web page will not be displayed.");
  LoggerDemo::write("\n  if the dependent source code file has not been generated.");
  LoggerDemo::title("Demonstrating Requirement #9 - Automated Test Unit");
  LoggerDemo::write("\n  Well, this is the automated test unit.");
  LoggerDemo::write("\n\n  All Requirements met. ");

}


// -----< DEMONSTRATION MAIN >---------------------------------------------
int main(int argc, char ** argv) {
  // the following processing of cmd is not official, it's only
  // done to allow for demo/debug modes BEFORE CodePublisher does
  // its own processing of cmds..

	std::cout << "OOD Project 2" << std::endl;
	std::cout << "-------------" << std::endl;
	std::cout << "Source Code Publisher for Projects" << std::endl;

  initializeLoggers(Utilities::DisplayMode::Debug); // by default go debug
  for (int i = 0; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "/demo") { // only go demo once told 
      stopLoggers();
      initializeLoggers(Utilities::DisplayMode::Demo);
      break;
    }
  }

  CodePublisher cp;
  int err = demoReq345(cp, argc, argv);
  
  if (err == 1) {
    LoggerDemo::write("\n  Invalid command line args.\n  Ending demonstration...\n");
    return 1;
  }
  else if (err == 2) {
    LoggerDemo::write("\n  The given path has no files matching patterns and/or regexes.");
    LoggerDemo::write("\n  Ending demonstration...\n");
    return 2;
  }
  

  demoReq67(cp);
  cp.demoReq78();
  demoReq9();

  stopLoggers();
  std::getchar();
  return 0;
}

#endif


#ifdef USE_CP

#include <iostream>

// -----< NORMAL USE MAIN > ------------------------------------------
int main(int argc, char ** argv) {
  CodePublisher cp;
  if (!cp.processCommandLineArgs(argc, argv)) {
    std::cout << "\n  Failed to process command line arguments. Terminating\n\n";
    return 1;
  }

  initializeLoggers(cp.displayMode());

  if (!cp.extractFiles()) {
    stopLoggers();
    std::cout << "\n  Found no files to process. Terminating.\n\n";
    return 0; 
  }

  cp.publish();
  stopLoggers();
  return 0;
}

#endif