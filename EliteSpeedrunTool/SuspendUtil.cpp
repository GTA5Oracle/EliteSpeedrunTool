#include "SuspendUtil.h"
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <windows.h>

typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);

Q_GLOBAL_STATIC(SuspendUtil, suspendUtilInstance)

SuspendUtil::SuspendUtil(QObject* parent)
    : QObject { parent }
{
    connect(&timer, &QTimer::timeout, this, &SuspendUtil::onTimeout);
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
    WinExec(".\\external\\PSTools\\pssuspend.exe GTA5_Enhanced.exe", SW_HIDE);
}

void SuspendUtil::resumeProcess()
{
    if (!isEulaAccepted()) {
        acceptEula();
    }
    WinExec(".\\external\\PSTools\\pssuspend.exe -r GTA5.exe", SW_HIDE);
    WinExec(".\\external\\PSTools\\pssuspend.exe -r GTA5_Enhanced.exe", SW_HIDE);
}

void SuspendUtil::suspendAndResumeProcess(long long durationMs)
{
    if (isSuspending) {
        if (timer.isActive()) {
            timer.stop();
        }
        onTimeout();
        return;
    }
    isSuspending = true;
    suspendProcess();
    timer.start(durationMs);
}

void SuspendUtil::onTimeout()
{
    if (isSuspending) {
        isSuspending = false;
        resumeProcess();
        emit onResume();
    }
}
