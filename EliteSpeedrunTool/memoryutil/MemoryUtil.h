#pragma once

#include <QString>
#include <QTimer>
#include <windows.h>

#define memoryUtil (MemoryUtil::instance())

class MemoryUtil : public QObject {
    Q_OBJECT
public:
    explicit MemoryUtil();

    static MemoryUtil* instance();

    DWORD64 globalPtr = 0;
    DWORD64 missionPtr = 0;

    virtual HWND getWindowHwnd();
    virtual HANDLE getProcessHandle(DWORD* pid, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS);

    virtual void refresh();

private:
    static MemoryUtil* utilInstance;
};
