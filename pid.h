#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetPid(const std::wstring& process_name);
int pid();
