#pragma once


#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include<vector>
#include<string>

class IPublisher {
public:
	virtual void convertFiles(std::vector<std::string> files) = 0;
	virtual std::vector<std::string> getConvertedFiles() = 0;
	virtual ~IPublisher() {}

};

extern "C" {
	// Instantiates CppClass* as ICppClass*
	struct ObjectFactory {
		// See CppLibrary.cpp for implementation
		DLL_DECL IPublisher* createPublisher();
	};
}
