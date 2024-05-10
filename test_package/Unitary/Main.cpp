#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <catch2/catch.hpp>
#include <cctype>
#include <compare>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <utility>

using namespace std::literals;

void show_result(std::string_view s1, std::string_view s2, std::strong_ordering o) {
    std::cout << quoted(s1) << " is ";
    (o < 0) ? std::cout << "less than " : (o > 0) ? std::cout << "greater than " : std::cout << "equal to ";
    std::cout << quoted(s2) << '\n';
}

int malgoin() {
    auto cmp_icase = [](char x, char y) {
        const auto ux{std::toupper(x)};
        const auto uy{std::toupper(y)};
        return (ux < uy)   ? std::strong_ordering::less
               : (ux > uy) ? std::strong_ordering::greater
                           : std::strong_ordering::equal;
    };

    for (const auto& [s1, s2] : {std::pair{"one"sv, "ONE"sv}, {"two"sv, "four"sv}, {"three"sv, "two"sv}}) {
        const auto res =
            std::lexicographical_compare_three_way(s1.cbegin(), s1.cend(), s2.cbegin(), s2.cend(), cmp_icase);
        show_result(s1, s2, res);
    }
    return 0;
}
