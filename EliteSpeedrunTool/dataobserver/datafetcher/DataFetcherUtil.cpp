#include "DataFetcherUtil.h"

Q_GLOBAL_STATIC(DataFetcherUtil, dataFetcherUtilInstance)

DataFetcherUtil::DataFetcherUtil(QObject* parent)
    : QObject { parent }
{
    for (auto f : dataFetchers) {
        dataFetcherMap[f->id()] = f;
        dataFetchersId.append(f->id());
    }
}

DataFetcherUtil* DataFetcherUtil::instance()
{
    return dataFetcherUtilInstance;
}
