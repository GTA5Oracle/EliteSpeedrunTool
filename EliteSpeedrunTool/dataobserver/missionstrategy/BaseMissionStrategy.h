#pragma once

#include <QLabel>
#include <QList>

class BaseMissionStrategy : public QObject {
    Q_OBJECT
public:
    explicit BaseMissionStrategy(QObject* parent = nullptr);
    ~BaseMissionStrategy();

    virtual QList<QLabel*> getDisplayLabels() = 0;
    virtual QList<QLabel*> getLabels() = 0;

    virtual void updateInfo() = 0;

    void remove();
};
