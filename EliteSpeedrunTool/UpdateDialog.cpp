#include "UpdateDialog.h"
#include "GlobalData.h"
#include "LanguageUtil.h"

#include <QDesktopServices>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include <QMovie>
#include <QNetworkReply>
#include <QTimeZone>

UpdateDialog::UpdateDialog(GitHubRelease* release, QWidget* parent)
    : QDialog(parent)
    , githubRelease(release)
{
    ui.setupUi(this);

    ui.labCurrentVersion->setText(tr("当前版本：%1").arg(QApplication::applicationVersion()));
    QMovie* movie = new QMovie("://image/ic_drum.gif");
    ui.labDrum->setMovie(movie);
    movie->start();

    connect(ui.pbDownload, &QPushButton::clicked, this, [=]() {
        if (githubRelease) {
            QDesktopServices::openUrl(QUrl(githubRelease->htmlUrl));
        }
    });

    connect(ui.pbIgnoreThisVersion, &QPushButton::clicked, this, [=]() {
        if (githubRelease) {
            globalData->setIgnoredNewVersion(githubRelease->tagName);
            close();
        }
    });

    if (githubRelease) {
        githubRelease->setParent(this);
        updateViewToSuccess();
    } else {
        updateViewToChecking();
        sendCheckRequest(
            this,
            [=](GitHubRelease* release) {
                if (githubRelease) {
                    delete githubRelease;
                }
                githubRelease = release;
                githubRelease->setParent(this);
                updateViewToSuccess();
            },
            [=](int code, QString msg) {
                ui.pbUpdate->setRange(0, 1);
                ui.pbUpdate->setValue(0);
                ui.labResult->setText(tr("错误码：%1\n错误信息：%2").arg(QString::number(code), msg));
                QMessageBox::information(this, QString(), tr("获取更新信息失败！"), QMessageBox::Ok);
            });
    }
}

void UpdateDialog::updateViewToChecking()
{
    ui.textEdit->setVisible(false);
    ui.pbDownload->setVisible(false);
    ui.pbIgnoreThisVersion->setVisible(false);
    ui.labReleaseTime->setVisible(false);
    ui.labResult->setText(tr("正在检查更新，请稍候..."));
    ui.pbUpdate->setRange(0, 0); // 繁忙状态
}

void UpdateDialog::sendCheckRequest(
    QObject* parent,
    std::function<void(GitHubRelease*)> const& onSuccess,
    std::function<void(int, QString)> const& onError)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager;

    connect(manager, &QNetworkAccessManager::finished, parent, [=](QNetworkReply* reply) {
        QByteArray data = reply->readAll();
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (code == 200) {
            onSuccess(getData(data));
        } else {
            onError(code, QString(data));
        }
        reply->deleteLater();
        manager->disconnect();
        manager->deleteLater();
    });

    manager->get(QNetworkRequest(QUrl("https://api.github.com/repos/GTA5Oracle/EliteSpeedrunTool/releases/latest")));
}

bool UpdateDialog::isNewVersion(QString remoteVersion)
{
    bool hasNewVersion = false;
    try {
        if (remoteVersion.front().toLower() == 'v') {
            remoteVersion.removeFirst();
        }
        QString version = QApplication::applicationVersion();
        if (version.front().toLower() == 'v') {
            version.removeFirst();
        }
        auto remoteVersions = remoteVersion.split("-");
        auto remoteSubVersions = remoteVersions[0].split(".");

        auto versions = version.split("-");
        auto subVersions = versions[0].split(".");

        for (int i = 0; i < qMin(remoteSubVersions.size(), subVersions.size()); i++) {
            if (subVersions[i].toInt() < remoteSubVersions[i].toInt()) {
                hasNewVersion = true;
                break;
            } else if (subVersions[i].toInt() > remoteSubVersions[i].toInt()) {
                hasNewVersion = false;
                break;
            }
        }
    } catch (const std::exception& ex) {
        qCritical() << "check isNewVersion failed" << ex.what();
    } catch (const std::string& ex) {
        qCritical() << "check isNewVersion failed" << ex;
    } catch (...) {
        qCritical() << "check isNewVersion failed";
    }

    return hasNewVersion;
}

bool UpdateDialog::isNewVersionTag(QString remoteVersion)
{
    bool hasNewVersion = false;
    try {
        bool ok = true;
        if (remoteVersion.toInt() > qEnvironmentVariableIntValue("ApplicationVersionCode", &ok)) {
            hasNewVersion = true;
        }
        if (!ok) {
            qCritical() << "check isNewVersionTag failed, ok is false";
        }
    } catch (const std::exception& ex) {
        qCritical() << "check isNewVersionTag failed" << ex.what();
    } catch (const std::string& ex) {
        qCritical() << "check isNewVersionTag failed" << ex;
    } catch (...) {
        qCritical() << "check isNewVersionTag failed";
    }

    return hasNewVersion;
}

GitHubRelease* UpdateDialog::getData(QByteArray raw)
{
    GitHubRelease* githubRelease = new GitHubRelease;

    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(raw, &jsonParseError);
    QJsonObject jsonObject = jsonDocument.object();

    QJsonValue name = jsonObject.take("name");
    QJsonValue tagName = jsonObject.take("tag_name");
    QJsonValue htmlUrl = jsonObject.take("html_url");
    QJsonValue body = jsonObject.take("body");
    QJsonValue publishedAt = jsonObject.take("published_at");
    if (name.isUndefined() || tagName.isUndefined() || htmlUrl.isUndefined() || body.isUndefined() || publishedAt.isUndefined()) {
        delete githubRelease;
        return nullptr;
    }
    githubRelease->name = name.toString();
    githubRelease->tagName = tagName.toString();
    githubRelease->htmlUrl = htmlUrl.toString();
    githubRelease->body = body.toString();
    githubRelease->publishedAt = publishedAt.toString();

    return githubRelease;
}

void UpdateDialog::updateViewToSuccess()
{
    if (!githubRelease) {
        return;
    }

    ui.pbUpdate->setRange(0, 100);
    ui.pbUpdate->setValue(100);

    bool hasNewVersion = isNewVersionTag(githubRelease->tagName);
    ui.textEdit->setVisible(hasNewVersion);
    ui.pbIgnoreThisVersion->setVisible(hasNewVersion);
    ui.pbDownload->setVisible(hasNewVersion);
    ui.labReleaseTime->setVisible(hasNewVersion);
    if (hasNewVersion) {
        auto name = githubRelease->name;
        if (name.startsWith('v')) {
            name = name.mid(1);
        }
        ui.labResult->setText(tr("发现新版本：%1").arg(name));
        auto dateTime = QDateTime::fromString(githubRelease->publishedAt, Qt::ISODate);
        dateTime.setTimeSpec(Qt::UTC);
        QLocale::Language language = LanguageUtil::getQLocaleLanguage(globalData->language());
        QLocale locale = QLocale(language);
        ui.labReleaseTime->setText(tr("发布时间：%1")
                                       .arg(dateTime
                                                .toLocalTime()
                                                .toString(locale.dateTimeFormat())));
        ui.textEdit->setText(githubRelease->body);
    } else {
        ui.labResult->setText(tr("当前已是最新版本！"));
    }
}

GitHubRelease::GitHubRelease(QObject* parent)
    : QObject(parent)
{
}
