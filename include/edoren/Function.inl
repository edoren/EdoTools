#include "TypeTraits.hpp"

#include <cassert>
#include <cstdlib>

#include <utility>

#ifndef LAMBDA_DEFAULT_SIZE
    #define LAMBDA_DEFAULT_SIZE 6
#endif

#define LAMBDA_FUNCTION_SIZE(x) (sizeof(void*) * x)

namespace edoren {

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>::Function() : m_data(),
                                              m_invoker(nullptr),
                                              m_manager(nullptr) {}

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>::Function(std::nullptr_t) noexcept : m_data(),
                                                                     m_invoker(nullptr),
                                                                     m_manager(nullptr) {}

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>::Function(const Function& other) : m_data(),
                                                                   m_invoker(nullptr),
                                                                   m_manager(nullptr) {
    if (other.m_manager) {
        other.m_manager(&m_data, &other.m_data, Operation::COPY);
        m_invoker = other.m_invoker;
        m_manager = other.m_manager;
    }
}

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>::Function(Function&& other) noexcept
      : m_data(),
        m_invoker(std::move(other.m_invoker)),
        m_manager(std::move(other.m_manager)) {
    other.m_manager(&m_data, &other.m_data, Operation::COPY);
    other.m_manager(&other.m_data, nullptr, Operation::DESTROY);
    other.m_invoker = nullptr;
    other.m_manager = nullptr;
}

template <typename Ret, typename... Args, size_t MaxSize>
template <typename T, typename>
Function<Ret(Args...), MaxSize>::Function(T&& fun) : m_data(),
                                                     m_invoker(nullptr),
                                                     m_manager(nullptr) {
    using lambda_type = std::decay_t<T>;
    static_assert(type::size_of<lambda_type>() <= type::size_of<Storage>(), "Size is off");
    static_assert(type::alignment_of<lambda_type>() <= type::alignment_of<Storage>(), "Align is off");
    new (&m_data) lambda_type(std::forward<T>(fun));
    m_invoker = &CallFunction<lambda_type>;
    m_manager = &ManageFunction<lambda_type>;
}

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>::~Function() {
    if (m_manager != nullptr) {
        m_manager(&m_data, nullptr, Operation::DESTROY);
    }
}

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>& Function<Ret(Args...), MaxSize>::operator=(const Function& other) {
    Function(other).swap(*this);
    return *this;
}

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>& Function<Ret(Args...), MaxSize>::operator=(Function&& other) noexcept {
    other.m_manager(&m_data, &other.m_data, Operation::COPY);
    other.m_manager(&other.m_data, nullptr, Operation::DESTROY);
    m_invoker = std::move(other.m_invoker);
    m_manager = std::move(other.m_manager);
    other.m_invoker = nullptr;
    other.m_manager = nullptr;
    return *this;
}

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>& Function<Ret(Args...), MaxSize>::operator=(std::nullptr_t) {
    if (m_manager != nullptr) {
        m_manager(&m_data, nullptr, Operation::DESTROY);
        m_manager = nullptr;
        m_invoker = nullptr;
    }
    return *this;
}

template <typename Ret, typename... Args, size_t MaxSize>
template <typename T>
Function<Ret(Args...), MaxSize>& Function<Ret(Args...), MaxSize>::operator=(T&& other) {
    Function(std::forward<T>(other)).swap(*this);
    return *this;
}

template <typename Ret, typename... Args, size_t MaxSize>
template <typename T>
Function<Ret(Args...), MaxSize>& Function<Ret(Args...), MaxSize>::operator=(std::reference_wrapper<T> other) {
    Function(other).swap(*this);
    return *this;
}

template <typename Ret, typename... Args, size_t MaxSize>
void Function<Ret(Args...), MaxSize>::swap(Function& other) {
    std::swap(m_data, other.m_data);
    std::swap(m_manager, other.m_manager);
    std::swap(m_invoker, other.m_invoker);
}

template <typename Ret, typename... Args, size_t MaxSize>
Function<Ret(Args...), MaxSize>::operator bool() const noexcept {
    return m_manager != nullptr;
}

template <typename Ret, typename... Args, size_t MaxSize>
Ret Function<Ret(Args...), MaxSize>::operator()(Args&&... args) {
    if (m_invoker == nullptr) {
        abort();
    }
    return m_invoker(&m_data, std::forward<Args>(args)...);
}

template <typename Ret, typename... Args, size_t MaxSize>
Ret Function<Ret(Args...), MaxSize>::operator()(Args&&... args) const {
    if (m_invoker == nullptr) {
        abort();
    }
    return m_invoker(&m_data, std::forward<Args>(args)...);
}

template <typename Ret, typename... Args, size_t MaxSize>
template <typename FunctionType>
Ret Function<Ret(Args...), MaxSize>::CallFunction(const void* data, Args&&... args) {
    auto callable = static_cast<const FunctionType*>(data);
    return (*callable)(std::forward<Args>(args)...);
}

template <typename Ret, typename... Args, size_t MaxSize>
template <typename FunctionType>
void Function<Ret(Args...), MaxSize>::ManageFunction(void* dest, const void* src, Operation op) {
    auto dest_cast = static_cast<FunctionType*>(dest);
    switch (op) {
        case Operation::COPY: {
            const auto* srcCast = static_cast<const FunctionType*>(src);
            new (dest_cast) FunctionType(*srcCast);
            break;
        }
        case Operation::DESTROY:
            dest_cast->~FunctionType();
            break;
    }
}

}  // namespace edoren
