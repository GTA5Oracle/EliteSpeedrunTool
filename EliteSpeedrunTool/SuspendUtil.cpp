#include "SuspendUtil.h"
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <windows.h>

typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);

Q_GLOBAL_STATIC(SuspendUtil, suspendUtilInstance)

SuspendUtil::SuspendUtil(QObject* parent)
    : QObject { parent }
{
}

SuspendUtil* SuspendUtil::instance()
{
    return suspendUtilInstance;
}

bool SuspendUtil::isEulaAccepted()
{
    QSettings qSettings("HKEY_CURRENT_USER\\Software\\Sysinternals\\PsSuspend", QSettings::NativeFormat);
    return qSettings.value("EulaAccepted").toInt() == 1;
}

void SuspendUtil::acceptEula()
{
    system(".\\external\\PSTools\\pssuspend.exe");
    emit onAcceptedChanged();
}

void SuspendUtil::suspendProcess()
{
    if (!isEulaAccepted()) {
        acceptEula();
    }
    WinExec(".\\external\\PSTools\\pssuspend.exe GTA5.exe", SW_HIDE);
}

void SuspendUtil::resumeProcess()
{
    if (!isEulaAccepted()) {
        acceptEula();
    }
    WinExec(".\\external\\PSTools\\pssuspend.exe -r GTA5.exe", SW_HIDE);
}

void SuspendUtil::suspendAndResumeProcess(long long durationMs, std::function<void(void)> onResumed)
{
    static bool isSuspending = false;
    if (isSuspending) {
        return;
    }
    isSuspending = true;
    suspendProcess();
    QTimer::singleShot(durationMs, [onResumed]() {
        isSuspending = false;
        suspendUtil->resumeProcess();
        onResumed();
    });
}
