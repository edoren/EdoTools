#include "UTF.hpp"

#include <algorithm>
#include <compare>
#include <functional>
#include <sstream>
#include <string>
#include <type_traits>

#include <cstring>

namespace edoren {

constexpr StringView::StringView(const char* utf8String) {
    if (utf8String && utf8String[0] != 0) {
        size_type length = std::char_traits<char>::length(utf8String);
        if (length > 0) {
            if (utf::IsValid<utf::UTF_8>(utf8String, utf8String + length)) {
                m_data = utf8String;
                m_size = length;
            } else {
                EDOTOOLS_THROW(std::runtime_error("invalid utf8 convertion."));
            }
        };
    }
}

constexpr StringView::StringView(const char* utf8String, size_type size) : m_data(utf8String), m_size(size) {}

constexpr StringView& StringView::operator=(const StringView& right) = default;

constexpr StringView& StringView::operator=(const char* right) {
    const auto* utf8String = right;
    if (utf8String && utf8String[0] != 0) {
        size_type length = std::char_traits<char>::length(utf8String);
        if (length > 0) {
            if (utf::IsValid<utf::UTF_8>(utf8String, utf8String + length)) {
                m_data = utf8String;
                m_size = length;
            } else {
                EDOTOOLS_THROW(std::runtime_error("invalid utf8 convertion."));
            }
        };
    }
    return *this;
}

constexpr utf::CodeUnit<utf::UTF_8> StringView::operator[](size_type index) const {
    // TODO: throw error
    return (cbegin() + index)->get();
}

constexpr StringView::size_type StringView::getSize() const {
    return utf::GetSize<utf::UTF_8>(m_data, m_data + m_size);
}

constexpr bool StringView::isEmpty() const {
    return m_size == 0;
}

constexpr StringView::size_type StringView::find(const StringView& str, size_type start) const {
    // Check the size is not past the max code units
    if (start >= getSize()) {
        return sInvalidPos;
    }
    // Find the string
    auto findIt = std::search(cbegin() + start, cend(), str.cbegin(), str.cend());
    return (findIt == cend()) ? sInvalidPos : (findIt - cbegin());
}

constexpr StringView::size_type StringView::findFirstOf(const StringView& str, size_type pos) const {
    if (pos >= getSize()) {
        return sInvalidPos;
    }

    // Find one of the UTF-8 codepoints
    for (auto it = cbegin() + pos; it != cend(); ++it) {
        auto found = std::find(str.cbegin(), str.cend(), *it);
        if (found != str.cend()) {
            return it - cbegin();
        }
    }
    return sInvalidPos;
}

constexpr StringView::size_type StringView::findLastOf(const StringView& str, size_type pos) const {
    size_type utf8StrSize = getSize();
    if (pos != sInvalidPos && pos >= utf8StrSize) {
        return sInvalidPos;
    }

    // Iterate to the start codepoint
    auto startIt = crbegin();
    if (pos != sInvalidPos) {
        startIt += (utf8StrSize - pos - 1);
    }

    // Find one of the UTF-8 codepoints
    for (auto it = startIt; it != crend(); ++it) {
        auto found = std::find(str.cbegin(), str.cend(), *it);
        if (found != str.cend()) {
            return crend() - it;
        }
    }
    return sInvalidPos;
}

constexpr StringView StringView::subString(size_type position, size_type length) const {
    size_type utf8StrSize = getSize();
    if ((position + length) > utf8StrSize) {
        EDOTOOLS_THROW(std::out_of_range("the specified position is out of the string range"));
    }

    // Iterate to the start and end codepoint
    auto startIt = cbegin() + position;
    auto endIt = startIt + length;

    // Create a new string view with given range
    return StringView(startIt.getPtr(), endIt.getPtr() - startIt.getPtr());
}

constexpr const char* StringView::getData() const {
    return m_data;
}

constexpr typename StringView::size_type StringView::getDataSize() const {
    return m_size;
}

constexpr StringView::const_iterator StringView::begin() {
    return StringView::iterator(std::make_pair(m_data, m_data + m_size), m_data);
}

constexpr StringView::const_iterator StringView::cbegin() const {
    return StringView::iterator(std::make_pair(m_data, m_data + m_size), m_data);
}

constexpr StringView::const_iterator StringView::end() {
    return StringView::iterator(std::make_pair(m_data, m_data + m_size), m_data + m_size);
}

constexpr StringView::const_iterator StringView::cend() const {
    return StringView::iterator(std::make_pair(m_data, m_data + m_size), m_data + m_size);
}

constexpr StringView::reverse_iterator StringView::rbegin() {
    auto maxRange = std::make_pair(m_data, m_data + m_size);
    const auto* begin = utf::Prior<utf::UTF_8>(maxRange.second, maxRange.first);
    return reverse_iterator(maxRange, begin);
}

constexpr StringView::const_reverse_iterator StringView::crbegin() const {
    auto maxRange = std::make_pair(m_data, m_data + m_size);
    const auto* begin = utf::Prior<utf::UTF_8>(maxRange.second, maxRange.first);
    return const_reverse_iterator(maxRange, begin);
}

constexpr StringView::reverse_iterator StringView::rend() {
    auto maxRange = std::make_pair(m_data, m_data + m_size);
    const auto* end = m_data;
    return reverse_iterator(maxRange, end);
}

constexpr StringView::const_reverse_iterator StringView::crend() const {
    auto maxRange = std::make_pair(m_data, m_data + m_size);
    const auto* end = m_data;
    return const_reverse_iterator(maxRange, end);
}

inline bool operator==(const StringView& left, const StringView& right) {
    return std::equal(left.getData(), left.getData() + left.getDataSize(), right.getData());
}

inline std::strong_ordering operator<=>(const StringView& left, const StringView& right) {
    auto f1 = left.cbegin();
    auto l1 = left.cend();
    auto f2 = right.cbegin();
    auto l2 = right.cend();
    auto comp = std::compare_three_way{};

    using ret_t = decltype(comp(*f1, *f2));
    static_assert(std::disjunction<std::is_same<ret_t, std::strong_ordering>,
                                   std::is_same<ret_t, std::weak_ordering>,
                                   std::is_same<ret_t, std::partial_ordering>>::value,
                  "The return type must be a comparison category type.");

    bool exhaust1 = (f1 == l1);
    bool exhaust2 = (f2 == l2);
    for (; !exhaust1 && !exhaust2; exhaust1 = (++f1 == l1), exhaust2 = (++f2 == l2)) {
        if (auto c = comp(*f1, *f2); c != 0) {
            return c;
        }
    }

    return !exhaust1   ? std::strong_ordering::greater
           : !exhaust2 ? std::strong_ordering::less
                       : std::strong_ordering::equal;
}

inline bool operator==(const StringView& left, const char* right) {
    return left == StringView(right);
}

inline bool operator==(const char* left, const StringView& right) {
    return StringView(left) == right;
}

inline std::ostream& operator<<(std::ostream& os, const StringView& str) {
    return os.write(str.getData(), str.getSize());
}

}  // namespace edoren

#ifdef EDOTOOLS_FMT_SUPPORT

constexpr auto fmt::formatter<edoren::StringView>::parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
    return ctx.begin();
}

template <typename FormatContext>
auto fmt::formatter<edoren::StringView, char, void>::format(const edoren::StringView& s, FormatContext& ctx)
    -> decltype(ctx.out()) {
    format_to_n(ctx.out(), s.getDataSize(), "{}", s.getData());
    return appender(ctx.out());
}

#endif  // EDOTOOLS_FMT_SUPPORT
