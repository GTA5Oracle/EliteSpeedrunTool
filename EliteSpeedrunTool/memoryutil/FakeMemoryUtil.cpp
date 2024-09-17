#include "FakeMemoryUtil.h"

FakeMemoryUtil::FakeMemoryUtil(QObject* parent)
    : MemoryUtil {}
{
}

void FakeMemoryUtil::refresh()
{
}

WINBOOL FakeMemoryUtil::read(unsigned long long address, LPVOID buffer, SIZE_T size)
{
    buffer = 0;
    return FALSE;
}

HMODULE FakeMemoryUtil::getProcessModuleHandle(DWORD pid, const TCHAR* moduleName)
{
    return 0;
}

unsigned long long FakeMemoryUtil::findPattern(QString pattern)
{
    return -1;
}

unsigned long long FakeMemoryUtil::rip37(unsigned long long address)
{
    return 0;
}

bool FakeMemoryUtil::initGlobalPtr()
{
    return false;
}

void FakeMemoryUtil::initMissionPtr()
{
}

int FakeMemoryUtil::getGlobalInt(int index)
{
    return 0;
}

int FakeMemoryUtil::getGlobalUInt(int index)
{
    return 0;
}

long long FakeMemoryUtil::getGlobalLongLong(int index)
{
    return 0;
}

unsigned long long FakeMemoryUtil::getGlobalULongLong(int index)
{
    return 0;
}

float FakeMemoryUtil::getGlobalFloat(int index)
{
    return 0;
}

int FakeMemoryUtil::getLocalInt(int index)
{
    return 0;
}

unsigned int FakeMemoryUtil::getLocalUInt(int index)
{
    return 0;
}

long long FakeMemoryUtil::getLocalLongLong(int index)
{
    return 0;
}

unsigned long long FakeMemoryUtil::getLocalULongLong(int index)
{
    return 0;
}

float FakeMemoryUtil::getLocalFloat(int index)
{
    return 0;
}

bool FakeMemoryUtil::getLocalBool(int index)
{
    return 0;
}

int FakeMemoryUtil::getBadSport()
{
    return 0;
}
