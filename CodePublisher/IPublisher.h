/////////////////////////////////////////////////////////////////////////////
// IPublisher.h  : defines an interface to a worker C++ class             //
// ver 1.0                                                                 //
//                                                                         //
// Application    : CSE-687 OOD Spring 2019 Project 3                      //
// Platform       : Visual Studio 2017 Community Edition                   //
//                  Windows 10 Professional 64-bit                         //
// Author         : Parth Jaradi                                           //
// Source         : Ammar Salman, OOD TA, Syracuse University              //
//                  313/788-4694, hoplite.90@hotmail.com assalman@syr.edu  //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Description:
* ======================
*  We need to use the C++ class in C++/CLI environment and under /CLR
*  code. To do so, we created an interface (IPublisher shown below) that
*  declares the methods we want the C++/CLI Shim to use. The concept of
*  inheritance allows the use of an interface pointer (IPublisher*) to
*  point into any derived (or implementation) class objects (Publisher).
*  Therefore, we declared an object factory that returned IPublisher*
*  and implemented the function in the .cpp file to return IPublisher*.
*  This way, the C++/CLI Shim will be able to use CppClass through
*  its interface without any knowledge about it and hence Publihser'
*  CLR incompatible types are of no issue anymore.
*
*  The ObjectFactory (shown below) can be used without the 'extern "C"'
*  marker but it is not recommended to do so. The marker will prevent
*  the compiler from doing any namemangeling on the function allowing
*  the use of PInvoke if needed. For this application it doesn't make
*  a difference so you can use either.
*
*  The created messages are meant to be shown in the WPF application.
*  the received messages are sent by the WPF application and displayed
*  on the console to demonstrate that native C++ code and C# code are
*  executing in the same program.
*
*  IMPORTANT NOTE: you cannot make an abstract declaration of CppClass like:
*                    class CppClass;
*                  This will make make Shim fail to build since it will not
*                  be able to find the real declaration and definitions of
*                  CppClass. The only way for Shim to use Publisher is through
*                  IPublisher pointer.
*
*  Public Interface:
* ===================
*  ObjectFactory factory;
*  IPublisher* instance = factory.createPublisher();
*
*  Required Files:
* =================
*  IPublisher.h CodePublisher.cpp
*
*  Build Command:
* ================
*  msbuild Executive.vcxproj
*
*  Maintainence History:
* =======================
*  ver 1.0 - April 09 2019
*    - first release
*
*/


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
	
	virtual void SendCmdLine(int argc, char** argv) = 0;
	virtual std::vector<std::string> getConvertedFiless()=0;
	virtual ~IPublisher() {}

};

extern "C" {
	// Instantiates CppClass* as ICppClass*
	struct ObjectFactory {
		// See CppLibrary.cpp for implementation
		DLL_DECL IPublisher* createPublisher();
	};
}


