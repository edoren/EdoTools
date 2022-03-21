#pragma once

#include <list>

namespace edoren {

template <typename T>
class List : public std::pmr::list<T> {
public:
    using std::pmr::list<T>::list;
    using std::pmr::list<T>::operator=;

    // C++ std::list alias
    CREATE_MEMBER_FUNCTION_ALIAS(front, getFront);
    CREATE_MEMBER_FUNCTION_ALIAS(back, getBack);

    CREATE_MEMBER_FUNCTION_ALIAS(empty, getEmpty);
    CREATE_MEMBER_FUNCTION_ALIAS(size, getSize);
    CREATE_MEMBER_FUNCTION_ALIAS(max_size, getMaxSize);

    CREATE_MEMBER_FUNCTION_ALIAS(push_back, pushBack);
    CREATE_MEMBER_FUNCTION_ALIAS(emplace_back, emplaceBack);
    CREATE_MEMBER_FUNCTION_ALIAS(pop_back, popBack);
    CREATE_MEMBER_FUNCTION_ALIAS(push_front, pushFront);
    CREATE_MEMBER_FUNCTION_ALIAS(emplace_front, emplaceFront);
    CREATE_MEMBER_FUNCTION_ALIAS(pop_front, popFront);

    CREATE_MEMBER_FUNCTION_ALIAS(remove_if, removeIf);
};

}  // namespace edoren

#include "List.inl"
