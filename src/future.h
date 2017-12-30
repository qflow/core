#ifndef FUTURE_H
#define FUTURE_H

#include "coreplugin_global.h"
#include "promise.h"
#include <QObject>
#include <QSharedDataPointer>
#include <functional>
#include <QJSValue>
#include <QVariant>
#include <future>
#include <QDebug>
#include <atomic>

namespace QFlow{

/*template<typename T>
class FutureBase;

template<>
class FutureBase<void>;

template<typename T>
class PromisePrivate
{
public:
    std::promise<T> _internal;
    FutureBase<T> _future;
};

template<typename T>
class COREPLUGIN_EXPORT Promise
{
public:
    Promise();
    Promise(const Promise<T>& other) = delete;
    Promise(Promise<T>&& other);
    FutureBase<T> get_future();
    void set_value( const T& value );
private:
    std::unique_ptr<PromisePrivate<T>> d_ptr;
};
template<>
class COREPLUGIN_EXPORT Promise<void>
{
public:
    Promise();
    Promise(const Promise<void>& other) = delete;
    Promise(Promise<void>&& other);
    FutureBase<void> get_future();
    void set_value();
private:
    std::unique_ptr<PromisePrivate<void>> d_ptr;
};

template<typename T>
class FutureBasePrivate
{
public:
    FutureBasePrivate() : _isReady(false)
    {

    }

    std::future<T> _stdFuture;
    std::function<void(T)> _onReady;
    std::atomic<bool> _isReady;
    T _value;
};
template<>
class FutureBasePrivate<void>
{
public:
    FutureBasePrivate() : _isReady(false)
    {

    }

    std::future<void> _stdFuture;
    std::function<void()> _onReady;
    std::atomic<bool> _isReady;
};

template<typename T>
class COREPLUGIN_EXPORT FutureBase
{
    friend class Promise<T>;
public:
    FutureBase() : d_ptr(new FutureBasePrivate<T>())
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
        d_ptr->_stdFuture.wait();
    }

    T get()
    {
        return d_ptr->_stdFuture.get();
    }
    bool is_ready() const
    {
        return d_ptr->_isReady.load();
    }
    template<typename Func, typename R = std::result_of_t<Func(T)>>
    FutureBase<R> then(Func&& func);
protected:
    void set_value(T value)
    {
        d_ptr->_value = value;
        d_ptr->_isReady = true;
        if(d_ptr->_onReady)
        {
            d_ptr->_onReady(value);
        }
    }


    std::shared_ptr<FutureBasePrivate<T>> d_ptr;
};
template<>
class COREPLUGIN_EXPORT FutureBase<void>
{
    friend class Promise<void>;
public:
    FutureBase() : d_ptr(new FutureBasePrivate<void>())
    {

    }
    FutureBase(std::future<void>&& other) : FutureBase()
    {
        d_ptr->_stdFuture = std::move(other);
    }
    void get()
    {
        d_ptr->_stdFuture.get();
    }
    void wait() const
    {
        d_ptr->_stdFuture.wait();
    }
    bool is_ready() const
    {
        return d_ptr->_isReady.load();
    }
    template<typename Func, typename R>
    FutureBase<R> then(Func&& func);
protected:
    void set_value()
    {
        d_ptr->_isReady = true;
        if(d_ptr->_onReady) d_ptr->_onReady();
    }
    std::shared_ptr<FutureBasePrivate<void>> d_ptr;
};

template<typename T>
Promise<T>::Promise() : d_ptr(new PromisePrivate<T>())
{

}
template<typename T>
Promise<T>::Promise(Promise<T>&& other) : d_ptr(std::move(other.d_ptr))
{

}
template<typename T>
FutureBase<T> Promise<T>::get_future()
{
    std::future<T> stdFuture = d_ptr->_internal.get_future();
    d_ptr->_future = FutureBase<T>(std::move(stdFuture));
    return d_ptr->_future;
}
template<typename T>
void Promise<T>::set_value( const T& value )
{
    d_ptr->_internal.set_value(value);
    d_ptr->_future.set_value(value);
}

template<typename R, typename T, typename Func>
void call(std::shared_ptr<Promise<R>> promise, Func&& func, T value)
{
    R resVal = func(value);
    promise->set_value(resVal);
}
template<typename R, typename T, typename Func>
void call(std::shared_ptr<Promise<R>> promise, Func&& func)
{
    R resVal = func();
    promise->set_value(resVal);
}
template<typename T, typename Func>
void call(std::shared_ptr<Promise<void>> promise, Func&& func, T value)
{
    func(value);
    promise->set_value();
}
template<typename T, typename Func>
void call(std::shared_ptr<Promise<void>> promise, Func&& func)
{
    func();
    promise->set_value();
}

template<typename T>
template<typename Func, typename R>
FutureBase<R> FutureBase<T>::then(Func&& func)
{
    auto promise = std::make_shared<Promise<R>>();
    FutureBase<R> future = promise->get_future();
    if(is_ready())
    {
        qDebug() << "called on main";
        call(promise, func, d_ptr->_value);
    }
    else
    {
        d_ptr->_onReady = [promise,func](T value){
            call(promise, func, value);
        };
    }
    return future;
}*/












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
Q_DECLARE_METATYPE(QFlow::Future)
#endif // FUTURE_H
