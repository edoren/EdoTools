#include <catch2/catch.hpp>

#include <edoren/String.hpp>
#include <edoren/container/Vector.hpp>

using namespace edoren;

TEST_CASE("Vector::Vector", "[Vector]") {
    Vector<size_t> numbers = {1, 2, 3, 4};

    SECTION("Copy a vector") {}
}

TEST_CASE("Vector::map", "[Vector]") {
    Vector<size_t> numbers = {1, 2, 3, 4};

    SECTION("Create a Vector from another") {
        Vector<String> numbers_str = numbers.map([](const size_t& num) { return "{}"_format(num); });
        REQUIRE(numbers_str[0] == "1");
        REQUIRE(numbers_str[1] == "2");
        REQUIRE(numbers_str[2] == "3");
        REQUIRE(numbers_str[3] == "4");
    }
}

TEST_CASE("Vector::mapIndexed", "[Vector]") {
    Vector<size_t> numbers = {1, 2, 3, 4};

    SECTION("Create a Vector from another") {
        Vector<String> numbers_str =
            numbers.mapIndexed([](size_t index, const size_t& num) { return "{}-{}"_format(num, index); });
        REQUIRE(numbers_str[0] == "1-0");
        REQUIRE(numbers_str[1] == "2-1");
        REQUIRE(numbers_str[2] == "3-2");
        REQUIRE(numbers_str[3] == "4-3");
    }
}

TEST_CASE("Vector::filter", "[Vector]") {
    Vector<size_t> numbers = {1, 2, 3, 4};

    SECTION("Create a Vector from another") {
        Vector<size_t> odd_numbers = numbers.filter([](const size_t& num) { return num & 1; });
        REQUIRE(odd_numbers[0] == 1);
        REQUIRE(odd_numbers[1] == 3);
    }
}

// TEST_CASE("Vector::filterIndexed", "[Vector]") {}
// TEST_CASE("Vector::find", "[Vector]") {}
// TEST_CASE("Vector::forEach", "[Vector]") {}
// TEST_CASE("Vector::forEachIndexed", "[Vector]") {}
// TEST_CASE("Vector::first", "[Vector]") {}
// TEST_CASE("Vector::firstOrNull", "[Vector]") {}
