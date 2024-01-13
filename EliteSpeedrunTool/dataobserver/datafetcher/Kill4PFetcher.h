#ifndef KILL4PFETCHER_H
#define KILL4PFETCHER_H

#include "BaseDataFetcher.h"

class Kill4PFetcher : public BaseDataFetcher<int> {
    Q_OBJECT
public:
    explicit Kill4PFetcher(QObject* parent = nullptr);

    int fetchData();
};

#endif // KILL4PFETCHER_H
