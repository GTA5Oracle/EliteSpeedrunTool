#pragma once

#include "displayinfo/DisplayInfoSubFunction.h"
#include <QMap>
#include <QVariant>

#define subFuncsData (SubFuncsData::instance())

class SubFuncsData : public QObject {
    Q_OBJECT

public:
    SubFuncsData();

    static SubFuncsData* instance();

    void insert(const DisplayInfoSubFunction& key, const QVariant& value);

    void remove(const DisplayInfoSubFunction& key);

    bool contains(const DisplayInfoSubFunction& key) const;

    QVariant value(const DisplayInfoSubFunction& key) const;

    void forEach(std::function<void(const DisplayInfoSubFunction& key, const QVariant& value)> block);

signals:
    void elementChanged(const DisplayInfoSubFunction& key, const QVariant& value);
    void elementRemoved(const DisplayInfoSubFunction& key);

private:
    QMap<DisplayInfoSubFunction, QVariant> map;
};
