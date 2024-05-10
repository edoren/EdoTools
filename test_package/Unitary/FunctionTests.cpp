#include <catch2/catch.hpp>

#include <array>

#include <edoren/Function.hpp>

using namespace edoren;

namespace {

int TestRawFunction() {
    return 2;
}

}  // namespace

TEST_CASE("Function::Function", "[Function]") {
    SECTION("Default constructor should create an empty function") {
        Function<int()> fn;
        REQUIRE(!fn);
    }
    SECTION("nullptr constructor should create an empty function") {
        Function<int()> fn(nullptr);
        REQUIRE(!fn);
    }
    SECTION("Constructor can receive lambda functions") {
        Function<int()> fn([]() { return 1; });
        REQUIRE(fn() == 1);
    }
    SECTION("Constructor can receive raw function pointers") {
        Function<int()> fn_raw(&TestRawFunction);
        REQUIRE(fn_raw() == 2);
    }
    SECTION("Constructor can specify the lambda capture list size") {
        std::array<int64_t, 100> values;
        values[0] = 10;
        values[50] = 20;
        values[values.size() - 1] = 30;
        Function<decltype(values)(), sizeof(values)> fn([values]() { return values; });
        REQUIRE(fn()[0] == values[0]);
        REQUIRE(fn()[50] == values[50]);
        REQUIRE(fn()[values.size() - 1] == values[values.size() - 1]);
    }
    SECTION("Copy constructor should copy the underlying function") {
        Function<int()> fn([]() { return 1; });
        Function<int()> fn_raw(&TestRawFunction);
        Function<int()> fn_new(fn);
        Function<int()> fn_raw_new(fn_raw);
        REQUIRE(fn() == 1);
        REQUIRE(fn_raw() == 2);
        REQUIRE(fn_new() == 1);
        REQUIRE(fn_raw_new() == 2);
    }
    SECTION("Move constructor should move the underlying function") {
        Function<int()> fn([]() { return 1; });
        Function<int()> fn_raw(&TestRawFunction);
        Function<int()> fn_new(std::move(fn));
        Function<int()> fn_raw_new(std::move(fn_raw));
        REQUIRE(!fn);
        REQUIRE(!fn_raw);
        REQUIRE(fn_new);
        REQUIRE(fn_raw_new);
        REQUIRE(fn_new() == 1);
        REQUIRE(fn_raw_new() == 2);
    }
}

TEST_CASE("Function::operator=", "[Function]") {
    SECTION("Copy assignment operator should copy the underlying function") {
        Function<int()> fn([]() { return 1; });
        Function<int()> fn_raw(&TestRawFunction);
        Function<int()> fn_new;
        Function<int()> fn_raw_new;
        fn_new = fn;
        fn_raw_new = fn_raw;
        REQUIRE(fn() == 1);
        REQUIRE(fn_raw() == 2);
        REQUIRE(fn_new() == 1);
        REQUIRE(fn_raw_new() == 2);
    }
    SECTION("Move assignment operator should move the underlying function") {
        Function<int()> fn([]() { return 1; });
        Function<int()> fn_raw(&TestRawFunction);
        Function<int()> fn_new;
        Function<int()> fn_raw_new;
        fn_new = std::move(fn);
        fn_raw_new = std::move(fn_raw);
        REQUIRE(!fn);
        REQUIRE(!fn_raw);
        REQUIRE(fn_new() == 1);
        REQUIRE(fn_raw_new() == 2);
    }
    SECTION("nullptr assignment operator should delete the underlying function") {
        Function<int()> fn([]() { return 1; });
        fn = nullptr;
        REQUIRE(!fn);
    }
}

TEST_CASE("Function::operator()", "[Function]") {
    SECTION("Function-call operator should call the underlying function") {
        Function<int()> fn([]() { return 1; });
        REQUIRE(fn() == 1);
    }
    SECTION("Function-call operator should support multiple parameters") {
        Function<int(int, int, int)> sum([](int a, int b, int c) { return a + b + c; });
        REQUIRE(sum(2, 5, 1) == 8);
    }
}
