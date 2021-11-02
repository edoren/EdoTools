#include <catch2/catch.hpp>

#include <edoren/String.hpp>
#include <edoren/StringView.hpp>
#include <edoren/container/Vector.hpp>

using namespace edoren;

TEST_CASE("String JSON", "[JSON]") {
    String hello = "Hello World";

    SECTION("Serialize to nlohmann::json") {
        nlohmann::json j = hello;
        REQUIRE(j == "Hello World");
    }
}

TEST_CASE("StringView JSON", "[JSON]") {
    StringView hello = "Hello World";

    SECTION("Serialize to nlohmann::json") {
        nlohmann::json j = hello;
        REQUIRE(hello == "Hello World");
    }
}

TEST_CASE("Vector JSON", "[JSON]") {
    Vector<String> hello = {"Hello", "World"};

    SECTION("Serialize to nlohmann::json") {
        nlohmann::json j = hello;
        REQUIRE(j[0] == "Hello");
        REQUIRE(j[1] == "World");
    }
}
