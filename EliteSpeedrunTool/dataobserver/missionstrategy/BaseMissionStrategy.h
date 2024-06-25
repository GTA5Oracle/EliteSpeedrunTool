#pragma once

#include "DisplayInfoSubFunction.h"
#include <QLabel>
#include <QList>

class BaseMissionStrategy : public QObject {
    Q_OBJECT
public:
    explicit BaseMissionStrategy(QObject* parent = nullptr);
    ~BaseMissionStrategy();

    void setCurrentStrategy(bool isCurrent);
    bool isCurrentStrategy();
    bool labelIsVisible(QLabel* label);
    void resetLabelData();

    virtual QList<QLabel*> getDisplayLabels() = 0;
    virtual QList<QPair<QLabel*, DisplayInfoSubFunctionItem*>> getDisplayLabelsAndItems() = 0;
    virtual QList<QLabel*> getLabels() = 0;

    virtual void updateInfo() = 0;

    virtual const QString getDisplayName() = 0;

protected:
    void initSettings();
    void initLabelFont();
    void setLabelDisplay(QLabel* label, DisplayInfoSubFunctionItem* item);
    void setLabelTextAlignment(QLabel* label, DisplayInfoSubFunctionItem* item);
    void setLabelFont(QLabel* label, DisplayInfoSubFunctionItem* item);
    void setLabelTextStyle(QLabel* label, DisplayInfoSubFunctionItem* item);

    QString getKillPattern();
    QString getHeadshotPattern();
    QString getVehicleDamagePattern();
    QString getPlaneDamagePattern(); // Valkyrie/Avenger机损
    QString getRashkovskyHurtPattern();

    void updateKillLabel(int newValue);
    void updateHeadshotLabel(int newValue);
    void updateVehicleDamageLabel(float newValue);
    void updatePlaneDamageLabel(float newValue); // Valkyrie/Avenger机损
    void updateRashkovskyHurtLabel(float newValue);

private:
    void initGlobalDataConnects();
    void initGlobalDataConnects(QLabel* label, DisplayInfoSubFunctionItem* item);

protected:
    bool isCurrent = false;
    bool isLaterInited = false;

    QMap<QLabel*, bool> labelVisibleMap = {};

    QFont missionNameFont = QFont();
    QFont font = QFont();

    QLabel labMissionName = QLabel();

    QLabel labDisplayVehicleDamage = QLabel();
    QLabel labVehicleDamage = QLabel();
    QLabel labDisplayPlaneDamage = QLabel();
    QLabel labPlaneDamage = QLabel();
    QLabel labDisplayHeadshot = QLabel();
    QLabel labDisplayRashkovskyDamage = QLabel();
    QLabel labRashkovskyDamage = QLabel();
    QLabel labHeadshot = QLabel();
    QLabel labDisplayKill = QLabel();
    QLabel labKill = QLabel();
};
