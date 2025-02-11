#include "MainFeatures.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include "event/Event.h"
#include <QWidget>
#include <ntdef.h>

Q_GLOBAL_STATIC(MainFeatures, mainFeaturesInstance)

MainFeatures::MainFeatures(QObject* parent)
    : QObject { parent }
{
}

MainFeatures* MainFeatures::instance()
{
    return mainFeaturesInstance;
}

typedef NTSTATUS(NTAPI* PNtTerminateProcess)(HANDLE, NTSTATUS);

void MainFeatures::terminateGta()
{
    const auto gamePid = memoryUtil->getGtaPid();
    if (!terminateProcess(gamePid) && !ntTerminateProcess(gamePid)) {
        WinExec("taskkill /f /t /im GTA5.exe", SW_HIDE);
        qWarning("Using taskkill command to terminate GTA5.exe");
    }
    CloseGameEvent event;
    eventBus->send(&event);
}

void MainFeatures::applyWindowDisplayAffinity(QWidget* widget)
{
    if (!widget) {
        return;
    }
    if (widget->isWindow()) {
        HWND hwnd = reinterpret_cast<HWND>(widget->winId());
        DWORD affinity = globalData->excludeFromCapture() ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE;
        DWORD currentAffinity;
        if (!GetWindowDisplayAffinity(hwnd, &currentAffinity) || currentAffinity != affinity) {
            SetWindowDisplayAffinity(hwnd, affinity);
        }
    }
}

bool MainFeatures::ntTerminateProcess(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    bool res = false;
    if (hProcess) {
        PNtTerminateProcess ntTerminateProcess = (PNtTerminateProcess)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtTerminateProcess");
        if (ntTerminateProcess) {
            if (NT_SUCCESS(ntTerminateProcess(hProcess, 0))) {
                qInfo("Execute NtTerminateProcess API succeeds");
                res = true;
            } else {
                qWarning("Execute NtTerminateProcess fails");
            }
        } else {
            qWarning("PNtTerminateProcess is null");
        }
        CloseHandle(hProcess);
    }
    return res;
}

bool MainFeatures::terminateProcess(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    bool res = false;
    if (TerminateProcess(hProcess, 1)) {
        qInfo("Execute TerminateProcess API succeeds");
        res = true;
    } else {
        qWarning("Execute TerminateProcess fails");
    }
    CloseHandle(hProcess);
    return res;
}
