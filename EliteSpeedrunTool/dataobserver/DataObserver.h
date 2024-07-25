#pragma once

#include "dataobserver/datafetcher/MissionHashFetcher.h"
#include "dataobserver/missionstrategy/BaseMissionStrategy.h"
#include <QObject>
#include <QTimer>

#define dataObserver (DataObserver::instance())

class DataObserver : public QObject {
    Q_OBJECT
public:
    explicit DataObserver(QObject* parent = nullptr);
    ~DataObserver();

    static DataObserver* instance();

    BaseMissionStrategy* getMissionStrategy() const;
    void setMissionStrategy(BaseMissionStrategy* newMissionStrategy);

    void startObserve();
    void stopObserve();
    void destruct(); // 析构整个DataObserver，这时会发出onXxxRemoved信号，让其他持有QLabel的控件取消持有

    QList<QLabel*> getDisplayLabels();

protected:
    void onTimeout();
    void onMissionHashTimeout();

private:
    QTimer* missionHashTimer = new QTimer(this);
    QTimer* timer = new QTimer(this);

    unsigned long long lastMissionHash = 0;
    MissionHashFetcher missionHashFetcher = MissionHashFetcher();
    BaseMissionStrategy* missionStrategy;

    bool isObserving = false;
    bool destructed = false;

signals:
    void onLabelsAdded(QList<QLabel*>);
    void onDisplayLabelsAdded(QList<QLabel*>);
    void onLabelsRemoved(QList<QLabel*>);
    void onDisplayLabelsRemoved(QList<QLabel*>);
    void onStartObserve();
    void onStopObserve();
    void onMissionChanged();

public:
    QMap<const unsigned long long, BaseMissionStrategy*> hashToMissionStrategy;
};
