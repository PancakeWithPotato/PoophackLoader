#include "includes.h"

int main()
{
    std::fstream file;
    std::array<std::string, 13> urls;
    std::array<std::string, 13> paths;
    Files::SetupFonts(paths, urls);
    system("curl https://raw.githubusercontent.com/PancakeWithPotato/PoophackLoader/main/version.txt > C:\\Poophack\\version.txt"); //store the version number in a txt
    system("cls"); //clear the output
    system("title Poophack Loader");
    if (!Files::CheckFolder("C:\\Poophack"))
        std::cout << "Created folder!\n";
    if (!Files::CheckFolder("C:\\Poophack\\Fonts"))
        std::cout << "Created fonts folder!\n";

    if (!Files::CheckFile("C:\\Poophack\\config.txt"))
        std::cout << "Created config file\n";
    file.open("C:\\Poophack\\config.txt", std::ios::in);

    if (!file.is_open())
        std::cout << "[!] Failed to open config file, error => " << GetLastError() << std::endl;

    int iConfig = 0;
    {
        std::string temp;
        std::getline(file, temp);
        if (temp == "")
            iConfig = 0;
        else
            iConfig = std::stoi(temp);
    }
    file.close();

    FILE* fp;
    fopen_s(&fp, "C:\\Poophack\\config.txt", "r+"); //god damm c-type files
    if (fp == NULL)
        std::cout << "[!] Failed to open config file!, error=>" << GetLastError() << std::endl;

    std::cout << "Fonts: " << (iConfig == 1 ? "INSTALLED" : "NOT INSTALLED") << std::endl;
    int iRet = 7;
    if(!iConfig)
        iRet = MessageBoxA(NULL, "Do you want to install fonts?", "Fonts", MB_YESNO | MB_ICONQUESTION); //6 yes, 7 no
    if (iRet == 6) 
    {
        Files::InstallFonts(paths, urls, "C:\\Poophack\\Fonts");
        std::cout << "Finished installing fonts!\n";
        fwrite("1", 1, 1, fp);
        fclose(fp);
    }
    std::cout << "\n"; //print an empty line so it looks better
    file.close();
    file.open("C:\\Poophack\\version.txt"); //open the file
    std::getline(file, User::strVersion); //read the version number from it
    file.close();
    std::cout << "Hi " << User::Getname() << ", version is " << User::strVersion << std::endl;
    int iDecision = 1;
    std::cout << "[1] Load LUA version\n";
    std::cout << "[2] Load NO-LUA version\n";
    std::cout << "[3] Install fonts\n";
    bool bCanProceed = false;
    std::string url;
    std::string path;
    while (!bCanProceed)
    { 
        std::cin >> iDecision;
        std::cout << "Decision: " << iDecision << std::endl;
        switch (iDecision)
        {
        case 1:
            url = "https://github.com/PancakeWithPotato/PoophackLoader/raw/main/poophack_lua.dll";
            path = "C:\\Poophack\\poophack_lua.dll";
            bCanProceed = true;
            break;
        case 2:
            url = "https://github.com/PancakeWithPotato/PoophackLoader/raw/main/poophack_nolua.dll";
            path = "C:\\Poophack\\poophack_nolua.dll";
            bCanProceed = true;
            break;
        case 3:
        {
            Files::InstallFonts(paths, urls, "C:\\Poophack\\Fonts");
            std::cout << "Finished installing fonts!\n";
            break;
        }
        default:
            std::cout << "Invalid decision!\n";
            break;
        }
    }
    
   
    Files::Download(path, url);
    if (Process::Inject(Process::Find("csgo.exe"), path))
    {
        MessageBoxA(NULL, "Succesfully Injected!", "Injection succesfull", MB_OK);
    }
    else {
        MessageBoxA(NULL, "Failed to inject :( ", "Injection failed", MB_ICONERROR);
    }
}
