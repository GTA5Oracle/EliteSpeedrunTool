#include "MemoryUtil.h"
#include <QDebug>
#include <QList>

MemoryUtil* MemoryUtil::utilInstance = nullptr;

MemoryUtil::MemoryUtil()
{
}

MemoryUtil* MemoryUtil::instance()
{
    if (utilInstance != nullptr) {
        return utilInstance;
    }
    utilInstance = new MemoryUtil();
    return utilInstance;
}

HWND MemoryUtil::getWindowHwnd()
{
    return FindWindow(NULL, L"Grand Theft Auto V");
}

DWORD MemoryUtil::getPid()
{
    HWND hWindow = getWindowHwnd();
    if (!hWindow) {
        return 0;
    }
    DWORD pid = 0;
    GetWindowThreadProcessId(hWindow, &pid);
    return pid;
}

HANDLE MemoryUtil::getProcessHandle(DWORD* pid, DWORD dwDesiredAccess)
{
    return OpenProcess(dwDesiredAccess, FALSE, getPid());
}
