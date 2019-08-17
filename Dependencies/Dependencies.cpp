///////////////////////////////////////////////////////////////////////////////
// Dependencies.cpp : Implements and provides Test Stub for Dependencies.h   //
//                    present in the directory tree                          //
// ver 1.0                                                                   //
//                                                                           // 
// Application      : OOD-S19 Project 3                                      //
// Platform         : Visual Studio Community 2017, Windows 10 Pro x64       //
// Author           : Parth Jaradi                                           //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include "Dependencies.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Logger/Logger.h"

Dependencies::Dependencies()
{
}


Dependencies::~Dependencies()
{
}

// ------------< Function to show Requirement 7 >------------
void Dependencies::showReq7() {

	std::cout << "\n Requirement #7 - Dependencies Package";
	std::cout << "\n --------------------------------------";
	std::cout << "\n Adding Dependency Links to the HTML File";

}

// ------------< Function to add Dependency Links to Source Code Web Page >------------
void Dependencies::addDependencyLinks(DependencyTable& dt,std::string file,std::ofstream& out) {

	std::string filename = FileSystem::Path::getName(file);
	if (!dt.has(filename)) { // in case of single file conversion
		Logging::LoggerDebug::write("\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links..");
		

		out << "    <h3>" << "Source Code File : " << filename << std::endl << "</h3>";
		//out_ << "<input type = \"button\" id = \"divBtn\" onclick = toggleDiv() value = \"Hide Div\">" << "</input>";
		out << "<input type = \"button\" id = \"commentsBtn\" onclick = toggleComments() value = \"Hide Comments\">";
		out << "<input type = \"button\" id = \"functionBtn\" onclick = toggleFunctions() value = \"Hide Functions\">";
		out << "<input type = \"button\" id = \"classBtn\" onclick = toggleClasses() value = \"Hide Classes\">";
		return;
	}

	if (dt[filename].size() == 0) { // in case the file has no dependencies
		Logging::LoggerDebug::write("\n    No dependencies found for [" + filename + "]. Skipping dependency links..");
		
		out << "    <h3>" << "Source Code File : " << filename << std::endl << "</h3>";
		//out_ << "<input type = \"button\" id = \"divBtn\" onclick = toggleDiv() value = \"Hide Div\">" << "</input>";
		out << "<input type = \"button\" id = \"commentsBtn\" onclick = toggleComments() value = \"Hide Comments\">";
		out << "<input type = \"button\" id = \"functionBtn\" onclick = toggleFunctions() value = \"Hide Functions\">";
		out << "<input type = \"button\" id = \"classBtn\" onclick = toggleClasses() value = \"Hide Classes\">";
		return;
	}

	
	out << "    <h3>" << "Source Code File : " << filename << "<br/> </h3>" << std::endl;
	out << "    <h4> " << "Dependencies: ";
	for (auto dep : dt[filename]) {
		out << "      <a href=\"" << FileSystem::Path::getName(dep) << ".html\">" << FileSystem::Path::getName(dep) << "</a>" << std::endl;
	}
	out << "    </h4>";
	//out_ << "<input type = \"button\" id = \"divBtn\" onclick = toggleDiv() value = \"Hide Div\">" << "</input>";
	out << "<input type = \"button\" id = \"commentsBtn\" onclick = toggleComments() value = \"Hide Comments\">";
	out << "<input type = \"button\" id = \"functionBtn\" onclick = toggleFunctions() value = \"Hide Functions\">";
	out << "<input type = \"button\" id = \"classBtn\" onclick = toggleClasses() value = \"Hide Classes\">";

}



//--------------< Test Stub For Dependencies Package >----------------
#ifdef TEST_DEPENDENCIES
int main() {

	Dependencies dp;
	std::ofstream out;
	std::string file = "../checkSupportDisplay.h";
	out.open("../ConvertedWebPages/checkDependencies.html", std::ofstream::out);
	out << "<!DOCTYPE HTML>" << std::endl;
	out << "<html>" << std::endl;
	out << "  <head>" << std::endl;
	out << "    <Title>" << "supportDisplayTest" << "</Title>" << std::endl;
	out << "    <style>" << std::endl;
	out << "      body {" << std::endl;
	out << "        padding:15px 10px;" << std::endl;
	out << "        font-family: Consolas;" << std::endl;
	out << "        font-size: 1.25em;" << std::endl;
	out << "        font-weight: normal;" << std::endl;
	out << "      }" << std::endl;
	out << "      </style>" << std::endl;
	out << "  <script src=\"myscripts.js\"></script> " << std::endl;
	out << "  </head>" << std::endl << std::endl;
	out << "  <body>" << std::endl;

	DependencyTable dt;
	dt.addDependency("checkSupportDisplay.h","Converter.h");
	dp.addDependencyLinks(dt,file,out);
	
	out << "</body>" << std::endl << "</html>";
	std::getchar();
	return 1;
}
#endif 
