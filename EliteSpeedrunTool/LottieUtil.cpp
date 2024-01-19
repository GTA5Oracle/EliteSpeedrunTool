#include "LottieUtil.h"

#include <QRandomGenerator>

Q_GLOBAL_STATIC(LottieUtil, lottieUtilInstance)

LottieUtil::LottieUtil()
{
    folder.setNameFilters(QStringList() << "*.json");
    lottieFiles = folder.entryList();
}

LottieUtil* LottieUtil::instance()
{
    return lottieUtilInstance;
}

// 随机选择一个文件
QString LottieUtil::randomLottie()
{
    if (lottieFiles.isEmpty()) {
        return "";
    }
    int randomIndex = QRandomGenerator::global()->bounded(lottieFiles.size());
    return folderPath + "/" + lottieFiles.at(randomIndex);
}
