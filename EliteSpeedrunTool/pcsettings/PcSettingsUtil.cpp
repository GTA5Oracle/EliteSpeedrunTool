#include "PcSettingsUtil.h"
#include "MemoryUtil.h"
#include <QFile>
#include <QDirIterator>
#include <QSaveFile>
#include <QtEndian>
#include <algorithm>
#include <iterator>

namespace {
constexpr quint32 pcSettingsWhitelist[] = {
    0, 1, 2, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
    100, 203, 204, 205, 207, 208, 211, 212, 213, 220, 221, 222, 223, 224, 225,
    226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
    241, 242, 243, 244, 245, 251, 260, 261, 262, 300, 301, 302, 303, 304, 305,
    306, 307, 308, 310, 311, 312, 313, 314, 315, 316, 317, 318, 320, 321, 322,
    323, 324, 325, 326, 327, 328, 329, 330, 331, 333, 334, 335, 336, 337, 338,
    339, 340, 412, 415, 710, 720, 721, 722, 723, 724, 725, 726, 727, 728, 729,
    750, 751, 752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762, 800, 801,
    802, 803, 804, 805, 806, 807, 810, 811, 900, 950, 951, 952, 953, 954, 955,
    956, 957, 958, 960, 961, 962, 964
};
}

Q_GLOBAL_STATIC(PcSettingsUtil, pcSettingsUtilInstance)

PcSettingsEditionScope PcSettingsEditionScopeUtil::fromString(const QString& scope)
{
    if (scope == QStringLiteral("Legacy")) {
        return PcSettingsEditionScope::Legacy;
    }
    if (scope == QStringLiteral("Enhanced")) {
        return PcSettingsEditionScope::Enhanced;
    }
    return PcSettingsEditionScope::Both;
}

QString PcSettingsEditionScopeUtil::toString(PcSettingsEditionScope scope)
{
    switch (scope) {
    case PcSettingsEditionScope::Legacy:
        return QStringLiteral("Legacy");
    case PcSettingsEditionScope::Enhanced:
        return QStringLiteral("Enhanced");
    case PcSettingsEditionScope::Both:
    default:
        return QStringLiteral("Both");
    }
}

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
    profilesDirPath = profilesDirectory(isEnhanced);
}

QDir PcSettingsUtil::profilesDirectory(bool isEnhanced) const
{
    return QDir(QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))
                    .filePath(QStringLiteral("Rockstar Games/%1/Profiles")
                                  .arg(isEnhanced ? QStringLiteral("GTAV Enhanced") : QStringLiteral("GTA V"))));
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

PcSettingsCleanResult PcSettingsUtil::cleanPcSettings(PcSettingsEditionScope scope)
{
    PcSettingsCleanResult result;
    if (scope == PcSettingsEditionScope::Legacy || scope == PcSettingsEditionScope::Both) {
        cleanProfilesDirectory(profilesDirectory(false), result);
    }
    if (scope == PcSettingsEditionScope::Enhanced || scope == PcSettingsEditionScope::Both) {
        cleanProfilesDirectory(profilesDirectory(true), result);
    }
    return result;
}

void PcSettingsUtil::cleanProfilesDirectory(const QDir& directory, PcSettingsCleanResult& result)
{
    if (!directory.exists()) {
        return;
    }

    const auto profileDirectories = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const auto& profileDirectory : profileDirectories) {
        const QString filePath = QDir(profileDirectory.absoluteFilePath()).filePath(pcSettingsFilename);
        if (!QFileInfo::exists(filePath)) {
            continue;
        }

        result.filesFound++;
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            result.failures.append({ filePath, tr("无法读取文件：%1").arg(file.errorString()) });
            continue;
        }
        const QByteArray originalData = file.readAll();
        const QString readError = file.errorString();
        const bool readFailed = file.error() != QFileDevice::NoError;
        file.close();
        if (readFailed) {
            result.failures.append({ filePath, tr("读取文件失败：%1").arg(readError) });
            continue;
        }
        if (originalData.size() % 8 != 0) {
            result.failures.append({ filePath, tr("文件长度不是 8 字节的倍数") });
            continue;
        }

        QByteArray cleanedData;
        cleanedData.reserve(originalData.size());
        for (qsizetype offset = 0; offset < originalData.size(); offset += 8) {
            const quint32 index = qFromLittleEndian<quint32>(
                reinterpret_cast<const uchar*>(originalData.constData() + offset));
            if (std::binary_search(std::begin(pcSettingsWhitelist), std::end(pcSettingsWhitelist), index)) {
                cleanedData.append(originalData.constData() + offset, 8);
            }
        }

        if (cleanedData == originalData) {
            result.filesUnchanged++;
            continue;
        }

        const QString backupPath = filePath + QStringLiteral(".bak");
        QSaveFile backupFile(backupPath);
        if (!backupFile.open(QIODevice::WriteOnly)
            || backupFile.write(originalData) != originalData.size()
            || !backupFile.commit()) {
            result.failures.append({ filePath, tr("备份失败：%1").arg(backupFile.errorString()) });
            continue;
        }

        QSaveFile outputFile(filePath);
        if (!outputFile.open(QIODevice::WriteOnly)
            || outputFile.write(cleanedData) != cleanedData.size()
            || !outputFile.commit()) {
            result.failures.append({ filePath, tr("写入清洗结果失败：%1").arg(outputFile.errorString()) });
            continue;
        }
        result.filesModified++;
    }
}
