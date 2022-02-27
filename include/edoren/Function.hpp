#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>

#include <type_traits>
#include <utility>

#include <edoren/util/Platform.hpp>

#ifndef LAMBDA_DEFAULT_SIZE
    #define LAMBDA_DEFAULT_SIZE 6
#endif

#define LAMBDA_FUNCTION_SIZE(x) (sizeof(void*) * x)

namespace edoren {

/**
 * @brief Class to hold a function reference with static size for capture arguments
 *
 * @tparam Func The function type
 * @tparam MaxSize The maximum stack size used for the function class
 *
 * @see Function<Ret(Args...), MaxSize> for concrete implementation
 */
template <typename Func, size_t MaxSize = LAMBDA_FUNCTION_SIZE(LAMBDA_DEFAULT_SIZE)>
class Function;

/**
 * @brief Class to hold a function reference with static size for capture arguments
 *
 * @tparam Ret The return type of the function
 * @tparam Args The parameters that the function receives
 * @tparam MaxSize The maximum stack size used for the function class
 */
template <typename Ret, typename... Args, size_t MaxSize>
class Function<Ret(Args...), MaxSize> {
    static_assert(sizeof(MaxSize) > 0, "MaxSize must be at least 1");

public:
    /**
     * @brief Default constructor
     */
    Function();

    /**
     * @brief Null constructor, this yields an empty function
     */
    Function(std::nullptr_t) noexcept;

    /**
     * @brief Copy constructor
     */
    Function(const Function& other);

    /**
     * @brief Move constructor
     */
    Function(Function&& other) noexcept;

    /**
     * @brief Construct a new Function object
     *
     * @note This constructor should be called only when T is a lambda or a function
     *
     * @param fun The lambda function or function to assign to
     */
    template <typename T,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, Function<Ret(Args...), MaxSize>>>>
    Function(T&& fun);

    /**
     * @brief Destructor
     */
    ~Function();

    /**
     * @brief Copy operator
     */
    Function& operator=(const Function& other);

    /**
     * @brief Move operator
     */
    Function& operator=(Function&& other) noexcept;

    /**
     * @brief Null assignment operator
     */
    Function& operator=(std::nullptr_t);

    /**
     * @brief Redirect assigment operator based on available constructors
     */
    template <typename T>
    Function& operator=(T&& other);

    /**
     * @brief Redirect assigment operator based on available constructors
     */
    template <typename T>
    Function& operator=(std::reference_wrapper<T> other);

    /**
     * @brief Swap two functions with the same return and arguments
     *
     * @param other The other function to swap with this
     */
    void swap(Function& other);

    /**
     * @brief Checks instance does not point to any function
     */
    explicit operator bool() const noexcept;

    /**
     * @brief Function-call operator
     *
     * @param args Arguments to call the internal function
     */
    Ret operator()(Args&&... args);

    /**
     * @brief Function-call const operator
     *
     * @param args Arguments to call the internal function
     */
    Ret operator()(Args&&... args) const;

private:
    enum class Operation {
        COPY,
        DESTROY,
    };

    template <typename FunctionType>
    static Ret CallFunction(const void* data, Args&&... args);

    template <typename FunctionType>
    static void ManageFunction(void* dest, const void* src, Operation op);

    using Invoker = Ret (*)(const void*, Args&&...);
    using Manager = void (*)(void*, const void*, Operation);
#if PLATFORM_IS(PLATFORM_ANDROID | PLATFORM_IOS)
    using Storage = typename std::aligned_storage<MaxSize, 16>::type;
#else
    using Storage = typename std::aligned_storage<MaxSize, alignof(std::max_align_t)>::type;
#endif

    Storage m_data;     ///< Stores a copy of the Functor
    Invoker m_invoker;  ///< Pointer to the caller function for m_data
    Manager m_manager;  ///< Pointer to the function that manages the m_data
};

}  // namespace edoren

#include "Function.inl"
