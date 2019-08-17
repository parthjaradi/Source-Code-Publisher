#pragma once
///////////////////////////////////////////////////////////////////////////
// supportDisplay.h: Add functionality to Show/Hide Comments, Functions  //
//                   and Classes in a Converted WebPage                  //
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
*  This package provides the functionality to Show/Hide Comments, Functions
*  and Classes in a Converted WebPage.
*
*  Required Files:
* =======================
*  supportDisplay.h
*
*  Maintainence History:
* =======================
*  ver 1.0 - 14 March 2019
*  - First Release
*/

#include<iostream>
#include<string>
#include<set>
#include<vector>
#include<fstream>

class supportDisplay
{
public:
	supportDisplay();
	~supportDisplay();
	bool addCommentBlocks(std::string &line, bool &commentsFlag);
	void addOpeningDivTags(std::vector<std::string>lineData, std::set<std::string> fileData, std::ofstream& out);
	void addClosingDivTags(int lineNumber,std::vector<int> endendingDivPositions, std::ofstream& out);
	void showReq8();
};

