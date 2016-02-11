#include "scheduler.h"
#include <QTimer>

namespace QFlow{

class SchedulerPrivate
{
public:
    QDateTime _startTime;
    Timespan _duration;
    SchedulerPrivate() : _startTime(QDateTime::currentDateTime())
    {

    }
};

Scheduler::Scheduler(QObject *parent) : QObject(parent), d_ptr(new SchedulerPrivate())
{

}
Scheduler::~Scheduler()
{

}

void Scheduler::setStartTime(QDateTime value)
{
    Q_D(Scheduler);
    d->_startTime = value;
    emit startTimeChanged();
}
QDateTime Scheduler::startTime() const
{
    Q_D(const Scheduler);
    return d->_startTime;
}
void Scheduler::setDuration(Timespan value)
{
    Q_D(Scheduler);
    d->_duration = value;
    emit durationChanged();
}
Timespan Scheduler::duration() const
{
    Q_D(const Scheduler);
    return d->_duration;
}
void Scheduler::onStarted()
{
    Q_D(Scheduler);
    QTimer::singleShot(d->_duration.msecsTotal(), this, SLOT(onFinished()));
    emit started();
}
void Scheduler::onFinished()
{
    emit finished();
}

void Scheduler::init()
{
    Q_D(Scheduler);
    QDateTime now = QDateTime::currentDateTime();
    qint64 start = now.msecsTo(d->_startTime);
    if(start < 0) start = 0;
    QTimer::singleShot(start, this, SLOT(onStarted()));
}
}
