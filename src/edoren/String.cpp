////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2016 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include <edoren/String.hpp>

#include <edoren/Platform.hpp>
#include <edoren/UTF.hpp>

#include <algorithm>

namespace edoren {

const String::size_type String::sInvalidPos = std::basic_string<char>::npos;

String::String() = default;

String::String(char asciiChar) {
    m_string += asciiChar;
}

String::String(char16_t utf16Char) {
    char16_t* ptr = &utf16Char;
    utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(ptr, ptr + 1, &m_string);
}

String::String(char32_t utf32Char) {
    char32_t* ptr = &utf32Char;
    utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(ptr, ptr + 1, &m_string);
}

String::String(const char* utf8String) {
    if (utf8String && utf8String[0] != 0) {
        size_type length = std::char_traits<char>::length(utf8String);
        if (length > 0) {
            if (utf::IsValid<utf::UTF_8>(utf8String, utf8String + length)) {
                m_string.assign(utf8String);
            } else {
                EDOTOOLS_THROW(std::runtime_error("invalid utf8 convertion."));
            }
        };
    }
}

String::String(const char8_t* utf8String) : String(reinterpret_cast<const char*>(utf8String)) {}

String::String(const char16_t* utf16String) {
    if (utf16String && utf16String[0] != 0) {
        // Find the lenght
        const char16_t* utf16StringEnd = utf16String;
        while (*(++utf16StringEnd) != 0) {}
        utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(utf16String, utf16StringEnd, &m_string);
    }
}

String::String(const char32_t* utf32String) {
    if (utf32String && utf32String[0] != 0) {
        // Find the lenght
        const char32_t* utf32StringEnd = utf32String;
        while (*(++utf32StringEnd) != 0) {}
        utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(utf32String, utf32StringEnd, &m_string);
    }
}

String::String(const wchar_t* wideString) {
    if (wideString && wideString[0] != 0) {
        const wchar_t* wideStringEnd = wideString;
        while (*(++wideStringEnd) != 0) {}
#if PLATFORM_IS(PLATFORM_WINDOWS)
        utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(wideString, wideStringEnd, &m_string);
#else
        utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(wideString, wideStringEnd, &m_string);
#endif
    }
}

String::String(const std::basic_string<char>& utf8String) {
    if (!utf8String.empty()) {
        if (utf::IsValid<utf::UTF_8>(utf8String.cbegin(), utf8String.cend())) {
            m_string.assign(utf8String);
        } else {
            EDOTOOLS_THROW(std::runtime_error("invalid utf8 convertion."));
        }
    };
}

String::String(std::basic_string<char>&& utf8String) {
    if (!utf8String.empty()) {
        if (utf::IsValid<utf::UTF_8>(utf8String.cbegin(), utf8String.cend())) {
            m_string = std::move(utf8String);
        } else {
            EDOTOOLS_THROW(std::runtime_error("invalid utf8 convertion."));
        }
    };
}

String::String(const std::basic_string<char8_t>& utf8String) {
    if (!utf8String.empty()) {
        if (utf::IsValid<utf::UTF_8>(utf8String.cbegin(), utf8String.cend())) {
            m_string.assign(reinterpret_cast<const char*>(utf8String.data()), utf8String.size());
        } else {
            EDOTOOLS_THROW(std::runtime_error("invalid utf8 convertion."));
        }
    };
}

String::String(const std::basic_string<char16_t>& utf16String) {
    utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(utf16String.cbegin(), utf16String.cend(), &m_string);
}

String::String(const std::basic_string<char32_t>& utf32String) {
    utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(utf32String.cbegin(), utf32String.cend(), &m_string);
}

String::String(const std::basic_string<wchar_t>& wideString) {
#if PLATFORM_IS(PLATFORM_WINDOWS)
    utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(wideString.cbegin(), wideString.cend(), &m_string);
#else
    utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(wideString.cbegin(), wideString.cend(), &m_string);
#endif
}

String::String(const StringView& stringView) : m_string(stringView.getData(), stringView.getDataSize()) {}

String::String(const String& other) = default;

String::String(String&& other) noexcept : m_string(std::move(other.m_string)) {}

String::~String() = default;

String String::FromUtf8(const char* begin, const char* end) {
    String string;
    if (utf::IsValid<utf::UTF_8>(begin, end)) {
        string.m_string.assign(begin, end);
    } else {
        EDOTOOLS_THROW(std::runtime_error("invalid utf8 convertion."));
    }
    return string;
}

String String::FromUtf8(iterator begin, iterator end) {
    return FromUtf8(begin.getPtr(), end.getPtr());
}

String String::FromUtf16(const char16_t* begin, const char16_t* end) {
    String string;
    utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(begin, end, &string.m_string);
    return string;
}

String String::FromUtf32(const char32_t* begin, const char32_t* end) {
    String string;
    utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(begin, end, &string.m_string);
    return string;
}

String String::FromWide(const wchar_t* begin, const wchar_t* end) {
#if PLATFORM_IS(PLATFORM_WINDOWS)
    return FromUtf16(reinterpret_cast<const char16_t*>(begin), reinterpret_cast<const char16_t*>(end));
#else
    return FromUtf32(reinterpret_cast<const char32_t*>(begin), reinterpret_cast<const char32_t*>(end));
#endif
}

String::operator std::basic_string<char>() const {
    return toUtf8();
}

String::operator std::basic_string<char16_t>() const {
    return toUtf16();
}

String::operator std::basic_string<char32_t>() const {
    return toUtf32();
}

String::operator std::basic_string<wchar_t>() const {
    return toWide();
}

const std::basic_string<char>& String::toUtf8() const {
    return m_string;
}

std::basic_string<char16_t> String::toUtf16() const {
    std::basic_string<char16_t> output;
    utf::UtfToUtf<utf::UTF_8, utf::UTF_16>(m_string.cbegin(), m_string.cend(), &output);
    return output;
}

std::basic_string<char32_t> String::toUtf32() const {
    std::basic_string<char32_t> output;
    utf::UtfToUtf<utf::UTF_8, utf::UTF_32>(m_string.cbegin(), m_string.cend(), &output);
    return output;
}

std::basic_string<wchar_t> String::toWide() const {
    std::basic_string<wchar_t> output;
#if PLATFORM_IS(PLATFORM_WINDOWS)
    utf::UtfToUtf<utf::UTF_8, utf::UTF_16>(m_string.cbegin(), m_string.cend(), &output);
#else
    utf::UtfToUtf<utf::UTF_8, utf::UTF_32>(m_string.cbegin(), m_string.cend(), &output);
#endif
    return output;
}

String& String::operator=(const String& right) = default;

String& String::operator=(const char* right) {
    m_string = right;
    return *this;
}

String& String::operator=(String&& right) noexcept {
    m_string = std::move(right.m_string);
    return *this;
}

String& String::operator+=(const String& right) {
    m_string += right.m_string;
    return *this;
}

String& String::operator+=(const char* right) {
    m_string += right;
    return *this;
}

String& String::operator+=(const char8_t* right) {
    m_string += reinterpret_cast<const char*>(right);
    return *this;
}

String& String::operator+=(char right) {
    if (right >= 0) {
        m_string += right;
    }
    return *this;
}

String& String::operator+=(char32_t right) {
    utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(&right, &right + 1, &m_string);
    return *this;
}

String& String::operator+=(const utf::CodeUnit<utf::UTF_8>& right) {
    // TODO: Missing test
    m_string.reserve(m_string.size() + right.getSize());
    for (auto data : right) {
        m_string.push_back(static_cast<char>(data));
    }
    return *this;
}

utf::CodeUnit<utf::UTF_8> String::operator[](size_type index) const {
    // TODO: throw error
    return (cbegin() + index)->get();
}

void String::clear() {
    m_string.clear();
}

String::size_type String::getSize() const {
    return utf::GetSize<utf::UTF_8>(m_string.begin(), m_string.end());
}

bool String::isEmpty() const {
    return m_string.empty();
}

void String::erase(size_type position, size_type count) {
    size_type utf8StrSize = getSize();
    if ((position + count) > utf8StrSize) {
        EDOTOOLS_THROW(std::out_of_range("the specified position is out of the string range"));
    }

    // Iterate to the start and end codepoint
    auto startIt = begin() + position;
    auto endIt = startIt + count;

    // Iterate to the end codepoint
    auto erasePos = startIt.getPtr() - m_string.data();
    auto eraseCount = endIt.getPtr() - startIt.getPtr();
    m_string.erase(erasePos, eraseCount);
}

void String::insert(size_type position, const StringView& str) {
    size_type utf8StrSize = getSize();
    if (position >= utf8StrSize) {
        EDOTOOLS_THROW(std::out_of_range("the specified position is out of the string range"));
    }

    // Iterate to the start codepoint
    auto startIt = begin() + position;

    // Insert the data in the correct position
    auto insertPos = startIt.getPtr() - m_string.data();
    m_string.insert(insertPos, str.getData(), str.getDataSize());
}

String::size_type String::find(const StringView& str, size_type start) const {
    // Check the size is not past the max code units
    if (start >= getSize()) {
        return sInvalidPos;
    }
    // Find the string
    auto findIt = std::search(cbegin() + start, cend(), str.cbegin(), str.cend());
    return (findIt == cend()) ? sInvalidPos : (findIt - cbegin());
}

String::size_type String::findFirstOf(const StringView& str, size_type pos) const {
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

String::size_type String::findLastOf(const StringView& str, size_type pos) const {
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

void String::replace(size_type position, size_type length, const StringView& replaceWith) {
    size_type utf8StrSize = getSize();
    if ((position + length) > utf8StrSize) {
        EDOTOOLS_THROW(std::out_of_range("the specified position is out of the string range"));
    }

    // Iterate to the start and end codepoint
    auto startIt = cbegin() + position;
    auto endIt = startIt + length;

    // Iterate to the end codepoint
    auto replacePos = startIt.getPtr() - m_string.data();
    auto replaceCount = endIt.getPtr() - startIt.getPtr();
    m_string.replace(replacePos, replaceCount, replaceWith.getData(), replaceWith.getDataSize());
}

void String::replace(uint32_t searchFor, uint32_t replaceWith) {
    if (searchFor == replaceWith) {
        return;
    }
    if (searchFor <= 0x7F && replaceWith <= 0x7F) {
        for (auto& ch : m_string) {
            if (ch == static_cast<char>(searchFor)) {
                ch = static_cast<char>(replaceWith);
            }
        }
    } else {
        String searchForStr(static_cast<char32_t>(searchFor));
        String replaceWithStr(static_cast<char32_t>(replaceWith));
        replace(searchForStr, replaceWithStr);
    }
}

void String::replace(const StringView& searchFor, const StringView& replaceWith) {
    size_type step = replaceWith.getSize();
    // Start the iterator at the beginning of the sequence
    size_t findItPos = 0;
    // Replace each occurrence of search
    while (true) {
        // Search the existence of the string searchFor in the range [begin() + find_it_pos, end())
        auto findIt = std::search(begin() + findItPos, end(), searchFor.cbegin(), searchFor.cend());
        // Check if we reach the end of the string
        if (findIt == end()) {
            return;
        }
        // Replace all the range between [find_it, find_it + len) with the string in replaceWith
        auto stringPos = findIt.getPtr() - m_string.data();
        m_string.replace(stringPos, searchFor.getDataSize(), replaceWith.getData(), replaceWith.getDataSize());
        // Add an additional step to continue the search
        findItPos += step;
    }
}

String String::subString(size_type position, size_type length) const {
    size_type utf8StrSize = getSize();
    if ((position + length) > utf8StrSize) {
        EDOTOOLS_THROW(std::out_of_range("the specified position is out of the string range"));
    }

    // Iterate to the start and end codepoint
    auto startIt = cbegin() + position;
    auto endIt = startIt + length;

    // Create a new string with given range
    return String::FromUtf8(startIt.getPtr(), endIt.getPtr());
}

const char* String::getData() const {
    return m_string.data();
}

typename String::size_type String::getDataSize() const {
    return m_string.size();
}

String::iterator String::begin() {
    auto maxRange = std::make_pair(m_string.data(), m_string.data() + m_string.size());
    return iterator(maxRange, maxRange.first);
}

String::const_iterator String::cbegin() const {
    auto maxRange = std::make_pair(m_string.data(), m_string.data() + m_string.size());
    return const_iterator(maxRange, maxRange.first);
}

String::iterator String::end() {
    auto maxRange = std::make_pair(m_string.data(), m_string.data() + m_string.size());
    return iterator(maxRange, maxRange.second);
}

String::const_iterator String::cend() const {
    auto maxRange = std::make_pair(m_string.data(), m_string.data() + m_string.size());
    return const_iterator(maxRange, maxRange.second);
}

String::reverse_iterator String::rbegin() {
    auto maxRange = std::make_pair(m_string.data(), m_string.data() + m_string.size());
    auto* begin = utf::Prior<utf::UTF_8>(maxRange.second, maxRange.first);
    return reverse_iterator(maxRange, begin);
}

String::const_reverse_iterator String::crbegin() const {
    auto maxRange = std::make_pair(m_string.data(), m_string.data() + m_string.size());
    const auto* begin = utf::Prior<utf::UTF_8>(maxRange.second, maxRange.first);
    return const_reverse_iterator(maxRange, begin);
}

String::reverse_iterator String::rend() {
    auto maxRange = std::make_pair(m_string.data(), m_string.data() + m_string.size());
    auto* end = m_string.data();
    return reverse_iterator(maxRange, end);
}

String::const_reverse_iterator String::crend() const {
    auto maxRange = std::make_pair(m_string.data(), m_string.data() + m_string.size());
    const auto* end = m_string.data();
    return const_reverse_iterator(maxRange, end);
}

bool operator==(const String& left, const String& right) {
    return StringView(left) == StringView(right);
}

std::strong_ordering operator<=>(const String& left, const String& right) {
    return StringView(left) <=> StringView(right);
}

bool operator==(const String& left, const char* right) {
    return StringView(left) == StringView(right);
}

std::strong_ordering operator<=>(const String& left, const char* right) {
    return StringView(left) <=> StringView(right);
}

bool operator==(const char* left, const String& right) {
    return StringView(left) == StringView(right);
}

std::strong_ordering operator<=>(const char* left, const String& right) {
    return StringView(left) <=> StringView(right);
}

bool operator==(const String& left, const char8_t* right) {
    return StringView(left) == StringView(right);
}

std::strong_ordering operator<=>(const String& left, const char8_t* right) {
    return StringView(left) <=> StringView(right);
}

bool operator==(const char8_t* left, const String& right) {
    return StringView(left) == StringView(right);
}

std::strong_ordering operator<=>(const char8_t* left, const String& right) {
    return StringView(left) <=> StringView(right);
}

bool operator==(const String& left, const StringView& right) {
    return StringView(left) == right;
}

std::strong_ordering operator<=>(const String& left, const StringView& right) {
    return StringView(left) <=> right;
}

bool operator==(const StringView& left, const String& right) {
    return left == StringView(right);
}

std::strong_ordering operator<=>(const StringView& left, const String& right) {
    return left <=> StringView(right);
}

String operator+(const String& left, const String& right) {
    String string = left;
    return string += right;
}

String operator+(const String& left, const char* right) {
    String string = left;
    return string += right;
}

String operator+(const char* left, const String& right) {
    String string = left;
    return string += right;
}

String operator+(const String& left, const char8_t* right) {
    String string = left;
    return string += right;
}

String operator+(const char8_t* left, const String& right) {
    String string = left;
    return string += right;
}

String operator+(const String& left, char right) {
    String string = left;
    return string += right;
}

String operator+(char left, const String& right) {
    String string = left;
    return string += right;
}

std::ostream& operator<<(std::ostream& os, const String& str) {
    return os << str.toUtf8();
}

StringFormatProxy<char> operator""_format(const char* str, size_t /*unused*/) {
    return {str};
}

StringFormatProxy<wchar_t> operator""_format(const wchar_t* str, size_t /*unused*/) {
    return {str};
}

}  // namespace edoren
