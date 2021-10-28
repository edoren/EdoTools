#pragma once

#include <mutex>
#include <thread>

namespace edoren {

template <typename TaskType>
ThreadPool<TaskType>::ThreadPool(size_t num_threads) : m_status(Status::RUNNING) {
    auto job = [this]() -> void {
        while (true) {
            std::unique_lock<std::mutex> lk(m_queue_mutex);
            if (m_status == Status::STOPPED) {
                break;
            }
            if (m_work_queue.empty()) {
                m_signaler.wait(lk);
            }
            if (m_status == Status::STOPPED) {
                break;
            }
            if (!m_work_queue.empty()) {
                Task task = std::move(m_work_queue.front());
                m_work_queue.pop_front();
                if (m_status == Status::STOPPING && m_work_queue.empty()) {
                    m_status = Status::STOPPED;
                }
                lk.unlock();
                task();
            }
        }
    };
    m_workers.reserve(num_threads);
    for (decltype(num_threads) i = 0; i < num_threads; i++) {
        m_workers.emplace_back(job);
    }
}

template <typename TaskType>
ThreadPool<TaskType>::~ThreadPool() {
    m_status = Status::STOPPED;
    m_signaler.notify_all();
}

template <typename TaskType>
void ThreadPool<TaskType>::execute(Task&& f) {
    if (m_status == Status::RUNNING) {
        std::lock_guard<std::mutex> lk(m_queue_mutex);
        m_work_queue.push_back(std::move(f));
        m_signaler.notify_one();
    }
}

template <typename TaskType>
void ThreadPool<TaskType>::joinAndStop() {
    {
        std::lock_guard<std::mutex> lk(m_queue_mutex);
        m_status = m_work_queue.empty() ? Status::STOPPED : Status::STOPPING;
        m_signaler.notify_all();
    }
    for (auto& worker : m_workers) {
        worker.join();
    }
}

}  // namespace edoren
