#include "thread.h"
#include <QDebug>

namespace QFlow{

class ThreadPrivate// : public QThread
{
public:
    QList<QObject*> _elements;
    QObject* _root;
    QThread _t;
    ThreadPrivate()// : QThread()
    {
        _root = new QObject();
        //_root->moveToThread(&_t);
    }
    ~ThreadPrivate()
    {
        //exit();
        //wait();
    }

protected:
    /*void run()
    {
        exec();
    }*/
};

Thread::Thread(QObject *) :
    QObject(), d_ptr(new ThreadPrivate())
{
    Q_D(Thread);
    //d->start();
}
Thread::~Thread()
{
}

QQmlListProperty<QObject> Thread::elements()
{
    Q_D(Thread);
    return QQmlListProperty<QObject>(d->_root, this, &Thread::append, &Thread::count, &Thread::at, &Thread::clear);
}
void Thread::append(QQmlListProperty<QObject> *list, QObject *obj)
{
    Thread* thread = (Thread*)list->data;
    QObject* p = obj->parent();
    if (obj)
    {
        obj->setParent(thread->d_ptr->_root);
        thread->d_ptr->_elements.append(obj);
    }
}
int Thread::count(QQmlListProperty<QObject> *list)
{
    Thread* thread = (Thread*)list->data;
    return thread->d_ptr->_elements.count();
}
QObject* Thread::at(QQmlListProperty<QObject> *list, int i)
{
    Thread* thread = (Thread*)list->data;
    return thread->d_ptr->_elements[i];
}
void Thread::clear(QQmlListProperty<QObject> *list)
{
    Thread* thread = (Thread*)list->data;
    thread->d_ptr->_elements.clear();
}
void Thread::classBegin()
{
}
void Thread::componentComplete()
{
    Q_D(Thread);
    d->_root->moveToThread(&d->_t);
    d->_t.start();
    //setParent(d->_root);
    //moveToThread(&d->_t);
    //d->_t.start();
}
void Thread::test()
{
    //QThread* t =  QThread::currentThread();
    //int i=0;
}
}
