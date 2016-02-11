#ifndef STARTABLE_H
#define STARTABLE_H

#include <QMutex>
#include <QWaitCondition>
#include <QMetaType>
#include <QObject>

class WaitController
{
    mutable QMutex _mutex;
    QWaitCondition _condition;
    mutable bool _finished;
public:
    WaitController() : _finished(false)
    {

    }
    void setFinished(bool finished)
    {
        _finished = finished;
    }
    bool finished()
    {
        return _finished;
    }

    QMutex* mutex()
    {
        return &_mutex;
    }
    QWaitCondition* condition()
    {
        return &_condition;
    }
};

class WaitHandle
{
    WaitController* _controller;
public:
    WaitHandle()
    {

    }

    WaitHandle(WaitController* controller) : _controller(controller)
    {
    }

    WaitHandle(const WaitHandle &b)
    {
        _controller = b._controller;
    }
    ~WaitHandle()
    {

    }
    void wait()
    {
        QMutexLocker lock(_controller->mutex());
        if(!_controller->finished())
        {
            _controller->condition()->wait(_controller->mutex());
        }
    }
};

class Startable
{
public:
    virtual void startModule() = 0;
    virtual void stopModule() = 0;
};

Q_DECLARE_METATYPE(WaitHandle)
#endif // STARTABLE_H
