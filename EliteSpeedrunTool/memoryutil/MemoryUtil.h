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

    virtual HWND getWindowHwnd();

private:
    static MemoryUtil* utilInstance;
};
