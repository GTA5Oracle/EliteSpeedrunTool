#pragma once
#include <QString>
#include <windows.h>

class MemoryUtil {
public:
    MemoryUtil();
    static DWORD64 globalPtr;
    static DWORD64 missionPtr;

    // 变量的签名
    const static int local_Time = 19728 + 987;
    const static int local_extTime = 19728 + 990; // 越狱的逃离时间
    const static int local_headshot = 19728 + 1740 + 1;
    const static int local_kill_0 = 19728 + 1725 + 1;
    const static int local_kill_1 = 19728 + 1725 + 2;
    const static int local_vehicleDmg = 24562 + 1231;
    const static int local_RashDmg = 24562 + 1230; // 拉什科夫斯基受伤程度
    const static int local_headshot_host = 31603 + 1 + (0 * 292) + 85; // 房主爆头数
    const static int local_headshot_client_1 = 31603 + 1 + (1 * 292) + 85; // 队友1爆头数

    const static int global_Time = 2685249 + 3485 + 94;
    const static int global_extTime = 2685249 + 3485 + 106;
    const static int global_headshot = 2685249 + 3485 + 114;
    const static int global_kill = 2685249 + 3485 + 110;
    const static int global_vehicleDmg = 2685249 + 3485 + 98;
    const static int global_RashDmg = 2685249 + 3485 + 102;

    const static int local_Flag_initTimer = 19728 + 985 + 1;
    const static int local_Flag_initExtTimer = 19728 + 988 + 1;

    static long long missionHash[14];

    static HWND getWindowHwnd();
    static HANDLE getProcessHandle(DWORD* pid, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS);
    static HMODULE getProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName);

    static WINBOOL read(unsigned long long address, LPVOID buffer, SIZE_T size);
    static unsigned long long findPattern(QString pattern);
    static unsigned long long rip37(unsigned long long address);

    static bool initGlobalPtr();
    static void initMissionPtr();

    static int getGlobalInt(int index);
    static int getGlobalUInt(int index);
    static long long getGlobalLongLong(int index);
    static float getGlobalFloat(int index);

    static int getLocalInt(int index);
    static float getLocalFloat(int index);
    static long long getLocalLongLong(int index);
    static bool getLocalBool(int index);
};
