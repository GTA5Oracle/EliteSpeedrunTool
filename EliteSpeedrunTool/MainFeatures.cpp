#include "MainFeatures.h"
#include "GlobalData.h"
#include "MemoryUtil.h"
#include "event/Event.h"
#include "net/wfp/WfpController.h"
#include <QWidget>
#include <QtConcurrent>
#include <fwptypes.h>
#include <ntdef.h>
#include <windows.h>

Q_GLOBAL_STATIC(MainFeatures, mainFeaturesInstance)

MainFeatures::MainFeatures(QObject* parent)
    : QObject { parent }
{
    auto ntdllHandle = GetModuleHandleW(L"ntdll.dll");
    auto user32Handle = GetModuleHandleW(L"User32.dll");
    if (ntdllHandle) {
        NtTerminateProcess = (PNtTerminateProcess)GetProcAddress(ntdllHandle, "NtTerminateProcess");
    } else {
        qCritical() << "MainFeatures: Cannot get ntdll.dll's module handle";
    }
    if (user32Handle) {
        EndTask = (PEndTask)GetProcAddress(user32Handle, "EndTask");
    } else {
        qCritical() << "MainFeatures: Cannot get User32.dll's module handle";
    }
}

MainFeatures* MainFeatures::instance()
{
    return mainFeaturesInstance;
}

bool MainFeatures::isRunOnLogon()
{
    QProcess process;
    process.start("schtasks", { "/query", "/tn", runOnLogonTaskName });
    if (!process.waitForStarted()) {
        qDebug() << "Failed to start process";
        return false;
    }
    process.waitForReadyRead();
    if (!process.waitForFinished()) {
        qDebug() << "Failed to finish process:" << process.readAllStandardOutput();
        return false;
    }
    QString output = process.readAllStandardOutput();
    qInfo() << output;
    return output.contains(runOnLogonTaskName);
}

void MainFeatures::setRunOnLogon(bool enable)
{
    QProcess process;
    if (enable) {
        auto command = "schtasks /create /tn \""
            + runOnLogonTaskName + "\" /tr " + "\""
            + runOnLogonValue + "\"" + " /sc ONLOGON /RL HIGHEST /F";
        WinExec(command.toLocal8Bit().toStdString().c_str(), SW_HIDE);
    } else {
        auto command = "schtasks /delete /tn \"" + runOnLogonTaskName + "\" /F";
        WinExec(command.toLocal8Bit().toStdString().c_str(), SW_HIDE);
    }
    qDebug() << "AutoStart with Admin set to:" << enable;
}

void MainFeatures::terminateGta()
{
    const HWND hwnd = memoryUtil->getGtaWindowHwnd();
    const DWORD gamePid = memoryUtil->getPid(hwnd);
    const QString gameProcessName = memoryUtil->getProcessName(gamePid);

    QFuture<bool> futures[] = {
        QtConcurrent::run(&MainFeatures::ntTerminateProcess, this, gamePid),
        QtConcurrent::run(&MainFeatures::terminateProcess, this, gamePid),
        QtConcurrent::run(&MainFeatures::endTask, this, hwnd),
    };
    auto result = false;
    for (int i = 0; i < sizeof(futures) / sizeof(futures[0]); i++) {
        if (futures[i].result()) {
            result = true;
            qInfo() << "terminateGta future:" << i << "successful";
            break;
        }
    }

    if (!result) {
        WinExec("taskkill /f /t /im GTA5.exe", SW_HIDE);
        WinExec("taskkill /f /t /im GTA5_Enhanced.exe", SW_HIDE);
        qWarning("Using taskkill command to terminate GTA5.exe");
    }
    CloseGameEvent event;
    eventBus->send(&event);
    if (!gameProcessName.isEmpty()) {
        if (gameProcessName.contains("Enhanced", Qt::CaseInsensitive)) {
            CloseEnhancedGameEvent event;
            eventBus->send(&event);
        } else {
            CloseLegacyGameEvent event;
            eventBus->send(&event);
        }
    }
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

void MainFeatures::enableWfpRules()
{
    auto rules = globalData->firewallRuleList();
    if (rules.isEmpty()) {
        wfpController->createGlobalBlockRule(QString("%1 Global Rule").arg(qApp->applicationName()));
    } else {
        for (auto rule : rules) {
            QWfpController::RuleCondition condition;
            condition.processPath = rule->path;
            condition.protocol = QWfpController::Protocol(rule->protocol);
            if (rule->direction == NET_FW_RULE_DIR_IN) {
                condition.direction = FWP_DIRECTION_INBOUND;
            } else if (rule->direction == NET_FW_RULE_DIR_OUT) {
                condition.direction = FWP_DIRECTION_OUTBOUND;
            }
            wfpController->createRule(condition, QString("%1 Rules").arg(qApp->applicationName()));
        }
    }
}

void MainFeatures::disableWfpRules()
{
    wfpController->cleanupAllRules();
}

bool MainFeatures::ntTerminateProcess(DWORD pid)
{
    bool res = false;
    if (NtTerminateProcess) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
        if (hProcess) {
            if (NT_SUCCESS(NtTerminateProcess(hProcess, 0))) {
                qInfo("Execute NtTerminateProcess succeeds");
                res = true;
            } else {
                qWarning("Execute NtTerminateProcess fails");
            }
            CloseHandle(hProcess);
        } else {
            qWarning("ntTerminateProcess: OpenProcess fails");
        }
    } else {
        qWarning("NtTerminateProcess is null");
    }
    return res;
}

bool MainFeatures::terminateProcess(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    bool res = false;
    if (TerminateProcess(hProcess, 1)) {
        qInfo("Execute TerminateProcess succeeds");
        res = true;
    } else {
        qWarning("Execute TerminateProcess fails");
    }
    CloseHandle(hProcess);
    return res;
}

bool MainFeatures::endTask(HWND hwnd)
{
    if (!EndTask) {
        return false;
    }
    bool res = false;
    if (EndTask(hwnd, FALSE, TRUE)) {
        qInfo("Execute EndTask succeeds");
        res = true;
    } else {
        qWarning("Execute EndTask fails");
    }
    return res;
}
