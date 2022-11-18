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
		std::cout << "Opening snapshot handle failed with error " << GetLastError() << std::endl;;
		CloseHandle(hSnap);
		return 0;
	}
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnap, &entry))
	{
		std::cout << "Failed to get information about first process, error: " << GetLastError() << std::endl;
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

void Files::Download(TCHAR path[], TCHAR url[])
{
	std::cout << "starting download...\n";
	HRESULT res = URLDownloadToFileA(NULL, url, path, 0, NULL); //download the file
	if (res == S_OK)
	{
		std::cout << "Succesfully downloaded!\n"; //succesfully download
		return;
	}
	else //any other error
	{
		std::cout << std::hex << "other error, res: 0x" << res << std::endl;
		return;
	}
	return;
}

bool Files::CheckFolder(std::string path)
{
	if (!_mkdir(path.c_str())) {
		return false;
	}
	return true;
}

bool Process::Inject(DWORD PID, std::string path)
{
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID); //open handle to process
	if (!hProc) {
		std::cout << "Failed to open handle to process, error: " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "Opened handle\n";
	void* loc = VirtualAllocEx(hProc, NULL, sizeof(path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); //allocate memory for the path of the dll
	if(!loc)
	{
		std::cout << "fialed to alloc memory, error: " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "Allocated shit\n";
	BOOL mem = WriteProcessMemory(hProc, loc, path.c_str(), sizeof(path), NULL); //write the path of the dll there
	if (!mem) {
		std::cout << "Failed to write memory, error: " << GetLastError() << std::endl;
		return false;
	}
	std::cout << "wrote shit\n";
	CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA /*god i love c-type casting*/, loc, NULL, nullptr); //create thread with LoadLib, with argument of the allocated memory
	std::cout << "called thread\n";
	return true;
}