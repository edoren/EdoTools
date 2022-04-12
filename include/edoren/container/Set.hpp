#pragma once

#include <set>

namespace edoren {

template <typename T>
class Set : public std::pmr::set<T> {
public:
    using std::pmr::set<T>::set;
    using std::pmr::set<T>::operator=;

    // C++ std::set alias
    CREATE_MEMBER_FUNCTION_ALIAS(empty, isEmpty);
    CREATE_CONST_MEMBER_FUNCTION_ALIAS_NOARGS(size, getSize);
    CREATE_CONST_MEMBER_FUNCTION_ALIAS_NOARGS(max_size, getMaxSize);

    CREATE_MEMBER_FUNCTION_ALIAS(emplace_hint, emplaceHint);

    CREATE_MEMBER_FUNCTION_ALIAS(equal_range, equalRange);
    CREATE_MEMBER_FUNCTION_ALIAS(lower_bound, lowerBound);
    CREATE_MEMBER_FUNCTION_ALIAS(upper_bound, upperBound);
};

}  // namespace edoren

#include "Set.inl"
