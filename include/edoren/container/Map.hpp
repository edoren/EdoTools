#pragma once

#include <optional>
#include <map>

namespace edoren {

template <typename Key, typename T>
class Map : public std::pmr::map<Key, T> {
public:
    using std::pmr::map<Key, T>::map;
    using std::pmr::map<Key, T>::operator=;
    using std::pmr::map<Key, T>::operator[];

    // C++ std::map alias
    CREATE_MEMBER_FUNCTION_ALIAS(at, getAt);

    CREATE_MEMBER_FUNCTION_ALIAS(empty, isEmpty);
    CREATE_CONST_MEMBER_FUNCTION_ALIAS_NOARGS(size, getSize);
    CREATE_CONST_MEMBER_FUNCTION_ALIAS_NOARGS(max_size, getMaxSize);

    CREATE_MEMBER_FUNCTION_ALIAS(insert_or_assign, insertOrAssign);
    CREATE_MEMBER_FUNCTION_ALIAS(emplace_hint, emplaceHint);
    CREATE_MEMBER_FUNCTION_ALIAS(try_emplace, tryEmplace);

    CREATE_MEMBER_FUNCTION_ALIAS(count, getCount);
    CREATE_MEMBER_FUNCTION_ALIAS(equal_range, equalRange);
    CREATE_MEMBER_FUNCTION_ALIAS(lower_bound, lowerBound);
    CREATE_MEMBER_FUNCTION_ALIAS(upper_bound, upperBound);
};

}  // namespace edoren

#include "Map.inl"
