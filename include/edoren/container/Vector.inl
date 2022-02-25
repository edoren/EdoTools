#pragma once

#include <stdexcept>

namespace edoren {

template <typename T>
template <typename Func>
auto Vector<T>::map(Func transform) const -> Vector<std::invoke_result_t<Func, const T&>> {
    Vector<std::invoke_result_t<Func, const T&>> new_vec;
    new_vec.reserve(this->size());
    for (const auto& element : *this) {
        new_vec.push_back(transform(element));
    }
    return new_vec;
}

template <typename T>
template <typename Func>
auto Vector<T>::mapIndexed(Func transform) const -> Vector<std::invoke_result_t<Func, size_t, const T&>> {
    Vector<std::invoke_result_t<Func, size_t, const T&>> new_vec;
    new_vec.reserve(this->size());
    for (size_t i = 0; i < this->size(); i++) {
        new_vec.push_back(transform(i, this->operator[](i)));
    }
    return new_vec;
}

template <typename T>
template <typename Func>
auto Vector<T>::filter(Func predicate) const -> Vector<T> {
    static_assert(std::is_convertible_v<std::invoke_result_t<Func, const T&>, bool>,
                  "The predicate return type should be bool");
    Vector<T> new_vec;
    for (const auto& element : *this) {
        if (predicate(element)) {
            new_vec.push_back(element);
        }
    }
    return new_vec;
}

template <typename T>
template <typename Func>
auto Vector<T>::filterIndexed(Func predicate) const -> Vector<T> {
    static_assert(std::is_convertible_v<std::invoke_result_t<Func, const T&>, bool>,
                  "The predicate return type should be bool");
    Vector<T> new_vec;
    for (decltype(this->size()) i = 0; i < this->size(); i++) {
        if (predicate(i, this->operator[](i))) {
            new_vec.push_back(this->operator[](i));
        }
    }
    return new_vec;
}

template <typename T>
template <typename Func>
auto Vector<T>::find(Func predicate) -> T* {
    static_assert(std::is_convertible_v<std::invoke_result_t<Func, const T&>, bool>,
                  "The predicate return type should be bool");
    auto it = std::find_if(this->cbegin(), this->cend(), predicate);
    if (it != this->cend()) {
        return *it;
    }
    return nullptr;
}

template <typename T>
template <typename Func>
auto Vector<T>::find(Func predicate) const -> const T* {
    static_assert(std::is_convertible_v<std::invoke_result_t<Func, const T&>, bool>,
                  "The predicate return type should be bool");
    return const_cast<const T*>(const_cast<std::remove_const_t<decltype(*this)>>(*this).find(predicate));
}

template <typename T>
template <typename Func>
auto Vector<T>::forEach(Func predicate) {
    for (auto& element : *this) {
        predicate(element);
    }
}

template <typename T>
template <typename Func>
auto Vector<T>::forEach(Func predicate) const {
    for (const auto& element : *this) {
        predicate(element);
    }
}

template <typename T>
template <typename Func>
auto Vector<T>::forEachIndexed(Func predicate) {
    for (decltype(this->size()) i = 0; i < this->size(); i++) {
        predicate(i, this->operator[](i));
    }
}

template <typename T>
template <typename Func>
auto Vector<T>::forEachIndexed(Func predicate) const {
    for (decltype(this->size()) i = 0; i < this->size(); i++) {
        predicate(i, this->operator[](i));
    }
}

template <typename T>
auto Vector<T>::first() -> T& {
    if (this->size() > 0) {
        return this->operator[](0);
    }
    EDOTOOLS_THROW(std::out_of_range("No such element in Vector"));
}

template <typename T>
auto Vector<T>::first() const -> const T& {
    return const_cast<const T&>(static_cast<std::remove_const_t<decltype(*this)>>(*this).first());
}

template <typename T>
template <typename Func>
auto Vector<T>::first(Func predicate) -> T& {
    for (auto& element : *this) {
        if (predicate(const_cast<std::add_const_t<decltype(element)>>(element))) {
            return element;
        }
    }
    EDOTOOLS_THROW(std::out_of_range("No such element in Vector"));
}

template <typename T>
template <typename Func>
auto Vector<T>::first(Func predicate) const -> const T& {
    return const_cast<const T&>(static_cast<std::remove_const_t<decltype(*this)>>(*this).first(predicate));
}

template <typename T>
auto Vector<T>::firstOrNull() -> T* {
    if (this->size() > 0) {
        return &(this->operator[](0));
    }
    return nullptr;
}

template <typename T>
auto Vector<T>::firstOrNull() const -> const T* {
    return const_cast<const T*>(const_cast<std::remove_const_t<decltype(*this)>>(*this).firstOrNull());
}

template <typename T>
template <typename Func>
auto Vector<T>::firstOrNull(Func predicate) -> T* {
    for (auto& element : *this) {
        if (predicate(const_cast<std::add_const_t<decltype(element)>>(element))) {
            return &element;
        }
    }
    return nullptr;
}

template <typename T>
template <typename Func>
auto Vector<T>::firstOrNull(Func predicate) const -> const T* {
    return const_cast<const T*>(const_cast<std::remove_const_t<decltype(*this)>>(*this).firstOrNull(predicate));
}

}  // namespace edoren

#ifdef EDOTOOLS_FMT_SUPPORT

template <typename T>
constexpr auto fmt::formatter<edoren::Vector<T>>::parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
    return ctx.begin();
}

template <typename T>
template <typename FormatContext>
auto fmt::formatter<edoren::Vector<T>>::format(const edoren::Vector<T>& v, FormatContext& ctx) -> decltype(ctx.out()) {
    return format_to(ctx.out(), "[{}]", join(v, ", "));
}

#endif  // EDOTOOLS_FMT_SUPPORT

#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT

namespace nlohmann {

template <typename T>
void adl_serializer<edoren::Vector<T>>::to_json(json& j, const edoren::Vector<T>& v) {
    j = json::array();
    for (auto& e : v) {
        j.push_back(e);
    }
}

template <typename T>
void adl_serializer<edoren::Vector<T>>::from_json(const json& j, edoren::Vector<T>& v) {
    if (j.is_array()) {
        v.clear();
        v.reserve(j.size());
        for (auto& e : j) {
            T val = e;
            v.push_back(std::move(val));
        }
    }
}

}  // namespace nlohmann

#endif  // EDOTOOLS_NLOHMANN_JSON_SUPPORT
