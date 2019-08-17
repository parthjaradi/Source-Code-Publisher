///////////////////////////////////////////////////////////////////////////
// Converter.cpp : Implements and Provides test stub for Converter.h     //
// ver 1.1                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 3                                     //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Parth Jaradi
//                 Source : Ammar Salman, Instructor  Solution           //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////

#include "Converter.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"
#include "../CppParser(1)/Utilities/Utilities.h"
#include "../CppParser(1)/Tokenizer/Toker.h"
#include "../CppParser(1)/SemiExpression/Semi.h"
#include "../CppParser(1)/Parser/Parser.h"
#include "../CppParser(1)/Parser/ActionsAndRules.h"
#include "../CppParser(1)/Parser/ConfigureParser.h"
#include "../CppParser(1)/ScopeStack/ScopeStack.h"
#include "../Dependencies/Dependencies.h"
#include "../Display/supportDisplay.h"

using namespace FileSystem;
using namespace Logging;

// -----< default ctor >--------------------------------------------------
CodeConverter::CodeConverter()
{
}

// -----< ctor to set dep table >-----------------------------------------
CodeConverter::CodeConverter(const DependencyTable & dt) : dt_(dt)
{
  LoggerDebug::write("\n  CodeConverter initialized with DependencyTable\n");
}

// -----< set dependency table function >---------------------------------
void CodeConverter::setDepTable(const DependencyTable & dt)
{
  dt_ = dt;
}

// -----< convert pre-set dependency table >------------------------------
std::vector<std::string> CodeConverter::convert()
{
  if (createOutpurDir()) {
    LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
    LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
  }
  LoggerDemo::write("\n\n  Converting files to webpages");
  LoggerDebug::write("\n\n  Converting files to webpages");

  LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_) + "\n");

  for (auto entry : dt_) {
    if (convertFile(entry.first)) {
      LoggerDemo::write("\n  -- Converted: ");
      LoggerDebug::write("\n  -- Converted: ");
    }
    else {
      //LoggerDemo::write("\n  -- Failed:    ");
      LoggerDebug::write("\n  -- Failed:    ");
    }

    std::string filename = Path::getName(entry.first);
    LoggerDebug::write(filename);
    LoggerDemo::write(filename);
  }
  LoggerDebug::write("\n");
  LoggerDemo::write("\n");
  return convertedFiles_;
}

// -----< convert single file given path >----------------------------------
std::string CodeConverter::convert(const std::string & filepath)
{
  if (createOutpurDir()) {
    LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
    LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
  }

  LoggerDemo::write("\n\n  Converting files to webpages");
  LoggerDebug::write("\n\n  Converting files to webpages");

  LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

  if (convertFile(filepath)) {
    //LoggerDemo::write("\n  -- Converted: ");
    LoggerDebug::write("\n  -- Converted: ");
  }
  else {
    //LoggerDemo::write("\n  -- Failed:    ");
    LoggerDebug::write("\n  -- Failed:    ");
  }
  std::string filename = Path::getName(filepath);
  LoggerDebug::write(filename);
  LoggerDemo::write(filename);

  return filename + ".html";
}

std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
  clear();
  dt_ = DependencyTable(files);
  return convert();
}

// -----< get set output directory >----------------------------------------
const std::string & CodeConverter::outputDir() const
{
  return outputDir_;
}

// -----< set output directory >--------------------------------------------
void CodeConverter::outputDir(const std::string & dir)
{
  outputDir_ = dir;
}

// -----< create output directory >-----------------------------------------
/* returns success of creation. it could fail if dir is sys protected */
bool CodeConverter::createOutpurDir()
{
  if (!Directory::exists(outputDir_)) {
    LoggerDebug::write("\n  Creating output directory. Path: " + Path::getFullFileSpec(outputDir_));
    return Directory::create(outputDir_);
  }
  LoggerDebug::write("\n  Output directory already exists. Proceeding...");
  return true;
}

// -----< get list of converted files >-------------------------------------
std::vector<std::string> CodeConverter::convertedFiles()
{
  return convertedFiles_;
}

void CodeConverter::clear()
{
  convertedFiles_.clear();
  dt_.clear();
}

// -----< private - read file and create webpage >--------------------------
bool CodeConverter::convertFile(std::string file){
  std::ifstream in(file);
  if (!in.is_open() || !in.good()) {
    LoggerDebug::write("Error -- unable to read file, path may be invalid.");
    in.close(); return false;
  }
  std::string filename = Path::getName(file); 
  std::string outputPath = outputDir_ + filename + ".html";
  out_.open(outputPath, std::ofstream::out);
  if (!out_.is_open() || !out_.good()) {
    LoggerDebug::write("Error -- unable to open output file for writing.");
    in.close(); return false;
  }
  fileData_ = parseFile(file); //parse File
  addDependencyFiles(file,fileData_); //add Dependent Files
  addPreCodeHTML(filename); addDependencyLinks(dt_,file); addPreTag();
  while (in.good()) {
		int lineCount = 0; std::string line;
		while (std::getline(in, line)) {
			bool cFlag=0;
			lineCount++;
			skipSpecialChars(line);
			cFlag = toggleComments(line);
			bool dflag = 0;
			if ((line.find("{") == line.size() - 1) && line.size() == 1)
			{
				addStartingDiv(lineCount, fileData_,line);dflag = 1; //Div tags Addition
			}
			else if (line.find("{") !=std::string::npos)
			{
				std::vector<int>::iterator itr;
				for (itr = startingDivPositions_.begin(); itr != startingDivPositions_.end(); itr++)
				{
					if (*itr == lineCount)
					{
						dflag = 1;addStartingDiv(lineCount, fileData_, line); //Div tags Addition
						break;
					}
				}
			}
			if (dflag == 0)
				out_ << line;
			addEndingDiv(lineCount);
			if(cFlag==0)
				out_ << std::endl;
		}
  }
  addClosingTags(); out_.close(); startingDivPositions_.clear();
   endingDivPositions_.clear(); convertedFiles_.push_back(outputPath);  return true;
}
  

// -----< private - add generic HTML preliminary markup >-------------------
void CodeConverter::addPreCodeHTML(const std::string& title)
{
  out_ << "<!DOCTYPE HTML>" << std::endl;
  out_ << "<html>" << std::endl;
  out_ << "  <head>" << std::endl;
  out_ << "    <Title>" << title << "</Title>" << std::endl;
  out_ << "    <style>" << std::endl;
  out_ << "     .comments {" << std::endl;
  out_ << "     color: green }" << std::endl;
  out_ << "      h3 {" << std::endl;
  out_ << "        font-family: Cambria,Consolas;" << "text-align: center;"  << "color: #f4602e;"<<std::endl;
  out_ << "      }" << std::endl;
  out_ << "      h4 {" << std::endl;
  out_ << "        font-family: Cambria,Consolas;" << std::endl;
  out_ << "      }" << std::endl;
  out_ << "      body {" << std::endl;
  out_ << "        padding:0px 10px;" << std::endl;
  out_ << "        font-family: Cambria,Consolas;" << std::endl;
  out_ << "        font-size: 0.9375em;" << std::endl;
  out_ << "        font-weight: normal;" << std::endl;
  out_ << "      }" << std::endl;
  out_ << "      .commentsBtn,.classBtn,.functionBtn {" << std::endl;
  out_ << "      background-image: linear-gradient(to right, rgba(208, 212, 219,0), rgba(208, 212, 219,0.35));" << std::endl;
  out_ << "      border: none;" << std::endl;
  out_ << "      color: white;" << std::endl;
  out_ << "      padding: 15px 32px;" << std::endl;
  out_ << "      text - align: center;" << std::endl;
  out_ << "      text - decoration: none;" << std::endl;
  out_ << "      display: inline - block;" << std::endl;
  out_ << "      margin: 4px 2px;" << std::endl;
  out_ << "      cursor: pointer;" << std::endl;
  out_ << "      }" << std::endl;
  out_ << "      </style>" << std::endl;
  out_ << "  <script src=\"myscripts.js\"></script> " << std::endl;
  out_ << "  </head>" << std::endl << std::endl;
  out_ << "  <body>";
  
}

// -----< private - add pre tag >------------------------------------------
/* seperated into seperate function to allow for dependencies addition
*  before the actual code of the file */
void CodeConverter::addPreTag()
{
  out_ << "    <pre>" << std::endl;
}

// -----< private - add depedency links markup code >----------------------
void CodeConverter::addDependencyLinks(DependencyTable& dt,std::string file)
{
	Dependencies dp;
	dp.addDependencyLinks(dt_,file,out_);
}



// -----< private - add generic HTML markup closing tags >-----------------
void CodeConverter::addClosingTags()
{
  out_ << "    </pre>" << std::endl;
  out_ << "  </body>" << std::endl;
  out_ << "</html>" << std::endl;
}

// -----< private - replace HTML special chars >---------------------------
/* note: the function uses while loop to make sure ALL special characters
*  are replaced instead of just the first encounter. */
void CodeConverter::skipSpecialChars(std::string & line)
{
  size_t pos = line.npos;
  while((pos = line.find('<')) != line.npos) 
    line.replace(pos, 1, "&lt;");

  while ((pos = line.find('>')) != line.npos)
    line.replace(pos, 1, "&gt;");
}


// --------< Parse File - Function to Parse Source Code File - Using Parser Package >-------------------
std::set<std::string> CodeConverter::parseFile(std::string file) {
	std::string fileSpec = FileSystem::Path::getFullFileSpec(file);
	std::string msg = "Processing file" + fileSpec;

	CodeAnalysis::ConfigParseForCodeAnal configure;
	CodeAnalysis::Parser* pParser = configure.Build();

	std::string name;	std::set<std::string> listOfFiles;
	try
	{
		if (pParser)
		{
			name = FileSystem::Path::getName(file);
			if (!configure.Attach(fileSpec))
			{
				std::cout << "\n  could not open file " << name << std::endl;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
		}
		// save current package name
		CodeAnalysis::Repository* pRepo = CodeAnalysis::Repository::getInstance();
		pRepo->package() = name;
		// parse the package
		while (pParser->next())
		{
			pParser->parse();
		}
		
		// final AST operations
		CodeAnalysis::ASTNode* pGlobalScope = pRepo->getGlobalScope();
		// walk AST, computing complexity for each node
		// and record in AST node's element
		CodeAnalysis::complexityEval(pGlobalScope);
		// Walk AST, displaying each element, indented
		// by generation using modified TreeWalk Function
		listOfFiles = CodeAnalysis::newTreeWalk(pGlobalScope);
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		std::cout << "\n  exception caught at line " << __LINE__ << " ";
		std::cout << "\n  in package \"" << name << "\"";
	}
	
	return listOfFiles;}


// ----------< Function to add Dependency Files to Dependency Table >----------
void CodeConverter::addDependencyFiles(std::string file,std::set<std::string> fileData) {
	std::string filename = Path::getName(file);
	std::set<std::string>::iterator itr;
	std::string temp;
	std::size_t startPos,endPos;
	for (itr = fileData.begin(); itr != fileData.end(); itr++)
	{
		if ((*itr).find("include") != std::string::npos && (*itr).find("/") != std::string::npos)
		{
			startPos = (*itr).find_last_of("/");
			endPos = (*itr).find_last_of("\"");
			temp = (*itr).substr(startPos + 1, endPos - startPos - 1);
			dt_.addDependency(filename,temp);
		}
		else if((*itr).find("include") != std::string::npos)
		{
			startPos = (*itr).find_first_of("\"");
			endPos = (*itr).find_last_of("\"");
			temp = (*itr).substr(startPos + 1, endPos - startPos - 1);
			dt_.addDependency(filename, temp);
		}
		else if ((*itr).find("Div") != std::string::npos) {
			startPos = (*itr).find_first_of(":");
			endPos = (*itr).find_last_of(":");
			temp = (*itr).substr(startPos + 1, endPos - startPos - 1);
			std::string::size_type sz;
			int sp = std::stoi(temp, &sz);
			temp = (*itr).substr(endPos + 1, temp.length()-1-endPos);
			int ep = std::stoi(temp, &sz);
			startingDivPositions_.push_back(sp);
			endingDivPositions_.push_back(ep);
		}
	}


}

// ----------< Function to store opening place Div Tags in HTML Page - Using Display Package >----------
void CodeConverter::addStartingDiv(int lineNumber,std::set<std::string> fileData,std::string line) {

	std::vector<std::string> lineData;
	lineData.push_back(std::to_string(lineNumber));
	lineData.push_back(line);
	supportDisplay suppDisplayObj;
	suppDisplayObj.addOpeningDivTags(lineData,fileData,out_);
	
}



// ----------< Function to store opening place Div Tags in HTML Page - Using Display Package >----------
void CodeConverter::addEndingDiv(int lineNumber) {
	supportDisplay suppDisplayObj;
	suppDisplayObj.addClosingDivTags(lineNumber,endingDivPositions_,out_);

}


// ----------< Function to toggle comments - Using Display Package >----------
bool CodeConverter::toggleComments(std::string& line) {
	supportDisplay suppDisplayObj;
	bool res = suppDisplayObj.addCommentBlocks(line, commentsFlag_);
	return res;
}


// ----------< Function to show Requirements 7 and 8 >----------
void CodeConverter::showReq78() {
	Dependencies dp;
	dp.showReq7();
	supportDisplay sp;
	sp.showReq8();
}
#ifdef TEST_CONVERTER

// -----< test stub for converter class >-----------------------------------
int main() {
  LoggerDebug::attach(&std::cout);
  LoggerDebug::start();

  LoggerDebug::title("Testing CodeConverter functions");

  LoggerDebug::stop(); // do not log DependencyTable logs
  
  DependencyTable dt;
  dt.addDependency("Converter.h", "Converter.cpp");
  dt.addDependency("Converter.cpp", "Converter.h");
  dt.addDependency("A.h", "A.h");
  
  LoggerDebug::start(); // log from now on

  CodeConverter cc(dt);
  dt.display();
  LoggerDebug::write("\n");
  //cc.outputDir("C:\\windows\\system32\\"); // will trigger errors which are handled
  cc.convert();

  LoggerDebug::stop();
  return 0;
}

#endif

