#ifndef KILLHOSTCLIENT1FETCHER_H
#define KILLHOSTCLIENT1FETCHER_H

#include "BaseDataFetcher.h"

class KillHostClient1Fetcher : public BaseDataFetcher<int> {
    Q_OBJECT
public:
    explicit KillHostClient1Fetcher(QObject* parent = nullptr);

    int fetchData();
};

#endif // KILLHOSTCLIENT1FETCHER_H
