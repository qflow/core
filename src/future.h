#ifndef FUTURE_H
#define FUTURE_H

#include "coreplugin_global.h"
#include <QObject>
#include <QSharedDataPointer>
#include <functional>
#include <QJSValue>
#include <QVariant>
#include <future>

namespace QFlow{

class COREPLUGIN_EXPORT Future
{
    Q_GADGET
public:
    Future()
    {

    }

    Future(std::shared_future<QVariant> stdFuture) : _internal(stdFuture)
    {

    }
    Future(const Future& other) : _internal(other._internal)
    {
    }

    Q_INVOKABLE void wait() const
    {
        return _internal.wait();
    }

    Q_INVOKABLE QVariant result()
    {
        return _internal.get();
    }

private:
    std::shared_future<QVariant> _internal;
};
}

#endif // FUTURE_H
