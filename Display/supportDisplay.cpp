//////////////////////////////////////////////////////////////////////////////////
// supportDisplay.cpp : Implements and provides Test Stub For supportDisplay.h  //
//                                                                              //
// ver 1.0                                                                      //
//                                                                              // 
// Application   : OOD-S19 Project 3                                            //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64             //
// Author        : Parth Jaradi                                                 //
//                                                                              //
//                                                                              // 
//////////////////////////////////////////////////////////////////////////////////

#include "supportDisplay.h"

supportDisplay::supportDisplay()
{
}

supportDisplay::~supportDisplay()
{
}

// ------------< Function to show Requirement 8 >------------
void supportDisplay::showReq8() {

	std::cout << "\n Requirement #8 - Display Package";
	std::cout << "\n --------------------------------------";
	std::cout << "\n Adding functionality show/hide comments to the HTML File";
}

// ------------< Function to add Show/Hide Comments to Source Code Web Page >------------
bool supportDisplay::addCommentBlocks(std::string& line,bool& commentsFlag) {

	std::size_t pos, cPos;
	pos = line.find('/');
	cPos = line.find_last_of('*');
	if (pos == std::string::npos)
		return 0;
	if (pos == line.size() - 1 && line[pos - 1] != '*')
		return 0;
	if (line[pos + 1] == '/')
	{
		line.replace(pos, 1, "<div class = \"comments\">");
		line += "</div>";
		//std::cout << "OUTPT" << std::endl << std::endl;
		//std::cout << line << std::endl << std::endl;
		return 1;
	}
	if (line[pos + 1] == '*' && commentsFlag != 1)
	{
		line.replace(pos, 1, "<div class = \"comments\">");
		commentsFlag = 1;
		cPos = line.find_last_of('*');
		if (line[cPos + 1] == '/')
		{
			line += "</div>";
			commentsFlag = 0;
			return 0;
		}

		return 0;
	}
	if (line[cPos + 1] == '/')
	{
		line += "</div>";
		commentsFlag = 0;
		return 0;
	}
	commentsFlag = 0;
	return 0;
}

// ------< Function to add Opening Tags to Show/Hide Function/Class Blocks to Source Code Web Page >-------
void supportDisplay::addOpeningDivTags(std::vector<std::string>lineData, std::set<std::string> fileData, std::ofstream& out) 
{
	std::string::size_type sz;
	int lineNumber = std::stoi(lineData[0], &sz);
	std::string line = lineData[1];
	std::vector<int> startingDivPositions, endingDivPositions;
	std::vector<int>::iterator itr;
	std::set<std::string>::iterator itr2;
	std::size_t pos,startPos,endPos;
	std::string classType, temp;
	for (itr2 = fileData.begin(); itr2 != fileData.end(); itr2++) {
		if ((*itr2).find(std::to_string(lineNumber)) != std::string::npos) {
			pos = (*itr2).find_first_of(':');
			classType = (*itr2).substr(3, pos - 3);
			if (classType == "class")
				classType = "cclass";
		}
		if ((*itr2).find("Div") != std::string::npos) {
			startPos = (*itr2).find_first_of(":");
			endPos = (*itr2).find_last_of(":");
			temp = (*itr2).substr(startPos + 1, endPos - startPos - 1);
			std::string::size_type sz;
			int sp = std::stoi(temp, &sz);
			temp = (*itr2).substr(endPos + 1, temp.length() - 1 - endPos);
			startingDivPositions.push_back(sp);
		}
	}
	for (itr = startingDivPositions.begin(); itr != startingDivPositions.end(); itr++)
	{
		if (*itr == lineNumber && *itr != 1)
		{
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] == '{')
					break;
				out << line[i];
			}
			
			out << "<div class=" << classType << " id=" << lineNumber << ">";
			pos = line.find('{');
			while (pos < line.size()) {
				out << line[pos];
				pos++;
			}
		}
	}
}

// ------< Function to add Opening Tags to Show/Hide Function/Class Blocks to Source Code Web Page >-------
void supportDisplay::addClosingDivTags(int lineNumber, std::vector<int> endingDivPositions, std::ofstream& out) 
{

	std::vector<int>::iterator itr;
	for (itr = endingDivPositions.begin(); itr != endingDivPositions.end(); itr++)
	{
		if (*itr == lineNumber && * itr != 1)
		{
			out << "</div>";
		}
	}
}


//--------------< Test Stub For Display Package >----------------
#ifdef TEST_SUPPORTDISPLAY
int main() {

	supportDisplay sd;
	std::vector<std::string> lineData;
	std::set<std::string> fileData;
	std::ofstream out;
	std::vector<int> endingDivPositions, startingDivPositions;
	out.open("../ConvertedWebPages/supportDisplayTest.html", std::ofstream::out);
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
	out << "  <body>" << std::endl << "<pre>" << std::endl;

	fileData.insert("#include \"anc.h\"");
	fileData.insert("Divnamespace:1:1");
	fileData.insert("Divfunction:6:8");
	endingDivPositions.push_back(1);
	endingDivPositions.push_back(8);
	startingDivPositions.push_back(1);
	startingDivPositions.push_back(6);
	std::ifstream in("../checkSupportDisplay.h");
	std::set<std::string>::iterator itr;
	if (!in.is_open() || !in.good()) {
		std::cout<<"Error -- unable to read file, path may be invalid.";
		in.close(); std::getchar(); return false;
	}
	while (in.good()) {
		int lineCount = 0; std::string line;
		while (std::getline(in, line)) {
			bool cFlag = 0;
			lineCount++;
			cFlag = sd.addCommentBlocks(line,cFlag);
			bool dflag = 0;
			lineData.push_back(std::to_string(lineCount));
			lineData.push_back(line);
			if ((line.find("{") == line.size() - 1) && line.size() == 1)
			{
				sd.addOpeningDivTags(lineData, fileData, out); dflag = 1;
			}
			else if (line.find("{") != std::string::npos)
			{
				std::vector<int>::iterator itr;
				for (itr = startingDivPositions.begin(); itr != startingDivPositions.end(); itr++)
				{
					if (*itr == lineCount)
					{
						
						dflag = 1; sd.addOpeningDivTags(lineData, fileData, out);
						
						break;
					}
				}
			}
			if (dflag == 0)
				out << line;
			sd.addClosingDivTags(lineCount,endingDivPositions,out);
			if (cFlag == 0)
				out << std::endl;
			lineData.clear();
		}
	}
	out << "  </pre>" << std::endl << "</body>" << std::endl << "</html>";
	std::getchar();
	return 1;
}

#endif