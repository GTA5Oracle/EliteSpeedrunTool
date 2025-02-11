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

    HWND getGtaWindowHwnd();

    HWND getWindowHwnd(QString className, QString title);

    HWND getWindowHwnd(QString title);

    DWORD getGtaPid();

    DWORD getPid(HWND hwnd);

    HANDLE getGtaProcessHandle(DWORD dwDesiredAccess);

private:
    static MemoryUtil* utilInstance;
};
