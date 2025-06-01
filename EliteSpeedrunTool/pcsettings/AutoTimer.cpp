#include "AutoTimer.h"
#include "PcSettingsUtil.h"
#include <QDateTime>

Q_GLOBAL_STATIC(AutoTimer, autoTimerInstance)

AutoTimer::AutoTimer(QObject* parent)
    : QObject { parent }
{
    connect(&timer, &QTimer::timeout, this, [this]() {
        emit onUpdate(QDateTime::currentDateTime().toMSecsSinceEpoch() - startTime);
    });
    // connect(pcSettingsUtil, &PcSettingsUtil::onMissionStateChanged, this, [this](bool inMission) {
    //     if (inMission) {
    //         start();
    //     } else {
    //         stop();
    //     }
    // });
}

AutoTimer* AutoTimer::instance()
{
    return autoTimerInstance;
}

void AutoTimer::start()
{
    if (!timer.isActive()) {
        startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        timer.start(20);
    }
}

void AutoTimer::stop()
{
    timer.stop();
}
