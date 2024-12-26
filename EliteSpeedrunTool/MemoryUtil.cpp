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
