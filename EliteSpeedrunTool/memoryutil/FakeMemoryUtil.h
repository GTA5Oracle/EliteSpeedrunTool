#pragma once

#include "MemoryUtil.h"

class FakeMemoryUtil : public MemoryUtil {
public:
    explicit FakeMemoryUtil(QObject* parent = nullptr);

    HMODULE getProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName) override;

    WINBOOL read(unsigned long long address, LPVOID buffer, SIZE_T size) override;
    unsigned long long findPattern(QString pattern) override;
    unsigned long long rip37(unsigned long long address) override;

    bool initGlobalPtr() override;
    void initMissionPtr() override;

    int getGlobalInt(int index) override;
    int getGlobalUInt(int index) override;
    long long getGlobalLongLong(int index) override;
    unsigned long long getGlobalULongLong(int index) override;
    float getGlobalFloat(int index) override;

    int getLocalInt(int index) override;
    unsigned int getLocalUInt(int index) override;
    float getLocalFloat(int index) override;
    long long getLocalLongLong(int index) override;
    unsigned long long getLocalULongLong(int index) override;
    bool getLocalBool(int index) override;

    // 恶意值
    int getBadSport() override;

    void refresh() override;
};
