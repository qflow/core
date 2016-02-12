#include "thread.h"
#include <QDebug>

namespace QFlow{

class ThreadPrivate : public QThread
{
public:
    QList<QObject*> _elements;
    ThreadPrivate() : QThread()
    {

    }
    ~ThreadPrivate()
    {
        exit();
        wait();
    }

protected:
    void run()
    {
        exec();
    }
};

Thread::Thread(QObject *) :
    QObject(), d_ptr(new ThreadPrivate())
{
    Q_D(Thread);
    d->start();
}
Thread::~Thread()
{
}

QQmlListProperty<QObject> Thread::elements()
{
    Q_D(Thread);
    return QQmlListProperty<QObject>(this, d->_elements);
}
void Thread::classBegin()
{
}
void Thread::componentComplete()
{
    Q_D(Thread);
    setParent(NULL);
    moveToThread(d);
}
void Thread::test()
{
    //QThread* t =  QThread::currentThread();
    //int i=0;
}
}
