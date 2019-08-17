#pragma once
///////////////////////////////////////////////////////////////////////////
// Loader.h:       Finds and Loads the files matching the patterns and   //
//                 regular expression present in the path                //
//                 provided by Executive Package.                        //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 2                                     //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Parth Jaradi                                          //
//                                                                       //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Dependencies class which uses DependencyTable
*  relationships to create links to the dependent source code files that
*  based on dependencies.
*
*  Required Files:
* =======================
* Loader.h Loader.cpp DirExplorerN.h CodeUtilities.h
*
*  Maintainence History:
* =======================
*  ver 1.0 - 14 March 2019
*  - First Release
*/

#include<vector>
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"

class Loader
{
public:
	Loader();
	~Loader();
	std::vector<std::string> extractFiles(CodeUtilities::ProcessCmdLine* pcl);
private:
	std::vector<std::string> filesList_;
};

