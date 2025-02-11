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

HWND MemoryUtil::getGtaWindowHwnd()
{
    return getWindowHwnd("Grand Theft Auto V");
}

HWND MemoryUtil::getWindowHwnd(QString className, QString title)
{
    return FindWindow(className.isEmpty() ? nullptr : className.toStdWString().c_str(), title.toStdWString().c_str());
}

HWND MemoryUtil::getWindowHwnd(QString title)
{
    return getWindowHwnd("", title);
}

DWORD MemoryUtil::getGtaPid()
{
    return getPid(getGtaWindowHwnd());
}

DWORD MemoryUtil::getPid(HWND hwnd)
{
    if (!hwnd) {
        return 0;
    }
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

HANDLE MemoryUtil::getGtaProcessHandle(DWORD dwDesiredAccess)
{
    return OpenProcess(dwDesiredAccess, FALSE, getGtaPid());
}
