#include "sun.h"
#include <iostream>
#include <TlHelp32.h>

using namespace std;

static DWORD_PTR GetModuleBaseAddr(DWORD pid, const wchar_t* module_name)
{
	DWORD_PTR base_addr = 0;
	HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

	if (h_snap == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(me32);

	if (!Module32First(h_snap, &me32))
	{
		CloseHandle(h_snap);
		return 0;
	}
	do
	{
		if (_wcsicmp(me32.szModule, module_name) == 0)
		{
			base_addr = (DWORD_PTR)me32.modBaseAddr;
			break;
		}
	} while (Module32Next(h_snap, &me32));
	CloseHandle(h_snap);
	return base_addr;
}

static bool SafeReadMemory(HANDLE h_process, DWORD_PTR addr, LPVOID buffer, SIZE_T size)
{
	return ReadProcessMemory(h_process, (LPCVOID)addr, buffer, size, NULL);
}

static bool SafeWriteMemory(HANDLE h_process, DWORD_PTR addr, LPVOID buffer, SIZE_T size)
{
	return WriteProcessMemory(h_process, (LPVOID)addr, buffer, size, NULL);
}

bool SetGameSun(DWORD pid, int new_sun)
{
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (h_process == NULL)
	{
		cout << "[Error] Failed to open process" << endl;
		return false;
	}

	DWORD_PTR game_base = GetModuleBaseAddr(pid, L"PlantsVsZombies.exe");
	if (game_base == 0)
	{
		cout << "[Error] Failed to get module base" << endl;
		CloseHandle(h_process);
		return false;
	}

	DWORD_PTR p1 = 0, p2 = 0;
	if (!SafeReadMemory(h_process, game_base + SUN_ODDSET_1, &p1, sizeof(p1)))
	{
		CloseHandle(h_process);
		cout << "[Error] Failed to get first pointer" << endl;
		return false;
	}
	if (!SafeReadMemory(h_process, p1 + SUN_ODDSET_2, &p2, sizeof(p2)))
	{
		CloseHandle(h_process);
		cout << "[Error] Failed to get second pointer" << endl;
		return false;
	}
	DWORD_PTR sun_addr = p2 + SUN_ODDSET_3;
	if (!SafeWriteMemory(h_process, sun_addr, &new_sun, sizeof(new_sun)))
	{
		CloseHandle(h_process);
		cout << "[Error] Failed to write sun" << endl;
		return false;
	}
	cout << "[写入成功]阳光值: " << new_sun << endl;
	//判断阳光值是不是9999
	//不是就写入9999是就不操作
	while (1)
	{
		int sunval = 0;
		if (ReadProcessMemory(h_process, (LPCVOID)sun_addr, &sunval, sizeof(sunval), NULL))
		{
			if (sunval < 9999)
			{
				system("cls");
				WriteProcessMemory(h_process, (LPVOID)sun_addr, &new_sun, sizeof(new_sun), NULL);
				std::cout << "写入成功9999" << endl;
			}
		}
		Sleep(150);
	}
	CloseHandle(h_process);
	return true;
}
