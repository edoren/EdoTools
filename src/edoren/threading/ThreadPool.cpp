#include <edoren/threading/ThreadPool.hpp>

#include <mutex>
#include <thread>

namespace edoren {

ThreadPool::ThreadPool(size_t num_threads) : m_is_running(true) {
    auto job = [this]() -> void {
        while (m_is_running) {
            std::unique_lock<std::mutex> lk(m_mutex);
            if (m_workQueue.isEmpty()) {
                m_signaler.wait(lk);
                if (m_workQueue.isEmpty()) {
                    if (!m_is_running) {
                        break;
                    }
                    continue;
                }
            }
            Task task = m_workQueue.pop();
            lk.unlock();
            task();
        }
    };
    auto maxThreads = num_threads;
    m_workers.reserve(maxThreads);
    for (decltype(maxThreads) i = 0; i < maxThreads; i++) {
        m_workers.emplace_back(job);
    }
}

ThreadPool::~ThreadPool() {
    m_is_running = false;
    m_signaler.notify_all();
    for (auto& worker : m_workers) {
        worker.join();
    }
}

void ThreadPool::execute(Task&& f) {
    m_workQueue.push(std::move(f));
    m_signaler.notify_one();
}

}  // namespace edoren
