#pragma once

#include <optional>
#include <vector>

#ifdef EDOTOOLS_FMT_SUPPORT
    #include <fmt/format.h>
#endif
#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT
    #include <nlohmann/json.hpp>
#endif

namespace edoren {

template <typename T, typename Allocator = std::allocator<T>>
class Vector : public std::vector<T, Allocator> {
public:
    using std::vector<T, Allocator>::vector;
    using std::vector<T, Allocator>::operator=;
    using std::vector<T, Allocator>::operator[];

    Vector(const Vector& other) = default;
    Vector& operator=(const Vector& other) = default;
    Vector(Vector&& other) noexcept = default;
    Vector& operator=(Vector&& other) noexcept = default;

    template <typename Func>
    auto map(Func transform) const -> Vector<std::invoke_result_t<decltype(transform), const T&>>;

    template <typename Func>
    auto mapIndexed(Func transform) const -> Vector<std::invoke_result_t<decltype(transform), size_t, const T&>>;

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
};

}  // namespace edoren

#ifdef EDOTOOLS_FMT_SUPPORT
// See https://fmt.dev/latest/api.html#formatting-user-defined-types

template <typename T, typename Allocator>
struct fmt::formatter<edoren::Vector<T, Allocator>> {
    constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin());

    template <typename FormatContext = format_context>
    auto format(const edoren::Vector<T, Allocator>& v, FormatContext& ctx) -> decltype(ctx.out());
};

#endif  // EDOTOOLS_FMT_SUPPORT

#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT

namespace nlohmann {

template <typename T, typename Allocator>
struct adl_serializer<edoren::Vector<T, Allocator>> {
    static void to_json(json& j, const edoren::Vector<T, Allocator>& v);
    static void from_json(const json& j, edoren::Vector<T, Allocator>& v);
};

}  // namespace nlohmann

#endif  // EDOTOOLS_NLOHMANN_JSON_SUPPORT

#include "Vector.inl"
