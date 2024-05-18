/**
 * @file: edgeTest.cpp
 * @author: 0Ykahil
 * 
 * Tests for Edge class
 */
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/Edge.h"


Airport yow("YOW", "Ottawa", "medium_airport", 45.3225, -75.6692);
Airport jfk("JFK", "New York", "large_airport", 40.6413, -73.7781);
Airport lax("LAX", "Los Angeles", "large_airport", 33.9416, -118.4085);

Edge e1(yow, jfk);
Edge e1_1(jfk, yow);
Edge e1_2(jfk, yow);
Edge e2(jfk, lax);
Edge e3(yow, lax);

TEST_CASE("Test Constructor") {
    // YOW -> JFK = ~293
    REQUIRE(e1.weight > 290);
    REQUIRE(e1.weight < 297);

    // testing weight of flipped
    REQUIRE(e1.weight == e1_1.weight);
    REQUIRE(e1.weight == e1_2.weight);

    // JFK -> LAX = ~2151
    REQUIRE(e2.weight > 2145);
    REQUIRE(e2.weight < 2155);

    // YOW -> LAX = ~ 2068
    REQUIRE(e3.weight > 2063);
    REQUIRE(e3.weight < 2072);
    
}

TEST_CASE("Test < operator") {
    REQUIRE(e1 < e2);
    REQUIRE(e3 < e2);
}

TEST_CASE("Test == operator") {
    REQUIRE(e1 == e1_1);
    REQUIRE(e1 == e1_2);
    REQUIRE_FALSE(e3 == e2);
}