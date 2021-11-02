#pragma once

#include <edoren/UTF.hpp>
#include <edoren/util/Config.hpp>

#ifdef EDOTOOLS_FMT_SUPPORT
    #include <fmt/format.h>
#endif
#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT
    #include <nlohmann/json.hpp>
#endif

namespace edoren {

class String;

/**
 * @brief Utility class to handle a string view to a UTF-8 character sequence
 */
class EDOTOOLS_API StringView {
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
    static inline const size_type sInvalidPos = size_type(-1);  ///< Represents an invalid position in the string view

    /**
     * @brief Default constructor
     *
     * This constructor creates an empty string view.
     */
    constexpr StringView() = default;

    /**
     * @brief Construct a string view from a null-terminated (value 0) UTF-8 string
     *
     * @param utf8String UTF-8 string to view
     */
    constexpr StringView(const char* utf8String);

    /**
     * @brief Construct a string view from a null-terminated (value 0) UTF-8 string
     *
     * @note No UTF-8 validation is performed
     *
     * @param utf8String UTF-8 string to view
     */
    constexpr StringView(const char* utf8String, size_type size);

    /**
     * @brief Construct a string view from a null-terminated (value 0) UTF-8 string
     *
     * @param utf8String UTF-8 string to view
     */
    StringView(const char8_t* utf8String);

    /**
     * @brief Construct a string view from an String
     *
     * @param string String to view
     */
    StringView(const String& string);

    /**
     * @brief Construct a string view from an StringView
     *
     * @param other StringView copy the view from
     */
    constexpr StringView(const StringView& other) = default;

    /**
     * @brief Destructor
     */
    ~StringView() = default;

    /**
     * @brief Overload of assignment operator
     *
     * @param right Instance to assign
     *
     * @return Reference to self
     */
    constexpr StringView& operator=(const StringView& right);

    /**
     * @brief Overload of assignment operator
     *
     * @param right Instance to assign
     *
     * @return Reference to self
     */
    StringView& operator=(const String& right);

    /**
     * @brief Overload of assignment operator
     *
     * @param right Instance to assign
     *
     * @return Reference to self
     */
    constexpr StringView& operator=(const char* right);

    /**
     * @brief Overload of assignment operator
     *
     * @param right Instance to assign
     *
     * @return Reference to self
     */
    StringView& operator=(const char8_t* right);

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
    constexpr utf::CodeUnit<utf::UTF_8> operator[](size_type index) const;

    /**
     * @brief Get the size of the string
     *
     * @return Number of UTF-8 codepoints in the string
     *
     * @see isEmpty
     */
    constexpr size_type getSize() const;

    /**
     * @brief Check whether the string is empty or not
     *
     * @return True if the string is empty (i.e. contains no character)
     *
     * @see clear, getSize
     */
    constexpr bool isEmpty() const;

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
    constexpr size_type find(const StringView& str, size_type start = 0) const;

    /**
     * @brief Finds the first character equal to one of the
     *        characters in the given character sequence.
     *
     * This function searches for the first character equal to
     * one of the characters in `str`. The search considers
     * only the interval [`pos`, `getSize()`]
     *
     * @param str   Characters to find
     * @param pos Where to begin searching
     *
     * @return Position of the first character of `str` found
     *         in the string, or @ref sInvalidPos if not found
     */
    constexpr size_type findFirstOf(const StringView& str, size_type pos = 0) const;

    /**
     * @brief Finds the last character equal to one of the
     *        characters in the given character sequence.
     *
     * This function searches for the last character equal to
     * one of the characters in `str`. The search considers
     * only the interval [`0`, `pos`]
     *
     * @param str Characters to find
     * @param pos Where to begin searching
     *
     * @return Position of the last character of `str` found
     *        in the string, or @ref sInvalidPos if not found
     */
    constexpr size_type findLastOf(const StringView& str, size_type pos = sInvalidPos) const;

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
    constexpr StringView subString(size_type position, size_type length = sInvalidPos) const;

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
    constexpr const char* getData() const;

    /**
     * @brief Get the size in bytes of the string
     *
     * @return Number of bytes the data has
     *
     * @see getData
     */
    constexpr size_type getDataSize() const;

    /**
     * @brief Return an iterator to the beginning of the string
     *
     * @return Read-write iterator to the beginning of the string characters
     *
     * @see end
     */
    constexpr iterator begin();

    /**
     * @brief Return an iterator to the beginning of the string
     *
     * @return Read-only iterator to the beginning of the string characters
     *
     * @see end
     */
    constexpr const_iterator cbegin() const;

    /**
     * @brief Return an iterator to the end of the string
     *
     * This iterator refers to 1 position past the last character;
     * thus it represents an invalid character and should never be
     * accessed.
     *
     * @return Read-write iterator to the end of the string characters
     *
     * @see begin
     */
    constexpr iterator end();

    /**
     * @brief Return an iterator to the end of the string
     *
     * This iterator refers to 1 position past the last character;
     * thus it represents an invalid character and should never be
     * accessed.
     *
     * @return Read-only iterator to the end of the string characters
     *
     * @see begin
     */
    constexpr const_iterator cend() const;

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
    constexpr reverse_iterator rbegin();

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
    constexpr const_reverse_iterator crbegin() const;

    /**
     * @brief Return an iterator to the reverse end of the string
     *
     * This iterator refers to the first element in the string
     *
     * @return Read-only iterator to the end of the string
     *
     * @see rbegin
     */
    constexpr reverse_iterator rend();

    /**
     * @brief Return an iterator to the end of the string
     *
     * This iterator refers to the first element in the string
     *
     * @return Read-only iterator to the end of the string
     *
     * @see crbegin
     */
    constexpr const_reverse_iterator crend() const;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const char* m_data = {nullptr};  ///< The begining of the string view
    size_type m_size = {0};          ///< The size of the string view
};

/**
 * @relates String
 * @brief Overload of == operator to compare two StringView
 *
 * @param left  Left operand (a StringView)
 * @param right Right operand (a StringView)
 *
 * @return True if both strings are equal
 */
bool operator==(const StringView& left, const StringView& right);

/**
 * @relates String
 * @brief Overload of == operator to compare two strings
 *
 * @param left  Left operand (a StringView)
 * @param right Right operand (a const char*)
 *
 * @return True if both strings are equal
 */
bool operator==(const StringView& left, const char* right);

/**
 * @relates String
 * @brief Overload of == operator to compare two strings
 *
 * @param left  Left operand (a const char*)
 * @param right Right operand (a StringView)
 *
 * @return True if both strings are equal
 */
bool operator==(const char* left, const StringView& right);

/**
 * @relates String
 * @brief Overload of <=> operator to compare two StringView
 *
 * @param left  Left operand (a StringView)
 * @param right Right operand (a StringView)
 *
 * @return True if both strings are equal
 */
EDOTOOLS_API std::strong_ordering operator<=>(const StringView& left, const StringView& right);

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
EDOTOOLS_API std::ostream& operator<<(std::ostream& os, const StringView& str);

}  // namespace edoren

#ifdef EDOTOOLS_FMT_SUPPORT
// See https://fmt.dev/latest/api.html#formatting-user-defined-types

template <>
struct fmt::formatter<edoren::StringView> {
    constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin());

    template <typename FormatContext = fmt::format_context>
    auto format(const edoren::StringView& s, FormatContext& ctx) -> decltype(ctx.out());
};

#endif  // EDOTOOLS_FMT_SUPPORT

#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT

namespace nlohmann {

template <>
struct adl_serializer<edoren::StringView> {
    static void to_json(json& j, const edoren::StringView& s);
};

}  // namespace nlohmann

#endif  // EDOTOOLS_NLOHMANN_JSON_SUPPORT

#include "StringView.inl"
