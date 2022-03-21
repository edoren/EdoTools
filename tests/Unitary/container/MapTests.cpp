#include <catch2/catch.hpp>

#include <edoren/StringView.hpp>
#include <edoren/container/Map.hpp>

using namespace edoren;

TEST_CASE("Map::Map", "[Map]") {
    Map<size_t, StringView> numbers = {{1, "H"}, {2, "O"}, {3, "L"}, {4, "A"}};

    SECTION("Copy a Map") {}
}
