#include "TimeUtil.h"
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
