#ifndef FUTURE_H
#define FUTURE_H

#include "coreplugin_global.h"
#include <QObject>
#include <QSharedDataPointer>
#include <functional>
#include <QJSValue>
#include <QVariant>
#include <future>
#include <QDebug>
#include <atomic>

namespace QFlow{

template<typename T>
class Promise;

template<typename T>
class COREPLUGIN_EXPORT FutureBase
{
    friend class Promise<T>;
public:
    FutureBase() : d_ptr(new FutureBasePrivate())
    {

    }
    FutureBase(const FutureBase& other) : d_ptr(other.d_ptr)
    {
    }
    FutureBase(std::future<T>&& other) : FutureBase()
    {
        d_ptr->_stdFuture = std::move(other);
    }

    void wait() const
    {
        return d_ptr->_stdFuture.wait();
    }

    T get()
    {
        return d_ptr->_stdFuture.get();
    }
    bool is_ready() const
    {
        return d_ptr->_stdFuture.wait_for(0) == std::future_status::ready;
    }
    template<typename Func>
    FutureBase<typename std::result_of<Func(FutureBase<T>)>::type> then(Func&& func)
    {
        auto resStd = std::async([this,func](){
            wait();
            auto resVal = func(*this);
            return resVal;
        });
        FutureBase<typename std::result_of<Func(FutureBase<T>)>::type> res(std::move(resStd));
        return res;
    }


protected:
    class FutureBasePrivate
    {
    public:
        std::future<T> _stdFuture;
    };
    std::shared_ptr<FutureBasePrivate> d_ptr;
};

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

    Q_INVOKABLE QVariant get()
    {
        return _internal.get();
    }

private:
    std::shared_future<QVariant> _internal;
};
}

#endif // FUTURE_H
