#include "PcSettingsUtil.h"
#include "MemoryUtil.h"
#include <QDirIterator>

Q_GLOBAL_STATIC(PcSettingsUtil, pcSettingsUtilInstance)

PcSettingsUtil::PcSettingsUtil(QObject* parent)
    : QObject { parent }
{
    updateProfilesDirPath(memoryUtil->isEnhanced());
    profilesDirWatcher.addPath(profilesDirPath.path());

    newestPcSettings = getNewestPcSettingsFile();
    pcSettingsWatcher.addPath(newestPcSettings);

    connect(&profilesDirWatcher, &QFileSystemWatcher::directoryChanged, this, [this](const QString& path) {
        auto f = getNewestPcSettingsFile();
        if (f != newestPcSettings) {
            pcSettingsWatcher.removePath(newestPcSettings);
            newestPcSettings = f;
            pcSettingsWatcher.addPath(f);
            emit onMissionStateChanged(readMissionState());
        }
    });
    connect(&pcSettingsWatcher, &QFileSystemWatcher::fileChanged, this, [this](const QString& path) {
        emit onMissionStateChanged(readMissionState());
    });
}

PcSettingsUtil* PcSettingsUtil::instance()
{
    return pcSettingsUtilInstance;
}

QString PcSettingsUtil::getNewestPcSettingsFile()
{
    QDirIterator it(profilesDirPath.path(), QDir::Files, QDirIterator::Subdirectories);
    long long newestPcSettingsLastModified = 0;
    QString newestPcSettings = "";
    while (it.hasNext()) {
        QString filePath = it.next();
        if (it.fileName() != pcSettingsFilename) {
            continue;
        }

        QFileInfo fileInfo(filePath);
        auto lastModified = fileInfo.lastModified().toMSecsSinceEpoch();

        if (lastModified > newestPcSettingsLastModified) {
            newestPcSettingsLastModified = lastModified;
            newestPcSettings = filePath;
        }
    }
    return newestPcSettings;
}

void PcSettingsUtil::updateProfilesDirPath(bool isEnhanced)
{
    profilesDirPath = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    profilesDirPath.cd("Rockstar Games");
    profilesDirPath.cd(isEnhanced ? "GTAV Enhanced" : "GTA V");
    profilesDirPath.cd("Profiles");
}

bool PcSettingsUtil::readMissionState()
{
    bool inMission = false;
    if (newestPcSettings.isEmpty()) {
        return inMission;
    }
    QFile file(newestPcSettings);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open pc_settings.bin: " << newestPcSettings;
        return inMission;
    }

    QByteArray data = file.readAll();
    file.close();
    QByteArray pattern = QByteArray::fromHex("8D03");

    qint64 pos = data.indexOf(pattern);
    if (pos != -1) {
        auto value = (data[pos + 4] << 8) + data[pos + 5];
        qDebug() << "pc_settings.bin state: " << value;
        inMission = value != 0;
    } else {
        qDebug() << "8D 03 not found";
    }
    return inMission;
}
