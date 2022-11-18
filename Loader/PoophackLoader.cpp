#include "includes.h"

int main()
{
    if (!Files::CheckFolder("C:\\Poophack"))
        std::cout << "Created folder!\n";
    system("curl https://raw.githubusercontent.com/PancakeWithPotato/PoophackLoader/main/version.txt > C:\\Poophack\\version.txt"); //store the version number in a txt
    std::fstream file;
    file.open("C:\\Poophack\\version.txt"); //open the file
    std::getline(file, User::strVersion); //read the version number from it
    std::cout << "Hi " << User::Getname() << ", version is " << User::strVersion << std::endl;
    TCHAR url[] = TEXT("https://github.com/PancakeWithPotato/PoophackLoader/raw/main/supremacy_csgo.dll");
    TCHAR path[] = TEXT("C:\\Poophack\\poophack.dll");
    Files::Download(path, url);
    if (Process::Inject(Process::Find("csgo.exe"), "C:\\Poophack\\poophack.dll"))
    {
        MessageBoxA(NULL, "Succesfully Injected!", "Injection succesfull", MB_OK);
    }
    else {
        MessageBoxA(NULL, "Failed to inject :( ", "Injection failed", MB_ICONERROR);
    }
}
