// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
//#include "AsProfiled_i.h"
#include "dllmain.h"

CAsProfiledModule _AtlModule;

class CAsProfiledApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAsProfiledApp, CWinApp)
END_MESSAGE_MAP()

CAsProfiledApp theApp;

BOOL CAsProfiledApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CAsProfiledApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
