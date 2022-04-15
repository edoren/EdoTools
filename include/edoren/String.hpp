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

#pragma once

#include <compare>
#include <sstream>
#include <string>

#include <edoren/util/Platform.hpp>
#include <edoren/StringView.hpp>
#include <edoren/UTF.hpp>
#include <edoren/util/Config.hpp>

#ifdef EDOTOOLS_FMT_SUPPORT
    #include <fmt/format.h>
#endif
#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT
    #include <nlohmann/json.hpp>
#endif

namespace edoren {

/**
 * @brief Utility string class that automatically handles
 *       conversions between types and encodings
 *
 * String is a utility string class defined mainly for
 * convenience. It is a Unicode string (implemented using
 * UTF-8), thus it can store any character in the world
 * (European, Chinese, Arabic, Hebrew, etc.).
 *
 * It automatically handles conversions from/to ASCII and
 * wide strings, so that you can work with standard string
 * classes and still be compatible with functions taking a
 * String.
 *
 * @code
 * String s;
 *
 * std::basic_string<char> s1 = s;  // automatically converted to ASCII string
 * std::basic_string<wchar_t> s2 = s; // automatically converted to wide string
 * s = "hello";         // automatically converted from ASCII string
 * s = L"hello";        // automatically converted from wide string
 * s += 'a';            // automatically converted from ASCII string
 * s += L'a';           // automatically converted from wide string
 * @endcode
 *
 * String defines the most important functions of the
 * standard std::basic_string<char> class: removing, random access, iterating,
 * appending, comparing, etc.
 */
class EDOTOOLS_API String {
public:
    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    using size_type = size_t;                                                      ///< Size type
    using const_iterator = utf::Iterator<utf::UTF_8, const char*>;                 ///< Read-only iterator type
    using iterator = const_iterator;                                               ///< Iterator type
    using const_reverse_iterator = utf::ReverseIterator<utf::UTF_8, const char*>;  ///< Read-only reverse iterator type
    using reverse_iterator = const_reverse_iterator;                               ///< Reverse iterator type

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const size_type sInvalidPos;  ///< Represents an invalid position in the string

    /**
     * @brief Default constructor
     *
     * This constructor creates an empty string.
     */
    String();

    /**
     * @brief Construct from a single ASCII character
     *
     * @param asciiChar ASCII character to convert
     */
    String(char asciiChar);

    /**
     * @brief Construct from single UTF-16 character
     *
     * @param utf16Char UTF-16 character to convert
     */
    String(char16_t utf16Char);

    /**
     * @brief Construct from single UTF-32 character
     *
     * @param utf32Char UTF-32 character to convert
     */
    String(char32_t utf32Char);

    /**
     * @brief Construct from a null-terminated (value 0) UTF-8 string
     *
     * @param utf8String UTF-8 string to assign
     */
    String(const char* utf8String);

    /**
     * @brief Construct from a null-terminated (value 0) UTF-8 string
     *
     * @param utf8String UTF-8 string to assign
     */
    String(const char8_t* utf8String);

    /**
     * @brief Construct from a null-terminated (value 0) UTF-16 string
     *
     * @param utf16String UTF-8 string to assign
     */
    String(const char16_t* utf16String);

    /**
     * @brief Construct from a null-terminated (value 0) UTF-32 string
     *
     * @param utf32String UTF-8 string to assign
     */
    String(const char32_t* utf32String);

    /**
     * @brief Construct from a null-terminated (value 0) Wide string
     *
     * @param wideString Wide string to assign
     */
    String(const wchar_t* wideString);

    /**
     * @brief Construct from an UTF-8 string
     *
     * @param utf8String UTF-8 string to assign
     */
    String(const std::basic_string<char>& utf8String);

    /**
     * @brief Construct from an UTF-8 string
     *
     * @param utf8String UTF-8 string to move
     */
    String(std::basic_string<char>&& utf8String);

    /**
     * @brief Construct from an UTF-8 string
     *
     * @param utf8String UTF-8 string to assign
     */
    String(const std::basic_string<char8_t>& utf8String);

    /**
     * @brief Construct from a UTF-16 string
     *
     * @param utf16String UTF-16 string to convert
     */
    String(const std::basic_string<char16_t>& utf16String);

    /**
     * @brief Construct from an UTF-32 string
     *
     * @param utf32String UTF-32 string to convert
     */
    String(const std::basic_string<char32_t>& utf32String);

    /**
     * @brief Construct from an Wide string
     *
     * @param wideString Wide string to convert
     */
    String(const std::basic_string<wchar_t>& wideString);

    /**
     * @brief Construct from an StringView
     *
     * @param stringView String view to assign
     */
    String(const StringView& stringView);

    /**
     * @brief Copy constructor
     *
     * @param other Instance to copy
     */
    String(const String& other);

    /**
     * @brief Move constructor
     *
     * @param other Instance to move
     */
    String(String&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~String();

    /**
     * @brief Create a new String from a UTF-8 encoded string
     *
     * This function is provided for consistency, it is equivalent to
     * using the constructors that takes a const char* or a
     * std::basic_string<char>.
     *
     * @param begin Pointer to the beginning of the UTF-8 sequence
     * @param end   Pointer to the end of the UTF-8 sequence
     *
     * @return A String containing the source string
     *
     * @see FromUtf16, FromUtf32, FromWide
     */
    static String FromUtf8(const char* begin, const char* end);

    /**
     * @brief Create a new String from a UTF-8 encoded string
     *
     * @param begin Pointer to the beginning of the UTF-8 sequence
     * @param end   Pointer to the end of the UTF-8 sequence
     *
     * @return A String containing the source string
     *
     * @see FromUtf16, FromUtf32, FromWide
     */
    static String FromUtf8(iterator begin, iterator end);

    /**
     * @brief Create a new String from a UTF-8 encoded string
     *
     * The iterators must point to an object of type char
     *
     * @param begin Pointer to the beginning of the UTF-8 sequence
     * @param end   Pointer to the end of the UTF-8 sequence
     *
     * @return A String containing the source string
     *
     * @see FromUtf16, FromUtf32, FromWide
     */
    template <typename It,
              typename = std::enable_if_t<type::is_forward_iterator<It>::value &&
                                          sizeof(type::iterator_underlying_type_t<It>) == sizeof(char)>>
    static String FromUtf8(It begin, It end) {
        String string;
        if (utf::IsValid<utf::UTF_8>(begin, end)) {
            string.m_string.assign(begin, end);
        } else {
            EDOTOOLS_THROW(std::runtime_error("invalid utf8 convertion."));
        }
        return string;
    }

    /**
     * @brief Create a new String from a UTF-16 encoded string
     *
     * @param begin Pointer to the beginning of the UTF-16 sequence
     * @param end   Pointer to the end of the UTF-16 sequence
     *
     * @return A String containing the source string
     *
     * @see FromUtf8, FromUtf32, FromWide
     */
    static String FromUtf16(const char16_t* begin, const char16_t* end);

    /**
     * @brief Create a new String from a UTF-16 encoded string
     *
     * The iterators must point to an object of type char16_t
     *
     * @param begin Pointer to the beginning of the UTF-16 sequence
     * @param end   Pointer to the end of the UTF-16 sequence
     *
     * @return A String containing the source string
     *
     * @see FromUtf8, FromUtf32, FromWide
     */
    template <typename Iterator>
    static String FromUtf16(Iterator begin, Iterator end) {
        String string;
        utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(begin, end, &string.m_string);
        return string;
    }

    /**
     * @brief Create a new String from a value
     *
     * This method make use of the std::ostream << operator
     *
     * @param value The value to be converted to a String
     *
     * @return A String containing the converted value
     */
    template <typename T>
    static String FromValue(T value) {
        std::basic_stringstream<char> stream;
        stream << value;
        return stream.str();
    }

    /**
     * @brief Create a new String from a UTF-32 encoded string
     *
     * @param begin Pointer to the beginning of the UTF-32 sequence
     * @param end   Pointer to the end of the UTF-32 sequence
     *
     * @return A String containing the source string
     *
     * @see FromUtf8, FromUtf16, FromWide
     */
    static String FromUtf32(const char32_t* begin, const char32_t* end);

    /**
     * @brief Create a new String from a UTF-32 encoded string
     *
     * The iterators must point to an object of type char32_t
     *
     * @param begin Pointer to the beginning of the UTF-32 sequence
     * @param end   Pointer to the end of the UTF-32 sequence
     *
     * @return A String containing the source string
     *
     * @see FromUtf8, FromUtf16, FromWide
     */
    template <typename Iterator>
    static String FromUtf32(Iterator begin, Iterator end) {
        String string;
        utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(begin, end, &string.m_string);
        return string;
    }

    /**
     * @brief Create a new String from a Wide string
     *
     * @param begin Pointer to the beginning of the Wide string
     * @param end   Pointer to the end of the Wide string
     *
     * @return A String containing the source string
     *
     * @see FromUtf8, FromUtf16, FromUtf32
     */
    static String FromWide(const wchar_t* begin, const wchar_t* end);

    /**
     * @brief Create a new String from a Wide string
     *
     * The iterators must point to an object of type char32_t
     *
     * @param begin Pointer to the beginning of the Wide string
     * @param end   Pointer to the end of the Wide string
     *
     * @return A String containing the source string
     *
     * @see FromUtf8, FromUtf16, FromWide
     */
    template <typename Iterator>
    static String FromWide(Iterator begin, Iterator end) {
        String string;
#if PLATFORM_IS(PLATFORM_WINDOWS)
        utf::UtfToUtf<utf::UTF_16, utf::UTF_8>(begin, end, &string.m_string);
#else
        utf::UtfToUtf<utf::UTF_32, utf::UTF_8>(begin, end, &string.m_string);
#endif
        return string;
    }

    /**
     * @brief Explicit conversion operator to std::basic_string<char>
     *        (UTF-8 string)
     *
     * @return Converted UTF-8 string
     *
     * @see toUtf8
     * @see operator std::basic_string<char16_t>
     * @see operator std::basic_string<char32_t>
     * @see operator std::basic_string<wchar_t>
     */
    explicit operator std::basic_string<char>() const;

    /**
     * @brief Explicit conversion operator to std::basic_string<char16_t>
     *        (UTF-16 string)
     *
     * @return Converted UTF-16 string
     *
     * @see ToUtf16
     * @see operator std::basic_string<char>
     * @see operator std::basic_string<char32_t>
     * @see operator std::basic_string<wchar_t>
     */
    explicit operator std::basic_string<char16_t>() const;

    /**
     * @brief Explicit conversion operator to std::basic_string<char32_t>
     *        (UTF-32 string)
     *
     * @return Converted UTF-32 string
     *
     * @see ToUtf32
     * @see operator std::basic_string<char>
     * @see operator std::basic_string<char16_t>
     * @see operator std::basic_string<wchar_t>
     */
    explicit operator std::basic_string<char32_t>() const;

    /**
     * @brief Implicit conversion operator to std::basic_string<wchar_t>
     *        (Wide string)
     *
     * @return Converted Wide string
     *
     * @see ToWide
     * @see operator std::basic_string<char>
     * @see operator std::basic_string<char16_t>
     * @see operator std::basic_string<char32_t>
     */
    explicit operator std::basic_string<wchar_t>() const;

    /**
     * @brief Return the internal UTF-8 string
     *
     * This function doesn't perform any conversion, since the
     * string is already stored as UTF-8 internally.
     *
     * @return Internal UTF-8 string
     *
     * @see ToUtf16, ToUtf32
     */
    const std::basic_string<char>& toUtf8() const;

    /**
     * @brief Convert the UTF-8 string to a UTF-16 string
     *
     * @return Converted UTF-16 string
     *
     * @see ToUtf8, ToUtf32
     */
    std::basic_string<char16_t> toUtf16() const;

    /**
     * @brief Convert the UTF-8 string to a UTF-32 string
     *
     * @return Converted UTF-32 string
     *
     * @see ToUtf8, ToUtf16
     */
    std::basic_string<char32_t> toUtf32() const;

    /**
     * @brief Convert the UTF-8 string to a UTF-32 string
     *
     * @return Converted UTF-32 string
     *
     * @see ToUtf8, ToUtf16
     */
    std::basic_string<wchar_t> toWide() const;

    /**
     * @brief Overload of assignment operator
     *
     * @param right Instance to assign
     *
     * @return Reference to self
     */
    String& operator=(const String& right);

    /**
     * @brief Overload of assignment operator
     *
     * @param right Instance to assign
     *
     * @return Reference to self
     */
    String& operator=(const char* right);

    /**
     * @brief Overload of move assignment operator
     *
     * @param right Instance to move
     *
     * @return Reference to self
     */
    String& operator=(String&& right) noexcept;

    /**
     * @brief Overload of += operator to append an UTF-8 string
     *
     * @param right String to append
     *
     * @return Reference to self
     */
    String& operator+=(const String& right);

    /**
     * @brief Overload of += operator to append an UTF-8 null
     *        terminated string
     *
     * @param right String to append
     *
     * @return Reference to self
     */
    String& operator+=(const char* right);

    /**
     * @brief Overload of += operator to append an UTF-8 null
     *        terminated string
     *
     * @param right String to append
     *
     * @return Reference to self
     */
    String& operator+=(const char8_t* right);

    /**
     * @brief Overload of += operator to append a single ASCII
     *        character
     *
     * @param right ASCII character to append
     *
     * @return Reference to self
     */
    String& operator+=(char right);

    /**
     * @brief Overload of += operator to append a UTF-32 character
     *
     * @param right Utf-32 character to append
     *
     * @return Reference to self
     */
    String& operator+=(char32_t right);

    /**
     * @brief Overload of += operator to append a UTF-8 CodeUnit
     *
     * @param right CodeUnit to append
     *
     * @return Reference to self
     */
    String& operator+=(const utf::CodeUnit<utf::UTF_8>& right);

    /**
     * @brief Overload of [] operator to access a character by its position
     *
     * This function provides read-only access to characters.
     * Note: the behavior is undefined if `index` is out of range.
     *
     * @param index Index of the character to get
     *
     * @return Character at position `index`
     */
    utf::CodeUnit<utf::UTF_8> operator[](size_type index) const;

    /**
     * @brief Clear the string
     *
     * This function removes all the characters from the string.
     *
     * @see isEmpty, erase
     */
    void clear();

    /**
     * @brief Get the size of the string
     *
     * @return Number of UTF-8 codepoints in the string
     *
     * @see isEmpty
     */
    size_type getSize() const;

    /**
     * @brief Check whether the string is empty or not
     *
     * @return True if the string is empty (i.e. contains no character)
     *
     * @see clear, getSize
     */
    bool isEmpty() const;

    /**
     * @brief Erase one or more characters from the string
     *
     * This function removes a sequence of `count` characters
     * starting from `position`.
     *
     * @param position Position of the first character to erase
     * @param count    Number of characters to erase
     */
    void erase(size_type position, size_type count = 1);

    /**
     * @brief Insert one or more characters into the string
     *
     * This function inserts the characters of `str`
     * into the string, starting from `position`.
     *
     * @param position Position of insertion
     * @param str      Characters to insert
     */
    void insert(size_type position, const StringView& str);

    /**
     * @brief Find a sequence of one or more characters in
     *        the string
     *
     * This function searches for the characters of `str`
     * in the string, starting from `start`.
     *
     * @param str   Characters to find
     * @param start Where to begin searching
     *
     * @return Position of `str` in the string, or @ref sInvalidPos
     *         if not found
     */
    size_type find(const StringView& str, size_type start = 0) const;

    /**
     * @brief Finds the first character equal to one of the
     *        characters in the given character sequence.
     *
     * This function searches for the first character equal to
     * one of the characters in `str`. The search considers
     * only the interval [`pos`, `getSize()`]
     *
     * @param str Characters to find
     * @param pos Where to begin searching
     *
     * @return Position of the first character of `str` found
     *        in the string, or @ref sInvalidPos if not found
     */
    size_type findFirstOf(const StringView& str, size_type pos = 0) const;

    /**
     * @brief Finds the last character equal to one of the
     *        characters in the given character sequence.
     *
     * This function searches for the last character equal to
     * one of the characters in `str`. The search considers
     * only the interval [`0`, `pos`]
     *
     * @param str Characters to find
     * @param pos Position of the last character in the string to be considered in the search
     *
     * @return Position of the last character of `str` found
     *         in the string, or @ref sInvalidPos if not found
     */
    size_type findLastOf(const StringView& str, size_type pos = sInvalidPos) const;

    /**
     * @brief Check if the string first characters are equal to a given string
     *
     * @param other String to check
     *
     * @return true` if equal, `false` otherwise
     */
    bool startsWith(const StringView& other) const;

    /**
     * @brief Check if the string last characters are equal to a given string
     *
     * @param other String to check
     *
     * @return true` if equal, `false` otherwise
     */
    bool endsWith(const StringView& other) const;

    /**
     * @brief Replace a SubString with another string
     *
     * This function replaces the SubString that starts at index `position`
     * and spans `length` characters with the string `replaceWith`.
     *
     * @param position    Index of the first character to be replaced
     * @param length      Number of characters to replace. You can pass
     *                    InvalidPos to replace all characters until the
     *                    end of the string.
     * @param replaceWith String that replaces the given SubString.
     */
    void replace(size_type position, size_type length, const StringView& replaceWith);

    /**
     * @brief Replace all occurrences of an Unicode code point with other
     *        Unicode code point
     *
     * This function replaces all occurrences of the code point `searchFor`
     * in this string with the `replaceWith` code point.
     *
     * @param searchFor   The code point being searched for
     * @param replaceWith The code point that replaces found `searchFor`
     *                    code point
     */
    void replace(uint32_t searchFor, uint32_t replaceWith);

    /**
     * @brief Replace all occurrences of a SubString with a replacement string
     *
     * This function replaces all occurrences of `searchFor` in this string
     * with the string `replaceWith`.
     *
     * @param searchFor   The value being searched for
     * @param replaceWith The value that replaces found `searchFor` values
     */
    void replace(const StringView& searchFor, const StringView& replaceWith);

    /**
     * @brief Return a part of the string
     *
     * This function returns the SubString that starts at index `position`
     * and spans `length` characters.
     *
     * @param position Index of the first character
     * @param length   Number of characters to include in the SubString (if
     *                 the string is shorter, as many characters as possible
     *                 are included). @ref sInvalidPos can be used to include
     *                 all characters until the end of the string.
     *
     * @return String object containing a SubString of this object
     */
    String subString(size_type position, size_type length = sInvalidPos) const;

    /**
     * @brief Get a pointer to the C-style array of characters
     *
     * This functions provides a read-only access to a
     * null-terminated C-style representation of the string.
     * The returned pointer is temporary and is meant only for
     * immediate use, thus it is not recommended to store it.
     *
     * @return Read-only pointer to the array of characters
     *
     * @see getDataSize
     */
    const char* getData() const;

    /**
     * @brief Get the size in bytes of the string
     *
     * @return Number of bytes the data has
     *
     * @see getData
     */
    size_type getDataSize() const;

    /**
     * @brief Return an iterator to the beginning of the string
     *
     * @return Read-only iterator to the beginning of the string
     *
     * @see end
     */
    iterator begin();

    /**
     * @brief Return an iterator to the beginning of the string
     *
     * @return Read-only iterator to the beginning of the string
     *
     * @see end
     */
    const_iterator cbegin() const;

    /**
     * @brief Return an iterator to the end of the string
     *
     * This iterator refers to 1 position past the last character;
     * thus it represents an invalid character and should never be
     * accessed.
     *
     * @return Read-only iterator to the end of the string
     *
     * @see begin
     */
    iterator end();

    /**
     * @brief Return an iterator to the end of the string
     *
     * This iterator refers to 1 position past the last character;
     * thus it represents an invalid character and should never be
     * accessed.
     *
     * @return Read-only iterator to the end of the string
     *
     * @see begin
     */
    const_iterator cend() const;

    /**
     * @brief Return an iterator to the reverse beginning of the string
     *
     * This iterator refers to 1 position prior the first character;
     * thus it represents an invalid character and should never be
     * accessed.
     *
     * @return Read-only iterator to the reverse beginning of the string
     *
     * @see rend
     */
    reverse_iterator rbegin();

    /**
     * @brief Return an iterator to the reverse beginning of the string
     *
     * This iterator refers to 1 position prior the first character;
     * thus it represents an invalid character and should never be
     * accessed.
     *
     * @return Read-only iterator to the reverse beginning of the string
     *
     * @see crend
     */
    const_reverse_iterator crbegin() const;

    /**
     * @brief Return an iterator to the reverse end of the string
     *
     * This iterator refers to the first element in the string
     *
     * @return Read-only iterator to the end of the string
     *
     * @see rbegin
     */
    reverse_iterator rend();

    /**
     * @brief Return an iterator to the end of the string
     *
     * This iterator refers to the first element in the string
     *
     * @return Read-only iterator to the end of the string
     *
     * @see crbegin
     */
    const_reverse_iterator crend() const;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::basic_string<char> m_string;  ///< Internal string of UTF-8 characters
};

/**
 * @relates String
 * @brief Overload of == operator to compare two UTF-8 strings
 *
 * @param left  Left operand (a String)
 * @param right Right operand (a String)
 *
 * @return True if both strings are equal
 */
EDOTOOLS_API bool operator==(const String& left, const String& right);

/**
 * @relates String
 * @brief Overload of == operator to compare two UTF-8 strings
 *
 * @param left  Left operand (a String)
 * @param right Right operand (a String)
 *
 * @return True if both strings are equal
 */
EDOTOOLS_API std::strong_ordering operator<=>(const String& left, const String& right);

/**
 * @relates String
 * @brief Overload of == operator to compare a UTF-8 strings and a null
 *        terminated (value 0) UTF-8 string
 *
 * @param left  Left operand (a String)
 * @param right Right operand (a null terminated UTF-8 string)
 *
 * @return True if both strings are equal
 */
EDOTOOLS_API bool operator==(const String& left, const char* right);

/**
 * @relates String
 * @brief Overload of == operator to compare a UTF-8 strings and a null
 *        terminated (value 0) UTF-8 string
 *
 * @param left  Left operand (a String)
 * @param right Right operand (a null terminated UTF-8 string)
 *
 * @return True if both strings are equal
 */
EDOTOOLS_API std::strong_ordering operator<=>(const String& left, const char* right);

/**
 * @relates String
 * @brief Overload of == operator to compare a UTF-8 strings and a null
 *        terminated (value 0) UTF-8 string
 *
 * @param left  Left operand (a null terminated UTF-8 string)
 * @param right Right operand (a String)
 *
 * @return True if both strings are equal
 */
EDOTOOLS_API bool operator==(const char* left, const String& right);

/**
 * @relates String
 * @brief Overload of == operator to compare a UTF-8 strings and a null
 *        terminated (value 0) UTF-8 string
 *
 * @param left  Left operand (a null terminated UTF-8 string)
 * @param right Right operand (a String)
 *
 * @return True if both strings are equal
 */
EDOTOOLS_API std::strong_ordering operator<=>(const char* left, const String& right);

/**
 * @copydoc std::strong_ordering operator<=>(const String& left, const char* right)
 */
EDOTOOLS_API bool operator==(const String& left, const char8_t* right);

/**
 * @copydoc std::strong_ordering operator<=>(const String& left, const char* right)
 */
EDOTOOLS_API std::strong_ordering operator<=>(const String& left, const char8_t* right);

/**
 * @copydoc bool operator==(const char* left, const String& right)
 */
EDOTOOLS_API bool operator==(const char8_t* left, const String& right);

/**
 * @copydoc std::strong_ordering operator<=>(const char* left, const String& right)
 */
EDOTOOLS_API std::strong_ordering operator<=>(const char8_t* left, const String& right);

/**
 * @copydoc bool operator==(const String& left, const char* right)
 */
EDOTOOLS_API bool operator==(const String& left, const StringView& right);

/**
 * @copydoc std::strong_ordering operator<=>(const String& left, const char* right)
 */
EDOTOOLS_API std::strong_ordering operator<=>(const String& left, const StringView& right);

/**
 * @copydoc bool operator==(const char* left, const String& right)
 */
EDOTOOLS_API bool operator==(const StringView& left, const String& right);

/**
 * @copydoc std::strong_ordering operator<=>(const char* left, const String& right)
 */
EDOTOOLS_API std::strong_ordering operator<=>(const StringView& left, const String& right);

/**
 * @relates String
 * @brief Overload of binary + operator to concatenate two strings
 *
 * @param left  Left operand (a String)
 * @param right Right operand (a String)
 *
 * @return Concatenated string
 */
EDOTOOLS_API String operator+(const String& left, const String& right);

/**
 * @relates String
 * @brief Overload of binary + operator to concatenate a UTF-8
 *        strings and a null terminated (value 0) UTF-8 string
 *
 * @param left  Left operand (a String)
 * @param right Right operand (a null terminated UTF-8 string)
 *
 * @return Concatenated string
 */
EDOTOOLS_API String operator+(const String& left, const char* right);

/**
 * @relates String
 * @brief Overload of binary + operator to concatenate a UTF-8
 *        strings and a null terminated (value 0) UTF-8 string
 *
 * @param left  Left operand (a null terminated UTF-8 string)
 * @param right Right operand (a String)
 *
 * @return Concatenated string
 */
EDOTOOLS_API String operator+(const char* left, const String& right);

/**
 * @copydoc String operator+(const String& left, const char* right)
 */
EDOTOOLS_API String operator+(const String& left, const char8_t* right);

/**
 * @copydoc String operator+(const char* left, const String& right)
 */
EDOTOOLS_API String operator+(const char8_t* left, const String& right);

/**
 * @relates String
 * @brief Overload of binary + operator to concatenate a UTF-8
 *        strings and a single ASCII character
 *
 * @param left  Left operand (a String)
 * @param right Right operand (an ASCII character)
 *
 * @return Concatenated string
 */
EDOTOOLS_API String operator+(const String& left, char right);

/**
 * @relates String
 * @brief Overload of binary + operator to concatenate a UTF-8
 *        strings and a single ASCII character
 *
 * @param left  Left operand (an ASCII character)
 * @param right Right operand (a String)
 *
 * @return Concatenated string
 */
EDOTOOLS_API String operator+(char left, const String& right);

/**
 * @relates String
 * @brief Overload of << operator add support to using the
 *        String class with STL ostream classes
 *
 * @param os  A std::ostream
 * @param str A String
 *
 * @return Returns os
 */
EDOTOOLS_API std::ostream& operator<<(std::ostream& os, const String& str);

template <typename Char>
struct StringFormatProxy {
    const Char* str;

    template <typename... Args>
    String operator()(Args&&... args) const {
        auto s = fmt::vformat(str, fmt::make_format_args(std::forward<Args>(args)...));
        return String(std::move(s));
    }
};

EDOTOOLS_API StringFormatProxy<char> operator""_format(const char* str, size_t /*unused*/);

EDOTOOLS_API StringFormatProxy<wchar_t> operator""_format(const wchar_t* str, size_t /*unused*/);

}  // namespace edoren

#ifdef EDOTOOLS_FMT_SUPPORT
// See https://fmt.dev/latest/api.html#formatting-user-defined-types

template <>
struct fmt::formatter<edoren::String> {
    constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext = fmt::format_context>
    auto format(const edoren::String& s, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), "{}", s.toUtf8());
    }
};

#endif  // EDOTOOLS_FMT_SUPPORT

#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT

namespace nlohmann {

template <>
struct adl_serializer<edoren::String> {
    static void to_json(json& j, const edoren::String& s) {
        j = nlohmann::json(s.toUtf8());
    }

    static void from_json(const json& j, edoren::String& s) {
        s = std::move(std::string(j));
    }
};

}  // namespace nlohmann

EDOTOOLS_API void to_json(nlohmann::json& j, const edoren::String& s);

EDOTOOLS_API void from_json(const nlohmann::json& j, edoren::String& s);

#endif  // EDOTOOLS_NLOHMANN_JSON_SUPPORT
