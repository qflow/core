#include "blockingqueue.h"
#include "waveform.h"
#include <QVariant>

namespace QFlow{

template<typename T>
BlockingQueue<T>::BlockingQueue(int capacity) : _flushed(false), _capacity(capacity)
{

}
template<typename T>
BlockingQueue<T>::~BlockingQueue()
{
    QMutexLocker lock(&the_mutex);
}
template<typename T>
void BlockingQueue<T>::push(T const& data)
{
    QMutexLocker lock(&the_mutex);
    while(_queue.size() >= _capacity && !_flushed)
    {
        _fullCondition.wait(&the_mutex);
    }

    _queue.enqueue(data);
    lock.unlock();
    _emptyCondition.wakeOne();
}
template<typename T>
BlockingQueue<T>* BlockingQueue<T>::operator<<(T const& data)
{
    push(data);
    return this;
}
template<typename T>
bool BlockingQueue<T>::empty() const
{
    QMutexLocker lock(&the_mutex);
    return _queue.isEmpty();
}
template<typename T>
bool BlockingQueue<T>::full()
{
    QMutexLocker lock(&the_mutex);
    if(_queue.size() >= _capacity) return true;
    return false;
}
template<typename T>
void BlockingQueue<T>::clear()
{
    QMutexLocker lock(&the_mutex);
    _queue.clear();
    lock.unlock();
    _fullCondition.wakeOne();
}
template<typename T>
bool BlockingQueue<T>::try_pop(T& popped_value)
{
    QMutexLocker lock(&the_mutex);
    if(_queue.isEmpty())
    {
        return false;
    }

    popped_value=_queue.dequeue();
    lock.unlock();
    _fullCondition.wakeOne();
    return true;
}
template<typename T>
void BlockingQueue<T>::waitAndPop(T* popped_value)
{
    QMutexLocker lock(&the_mutex);
    while(_queue.isEmpty() && !_flushed)
    {
        _emptyCondition.wait(&the_mutex);
    }

    if(!_queue.isEmpty()) *popped_value=_queue.dequeue();
    lock.unlock();
    _fullCondition.wakeOne();
}
template<typename T>
void BlockingQueue<T>::flush()
{
    _flushed = true;
    _emptyCondition.wakeOne();
    _fullCondition.wakeOne();
}

template class BlockingQueue<QVariant>;
template class BlockingQueue<Waveform>;
}
