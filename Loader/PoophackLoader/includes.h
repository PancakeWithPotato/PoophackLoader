#pragma once
#include <iostream>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <urlmon.h>
#include <tchar.h>
#include <WinInet.h>
#include <direct.h>
#include <fstream>
#pragma comment(lib, "urlmon")
#pragma comment(lib, "WinInet")



namespace User
{
	inline std::string Getname();
	inline std::string strName{ User::Getname() }; //assign username to it
	inline std::string strVersion;
}

namespace Process {
	DWORD Find(std::string strProcname);
	bool Inject(DWORD PID, std::string path);
}

namespace Files
{
	void Download(TCHAR path[], TCHAR url[]);
	bool CheckFolder(std::string path); //this checks if the folder exists, and creates it if it doesnt
}
