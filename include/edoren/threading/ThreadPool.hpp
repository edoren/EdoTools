#pragma once

#include <edoren/Config.hpp>
#include <edoren/Function.hpp>
#include <edoren/container/SafeQueue.hpp>
#include <edoren/container/Vector.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

namespace edoren {

class EDOREN_API ThreadPool {
public:
    using Task = Function<void()>;

    ThreadPool(size_t num_threads);

    ~ThreadPool();

    void execute(Task&& f);

private:
    bool m_is_running;
    SafeQueue<Task> m_workQueue;
    Vector<std::thread> m_workers;
    std::condition_variable m_signaler;
    std::mutex m_mutex;
};

}  // namespace edoren
