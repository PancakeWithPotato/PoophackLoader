#pragma once
#include <iostream>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <urlmon.h>
#include <WinInet.h>
#include <direct.h>
#include <fstream>
#include <vector>
#include <filesystem>
#include <array>


#pragma comment(lib, "urlmon")
#pragma comment(lib, "WinInet")
#pragma comment(lib, "gdi32")

namespace file = std::filesystem;

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
	bool Download(std::string path, std::string url);
	bool CheckFolder(std::string path); //this checks if the folder exists, and creates it if it doesnt
	bool CheckFile(std::string path);
	void WriteConfig(int line, std::string entry); //this is horrible configing, but for this project it's fine
	bool InstallFonts(std::array<std::string, 13> path, std::array<std::string, 13> url, std::string folder);
	bool SetupFonts(std::array<std::string, 13>& paths, std::array<std::string, 13>& urls);

	inline std::array<std::string, 13> strFonts = {"BITSUMIS.TTF", "Monkey.otf", "Pixels.ttf", "Poop-Regular.ttf", "Rainbow.otf", "Road_Rage.otf", "Skygraze.otf", "StarsHandwriting-Regular.ttf", //pancake not using c-type array (unreal)
		"Tab_icons-Regular_4.ttf", "airstrike.ttf", "dogicapixel.otf", "icons.ttf", "undefeated.ttf" };
	inline int iCounter = 0;

	inline std::vector<std::string> strFiles;

}


