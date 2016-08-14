#include <future>
#include <vector>
#include <tuple>

using namespace std;

template <typename C>
struct get_template_type;

template <template <typename > class C, typename T>
struct get_template_type<C<T>>
{
    using type = T;
};

struct unknown_execution_tag {};
struct unknown_future_tag {};
struct parallel_executor_tag {};

template<class Executor>
class executor_traits
{
public:
// the type of the executor
using executor_type = Executor;
// the category of agents created by calls to operations with "execute" in their name
using execution_category = unknown_execution_tag;
// the type of index passed to functions invoked by agents created by
// the multi-agent creation functions
using index_type = size_t;
// the type of the shape parameter passed to the multi-agent creation functions
using shape_type = size_t;
// the type of future returned by asynchronous functions
template<class T>
using future = unknown_future_tag;
// the type of container used to return multiple results
// from multi-agent operations
template<class T>
using container = std::vector<T>;
// creates an immediately ready future containing a T contructed from
// the given constructor arguments
template<class T, class... Args>
static future<T> make_ready_future(executor_type& ex, Args&&... args);
// converts a (possibly foreign) some_future<U> to this executor's future<T>
template<class T, class Future>
static future<T> future_cast(executor_type& ex, Future& fut);
// returns the largest shape the executor can accomodate in a single operation
template<class Function>
static shape_type max_shape(const executor_type& ex, const Function& f);
// returns a future to a tuple-like type containing the values of
// the given futures. The result becomes ready when all the given futures are ready
template<class... Futures>
static future<std::tuple<typename get_template_type<Futures>::type...>> when_all(executor_type& ex, Futures&&... futures);
// single-agent when_all_execute_and_select
// invokes the function when all the input futures are ready
// the values of the input futures are passed through to the result future as a tuple.
// the caller may select which values to passthrough
template<size_t... Indices, class Function, class TupleOfFutures>
static future<result_of_t<Function()>> when_all_execute_and_select(executor_type& ex, Function&& f,
TupleOfFutures&& futures);
// multi-agent when_all_execute_and_select
template<size_t... Indices, class Function, class TupleOfFutures, class... Factories>
static future<result_of_t<Function()>> when_all_execute_and_select(executor_type& ex, Function f,
shape_type shape,
TupleOfFutures&& futures,
Factories... factories);
// single-agent then_execute
// asynchronously invokes f(value) when the input future's value is ready
// returns the result f(value) via future
template<class Function, class Future>
static future<result_of_t<Function()>> then_execute(executor_type& ex, Function&& f, Future& fut);
// multi-agent then_execute returning user-specified container
// asynchronously invokes f(idx, value, shared_args...) when the input future's value is ready
// returns the results of f(idx, value, shared_args...) via future<Container>
template<class Container, class Function, class Future, class... Factories>
static future<Container> then_execute(executor_type& ex, Function f,
shape_type shape,
Future& fut,
Factories... factories);
// multi-agent then_execute returning default container
// asynchronously invokes f(idx, value) when the input future's value is ready
// returns the results of f(idx, value, shared_args...) via future<container<...>>
template<class Function, class Future, class... Factories>
static future<container<result_of_t<Function()>>> then_execute(executor_type& ex, Function f,
shape_type shape,
Future& fut,
Factories... factories);
// single-agent async_execute
// asynchronously invokes f()
// returns the result of f()'s via future
template<class Function>
static future<result_of_t<Function()>> async_execute(executor_type& ex, Function&& f);
// multi-agent async_execute returning user-specified container
// asynchronously invokes f(idx, shared_args...)
// returns the results of f(idx, shared_args...) via future<Container>
template<class Container, class Function, class... Factories>
static future<Container> async_execute(executor_type& ex, Function f,
shape_type shape,
Factories... factories);
// multi-agent async_execute returning default container
// asynchronously invokes f(idx, shared_args...)
// returns the results of f(idx, shared_args...) via future<container<...>>
template<class Function, class... Factories>
static future<container<result_of_t<Function()>>> async_execute(executor_type& ex, Function f,
shape_type shape,
Factories... factories);
// single-agent execute
// synchronously invokes f()
// returns the result of f()
template<class Function>
static result_of_t<Function()> execute(executor_type& ex, Function&& f);
// multi-agent execute returning user-specified container
// synchronously invokes f(idx, shared_args...)
// returns the results of f(idx, shared_args...) via Container
template<class Container, class Function, class... Factories>
static Container execute(executor_type& ex, Function f,
shape_type shape,
Factories... factories);
// multi-agent execute returning default container
// synchronously invokes f(idx, shared_args...)
// returns the results of f(idx, shared_args...) via container<...>
template<class Function, class... Factories>
static container<result_of_t<Function()>> execute(executor_type& ex, Function f,
shape_type shape,
Factories... factories);
};
// inherits from true_type if T satisfies the Executor concept implied by executor_traits;
// otherwise false_type
template<class T>
struct is_executor;
