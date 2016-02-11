#ifndef COMMON_H
#define COMMON_H

#ifdef Q_OS_UNIX
#include <time.h>
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <waveform.h>
#include <QThread>

#ifdef Q_OS_UNIX
#define CLOCK CLOCK_MONOTONIC
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#include <time.h>
#endif

namespace QFlow{

class Time
{
public:
static inline Timestamp now()
{
    Timestamp t;
#ifdef Q_OS_WIN
    LARGE_INTEGER ticksPerSecond;
    QueryPerformanceFrequency(&ticksPerSecond);
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    t = now.QuadPart/(ticksPerSecond.QuadPart/1E6);
#endif
#ifdef Q_OS_UNIX
    timespec now;
    clock_gettime(CLOCK, &now);
    t = (now.tv_sec * 1E6) + now.tv_nsec/1E3;
#endif
    return t;
}
static inline void sleep(Timestamp wakeTime)
{
#ifdef Q_OS_UNIX
                timespec wakeTimespec = TimestampToTimespec(wakeTime);
                clock_nanosleep(CLOCK, TIMER_ABSTIME, &wakeTimespec, NULL);
#endif
#ifdef Q_OS_WIN
                long long period = wakeTime - Time::now();
                if(period > 0) QThread::usleep(period);
#endif
}

#ifdef Q_OS_UNIX
static inline timespec TimestampToTimespec(Timestamp t)
{
    timespec result;
    result.tv_sec = 0;
    if(t >= 1E6) result.tv_sec = t/1E6;
    result.tv_nsec = (t%(long)1E6) * 1E3;
    return result;
}

static inline timespec diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}
static inline timespec add(timespec t1, timespec t2)
{
    timespec result;
    result.tv_sec = t1.tv_sec + t2.tv_sec;
    unsigned long long nanoseconds = t1.tv_nsec + t2.tv_nsec;
    lldiv_t divresult = lldiv (nanoseconds, 1E9);
    result.tv_sec += divresult.quot;
    result.tv_nsec = divresult.rem;
    return result;
}
static inline int compare(timespec t1, timespec t2)
{
    if(t1.tv_sec > t2.tv_sec) return 1;
    if(t1.tv_sec < t2.tv_sec) return -1;
    if(t1.tv_sec == t2.tv_sec)
    {
        if(t1.tv_nsec > t2.tv_nsec) return 1;
        if(t1.tv_nsec < t2.tv_nsec) return -1;
        if(t1.tv_nsec == t2.tv_nsec) return 0;
    }
    return -100;
}
#endif
};
}

#endif // COMMON_H
