#pragma once
///////////////////////////////////////////////////////////////////////////
// Dependencies.h: Add links to the dependent source code files          //
//                 present in the directory tree                         //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 3                                     //
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
*  DependencyTable.h DependencyTable.coo
*
*  Maintainence History:
* =======================
*  ver 1.0 - 14 March 2019
*  - First Release
*/

#include<string>
#include "../DependencyTable/DependencyTable.h"

class Dependencies
{
public:
	Dependencies();
	~Dependencies();
	void addDependencyLinks(DependencyTable& dt,std::string file, std::ofstream& out);
	void showReq7();
};

