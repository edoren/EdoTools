#pragma once

#include <edoren/String.hpp>
#include <edoren/StringView.hpp>

// #define FMT_HEADER_ONLY
// #define FMT_USE_WINDOWS_H 0
// #define FMT_USE_USER_DEFINED_LITERALS 0
// #ifndef EDOREN_EXPORTS
//     #define FMT_SHARED 0
// #endif
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace edoren {

template <typename T, typename Allocator>
class Vector;

template <typename Char>
struct UdlStringFormatProxy {
    const Char* str;

    template <typename... Args>
    String operator()(Args&&... args) const {
        return String(std::move(fmt::format(str, std::forward<Args>(args)...)));
    }
};

inline UdlStringFormatProxy<char> operator""_format(const char* str, size_t /*unused*/) {
    return {str};
}

inline UdlStringFormatProxy<wchar> operator""_format(const wchar* str, size_t /*unused*/) {
    return {str};
}

}  // namespace edoren

FMT_BEGIN_NAMESPACE

template <typename T, typename Allocator>
struct formatter<engine::Vector<T, Allocator>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext = format_context>
    auto format(const engine::Vector<T, Allocator>& v, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "[{}]", fmt::join(v, ", "));
    }
};

FMT_END_NAMESPACE
