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

    virtual const QString getDisplayName() = 0;

    void remove();

protected:
    QFont missionNameFont = QFont();
    QFont font = QFont();

    QString kill4PPattern = tr("4人总击杀：%1");
    QString headshotPattern = tr("爆头：%1");
    QString vehicleDamagePattern = tr("载具损坏：%1");
    QString rashkovskyDamagePattern = tr("拉什科夫斯基受伤：%1");
};
