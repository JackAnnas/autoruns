#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

struct StartupProgram {
    std::wstring name;
    std::wstring path;
};

void QueryRegistry(HKEY hKey, const std::wstring& subKey, std::vector<StartupProgram>& programs) {
    HKEY hRegistryKey;
    if (RegOpenKeyEx(hKey, subKey.c_str(), 0, KEY_READ, &hRegistryKey) == ERROR_SUCCESS) {
        DWORD index = 0;
        WCHAR valueName[16383];
        DWORD valueNameSize;
        BYTE data[16383];
        DWORD dataSize;
        DWORD type;

        while (true) {
            valueNameSize = sizeof(valueName) / sizeof(valueName[0]);
            dataSize = sizeof(data);

            LONG result = RegEnumValue(
                hRegistryKey,
                index,
                valueName,
                &valueNameSize,
                NULL,
                &type,
                data,
                &dataSize
            );

            if (result == ERROR_NO_MORE_ITEMS) {
                break;
            }

            if (result == ERROR_SUCCESS) {
                if (type == REG_SZ) {
                    programs.push_back({ valueName, (WCHAR*)data });
                }
            }
            index++;
        }

        RegCloseKey(hRegistryKey);
    }
}

int main()
{
    std::vector<StartupProgram> programs;

    QueryRegistry(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", programs);

    QueryRegistry(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", programs);
    std::wcout << L"Programs that run on startup:\n" << std::endl;
    for (size_t i = 0; i < programs.size(); ++i) {
        std::wcout << i + 1 << L". " << programs[i].name << L" - " << programs[i].path << std::endl;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
