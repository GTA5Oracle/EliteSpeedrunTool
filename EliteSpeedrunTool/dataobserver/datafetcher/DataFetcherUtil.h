#pragma once

#include "HeadshotFetcher.h"
#include "Kill1Fetcher.h"
#include "Kill2Fetcher.h"
#include "Kill3Fetcher.h"
#include "Kill4Fetcher.h"
#include "KillFetcher.h"
#include "MissionHashFetcher.h"
#include "RashkovskyHurtFetcher.h"
#include "VehicleDamageFetcher.h"

#include <QObject>

#define dataFetcherUtil (DataFetcherUtil::instance())

class DataFetcherUtil : public QObject {
public:
    explicit DataFetcherUtil(QObject* parent = nullptr);

    static DataFetcherUtil* instance();

    HeadshotFetcher* headshotFetcher = new HeadshotFetcher(this);
    KillFetcher* killFetcher = new KillFetcher(this);
    Kill1Fetcher* kill1Fetcher = new Kill1Fetcher(this);
    Kill2Fetcher* kill2Fetcher = new Kill2Fetcher(this);
    Kill3Fetcher* kill3Fetcher = new Kill3Fetcher(this);
    Kill4Fetcher* kill4Fetcher = new Kill4Fetcher(this);
    MissionHashFetcher* missionHashFetcher = new MissionHashFetcher(this);
    RashkovskyHurtFetcher* rashkovskyDamageFetcher = new RashkovskyHurtFetcher(this);
    VehicleDamageFetcher* vehicleDamageFetcher = new VehicleDamageFetcher(this);

    QList<BaseDataFetcher*> dataFetchers = {
        headshotFetcher, killFetcher, kill1Fetcher,
        kill2Fetcher, kill3Fetcher,
        kill4Fetcher, rashkovskyDamageFetcher,
        vehicleDamageFetcher
    };

    QList<QString> dataFetchersId;

    QMap<QString, BaseDataFetcher*> dataFetcherMap;
};
