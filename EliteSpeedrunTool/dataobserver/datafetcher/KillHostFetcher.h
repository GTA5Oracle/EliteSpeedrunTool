#ifndef KILLHOSTFETCHER_H
#define KILLHOSTFETCHER_H

#include "BaseDataFetcher.h"

class KillHostFetcher : public BaseDataFetcher<int> {
    Q_OBJECT
public:
    explicit KillHostFetcher(QObject* parent = nullptr);

    int fetchData();
};

#endif // KILLHOSTFETCHER_H
