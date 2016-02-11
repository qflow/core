#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include "coreplugin_global.h"
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

namespace QFlow{

template<typename T>
class COREPLUGIN_EXPORT BlockingQueue
{
private:
    QQueue<T> _queue;
    mutable QMutex the_mutex;
    mutable bool _flushed;
    mutable int _capacity;
    QWaitCondition _emptyCondition;
    QWaitCondition _fullCondition;
public:
    BlockingQueue(int capacity = 2);
    ~BlockingQueue();
    void push(T const& data);

    BlockingQueue<T>* operator<<(T const& data);

    bool empty() const;

    bool full();

    void clear();

    bool try_pop(T& popped_value);

    void waitAndPop(T* popped_value);
    void flush();

};
}

#endif // BLOCKINGQUEUE_H

