#pragma once

#include <optional>
#include <stdexcept>

namespace edoren {

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::map(Func transform) const -> Vector<std::invoke_result_t<decltype(transform), const T&>> {
    Vector<std::invoke_result_t<decltype(transform), const T&>> new_vec;
    new_vec.reserve(this->size());
    for (const auto& element : *this) {
        new_vec.push_back(transform(element));
    }
    return new_vec;
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::mapIndexed(Func transform) const
    -> Vector<std::invoke_result_t<decltype(transform), size_t, const T&>> {
    Vector<std::invoke_result_t<decltype(transform), size_t, const T&>> new_vec;
    new_vec.reserve(this->size());
    for (decltype(this->size()) i = 0; i < this->size(); i++) {
        new_vec.push_back(transform(i, this->operator[](i)));
    }
    return new_vec;
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::filter(Func predicate) const -> Vector<T> {
    Vector<T> new_vec;
    for (const auto& element : *this) {
        if (predicate(element)) {
            new_vec.push_back(element);
        }
    }
    return new_vec;
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::filterIndexed(Func predicate) const -> Vector<T> {
    Vector<T> new_vec;
    for (decltype(this->size()) i = 0; i < this->size(); i++) {
        if (predicate(i, this->operator[](i))) {
            new_vec.push_back(this->operator[](i));
        }
    }
    return new_vec;
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::find(Func predicate) -> T* {
    auto it = std::find_if(this->cbegin(), this->cend(), predicate);
    if (it != this->cend()) {
        return *it;
    }
    return nullptr;
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::find(Func predicate) const -> const T* {
    return const_cast<const T*>(const_cast<std::remove_const_t<decltype(*this)>>(*this).find(predicate));
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::forEach(Func predicate) {
    for (auto& element : *this) {
        predicate(element);
    }
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::forEach(Func predicate) const {
    for (const auto& element : *this) {
        predicate(element);
    }
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::forEachIndexed(Func predicate) {
    for (decltype(this->size()) i = 0; i < this->size(); i++) {
        predicate(i, this->operator[](i));
    }
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::forEachIndexed(Func predicate) const {
    for (decltype(this->size()) i = 0; i < this->size(); i++) {
        predicate(i, this->operator[](i));
    }
}

template <typename T, typename Allocator>
auto Vector<T, Allocator>::first() -> T& {
    if (this->size() > 0) {
        return this->operator[](0);
    }
    EDOTOOLS_THROW(std::out_of_range("No such element in Vector"));
}

template <typename T, typename Allocator>
auto Vector<T, Allocator>::first() const -> const T& {
    return const_cast<const T&>(static_cast<std::remove_const_t<decltype(*this)>>(*this).first());
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::first(Func predicate) -> T& {
    for (auto& element : *this) {
        if (predicate(const_cast<std::add_const_t<decltype(element)>>(element))) {
            return element;
        }
    }
    EDOTOOLS_THROW(std::out_of_range("No such element in Vector"));
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::first(Func predicate) const -> const T& {
    return const_cast<const T&>(static_cast<std::remove_const_t<decltype(*this)>>(*this).first(predicate));
}

template <typename T, typename Allocator>
auto Vector<T, Allocator>::firstOrNull() -> T* {
    if (this->size() > 0) {
        return &(this->operator[](0));
    }
    return nullptr;
}

template <typename T, typename Allocator>
auto Vector<T, Allocator>::firstOrNull() const -> const T* {
    return const_cast<const T*>(const_cast<std::remove_const_t<decltype(*this)>>(*this).firstOrNull());
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::firstOrNull(Func predicate) -> T* {
    for (auto& element : *this) {
        if (predicate(const_cast<std::add_const_t<decltype(element)>>(element))) {
            return &element;
        }
    }
    return nullptr;
}

template <typename T, typename Allocator>
template <typename Func>
auto Vector<T, Allocator>::firstOrNull(Func predicate) const -> const T* {
    return const_cast<const T*>(const_cast<std::remove_const_t<decltype(*this)>>(*this).firstOrNull(predicate));
}

}  // namespace edoren
