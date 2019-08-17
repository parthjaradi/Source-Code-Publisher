///////////////////////////////////////////////////////////////////////////
// Loader.cpp:       Implements and provides Test Stub for Loader.h      //
//                                                                       //

// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 2                                     //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Parth Jaradi                                          //
//                                                                       //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
#include "Loader.h"


Loader::Loader()
{
}


Loader::~Loader()
{
}

std::vector<std::string> Loader::extractFiles(Utilities::ProcessCmdLine* pcl) {

	FileSystem::DirExplorerN de(pcl->path());

	for (auto patt : pcl->patterns())
	{
		de.addPattern(patt);
	}

	if (pcl->hasOption("s"))
	{
		de.recurse();
	}

	bool res = de.search() && de.match_regexes(pcl->regexes());
	de.showStats();
	filesList_ = de.filesList();
	return filesList_;

}

// ---------< Test Stub For Loader Package >---------------
#ifdef TEST_LOADER
int main(int argc, char ** argv) {
	Loader l;
	Utilities::ProcessCmdLine* p = new Utilities::ProcessCmdLine(argc, argv);
	std::vector<std::string> filesList = l.extractFiles(p);
	bool res = filesList.empty();
	if (res) {
		std::cout << "No Files Found";
		std::getchar();
		return 0;
	}	
	std::cout << "Files Proccessed"<<std::endl;
	std::cout << "----------------" << std::endl;
	std::vector<std::string>::iterator itr;
	for (itr = filesList.begin(); itr != filesList.end(); itr++)
		std::cout << *itr << std::endl;
	std::getchar();
	return 1;
}
#endif 

