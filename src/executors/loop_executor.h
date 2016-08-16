#include "executor_traits.h"
#include "future.h"
#include <functional>
#include <future>

namespace qflow{
class loop_executor
{
public:
    using task = std::function<void()>;
    loop_executor();
    ~loop_executor();
    loop_executor(const loop_executor& other);
    void enqueueTask(task t);
private:
    class loop_executor_private;
    const std::shared_ptr<loop_executor_private> d_ptr;
};
}

template<>
class executor_traits<qflow::loop_executor>
{
public:
    using executor_type = qflow::loop_executor;
    using execution_category = parallel_executor_tag;
    template<class T>
    using future = QFlow::FutureBase<T>;
    template<class T>
    using promise_type = QFlow::Promise<T>;
    template<class Function>
    static future<std::result_of_t<Function()>> async_execute(qflow::loop_executor& ex, Function&& f)
    {
        std::shared_ptr<promise_type<std::result_of_t<Function()>>> p = std::make_shared<promise_type<std::result_of_t<Function()>>>();
        std::function<void()> task = [p, f](){
            auto res = f();
            p->set_value(res);
        };
        ex.enqueueTask(task);
        return p->get_future();
    }
    template<class Function, class T, class R = std::result_of_t<Function(T)>>
    static future<R> then_execute(executor_type& ex, Function&& f, future<T>& fut)
    {
        auto promise = std::make_shared<promise_type<R>>();
        std::promise<int> p;
        future<R> resF = promise->get_future();
        fut.then([&ex, f, promise](T value){
            qDebug() << "then called";
            future<R> fut2 = executor_traits<executor_type>::async_execute(ex, std::bind(f, value));
            fut2.then([promise](R value2){
                qDebug() << "then then called";
                promise->set_value(value2);
            });
        });
        return resF;
    }
};

