#pragma once
#include <Windows.h>

#define SUN_ODDSET_1 0x357E0C
#define SUN_ODDSET_2 0x868
#define SUN_ODDSET_3 0x5578

bool SetGameSun(DWORD pid, int new_sun);
