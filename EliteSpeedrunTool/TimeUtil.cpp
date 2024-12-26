#include "TimeUtil.h"
#include <QEventLoop>
#include <QTimer>
#include <chrono>

TimeUtil::TimeUtil()
{
}

BOOLEAN TimeUtil::nanosleep(LONGLONG ns)
{
    /* Declarations */
    HANDLE timer; /* Timer handle */
    LARGE_INTEGER li; /* Time defintion */
    /* Create timer */
    if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
        return FALSE;
    /* Set timer properties */
    li.QuadPart = -ns;
    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
        CloseHandle(timer);
        return FALSE;
    }
    /* Start & wait for timer */
    WaitForSingleObject(timer, INFINITE);
    /* Clean resources */
    CloseHandle(timer);
    /* Slept without problems */
    return TRUE;
}

long long TimeUtil::currentTimestamp()
{
    // 获取时间点与 UNIX 时间原点之间的时间间隔
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    // 转换为毫秒级的时间戳
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void TimeUtil::delay(long long ms)
{
    QEventLoop loop;

    // 创建一个定时器，在ms后触发停止事件循环
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);

    // 进入事件循环，等待ms后定时器触发
    qDebug() << "Waiting for " << ms << " ms...";
    loop.exec(); // 事件循环会阻塞，直到 quit() 被调用
    qDebug() << "Finished waiting.";
}
