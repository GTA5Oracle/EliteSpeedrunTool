#pragma once

#include "ui_UpdateDialog.h"
#include <QDialog>
#include <QNetworkAccessManager>

class GitHubRelease : public QObject {
    Q_OBJECT
public:
    GitHubRelease(QObject* parent = nullptr);
    QString tagName;
    QString name;
    QString publishedAt;
    QString htmlUrl;
    QString body;
};

class UpdateDialog : public QDialog {
    Q_OBJECT

public:
    explicit UpdateDialog(GitHubRelease* release = nullptr, QWidget* parent = nullptr);

    static bool isNewVersion(QString remoteVersion);

    static bool isNewVersionTag(QString remoteVersion);

    static void sendCheckRequest(
        QObject* parent,
        std::function<void(GitHubRelease*)> const& onSuccess,
        std::function<void(int, QString)> const& onError);

    static GitHubRelease* getData(QByteArray raw);

protected:
    void updateViewToChecking();

    void updateViewToSuccess();

private:
    Ui::UpdateDialog ui;

    GitHubRelease* githubRelease = nullptr;
};
