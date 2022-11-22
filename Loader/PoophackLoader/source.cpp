#include "includes.h"

std::string User::Getname()
{
	char tmp[MAX_PATH];
	DWORD lenght = MAX_PATH + 1;
	GetUserNameA(tmp, &lenght); //get the user name
	return std::string(&tmp[0]); //convert it to string and return it as one
}

DWORD Process::Find(std::string strProcname)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!hSnap || hSnap == INVALID_HANDLE_VALUE) //failed to open handle, print some info for the user then return 0 (our error)
	{
		std::cout << "[!] Opening snapshot handle failed with error => " << GetLastError() << std::endl;;
		CloseHandle(hSnap);
		return 0;
	}
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnap, &entry))
	{
		std::cout << "[!] Failed to get information about first process, error => " << GetLastError() << std::endl;
		CloseHandle(hSnap);
		return 0;
	}
	//at this point it has to be valid
	while (Process32Next(hSnap, &entry)) //runs till there is any process stored
	{
		if (!_strcmpi(entry.szExeFile, strProcname.c_str())) //compares the name of processes with our string
		{
			std::cout << "Found " << strProcname << " with PID " << entry.th32ProcessID << "!\n";
			//close the handle and return the process id of the process
			CloseHandle(hSnap);
			return entry.th32ProcessID;
		}
	}
	return 0; //we will return 0 if we cant find the process
}

bool Files::Download(std::string path, std::string url)
{
	DeleteUrlCacheEntry(url.c_str());
	std::cout << "starting download...\n";
	HRESULT res = URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL); //download the file
	if (res == S_OK)
	{
		std::cout << "Succesfully downloaded!\n"; //succesfully download
		return true;
	}
	else //any other error
	{
		std::cout << std::hex << "[!] other error, res: 0x" << res << std::endl;
		return false;
	}
	return false;
}

bool Files::CheckFolder(std::string path)
{
	if (!_mkdir(path.c_str())) {
		return false;
	}
	return true;
}

bool Files::CheckFile(std::string path)
{
	std::ifstream file{ path.c_str() };
	if (file.is_open())
		return true;
	std::ofstream{ path.c_str() };
	return false;
}

void Files::WriteConfig(int line, std::string entry)
{
}

bool Files::InstallFonts(std::array<std::string, 13> path, std::array<std::string, 13> url, std::string folder)
{
	//!!!this code is horrible, proceed with caution!!! 
	for (size_t i = 0; i < 13; i++)
	{
		if (!Files::Download(path[i], url[i]))
			std::cout << "[!] Failed to download " << url[i] << ", error => " << GetLastError() << std::endl;
	}

	for (auto& entry : file::directory_iterator(folder))
		Files::strFiles.emplace_back(entry.path().generic_string());

	for (size_t i = 0; i < Files::strFiles.size(); i++)
	{
		for (size_t k = 0; k < Files::strFiles.at(i).length(); k++)
		{
			if (Files::strFiles.at(i).at(k) == '/')
				Files::strFiles.at(i).at(k) = '\\';
		}
	}

	for (size_t i = 0; i < Files::strFiles.size(); i++)
	{
		std::string command = "copy ";
		command.append(Files::strFiles.at(i));
		command.append(" C:\\Windows\\Fonts > NUL");
		system(command.c_str());
	}
	
	return true;
}



bool Process::Inject(DWORD PID, std::string path)
{
	static int iAllocsize = (path.length() == 30 ? 30 : sizeof(path)); //GHETTO CODE!!!!
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID); //open handle to process
	if (!hProc) {
		std::cout << "[!] Failed to open handle to process, error => " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "Opened handle\n";
	void* loc = VirtualAllocEx(hProc, NULL, iAllocsize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); //allocate memory for the path of the dll
	if(!loc)
	{
		std::cout << "[!] Failed to alloc memory, error => " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "Allocated " << iAllocsize << " bytes at 0x" << std::hex << loc << std::endl;
	BOOL mem = WriteProcessMemory(hProc, loc, path.c_str(), iAllocsize, NULL); //write the path of the dll there
	if (!mem) {
		std::cout << "[!] Failed to write memory, error => " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "Wrote " << path << " to 0x" << loc << std::endl;;
	HANDLE hThread = CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA /*god i love c-type casting*/, loc, NULL, NULL); //create thread with LoadLib, with argument of the allocated memory
	if (!hThread) {
		std::cout << "[!] Failed to create remote thread, error => " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "Called thread\n";
	CloseHandle(hProc);
	CloseHandle(hThread);
	return true;
}

bool Files::SetupFonts(std::array<std::string, 13>& paths, std::array<std::string, 13>& urls)
{
	for (int i = 0; i < 13; i++)
	{
		std::string strFont = "https://github.com/PancakeWithPotato/PoophackLoader/raw/main/Fonts/";
		std::string location = "C:\\Poophack\\Fonts\\";
		location.append(Files::strFonts[i]);
		strFont.append(Files::strFonts[i]);
		paths[i] = location;
		urls[i] = strFont;
	}
	
	return true;
}
