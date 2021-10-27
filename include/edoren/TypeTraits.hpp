#pragma once

#include <type_traits>

#include <edoren/Config.hpp>

namespace edoren {

namespace type {

template <typename T>
using iterator_underlying_type = std::decay<decltype(*(std::declval<T>()))>;

template <typename T>
using iterator_underlying_type_t = typename iterator_underlying_type<T>::type;

template <typename T>
struct EDOREN_API is_bidirectional_iterator
      : public std::integral_constant<bool,
                                      (std::is_arithmetic<iterator_underlying_type_t<T>>::value ||
                                       std::is_compound<iterator_underlying_type_t<T>>::value) &&
                                          std::is_same<decltype(++(std::declval<T&>())), T&>::value &&
                                          std::is_same<decltype(--(std::declval<T&>())), T&>::value> {};

template <typename T>
struct EDOREN_API is_forward_iterator
      : public std::integral_constant<bool,
                                      (std::is_arithmetic<iterator_underlying_type_t<T>>::value ||
                                       std::is_compound<iterator_underlying_type_t<T>>::value) &&
                                          std::is_same<decltype(++(std::declval<T&>())), T&>::value> {};

template <typename T>
inline constexpr bool is_bidirectional_iterator_v = is_bidirectional_iterator<T>::value;  // NOLINT

template <typename T>
inline constexpr bool is_forward_iterator_v = is_forward_iterator<T>::value;  // NOLINT

template <typename T>
struct EDOREN_API alignment_of : std::integral_constant<size_t, alignof(T)> {};

template <typename T>
struct EDOREN_API size_of : std::integral_constant<size_t, sizeof(T)> {};

template <typename T>
inline constexpr bool alignment_of_v = alignment_of<T>::value;  // NOLINT

template <typename T>
inline constexpr bool size_of_v = size_of<T>::value;  // NOLINT

}  // namespace type

}  // namespace edoren
