#pragma once

#include <edoren/Config.hpp>
#include <edoren/Function.hpp>
#include <edoren/container/Vector.hpp>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

namespace edoren {

template <typename TaskType = Function<void()>>
class ThreadPool {
public:
    enum class Status { RUNNING, STOPPING, STOPPED };

    using Task = TaskType;

    ThreadPool(size_t num_threads);

    ~ThreadPool();

    void execute(Task&& f);

    void joinAndStop();

private:
    std::atomic<Status> m_status;
    std::deque<Task> m_work_queue;
    Vector<std::thread> m_workers;
    std::condition_variable m_signaler;
    std::mutex m_queue_mutex;
};

}  // namespace edoren

#include "ThreadPool.inl"
