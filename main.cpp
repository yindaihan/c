#include "pid.h"
#include "sun.h"

using namespace std;

int main()
{
    DWORD pid = GetPid(L"PlantsVsZombies.exe");
    
    if (pid == 0)
    {
        cout << "Process not found!" << endl;
        system("pause");
        return 1;
    }
    
    cout << "Found process PID: " << pid << endl;
    
    SetGameSun(pid, 9999);
    
    system("pause");
    return 0;
}
