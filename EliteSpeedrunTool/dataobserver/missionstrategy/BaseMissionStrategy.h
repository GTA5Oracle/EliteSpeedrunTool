#pragma once

#include "DisplayInfoSubFunction.h"
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
    void setLabelTextStyle(
        QLabel* label,
        const QColor& textColor,
        const QColor& textShadowColor,
        qreal textShadowBlurRadius,
        const QPointF& textShadowOffset);

    void initGlobalDataConnects(QLabel* label, DisplayInfoSubFunctionItem* item);

protected:
    QFont missionNameFont = QFont();
    QFont font = QFont();

    QString killPattern = tr("总击杀：%1");
    QString headshotPattern = tr("总爆头：%1");
    QString vehicleDamagePattern = tr("车损：%1");
    QString rashkovskyDamagePattern = tr("拉什科夫斯基受伤：%1");
};
