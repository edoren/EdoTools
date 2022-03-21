#include <catch2/catch.hpp>

#include <edoren/String.hpp>
#include <edoren/container/Set.hpp>

using namespace edoren;

TEST_CASE("Set::Set", "[Set]") {
    Set<size_t> numbers = {1, 2, 3, 4};

    SECTION("Copy a Set") {}
}
