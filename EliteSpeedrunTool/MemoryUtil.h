#pragma once
#include <QString>
#include <windows.h>

class MemoryUtil {
public:
    MemoryUtil();
    static DWORD64 globalPtr;
    static DWORD64 missionPtr;

    // 变量的签名
    const static int localTime = 19728 + 987; // 任务每一段的时间
    const static int localPrisonBreakTime = 19728 + 990; // 越狱的逃离时间
    const static int localHeadshot = 19728 + 1740 + 1; // 总的爆头数
    const static int localKillHost = 19728 + 1725 + 1; // 房主击杀数
    const static int localKillClient1 = 19728 + 1725 + 2; // 队友1击杀数
    const static int localVehicleDamage = 24562 + 1231; // 车辆受损程度
    const static int localRashkovskyDamage = 24562 + 1230; // 拉什科夫斯基受伤程度
    const static int localHeadshotHost = 31603 + 1 + (0 * 292) + 85; // 房主爆头数
    const static int localHeadshotClient1 = 31603 + 1 + (1 * 292) + 85; // 队友1爆头数

    // 结算页面的数据
    const static int globalTime = 2685249 + 3485 + 94; // 时间
    const static int globalPrisonBreakTime = 2685249 + 3485 + 106; // 越狱的逃离时间
    const static int globalHeadshot = 2685249 + 3485 + 114; // 总的爆头数
    const static int globalKill = 2685249 + 3485 + 110; // 总的击杀数
    const static int globalVehicleDamage = 2685249 + 3485 + 98; // 车辆受损程度
    const static int globalRashkovskyDamage = 2685249 + 3485 + 102; // 拉什科夫斯基受伤程度

    const static int localFlagInitTimer = 19728 + 985 + 1; // 是否开始计时，1开始，0暂停
    const static int localFlagInitPrisonBreakTimer = 19728 + 988 + 1; // 越狱逃离是否开始计时，1开始，0暂停

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
