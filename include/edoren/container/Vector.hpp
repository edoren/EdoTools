#pragma once

#include <edoren/util/Config.hpp>
#include <optional>
#include <vector>

#ifdef EDOTOOLS_FMT_SUPPORT
    #include <fmt/format.h>
#endif
#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT
    #include <nlohmann/json.hpp>
#endif

namespace edoren {

template <typename T>
class Vector : public std::pmr::vector<T> {
public:
    using std::pmr::vector<T>::vector;
    using std::pmr::vector<T>::operator=;
    using std::pmr::vector<T>::operator[];

    Vector(const Vector& other) = default;
    Vector& operator=(const Vector& other) = default;
    Vector(Vector&& other) noexcept = default;
    Vector& operator=(Vector&& other) noexcept = default;

    template <typename Func>
    auto map(Func transform) const -> Vector<std::invoke_result_t<Func, const T&>>;

    template <typename Func>
    auto mapIndexed(Func transform) const -> Vector<std::invoke_result_t<Func, size_t, const T&>>;

    template <typename Func>
    auto filter(Func predicate) const -> Vector<T>;

    template <typename Func>
    auto filterIndexed(Func predicate) const -> Vector<T>;

    template <typename Func>
    auto find(Func predicate) -> T*;

    template <typename Func>
    auto find(Func predicate) const -> const T*;

    template <typename Func>
    auto forEach(Func predicate);

    template <typename Func>
    auto forEach(Func predicate) const;

    template <typename Func>
    auto forEachIndexed(Func predicate);

    template <typename Func>
    auto forEachIndexed(Func predicate) const;

    auto first() -> T&;

    auto first() const -> const T&;

    template <typename Func>
    auto first(Func predicate) -> T&;

    template <typename Func>
    auto first(Func predicate) const -> const T&;

    auto firstOrNull() -> T*;

    auto firstOrNull() const -> const T*;

    template <typename Func>
    auto firstOrNull(Func predicate) -> T*;

    template <typename Func>
    auto firstOrNull(Func predicate) const -> const T*;

    // C++ std::list alias
    CREATE_MEMBER_FUNCTION_ALIAS(at, getAt);
    CREATE_MEMBER_FUNCTION_ALIAS(front, getFront);
    CREATE_MEMBER_FUNCTION_ALIAS(back, getBack);
    CREATE_MEMBER_FUNCTION_ALIAS(data, getData);

    CREATE_CONST_MEMBER_FUNCTION_ALIAS_NOARGS(empty, isEmpty);
    CREATE_CONST_MEMBER_FUNCTION_ALIAS_NOARGS(size, getSize);
    CREATE_CONST_MEMBER_FUNCTION_ALIAS_NOARGS(max_size, getMaxSize);
    CREATE_MEMBER_FUNCTION_ALIAS(shrink_to_fit, shrinkToFit);

    CREATE_MEMBER_FUNCTION_ALIAS(push_back, pushBack);
    CREATE_MEMBER_FUNCTION_ALIAS(emplace_back, emplaceBack);
    CREATE_MEMBER_FUNCTION_ALIAS(pop_back, popBack);
};

}  // namespace edoren

#ifdef EDOTOOLS_FMT_SUPPORT
// See https://fmt.dev/latest/api.html#formatting-user-defined-types

template <typename T>
struct fmt::formatter<edoren::Vector<T>> : fmt::formatter<std::string_view> {
    auto format(const edoren::Vector<T>& value, format_context& ctx) const -> format_context::iterator;
};

#endif  // EDOTOOLS_FMT_SUPPORT

#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT

namespace nlohmann {

template <typename T>
struct adl_serializer<edoren::Vector<T>> {
    static void to_json(json& j, const edoren::Vector<T>& v);
    static void from_json(const json& j, edoren::Vector<T>& v);
};

}  // namespace nlohmann

#endif  // EDOTOOLS_NLOHMANN_JSON_SUPPORT

#include "Vector.inl"
