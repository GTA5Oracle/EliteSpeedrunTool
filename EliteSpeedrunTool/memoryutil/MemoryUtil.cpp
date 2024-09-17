#include "MemoryUtil.h"
#include <Psapi.h>
#include <QDebug>
#include <QList>
#include <Tlhelp32.h>
#include <tchar.h>

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
    utilInstance->refresh();
    return utilInstance;
}

void MemoryUtil::refresh()
{
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
