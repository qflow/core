#ifndef PROMISE_H
#define PROMISE_H

#include "future.h"
#include "coreplugin_global.h"
#include <QObject>
#include <functional>

namespace QFlow{

template<typename T>
class COREPLUGIN_EXPORT Promise
{
public:
    Promise()
    {

    }
    FutureBase<T> get_future()
    {
        _future.d_ptr->_stdFuture = _internal.get_future();
        return _future;
    }
    void set_value( const T& value )
    {
        _internal.set_value(value);
    }

private:
    std::promise<T> _internal;
    FutureBase<T> _future;
};

}

#endif // PROMISE_H
