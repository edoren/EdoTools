#include <catch2/catch.hpp>

#include <edoren/String.hpp>
#include <edoren/StringView.hpp>
#include <edoren/container/Vector.hpp>

using namespace edoren;

TEST_CASE("StringView formatting", "[Formatting]") {
    StringView hello = "Hello World";

    SECTION("fmt::format can receive type") {
        String formatted = fmt::format("{}-{}", hello.subString(0, 5), 10);
        REQUIRE(formatted == "Hello-10");
    }
}

TEST_CASE("String formatting", "[Formatting]") {
    String hello = "Hello World";

    SECTION("fmt::format can receive type") {
        String formatted = fmt::format("{}-{}", hello.subString(0, 5), 10);
        REQUIRE(formatted == "Hello-10");
    }
    SECTION("_format string literal") {
        String formatted = "{}-{}"_format(hello.subString(0, 5), 10);
        REQUIRE(formatted == "Hello-10");
    }
}

TEST_CASE("Vector formatting", "[Formatting]") {
    Vector<String> vocabulary = {"A", "B", "C", "D", "E", "F", "G"};

    SECTION("fmt::format can receive type") {
        String formatted = fmt::format("{}", vocabulary);
        REQUIRE(formatted == "[A, B, C, D, E, F, G]");
    }
}
