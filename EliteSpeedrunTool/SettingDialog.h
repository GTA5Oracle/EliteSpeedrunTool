#pragma once

#include "DisplayInfoSubFunction.h"
#include "ui_SettingDialog.h"
#include <QDialog>

class SettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingDialog(QWidget* parent = nullptr);
    ~SettingDialog();

    QString pageName[8] = { tr("常规"), tr("信息展示"), tr("断网"), tr("爆头"), tr("计时器"), tr("结束游戏"), tr("社交"), tr("语言") };
    QString pageIcon[8] = {
        "://image/ic_setting.ico",
        "://image/ic_lighton.ico",
        "://image/ic_network.ico",
        "://image/ic_spy.ico",
        "://image/ic_clock.ico",
        "://image/ic_plug.ico",
        "://image/ic_users.ico",
        "://image/ic_language.ico",
    };

protected:
    void initGeneralSettings();

    QString getSoundFile(QString dir = QString());

    int currentSubFunctionIndex = 0;
    DisplayInfoSubFunction currentSubFunction = DisplayInfoSubFunction::Headshot;

    void initFirewallSettings();

    void initHeadshotSettings();

    void initTimerSettings();

    void initDisplayInfoSettings();

    void initCloseGameImmediatelySettings();

    void initSocialSettings();

    void setDisplayInfoCententSettings(DisplayInfoSubFunction f);

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
