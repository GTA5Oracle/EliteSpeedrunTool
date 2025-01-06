#pragma once

#include "SubFuncsData.h"

Q_GLOBAL_STATIC(SubFuncsData, subFuncsDataInstance)

SubFuncsData::SubFuncsData()
    : QObject(nullptr)
{
    map = {
        qMakePair(DisplayInfoSubFunction::Timer, QVariant(0ull)),
        qMakePair(DisplayInfoSubFunction::Act3Headshot, QVariant(0)),
    };
}

SubFuncsData* SubFuncsData::instance()
{
    return subFuncsDataInstance;
}

void SubFuncsData::insert(const DisplayInfoSubFunction& key, const QVariant& value)
{
    map[key] = value;
    emit elementChanged(key, value);
}

void SubFuncsData::remove(const DisplayInfoSubFunction& key)
{
    map.remove(key);
    emit elementRemoved(key);
}

bool SubFuncsData::contains(const DisplayInfoSubFunction& key) const
{
    return map.contains(key);
}

QVariant SubFuncsData::value(const DisplayInfoSubFunction& key) const
{
    return map.value(key);
}

void SubFuncsData::forEach(std::function<void(const DisplayInfoSubFunction&, const QVariant&)> block)
{
    for (auto i = map.constBegin(); i != map.constEnd(); i++) {
        block(i.key(), i.value());
    }
}
