#ifndef LOTTIEUTIL_H
#define LOTTIEUTIL_H

#include <QDir>
#define lottieUtil (LottieUtil::instance())

class LottieUtil : public QObject {
    Q_OBJECT
public:
    explicit LottieUtil();
    static LottieUtil* instance();

    QString randomLottie();

private:
    QString folderPath = "./lottie";
    QDir folder = QDir(folderPath);
    QStringList lottieFiles;

signals:
};

#endif // LOTTIEUTIL_H
