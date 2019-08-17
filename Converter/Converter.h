#pragma once
///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 1.1                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 3                                     //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Parth Jaradi                                          //
//                 Source : Ammar Salman Instructor Solution             //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodeConverter class which accepts DependencyTable
*  and uses its relationships to create linked webpages that point to each
*  other based on dependencies. It can also convert single files if needed.
*  The conversion process filters HTML special characters before printing
*  them into output files. The resulting output of this converter is a list
*  of files (vector<string>) of the created webpages. 
*
*  Required Files:
* =======================
*  Converter.h Converter.cpp DependencyTable.h DependencyTable.cpp
*  Logger.h Logger.cpp FileSystem.h FileSystem.cpp 
*  Dependencies.h Dependencies.cpp supportDisplay.h supportDisplay.cpp
*  CppParser Package Files
*
*  Maintainence History:
* =======================
*  ver 1.1 - 14 March 2019
*  - Modified the package to include the functionality of hiding comments,
*  classes and functions present in a source code file.
*  ver 1.0 - 11 Feb 2019
*  - first release
*/

#include "../DependencyTable/DependencyTable.h"
#include <vector>
#include <string>
#include <fstream>
#include <set>
class CodeConverter
{
public:
  CodeConverter();
  CodeConverter(const DependencyTable& dt);

  const std::string& outputDir() const;
  void outputDir(const std::string& dir);
  bool createOutpurDir();

  void setDepTable(const DependencyTable& dt);

  std::vector<std::string> convert();
  std::string convert(const std::string& filepath);
  std::vector<std::string> convert(const std::vector<std::string>& files);
  
  std::vector<std::string> convertedFiles();
  void showReq78();

  void clear();

private:
  bool convertFile(std::string file);
  void addPreCodeHTML(const std::string& title);
  void addPreTag();
  void addDependencyLinks(DependencyTable& dt,std::string file);
  void addClosingTags();
  void skipSpecialChars(std::string& line);
  std::set<std::string> parseFile(std::string);	
  void addDependencyFiles(std::string, std::set<std::string>);
  void addStartingDiv(int, std::set<std::string>,std::string);
  void addEndingDiv(int);
  bool toggleComments(std::string& line);

private:
  DependencyTable dt_;
  std::string outputDir_ = "..\\SendFiles\\";
  std::vector<std::string> convertedFiles_;
  std::set<std::string> fileData_;
  std::ofstream out_;
  std::vector<int> startingDivPositions_;
  std::vector<int> endingDivPositions_;
  bool commentsFlag_;
};

