#ifndef RTSSUTIL_H
#define RTSSUTIL_H

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
    bool updateTimer(unsigned long long deltaTime);
    bool updateAct3Headshot(int headshot);

private:
    void initDisplayInfo();
    void initCrosshair();

    bool check();

    bool updateDisplayInfo();
    bool updateCrosshair();
    bool updateOsd(QString text, QString osdOwner);

    QString getTimeString(unsigned long long deltaTime);

    const int rtssSignature = 'S' | 'S' << 8 | 'T' << 16 | 'R' << 24;
    QString displayInfoOsdOwner = qAppName() + "-DisplayInfo";
    QString crosshairOsdOwner = qAppName() + "-Crosshair";

    bool overlayInited = false;

    QMap<DisplayInfoSubFunction, QString> subFunctionTextMap = {
        { DisplayInfoSubFunction::Timer, "" },
        { DisplayInfoSubFunction::Act3Headshot, "" }
    };
signals:
};

#endif // RTSSUTIL_H
