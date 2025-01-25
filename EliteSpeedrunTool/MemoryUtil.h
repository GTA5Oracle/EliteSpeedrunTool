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

    HWND getWindowHwnd();

    DWORD getPid();

    HANDLE getProcessHandle(DWORD* pid, DWORD dwDesiredAccess);

private:
    static MemoryUtil* utilInstance;
};
