#define IN_DLL

#include "IPublisher.h"
#include <vector>
#include <string>
#include <iostream>  // std::cout 
#include "../Converter/Converter.h"

// Publisher - this is the class we wish to use in the WPF application
class Publisher : public IPublisher {
public:
	Publisher();
	~Publisher() override;

	// converts the list of files
	void convertFiles(std::vector<std::string> files) override;
	//fetches list of converted files
	std::vector<std::string> getConvertedFiles() override;
	
private:
	const std::vector<std::string> fetchwebFiles();
};

// -----< Function to convert the list of matching files >----------------------------------
void Publisher::convertFiles(std::vector<std::string> files)
{
	CodeConverter cconv_;
	cconv_.convert(files);
}

// -----< Function to fetch list of converted files >----------------------------------
std::vector<std::string> Publisher::getConvertedFiles()
{
	return fetchwebFiles();
}


// -----< Function to fetch web Files >----------------------------------
const std::vector<std::string> Publisher::fetchwebFiles()
{
	CodeConverter cconv_;
	return cconv_.convertedFiles();
}

// -----< constructor for Publisher Class >----------------------------------
Publisher::Publisher() {
	std::cout << "\n  Publisher instance created. ";
}

// -----< destructor for Publisher Class >-----------------------------------
Publisher::~Publisher()
{
	std::cout << "\n  Publisher instance destroyed";
}

// -----< Factory for returning Publisher as IPublisher* >--------------
IPublisher* ObjectFactory::createPublisher() {
	return new Publisher;
}
