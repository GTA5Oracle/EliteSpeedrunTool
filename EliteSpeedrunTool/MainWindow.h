#pragma once

#include "DisplayInfoDialog.h"
#include "ui_MainWindow.h"
#include <QDateTime>
#include <QHotkey>
#include <QLabel>
#include <QMainWindow>
#include <QStateMachine>
#include <QSystemTrayIcon>
#include <QTimer>
#include <windows.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void initSystemTray();
    void closeSystemTray();

protected:
    void closeEvent(QCloseEvent* event) override;

    void initGlobalDataConnects();

    void checkUpdate();

    void removeAllHotkeys();

    void removeHotkey(QHotkey*& h);

    void setHotkey();

    void initMenu();

    void initFirewall();

    void initCloseGameImmediately();

    void initAutoTimer();

    void initMissionData();

    void initBadSport();

    void initDisplayInfoDialogData();

    void showDisplayInfo();

    void hideDisplayInfo();

    void startTimer(bool isContinue = false);

    void pauseTimer();

    void stopTimer();

    void zeroTimer();

    QString getFormattedTime(
        unsigned long long deltaTime,
        unsigned int* m,
        unsigned int* s,
        unsigned int* ms);

    void updateTimerString(qint64 currentDateTime = QDateTime::currentDateTime().toMSecsSinceEpoch());

    void updateAutoTimerString(unsigned long long deltaTime);

    void initTimerStateMachine();

    void closeGameImmediately();

signals:
    void toTimerStoppedAndZeroState();
    void toTimerStoppedOrStoppedAndZeroState();
    void toTimerRunningState();
    void tryToTimerRunningState();
    void tryToTimerStoppedOrStoppedAndZeroState();

private:
    Ui::MainWindow ui;

    bool waitingToExit = false;

    QSystemTrayIcon* systemTray = nullptr;
    QMenu* systemTrayMenu = nullptr;

    static const QString hotkeyStatePattern;

    // 保证多线程原子性，不过现在用的是QTimer，并没有涉及到多线程
    std::atomic<bool> displayInfoDialogIsShowing = false;
    DisplayInfoDialog* displayInfoDialog = nullptr;

    QHotkey* startFirewallHotkey = nullptr;
    QHotkey* stopFirewallHotkey = nullptr;

    QHotkey* startTimerHotkey = nullptr;
    QHotkey* pauseTimerHotkey = nullptr;
    QHotkey* stopTimerHotkey = nullptr;

    QHotkey* closeGameImmediatelyHotkey = nullptr;

    QLabel* labCurrentHotkey;

    QLabel* labState;

    QTimer* topMostTimer = nullptr;

    QTimer* timer = nullptr;
    qint64 pausedTime = 0L; // 上次暂停的时间
    qint64 timerTime = 0L; // 计时器开始的时间
    QStateMachine timerStateMachine = QStateMachine(this); // 计时器状态

    DWORD pid = 0;

    HANDLE gtaHandle = NULL;

    QString license = "<h3>" + QApplication::applicationDisplayName()
        + " v" + QApplication::applicationVersion() + "</h3>"
        + tr("<p>一个 GTAOL 精英、速通工具，支持快速启动防火墙、自动统计任务数据、计时等。本工具仅读取内存，不会修改任何内存。</p>")
        + "<p>"
        + tr("程序开发：SkyD666") + "<br>"
        + tr("游戏逆向：Zerkalo2147")
        + "</p>"
        + "<p>"
        + tr("加入 QQ 群：431228020") + "<br>"
        + "<a href='https://t.me/SkyD666Chat'>" + tr("加入 Telegram 群组一起划水~") + "</a><br>"
        + "<a href='https://discord.gg/pEWEjeJTa3'>" + tr("加入 Discord 一起划水~") + "</a>"
        + "</p>"
        + QString("<p>Copyright <font>&#169;</font> 2024 SkyD666</p>"
                  R"(<p>This program is free software: you can redistribute it and/or modify
                     it under the terms of the GNU General Public License as published by
                     the Free Software Foundation, either version 3 of the License, or
                     (at your option) any later version.</p>
                     <p>This program is distributed in the hope that it will be useful,
                     but WITHOUT ANY WARRANTY; without even the implied warranty of
                     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
                     GNU General Public License for more details.</p>
                     <p>You should have received a copy of the GNU General Public License
                     along with this program. If not, see
                     &lt;<a href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a>&gt;.</p>)");
};
