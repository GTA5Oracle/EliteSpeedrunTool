#pragma once

#include <QLabel>
#include <QList>

class BaseMissionStrategy : public QObject {
    Q_OBJECT
public:
    explicit BaseMissionStrategy(QObject* parent = nullptr);
    ~BaseMissionStrategy();

    virtual QString id() = 0;

    virtual const QSet<unsigned long long> missionHash() = 0;

    void setCurrentStrategy(bool isCurrent);
    bool isCurrentStrategy();

    QList<QLabel*> getDisplayLabels();
    QList<QLabel*> getLabels();

    void updateInfo();

    virtual const QString getDisplayName() = 0;
    virtual const QString getUniqueDisplayName();

    virtual QSet<QString> defaultDataFetchers();

protected:
    QFont missionNameFont = QFont();
    bool isCurrent = false;
    bool isLaterInited = false;

    QLabel labMissionName = QLabel();
};
