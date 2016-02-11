#include "Sync.h"
#include "waveform.h"

/*template<typename FunctorClass, typename T>
Functor<FunctorClass,T>::Functor(FunctorClass* object, void(FunctorClass::*fn)(T data)) : _object(object), _fn(fn)
{

}
template<typename FunctorClass, typename T>
void Functor<FunctorClass,T>::operator()(T data)
{
    (*_object.*_fn)(data);
}*/

namespace QFlow{

template<typename T>
void Sync<T>::run()
{
    QMutexLocker lock(&mutex);
    while(_running)
    {
        QVector<T> result;
        result.resize(_queues.count());
        for(int i=0;i<_queues.count();i++)
        {
            T value;
            _queues[i]->waitAndPop(&value);
            result[i] = value;
        }
        if(_running && _callback && _queues.length() > 0) (*_callback)(result);
    }
}
template<typename T>
void Sync<T>::stop()
{
    if(_running)
    {
        _running = false;
        foreach(BlockingQueue<T>* queue, _queues)
        {
            queue->flush();
        }
        QMutexLocker lock(&mutex);
        foreach(BlockingQueue<T>* queue, _queues)
        {
            delete queue;
        }
    }
}
template<typename T>
Sync<T>::Sync(int capacity) : _running(false), _callback(NULL)
{
    setCapacity(capacity);
}
template<typename T>
Sync<T>::~Sync()
{
    stop();
}
template<typename T>
void Sync<T>::setCapacity(int capacity)
{
    stop();
    QMutexLocker lock(&mutex);
    _queues.clear();
    for(int i=0;i<capacity;i++)
    {
        _queues.push_back(new BlockingQueue<T>());
    }
    _running = true;
    if(capacity > 0) QtConcurrent::run(this, &Sync<T>::run);
}
template<typename T>
BlockingQueue<T>* Sync<T>::operator[](int index)
{
    return _queues[index];
}
template<typename T>
BlockingQueue<T>* Sync<T>::at(int index)
{
    return _queues[index];
}
template<typename T>
void Sync<T>::push(int index, T data)
{
    _queues[index]->push(data);
}

/*template<typename T>
template<typename Class>
void Sync<T>::registerCallback(Class* object, void (Class::*fn)(QVector<T> data))
{
    _callback = new Functor<Class,QVector<T>>(object, fn);
}*/
template<typename T>
bool Sync<T>::running()
{
    return _running;
}
template<typename T>
Value<T>::Value(T value, int index) : _value(value), _index(index), _isSet(true)
{

}
template<typename T>
Value<T>::Value() : _isSet(false)
{

}
template<typename T>
T Value<T>::operator()()
{
    return _value;
}
template<typename T>
int Value<T>::index()
{
    return _index;
}
template<typename T>
void LooseSync<T>::run()
{
    QMutexLocker lock(&mutex);
    while(running)
    {
        Data result;
        ValType value;
        _queue.waitAndPop(value);
        result.push_back(value);
        while(_queue.try_pop(value))
        {
            result.push_back(value);
        }

        if(running && _callback) (*_callback)(result);
    }
}
template<typename T>
void LooseSync<T>::stop()
{
    if(running)
    {
        running = false;
        _queue.flush();
        QMutexLocker lock(&mutex);
    }
}
template<typename T>
LooseSync<T>::LooseSync(int capacity) : running(false), _callback(NULL), _capacity(capacity)
{
    running = true;
    mutex.lock();
    QtConcurrent::run(this, &LooseSync<T>::run);
}
template<typename T>
LooseSync<T>::~LooseSync()
{
    stop();
}
template<typename T>
template<typename Class>
void LooseSync<T>::registerCallback(Class* object, void (Class::*fn)(Data data))
{
    _callback = new Functor<Class,Data>(object, fn);
}
template<typename T>
void LooseSync<T>::operator()(T const& data, int index)
{
    ValType val(data, index);
    _queue << val;
}
void VariantSync::callback(VariantList data)
{
    emit out(data);
}
VariantSync::VariantSync(int capacity) : Sync<QVariant>(capacity)
{
    registerCallback(this, &VariantSync::callback);
}
void VariantSync::push(int index, QVariant data)
{
    Sync<QVariant>::push(index, data);
}
template class Sync<Waveform>;
template class Sync<QVariant>;
}
