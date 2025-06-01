#pragma once

#include "DisplayInfoSubFunction.h"
#include <QApplication>
#include <QMap>

#define rtssUtil (RtssUtil::instance())

class RtssUtil : public QObject {
    Q_OBJECT
public:
    explicit RtssUtil(QObject* parent = nullptr);

    static RtssUtil* instance();

    void initOverlay();
    void releaseOverlay();
    void releaseOverlay(QString osdOwner);

    void refreshAll();

private:
    void initDisplayInfo();
    void initCrosshair();

    bool check();

    bool updateDisplayInfo();
    bool updateCrosshair();
    bool updateOsd(QString text, QString osdOwner);

    QString getTimeString(QVariant v);
    QString getAutoTimeString(QVariant v);
    QString getFirewallString(QVariant v);
    QString getAct3Headshot(QVariant v);

    const int rtssSignature = 'S' | 'S' << 8 | 'T' << 16 | 'R' << 24;
    QString displayInfoOsdOwner = qAppName() + "-DisplayInfo";
    QString crosshairOsdOwner = qAppName() + "-Crosshair";

    bool overlayInited = false;

    QMap<DisplayInfoSubFunction, std::function<QString(QVariant)>> subFuncs = {
        qMakePair(DisplayInfoSubFunction::Timer, [this](QVariant v) { return getTimeString(v); }),
        // qMakePair(DisplayInfoSubFunction::AutoTimer, [this](QVariant v) { return getAutoTimeString(v); }),
        qMakePair(DisplayInfoSubFunction::Firewall, [this](QVariant v) { return getFirewallString(v); }),
        qMakePair(DisplayInfoSubFunction::Act3Headshot, [this](QVariant v) { return getAct3Headshot(v); })
    };
signals:
};
