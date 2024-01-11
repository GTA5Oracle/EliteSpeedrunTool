#include "LanguageUtil.h"
#include "GlobalData.h"
#include <QCoreApplication>

Q_GLOBAL_STATIC(LanguageUtil, instance)

QTranslator* LanguageUtil::translator = nullptr;
QTranslator* LanguageUtil::systemTranslator = nullptr;

LanguageUtil::LanguageUtil()
{
}

LanguageUtil* LanguageUtil::getInstance()
{
    return instance;
}

QString LanguageUtil::getFileName(const QString name)
{
    for (auto l : languages) {
        if (l.name == name) {
            return l.fileName;
        }
    }
    return "";
}

QString LanguageUtil::getDisplayName(const QString name)
{
    if (name == "default") {
        return tr("跟随系统");
    } else if (name == "zh") {
        return "简体中文";
    } else if (name == "en") {
        return "English";
    }
    return "";
}

QLocale::Language LanguageUtil::getQLocaleLanguage(const QString name)
{
    if (name == "default") {
        return QLocale().language();
    } else if (name == "zh") {
        return QLocale::Chinese;
    } else if (name == "en") {
        return QLocale::English;
    }
    return QLocale().language();
}

void LanguageUtil::applyLanguage()
{
    if (translator) {
        QCoreApplication::removeTranslator(translator);
        translator->deleteLater();
    }
    translator = new QTranslator();
    if (systemTranslator) {
        QCoreApplication::removeTranslator(systemTranslator);
        systemTranslator->deleteLater();
    }
    systemTranslator = new QTranslator();

    bool setTranslator = false;
    if (!globalData->language().isEmpty()) {
        tryApplySystemLanguage();
        QString fileName = LanguageUtil::getInstance()->getFileName(globalData->language());
        if (fileName == "*") { // 简体中文，无需更改自定义组件的翻译
            setTranslator = true;
        } else if (!fileName.isEmpty() && translator->load(fileName, "./translations")) {
            QCoreApplication::installTranslator(translator);
            setTranslator = true;
        }
    }
    if (!setTranslator) {
        QLocale locale;
        tryApplySystemLanguage(locale);
        if (locale.language() != QLocale::Chinese) {
            if (translator->load(locale, "", "", "./translations", ".qm")) {
                QCoreApplication::installTranslator(translator);
            }
        }
    }
}

// 加载系统控件的翻译
void LanguageUtil::tryApplySystemLanguage(QLocale locale)
{
    if (systemTranslator->load(locale, "qt", "_", "./translations", ".qm")) {
        QCoreApplication::installTranslator(systemTranslator);
    }
}

// 加载系统控件的翻译
void LanguageUtil::tryApplySystemLanguage()
{
    tryApplySystemLanguage(getQLocaleLanguage(globalData->language()));
}
