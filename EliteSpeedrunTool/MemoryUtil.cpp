#include "MemoryUtil.h"
#include <Psapi.h>
#include <QDebug>
#include <QList>
#include <Tlhelp32.h>
#include <tchar.h>

Q_GLOBAL_STATIC(MemoryUtil, memoryUtilInstance)

MemoryUtil::MemoryUtil()
{
    // 不断刷新GTA进程的信息，防止先启动工具后启动游戏导致地址不正确
    initGlobalPtr();
    initMissionPtr();
    connect(gtaProcessTimer, &QTimer::timeout, this, [=]() {
        initGlobalPtr();
        initMissionPtr();
    });
    gtaProcessTimer->start(15000);
}

MemoryUtil* MemoryUtil::instance()
{
    return memoryUtilInstance;
}

WINBOOL MemoryUtil::read(unsigned long long address, LPVOID buffer, SIZE_T size)
{
    DWORD pid;
    HANDLE processHandle = getProcessHandle(&pid);
    WINBOOL result = ReadProcessMemory(processHandle, (HMODULE)address, buffer, size, 0);
    CloseHandle(processHandle);
    return result;
}

HWND MemoryUtil::getWindowHwnd()
{
    return FindWindow(NULL, L"Grand Theft Auto V");
}

HANDLE MemoryUtil::getProcessHandle(DWORD* pid, DWORD dwDesiredAccess)
{
    HWND hWindow = getWindowHwnd(); // 获取窗口句柄
    GetWindowThreadProcessId(hWindow, pid); // 获取进程id
    return OpenProcess(dwDesiredAccess, FALSE, *pid); // 打开进程
}

HMODULE MemoryUtil::getProcessModuleHandle(DWORD pid, const TCHAR* moduleName)
{
    MODULEENTRY32 moduleEntry;
    HANDLE handle = NULL;
    handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid); //  获取进程快照中包含在th32ProcessID中指定的进程的所有的模块。
    if (!handle) {
        CloseHandle(handle);
        return NULL;
    }
    ZeroMemory(&moduleEntry, sizeof(MODULEENTRY32));
    moduleEntry.dwSize = sizeof(MODULEENTRY32);
    if (!Module32First(handle, &moduleEntry)) {
        CloseHandle(handle);
        return NULL;
    }
    do {
        if (_tcscmp(moduleEntry.szModule, moduleName) == 0) {
            CloseHandle(handle);
            return moduleEntry.hModule;
        }
    } while (Module32Next(handle, &moduleEntry));
    CloseHandle(handle);
    return 0;
}

unsigned long long MemoryUtil::findPattern(QString pattern)
{
    unsigned long long failedAddress = -1;

    auto tempArray = QList<byte>();
    for (auto&& each : pattern.split(' ')) {
        if (each == "??") {
            tempArray.append(0);
        } else {
            tempArray.append(each.toUInt(nullptr, 16));
        }
    }

    auto patternByteArray = tempArray;

    DWORD pid;
    HANDLE processHandle = getProcessHandle(&pid);
    HMODULE mainModuleHandle = getProcessModuleHandle(pid, L"GTA5.exe");
    MODULEINFO moduleInfo;
    if (!GetModuleInformation(
            processHandle,
            mainModuleHandle,
            &moduleInfo,
            sizeof(moduleInfo))) {
        CloseHandle(processHandle);
        return failedAddress;
    }
    DWORD moduleSize = moduleInfo.SizeOfImage;
    if (moduleSize == 0) {
        CloseHandle(processHandle);
        return failedAddress;
    }

    auto localModulebytes = new byte[moduleSize];
    ReadProcessMemory(processHandle, mainModuleHandle, localModulebytes, moduleSize, 0);

    for (unsigned long indexAfterBase = 0; indexAfterBase < moduleSize; indexAfterBase++) {
        bool noMatch = false;

        if (localModulebytes[indexAfterBase] != patternByteArray.at(0))
            continue;

        for (int matchedIndex = 0;
             matchedIndex < patternByteArray.count() && indexAfterBase + matchedIndex < moduleSize;
             matchedIndex++) {
            if (patternByteArray.at(matchedIndex) == 0x0)
                continue;
            if (patternByteArray.at(matchedIndex) != localModulebytes[indexAfterBase + matchedIndex]) {
                noMatch = true;
                break;
            }
        }

        if (!noMatch) {
            CloseHandle(processHandle);
            delete[] localModulebytes;
            return (DWORD64)mainModuleHandle + indexAfterBase;
        }
    }

    CloseHandle(processHandle);
    delete[] localModulebytes;
    return failedAddress;
}

unsigned long long MemoryUtil::rip37(unsigned long long address)
{
    int buffer;
    read(address + 0x03, &buffer, 4);
    return address + buffer + 0x07;
}

bool MemoryUtil::initGlobalPtr()
{
    globalPtr = rip37(findPattern("4C 8D 05 ?? ?? ?? ?? 4D 8B 08 4D 85 C9 74 11"));
    return globalPtr >= 0x10000 && globalPtr < 0x000F000000000000;
}

void MemoryUtil::initMissionPtr()
{
    DWORD64 newMissionPtr = 0;
    read(globalPtr - 0x120, &newMissionPtr, 8);
    read(newMissionPtr + 0x1180, &newMissionPtr, 8);
    if (newMissionPtr != missionPtr && newMissionPtr) {
        missionPtr = newMissionPtr;
        emit onMissionPtrChanged();
    }
}

int MemoryUtil::getGlobalInt(int index)
{
    long long buffer;
    read(globalPtr + (0x8 * (index >> 0x12 & 0x3F)), &buffer, 8);
    int data;
    read(buffer + (8 * (index & 0x3FFFF)), &data, 4);
    return data;
}

int MemoryUtil::getGlobalUInt(int index)
{
    long long buffer;
    read(globalPtr + (0x8 * (index >> 0x12 & 0x3F)), &buffer, 8);
    unsigned int data;
    read(buffer + (8 * (index & 0x3FFFF)), &data, 4);
    return data;
}

long long MemoryUtil::getGlobalLongLong(int index)
{
    long long buffer;
    read(globalPtr + (8 * (index >> 0x12 & 0x3F)), &buffer, 8);
    long long data;
    read(buffer + (8 * (index & 0x3FFFF)), &data, 8);
    return data;
}

unsigned long long MemoryUtil::getGlobalULongLong(int index)
{
    long long buffer;
    read(globalPtr + (8 * (index >> 0x12 & 0x3F)), &buffer, 8);
    unsigned long long data;
    read(buffer + (8 * (index & 0x3FFFF)), &data, 8);
    return data;
}

float MemoryUtil::getGlobalFloat(int index)
{
    DWORD64 buffer;
    read(globalPtr + (0x8 * (index >> 0x12 & 0x3F)), &buffer, 8);
    float data;
    read(buffer + (8 * (index & 0x3FFFF)), &data, 4);
    return data;
}

int MemoryUtil::getLocalInt(int index)
{
    int buffer;
    read(missionPtr + (0x8 * index), &buffer, 4);
    return buffer;
}

unsigned int MemoryUtil::getLocalUInt(int index)
{
    unsigned int buffer;
    read(missionPtr + (0x8 * index), &buffer, 4);
    return buffer;
}

long long MemoryUtil::getLocalLongLong(int index)
{
    long long buffer;
    read(missionPtr + (0x8 * index), &buffer, 8);
    return buffer;
}

unsigned long long MemoryUtil::getLocalULongLong(int index)
{
    unsigned long long buffer;
    read(missionPtr + (0x8 * index), &buffer, 8);
    return buffer;
}

float MemoryUtil::getLocalFloat(int index)
{
    DWORD64 buffer;
    read(missionPtr + (0x8 * index), &buffer, 4);
    return (float)buffer;
}

bool MemoryUtil::getLocalBool(int index)
{
    DWORD64 buffer;
    read(missionPtr + (0x8 * index), &buffer, 1);
    return (bool)buffer;
}

int MemoryUtil::getBadSport()
{
    DWORD64 buffer;
    read(globalPtr - 0x120, &buffer, 8);
    read(buffer + 0xDD8, &buffer, 8);
    float badSport = 0;
    read(buffer + 323 * 8, &badSport, 4);
    qInfo() << "BadSport is" << badSport;
    return badSport;
}
