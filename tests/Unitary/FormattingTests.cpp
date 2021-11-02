#include <catch2/catch.hpp>

#include <edoren/String.hpp>
#include <edoren/StringView.hpp>
#include <edoren/container/Vector.hpp>

using namespace edoren;

TEST_CASE("StringView", "[Formatting]") {
    // "水、火、地、風、空"
    StringView hello = "Hello World";

    SECTION("must be able to find any UTF-8 string") {
        String formatted = fmt::format("{}-{}", hello.subString(0, 5), 10);
        REQUIRE(formatted == "Hello-10");
    }
}

TEST_CASE("String", "[Formatting]") {
    // "水、火、地、風、空"
    String hello = "Hello World";

    SECTION("must be able to find any UTF-8 string") {
        String formatted = fmt::format("{}-{}", hello.subString(0, 5), 10);
        REQUIRE(formatted == "Hello-10");
    }
}

TEST_CASE("Vector", "[Formatting]") {
    Vector<String> vocabulary = {"A", "B", "C", "D", "E", "F", "G"};

    SECTION("must be able to find any UTF-8 string") {
        String formatted = fmt::format("{}", vocabulary);
        REQUIRE(formatted == "[A, B, C, D, E, F, G]");
    }
}
