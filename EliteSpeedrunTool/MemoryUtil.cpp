#include "MemoryUtil.h"
#include <QDebug>
#include <QFileInfo>
#include <QList>
#include <psapi.h>

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

QString MemoryUtil::getProcessName(DWORD pid)
{
    QString processName;

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == nullptr) {
        hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (hProcess == nullptr) {
            return processName;
        }
    }

    WCHAR szProcessName[MAX_PATH] = L"";
    DWORD size = GetModuleFileNameExW(hProcess, nullptr, szProcessName, MAX_PATH);
    if (size == 0) {
        size = GetProcessImageFileNameW(hProcess, szProcessName, MAX_PATH);
    }

    CloseHandle(hProcess);

    if (size) {
        processName = QFileInfo(QString::fromWCharArray(szProcessName)).fileName();
    }

    return processName;
}

bool MemoryUtil::isEnhanced()
{
    return getProcessName(getGtaPid()).contains("Enhanced", Qt::CaseInsensitive);
}
