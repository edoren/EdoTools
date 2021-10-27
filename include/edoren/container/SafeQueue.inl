#pragma once

#include <deque>
#include <mutex>
#include <thread>

namespace edoren {

template <typename T>
bool SafeQueue<T>::isEmpty() const {
    return m_impl.empty();
}

template <typename T>
size_t SafeQueue<T>::getSize() const {
    return m_impl.size();
}

template <typename T>
void SafeQueue<T>::clear() {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_impl.clear();
}

template <typename T>
void SafeQueue<T>::push(const T& value) {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_impl.push_back(value);
}

template <typename T>
void SafeQueue<T>::push(T&& value) {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_impl.push_back(value);
}

template <typename T>
template <class... Args>
void SafeQueue<T>::emplace(Args&&... args) {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_impl.emplace_back(std::forward<Args>(args)...);
}

template <typename T>
T SafeQueue<T>::pop() {
    T value = m_impl.front();
    m_impl.pop_front();
    return value;
}

}  // namespace edoren
