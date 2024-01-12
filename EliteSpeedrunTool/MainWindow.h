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

    void showDisplayInfo();

    void hideDisplayInfo();

    bool startReadHeadShot();

    void stopReadHeadShot();

    void startTimer(bool isContinue = false);

    void pauseTimer();

    void stopTimer();

    void zeroTimer();

    QString getFormattedTime(unsigned long long deltaTime, int* m, int* s, int* ms);

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

    bool displayInfoDialogIsShowing = false;
    DisplayInfoDialog* displayInfoDialog = nullptr;

    QHotkey* startFirewallHotkey = nullptr;
    QHotkey* stopFirewallHotkey = nullptr;

    QHotkey* startTimerHotkey = nullptr;
    QHotkey* pauseTimerHotkey = nullptr;
    QHotkey* stopTimerHotkey = nullptr;

    QHotkey* closeGameImmediatelyHotkey = nullptr;

    QLabel* labCurrentHotkey;

    QLabel* labState;

    short headshotCount = 0;
    QTimer* headShotTimer = nullptr;
    QTimer* topMostTimer = nullptr;

    QTimer* timer = nullptr;
    qint64 pausedTime = 0L; // 上次暂停的时间
    qint64 timerTime = 0L; // 计时器开始的时间
    QStateMachine timerStateMachine = QStateMachine(this); // 计时器状态

    DWORD pid = 0;

    HANDLE gtaHandle = NULL;

    QString acknowledgment = tr("感谢所有提供寻址协助的朋友：\n%1")
                                 .arg("CZ9946, Adstargod, limerenxey")
        + "\n\n" + tr("感谢所有提供赞助的朋友：\n%1，以及位未知昵称的朋友。");

    QString license = "<h3>" + QApplication::applicationDisplayName()
        + " v" + QApplication::applicationVersion() + "</h3>"
        + tr("<p>一个 GTAOL 末日降至：三号行动任务速通工具，支持快速启动防火墙、自动统计爆头数、计时。本工具仅读取内存，不会修改任何内存。</p>")
        + "<p>"
        + tr("程序开发：%1").arg(QApplication::organizationName()) + "<br>"
        + tr("游戏逆向：Zerkalo2147")
        + "</p>"
        + "<p>"
        + "<a href='https://t.me/SkyD666Chat'>" + tr("加入 Telegram 群组一起划水~") + "</a><br>"
        + "<a href='https://discord.gg/pEWEjeJTa3'>" + tr("加入 Discord 一起划水~") + "</a>"
        + "</p>"
        + "<p>" + tr("特别感谢 <b>ACT3 帮会</b>对本工具的宣传和支持。特别感谢 <b>CZ9946 和 Adstargod</b> 两位大佬，他们对本工具的研发提供了大量的帮助！") + "</p>"
        + "<p>" + tr("感谢所有提供赞助的朋友们！") + "</p>"
        + QString("<p>Copyright <font>&#169;</font> 2023 SkyD666</p>"
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
