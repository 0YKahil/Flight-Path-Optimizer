/*
 * @file: airportTest.cpp
 * @author: 0Ykahil
 * 
 * Tests for Edge class
 */
#define CATCH_CONFIG_MAIN
#include <iostream>
#include "catch.hpp"
#include "Airport.h"

TEST_CASE("Test Haversine") {
    double yowLat = 45.3225;
    double yowLon = -75.6692;
    double jfkLat = 40.6413;
    double jfkLon = -73.7781;

    double mexLat = 19.4363;
    double mexLon = -99.072098;

    double distance1 = haversine(yowLat, yowLon, jfkLat, jfkLon);
    double distance2 = haversine(yowLat, yowLon, mexLat, mexLon);
    // Distance should be between 280nm and 300nm
    REQUIRE(distance1 < 300);
    REQUIRE(distance1 > 280);
    std::cout << "distance is around: " << distance1 << std::endl;

    REQUIRE(distance2 > 1930);
    REQUIRE(distance2 < 1941);
    std::cout << "distance is around: " << distance2 << std::endl;
}
