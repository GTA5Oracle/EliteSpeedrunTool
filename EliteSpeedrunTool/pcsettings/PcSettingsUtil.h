#pragma once

#include <QDir>
#include <QFileSystemWatcher>
#include <QList>
#include <QStandardPaths>
#include <QString>

#define pcSettingsUtil (PcSettingsUtil::instance())

enum class PcSettingsEditionScope {
    Legacy,
    Enhanced,
    Both
};

namespace PcSettingsEditionScopeUtil {
PcSettingsEditionScope fromString(const QString& scope);
QString toString(PcSettingsEditionScope scope);
}

struct PcSettingsCleanFailure {
    QString filePath;
    QString reason;
};

struct PcSettingsCleanResult {
    int filesFound = 0;
    int filesModified = 0;
    int filesUnchanged = 0;
    QList<PcSettingsCleanFailure> failures;
};

class PcSettingsUtil : public QObject {
    Q_OBJECT
public:
    explicit PcSettingsUtil(QObject* parent = nullptr);

    static PcSettingsUtil* instance();

    QString getNewestPcSettingsFile();

    void updateProfilesDirPath(bool isEnhanced);

    bool readMissionState();

    PcSettingsCleanResult cleanPcSettings(PcSettingsEditionScope scope);
signals:
    void onMissionStateChanged(bool inMission);

private:
    QDir profilesDirectory(bool isEnhanced) const;
    void cleanProfilesDirectory(const QDir& directory, PcSettingsCleanResult& result);

    QString pcSettingsFilename = "pc_settings.bin";
    QDir profilesDirPath;
    QFileSystemWatcher profilesDirWatcher;
    QFileSystemWatcher pcSettingsWatcher;
    QString newestPcSettings;
};
