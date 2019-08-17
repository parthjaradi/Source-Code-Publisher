#pragma once
///////////////////////////////////////////////////////////////////////////
// CodePublisher.h : defines facade/executive for OOD Pr3 S19            //
// ver 1.1                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 3                                     //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Parth Jaradi                                          //
//                 Source : Ammar Salman, Instructor Solution            //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodePublisher which uses different components of
*  Loader, CodeConverter, Display and DirectoryN to put together the entire 
*  functionality of the CodePublisher. The mentioned packages are indep-
*  ndent of each other and their only interaction is through this facade
*
*  NOTE: there are TWO modes for compiling this file:
*   1) Preprocessor DEMO_CP : runs in full demo mode for Pr1.
*   2) Preprocessor USE_CP  : runs normal functionality. 
*
*  Required Files:
* =======================
*  CodePublisher.h CodePublisher.cpp CodeUtilities.h
*  Converter.h Converter.cpp DirExplorerN.h DirExplorerN.cpp
*  Display.h Display.cpp Logger.h Logger.cpp Loader.h Loader.cpp
*
*  Maintainence History:
* =======================
*  ver 1.2 - 09 April 2019
*  - Modified the code as per the requirement of Project 3.
*    Now it Publisher Class inherited from IPublisher so that
*    it can be used to communicate to WPF Application.
*
*  ver 1.1 - 14 March 2019
*  - Modified the code as per the requirement of Project 2.
*    Now it uses Loader Package to extract the files matching 
*    patterns and regeular expression mentioned in Command Line 
*    Arguments. Also modified to display new requirements of Project 2.
*  ver 1.0 - 14 Feb 2019
*  - first release
*/

#include <string>
#include <vector>

#include "../Converter/Converter.h"
#include "../DisplayPages/Display.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"

class CodePublisher
{
public:
  CodePublisher();
  bool processCommandLineArgs(int argc, char ** argv);
  
  void workingDirectory(const std::string& dir);
  const std::string& workingDirectory() const;

  void outputDirectory(const std::string& dir);
  const std::string& outputDirectory() const; 

  bool extractFiles(int argc, char ** argv);

  std::vector<std::string> extractedFiles();
  

  std::vector<std::string> publish();
  void publish(const std::string& file);
  void publish(const std::vector<std::string>& files);
  void demoReq78();
  CodeUtilities::DisplayMode displayMode() const;

private:
  CodeUtilities::ProcessCmdLine *pcl_;
  CodeConverter cconv_;
  Display display_;

  std::string dirIn_; 
  std::string dirOut_;

  std::vector<std::string> files_;
};

