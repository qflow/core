#ifndef SYNC_H
#define SYNC_H

#include "coreplugin_global.h"
#include <QList>
#include <QMutex>
#include "blockingqueue.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QVariant>

namespace QFlow{

template<typename T>
class COREPLUGIN_EXPORT IFunctor
{
public:
    virtual void operator()(T data) = 0;
};

template<typename FunctorClass, typename T>
class Functor : public IFunctor<T>
{
private:
    FunctorClass* _object;
    void (FunctorClass::*_fn)(T data);
public:
    Functor(FunctorClass* object, void(FunctorClass::*fn)(T data)) : _object(object), _fn(fn)
    {

    }
    void operator()(T data)
    {
        (*_object.*_fn)(data);
    }
};

template<typename T>
class COREPLUGIN_EXPORT Sync
{
protected:
    QList<BlockingQueue<T>*> _queues;
    QMutex mutex;
    volatile bool _running;
    typedef QVector<T> Vector;
    IFunctor<Vector>* _callback;

    void run();
public:
    void stop();
    Sync(int capacity = 0);
    ~Sync();
    void setCapacity(int capacity);
    BlockingQueue<T>* operator[](int index);
    BlockingQueue<T>* at(int index);
    virtual void push(int index, T data);

    template<typename Class>
    void registerCallback(Class* object, void (Class::*fn)(QVector<T> data))
    {
        _callback = new Functor<Class,Vector>(object, fn);
    }
    bool running();
};

template<typename T>
class COREPLUGIN_EXPORT Value
{
    T _value;
    int _index;
    bool _isSet;
public:
    Value(T value, int index);
    Value();
    T operator()();
    int index();
};



template<typename T>
class COREPLUGIN_EXPORT LooseSync
{
public:
    typedef Value<T> ValType;
    typedef QList<ValType> Data;
private:
    BlockingQueue<ValType> _queue;
    QMutex mutex;
    volatile bool running;
    IFunctor<Data>* _callback;
    int _capacity;
    void run();
    void stop();
public:
    LooseSync(int capacity = 0);
    ~LooseSync();
    template<typename Class>
    void registerCallback(Class* object, void (Class::*fn)(Data data));
    void operator()(T const& data, int index);
};

typedef QVector<QVariant> VariantList;
class COREPLUGIN_EXPORT VariantSync : public QObject, public Sync<QVariant>
{
    Q_OBJECT
    void callback(VariantList data);
public:
    Q_INVOKABLE VariantSync(int capacity = 0);
    Q_INVOKABLE void push(int index, QVariant data);
signals:
    void out(VariantList data);

};
}
Q_DECLARE_METATYPE(QFlow::VariantList)

#endif // SYNC_H

