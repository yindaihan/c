#include "pid.h"

using namespace std;

DWORD GetPid(const wstring& process_name)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        wcout << L"Snapshot failed!" << endl;
        return 0;
    }
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    if (!Process32First(hSnapshot, &pe32))
    {
        CloseHandle(hSnapshot);
        wcout << L"Can't read processes!" << endl;
        return 0;
    }
    do
    {
        if (lstrcmpiW(pe32.szExeFile, process_name.c_str()) == 0)
        {
            DWORD pid = pe32.th32ProcessID;
            CloseHandle(hSnapshot);
            return pid;
        }
    } while (Process32Next(hSnapshot, &pe32));
    CloseHandle(hSnapshot);
    return 0;
}
