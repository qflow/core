#include "future.h"

namespace QFlow{
Promise<void>::Promise() : d_ptr(new PromisePrivate<void>())
{

}
Promise<void>::Promise(Promise<void>&& other) : d_ptr(std::move(other.d_ptr))
{

}
void Promise<void>::set_value()
{
    d_ptr->_internal.set_value();
    d_ptr->_future.set_value();
}
FutureBase<void> Promise<void>::get_future()
{
    std::future<void> stdFuture = d_ptr->_internal.get_future();
    d_ptr->_future = FutureBase<void>(std::move(stdFuture));
    return d_ptr->_future;
}
template<typename Func, typename R>
FutureBase<R> FutureBase<void>::then(Func&& func)
{
    auto promise = std::make_shared<Promise<R>>();
    FutureBase<R> future = promise->get_future();
    if(is_ready())
    {
        qDebug() << "called on main";
        call(promise, func);
    }
    else
    {
        d_ptr->_onReady = [promise,func](){
            call(promise, func);
        };
    }
    return future;
}
}
