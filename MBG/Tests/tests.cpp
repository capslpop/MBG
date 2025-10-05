#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <iostream>

TEST_CASE("Simple Triangle") {

    std::cout << "Hello TESTS!" << std::endl;

    REQUIRE(1 + 1 == 2);
}

TEST_CASE("Simple Window") {

    std::cout << "Hello Window!" << std::endl;

    REQUIRE(1 + 1 == 2);
}