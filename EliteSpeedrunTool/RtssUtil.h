#ifndef RTSSUTIL_H
#define RTSSUTIL_H

#include "DisplayInfoSubFunction.h"
#include <QMap>

#define rtssUtil (RtssUtil::instance())

class RtssUtil : public QObject {
    Q_OBJECT
public:
    explicit RtssUtil(QObject* parent = nullptr);

    static RtssUtil* instance();

    void initOverlay();
    void releaseOverlay();

    bool updateTimer(unsigned long long deltaTime);

    bool updateAct3Headshot(int headshot);

private:
    bool check();

    bool updateOsd();
    bool updateOsd(QString text);

    QString getTimeString(unsigned long long deltaTime);

    const int rtssSignature = 'S' | 'S' << 8 | 'T' << 16 | 'R' << 24;

    bool overlayInited = false;

    QMap<DisplayInfoSubFunction, QString> subFunctionTextMap = {
        { DisplayInfoSubFunction::Timer, "" },
        { DisplayInfoSubFunction::Act3Headshot, "" }
    };
signals:
};

#endif // RTSSUTIL_H
