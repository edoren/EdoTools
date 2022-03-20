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
    CREATE_MEMBER_FUNCTION_ALIAS(front, getFront);
    CREATE_MEMBER_FUNCTION_ALIAS(back, getBack);
    CREATE_MEMBER_FUNCTION_ALIAS(data, getData);

    CREATE_MEMBER_FUNCTION_ALIAS(empty, getEmpty);
    CREATE_MEMBER_FUNCTION_ALIAS(size, getSize);
    CREATE_MEMBER_FUNCTION_ALIAS(max_size, getMaxSize);
    CREATE_MEMBER_FUNCTION_ALIAS(shrink_to_fit, shrinkToFit);

    CREATE_MEMBER_FUNCTION_ALIAS(push_back, pushBack);
    CREATE_MEMBER_FUNCTION_ALIAS(emplace_back, emplaceBack);
    CREATE_MEMBER_FUNCTION_ALIAS(pop_back, popBack);
};

}  // namespace edoren

#include "Map.inl"
