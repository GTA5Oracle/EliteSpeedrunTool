#include <QLocale>
#include <QObject>
#include <QString>
#include <QTranslator>

#pragma once

struct Language {
    Language(const QString name, const QString fileName)
    {
        this->name = name;
        this->fileName = fileName;
    }
    QString name;
    QString fileName;
};

class LanguageUtil : public QObject {

    Q_OBJECT

public:
    LanguageUtil();

    static LanguageUtil* getInstance();

    static QString getDisplayName(const QString name);

    static QLocale::Language getQLocaleLanguage(const QString name);

    QString getFileName(const QString name);

    static QTranslator* translator;
    static QTranslator* systemTranslator;

    static void applyLanguage();

    Language languages[3] = {
        Language("default", ""),
        Language("en", "en.qm"),
        Language("zh", "*")
    };

private:
    static void tryApplySystemLanguage(QLocale locale);
    static void tryApplySystemLanguage();
};
