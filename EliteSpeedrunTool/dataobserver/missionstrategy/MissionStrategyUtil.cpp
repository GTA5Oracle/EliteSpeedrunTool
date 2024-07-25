#include "MissionStrategyUtil.h"
#include "GlobalData.h"
#include "dataobserver/datafetcher/DataFetcherUtil.h"

#include <QCoreApplication>

Q_GLOBAL_STATIC(MissionStrategyUtil, missionStrategyUtilInstance)

MissionStrategyUtil::MissionStrategyUtil(QObject* parent)
    : QObject { parent }
{
    for (auto m : missionStrategies) {
        missionIdToObject[m->id()] = m;
    }
    for (auto m : missionStrategies) {
        mMissionToDataFetchers[m->id()] = {};
    }
    readSettings();

    connect(qApp, &QCoreApplication::aboutToQuit, this, [=]() {
        writeSettings();
    });
}

MissionStrategyUtil* MissionStrategyUtil::instance()
{
    return missionStrategyUtilInstance;
}

void MissionStrategyUtil::readSettings()
{
    QSettings settings(globalData->getSettingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("MissionData");
    for (auto it = mMissionToDataFetchers.begin(); it != mMissionToDataFetchers.end(); ++it) {
        settings.beginGroup(it.key());
        QList<QString> fetchers = settings
                                      .value("DataFetchers",
                                          missionIdToObject[it.key()]->defaultDataFetchers().values())
                                      .toStringList();
        // Remove invalid data
        fetchers.removeIf([](const QString& id) {
            return !dataFetcherUtil->dataFetchersId.contains(id);
        });
        mMissionToDataFetchers[it.key()] += QSet<QString>(fetchers.begin(), fetchers.end());
        settings.endGroup();
    }
    settings.endGroup();
}

void MissionStrategyUtil::writeSettings()
{
    QSettings settings(globalData->getSettingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("MissionData");
    for (auto it = mMissionToDataFetchers.begin(); it != mMissionToDataFetchers.end(); ++it) {
        settings.beginGroup(it.key());
        settings.setValue("DataFetchers", it.value().values()); // Convert QSet<QString> to QStringList
        settings.endGroup();
    }
    settings.endGroup();
}

const QMap<QString, QSet<QString>>& MissionStrategyUtil::missionToDataFetchers() const
{
    return mMissionToDataFetchers;
}

void MissionStrategyUtil::updateMissionToDataFetchers(QString missionId, QSet<QString> fetchers)
{
    auto oldFetchers = mMissionToDataFetchers[missionId];
    mMissionToDataFetchers[missionId].clear();
    mMissionToDataFetchers[missionId] += fetchers;

    emit onMissionToDataFetchersChanged(missionId, oldFetchers, mMissionToDataFetchers[missionId]);
}

void MissionStrategyUtil::insertToMissionToDataFetchers(QString missionId, QString fetcherId)
{
    auto oldFetchers = mMissionToDataFetchers[missionId];
    mMissionToDataFetchers[missionId].insert(fetcherId);

    emit onMissionToDataFetchersChanged(missionId, oldFetchers, mMissionToDataFetchers[missionId]);
}

void MissionStrategyUtil::removeFromMissionToDataFetchers(QString missionId, QString fetcherId)
{
    auto oldFetchers = mMissionToDataFetchers[missionId];
    mMissionToDataFetchers[missionId].remove(fetcherId);

    emit onMissionToDataFetchersChanged(missionId, oldFetchers, mMissionToDataFetchers[missionId]);
}
