#include <catch2/catch.hpp>

#include <edoren/String.hpp>
#include <edoren/container/List.hpp>

using namespace edoren;

TEST_CASE("List::List", "[List]") {
    List<size_t> numbers = {1, 2, 3, 4};

    SECTION("Copy a List") {}
}
