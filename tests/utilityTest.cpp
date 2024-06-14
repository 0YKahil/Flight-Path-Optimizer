/**
 * @file: utilityTest.cpp
 * @author: 0Ykahil
 * 
 * Tests for utility functions and Config class
 */
#include <iostream>
#include "catch.hpp"
#include "utility_functions.h"

TEST_CASE("Test isInteger") {
    REQUIRE_FALSE(isInteger(""));
    REQUIRE_FALSE(isInteger(" "));
    REQUIRE_FALSE(isInteger("Hello"));
    REQUIRE_FALSE(isInteger("500."));
    REQUIRE_FALSE(isInteger("50 0"));
    REQUIRE_FALSE(isInteger("500") == false);
}

TEST_CASE("Test toInteger") {
    REQUIRE(toInteger("") == -1);
    REQUIRE(toInteger(" ") == -1);
    REQUIRE(toInteger("Hello") == -1);
    REQUIRE(toInteger("500.") == -1);
    REQUIRE(toInteger("50 0") == -1);
    REQUIRE(toInteger("500") == 500);
}

TEST_CASE("Test toUpperCase") {
    std::string s = "hello world";
    REQUIRE(toUpperCase("") == "");
    REQUIRE(toUpperCase(" ") == " ");
    REQUIRE(toUpperCase("name") == "NAME");
    REQUIRE(toUpperCase(s) == "HELLO WORLD");
}