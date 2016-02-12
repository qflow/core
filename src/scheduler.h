#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include "timespan.h"
#include <QDateTime>
#include <QDataStream>

namespace QFlow{

class SchedulerPrivate;
class COREPLUGIN_EXPORT Scheduler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(Timespan duration READ duration WRITE setDuration NOTIFY durationChanged)
private slots:
    void onStarted();
    void onFinished();
public:
    explicit Scheduler(QObject *parent = 0);
    ~Scheduler();
    void setStartTime(QDateTime value);
    QDateTime startTime() const;
    void setDuration(Timespan value);
    Timespan duration() const;
    void init();
Q_SIGNALS:
    void startTimeChanged();
    void durationChanged();
    void started();
    void finished();
private:
    const QScopedPointer<SchedulerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Scheduler)
};
}

#endif // SCHEDULE_H
