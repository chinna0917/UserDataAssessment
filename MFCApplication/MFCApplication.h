
// MFCApplication.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/allocators.h"

#include <iostream>
#include <string>
#include "curl/curl.h"
#include "api_helper.h"
#include "vector"
#include "map"
// CMFCApplicationApp:
// See MFCApplication.cpp for the implementation of this class
//


static std::vector<std::string>IdList;
using namespace std;
struct location
{
	string street;
	string city;
	string state;
	string country;
	string timezone;

};
struct attributes
{
	string title;
	string fName;
	string lName;
	string pic;
	string genter;
	string email;
	string dob;
	string mbl;
	string regDate;
	string updateDate;
	location loc;
};

static std::map<string, attributes>userData;

class CMFCApplicationApp : public CWinApp
{
public:
	CMFCApplicationApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationApp theApp;
