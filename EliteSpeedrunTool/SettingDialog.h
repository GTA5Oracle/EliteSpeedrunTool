#pragma once

#include "DisplayInfoSubFunction.h"
#include "ui_SettingDialog.h"
#include <QDialog>

class SettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingDialog(QWidget* parent = nullptr);
    ~SettingDialog();

    QString pageName[11] = {
        tr("常规"),
        tr("信息展示"),
        tr("RTSS"),
        tr("断网"),
        tr("计时器"),
        tr("卡单"),
        tr("末日将至爆头"),
        tr("结束游戏"),
        tr("社交"),
        tr("语言"),
        tr("开发者选项"),
    };
    QString pageIcon[11] = {
        "://image/ic_setting.ico",
        "://image/ic_lighton.ico",
        "://image/ic_monitor_blue_grad.ico",
        "://image/ic_network.ico",
        "://image/ic_clock.ico",
        "://image/ic_user.ico",
        "://image/ic_file_eye.ico",
        "://image/ic_plug.ico",
        "://image/ic_users.ico",
        "://image/ic_language.ico",
        "://image/ic_doctor_watson.ico",
    };

protected:
    void initGeneralSettings();

    QString getSoundFile(QString dir = QString());

    int currentSubFunctionIndex = 0;
    DisplayInfoSubFunction currentSubFunction = DisplayInfoSubFunction::Timer;

    int currentRtssSubFunctionIndex = 0;
    DisplayInfoSubFunction currentRtssSubFunction = DisplayInfoSubFunction::Timer;

    void initFirewallSettings();

    void initTimerSettings();

    void initDisplayInfoSettings();

    void initRtssSettings();

    void initAct3Headshot();

    void initSuspendProcess();

    void initCloseGameImmediatelySettings();

    void initSocialSettings();

    void initLanguageSettings();

    void initDevelopOptionsSettings();

    void setDisplayInfoSubFunctionSettings(DisplayInfoSubFunction f);

    void setRtssSubFunctionSettings(DisplayInfoSubFunction f);

private:
    Ui::SettingDialog ui;

    // 内容对齐
    QList<QPair<int, QString>> horiAlign = {
        std::pair(Qt::AlignLeft, tr("左对齐")),
        std::pair(Qt::AlignHCenter, tr("居中")),
        std::pair(Qt::AlignRight, tr("右对齐"))
    };

    QList<QPair<int, QString>> vertAlign = {
        std::pair(Qt::AlignTop, tr("上对齐")),
        std::pair(Qt::AlignVCenter, tr("居中")),
        std::pair(Qt::AlignBottom, tr("下对齐"))
    };
};
