#pragma once

#include <QObject>
#include <windows.h>

#define mainFeatures (MainFeatures::instance())

typedef NTSTATUS(NTAPI* PNtTerminateProcess)(HANDLE, NTSTATUS);
typedef BOOL(WINAPI* PEndTask)(
    HWND hWnd,
    BOOL fShutDown,
    BOOL fForce);

class MainFeatures : public QObject {
    Q_OBJECT
public:
    explicit MainFeatures(QObject* parent = nullptr);

    static MainFeatures* instance();

    void terminateGta();

    void applyWindowDisplayAffinity(QWidget* widget);

private:
    PNtTerminateProcess NtTerminateProcess;
    PEndTask EndTask;
    bool ntTerminateProcess(DWORD pid);
    bool terminateProcess(DWORD pid);
    bool endTask(HWND hwnd);
signals:
};
