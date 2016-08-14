#include "executor_traits.h"

class thread_executor
{

};

template<>
class executor_traits<thread_executor>
{
public:
    using execution_category = parallel_executor_tag;
    template<class T>
    using future = std::future<T>;
    template<class Function>
    static future<result_of_t<Function()>> async_execute(thread_executor& /*ex*/, Function&& f)
    {
        return std::async(f);
    }
    template<class Function, class T>
    static future<result_of_t<Function(future<T>)>> then_execute(thread_executor& /*ex*/, Function&& f, future<T>& fut)
    {
        future<result_of_t<Function(future<T>)>> res;
        /*auto res = std::async([fut,f](){
            fut.wait();
            return std::move(f(fut));
        });*/
        return res;
}
};
