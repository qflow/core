#include "loop_executor.h"
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>

using namespace std;


namespace qflow{

enum class state {
        READY_WRITE,
        READY_READ,
        WRITING,
        READING,
        LOCKING
    };
class loop_executor::loop_executor_private
{
public:
    queue<task> _L1;
    queue<task> _L2;
    atomic<state> _state;
    std::thread _mainThread;
    bool _finished = false;
    std::condition_variable cv;
    mutex m;
    loop_executor_private()
    {
        _state = state::READY_WRITE;
        _mainThread = std::thread(&loop_executor_private::main, this);
    }
    ~loop_executor_private()
    {
        _finished = true;
        cv.notify_one();
        _mainThread.join();
    }
    void main()
    {
        while(!_finished)
        {
            state expected = state::READY_READ;
            if(_state.compare_exchange_weak(expected, state::READING))
            {
                swap(_L1, _L2);
                _state = state::READY_WRITE;
                while(!_L2.empty())
                {
                    _L2.front()();
                    _L2.pop();
                }
            }
            else if(expected == state::READY_WRITE)
            {
                unique_lock<mutex> lock(m);
                cv.wait(lock);
            }
        }
    }
};
loop_executor::loop_executor() : d_ptr(new loop_executor_private())
{

}
loop_executor::~loop_executor()
{

}
loop_executor::loop_executor(const loop_executor& other) : d_ptr(other.d_ptr)
{

}
void loop_executor::enqueueTask(task t)
{
    state expected = state::READY_WRITE;
    while(!d_ptr->_state.compare_exchange_weak(expected, state::WRITING));
    d_ptr->_L1.push(t);
    d_ptr->_state = state::READY_READ;
    d_ptr->cv.notify_one();

}
}
