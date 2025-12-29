#pragma once

#include <QDir>
#include <QFileSystemWatcher>
#include <QStandardPaths>

#define pcSettingsUtil (PcSettingsUtil::instance())

class PcSettingsUtil : public QObject {
    Q_OBJECT
public:
    explicit PcSettingsUtil(QObject* parent = nullptr);

    static PcSettingsUtil* instance();

    QString getNewestPcSettingsFile();

    void updateProfilesDirPath(bool isEnhanced);

    bool readMissionState();
signals:
    void onMissionStateChanged(bool inMission);

private:
    QString pcSettingsFilename = "pc_settings.bin";
    QDir profilesDirPath;
    QFileSystemWatcher profilesDirWatcher;
    QFileSystemWatcher pcSettingsWatcher;
    QString newestPcSettings;
};
