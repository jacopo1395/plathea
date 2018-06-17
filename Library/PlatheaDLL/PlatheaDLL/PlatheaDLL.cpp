// PlatheaDLL.cpp: definisce le funzioni esportate per l'applicazione DLL.


#include "stdafx.h"
#include <utility>
#include <limits.h>
#include "PlatheaDLL.h"


#include <LeoLog4CPP.h>
#include <LeoWindowsGUI.h>
#include <LeoWindowsConsole.h>
#include <LeoSettingsPersistence.h>


#include <Shlwapi.h>

// DLL internal state variables:

#include <Windows.h>
#include <iostream>
#include <sstream>


#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s << "\n";                   \
   OutputDebugStringW( os_.str().c_str() );  \
}


void system_loadconfigurationfile() {
	COMDLG_FILTERSPEC filtro[] = { L"XML Configuration File", L"*.xml" };
	/*
	if (UseCommonItemDialog(buf, sizeof(buf), hwnd, CLSID_FileOpenDialog, 0, filtro, 1)) {
		HCURSOR originalCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
		char currentDirectory[_MAX_PATH]; strcpy(currentDirectory, buf);
		PathRemoveFileSpecA(currentDirectory); SetCurrentDirectoryA(currentDirectory);
		SettingsPersistence::GetInstance()->LoadFromFile(buf);
		UpdateWindowTitle(hwnd);
		SetCursor(originalCursor);
		*/

}


