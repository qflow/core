#include "executor_traits.h"

namespace qflow{
class thread_executor
{

};
}

template<>
class executor_traits<qflow::thread_executor>
{
public:
    using execution_category = parallel_executor_tag;
    template<class T>
    using future = std::future<T>;
    template<class Function>
    static future<result_of_t<Function()>> async_execute(qflow::thread_executor& /*ex*/, Function&& f)
    {
        return std::async(f);
    }
};
