#pragma once

#include <QTimer>

#define autoTimer (AutoTimer::instance())

class AutoTimer : public QObject {
    Q_OBJECT
public:
    explicit AutoTimer(QObject* parent = nullptr);

    static AutoTimer* instance();

    void start();

    void stop();

signals:
    void onUpdate(long long time);

private:
    long long startTime = 0;
    QTimer timer;
};
