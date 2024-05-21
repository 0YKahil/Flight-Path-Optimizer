/**
 * @file: airportTest.cpp
 * @author: 0Ykahil
 * 
 * Tests for Airport class
 */
#define CATCH_CONFIG_MAIN
#include <iostream>
#include "catch.hpp"
#include "../include/Airport.h"

double yowLat = 45.3225;
double yowLon = -75.6692;
double jfkLat = 40.6413;
double jfkLon = -73.7781;


TEST_CASE("Test Constructor") {
    Airport a1 = Airport("YYK", "Vancouver International", "large_airport", -30.14, 20.15);
    REQUIRE(a1.latitude == -30.14);
    REQUIRE(a1.name == "Vancouver International");
    REQUIRE(a1.id == "YYK");
}


TEST_CASE("Test Haversine") {
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


TEST_CASE("Test distanceTo method") {
    Airport yow = Airport("YOW", "Ottawa international", "large_airport", yowLat, yowLon);
    Airport jfk = Airport("JFK", "John F Kennedy international", "large_airport", jfkLat, jfkLon);
    REQUIRE(jfk.distanceTo(yow) == yow.distanceTo(jfk));
    REQUIRE(yow.distanceTo(jfk) > 290);
    REQUIRE(yow.distanceTo(jfk) < 300);
}

TEST_CASE("Test comparing operator") {
    Airport yow1 = Airport("YOW", "Ottawa Macdonald international", "large_airport", yowLat, yowLon);
    Airport yow = Airport("YOW", "Ottawa international", "large_airport", yowLat, yowLon);
    Airport jfk = Airport("JFK", "John F Kennedy international", "large_airport", jfkLat, jfkLon);
    REQUIRE_FALSE(yow == jfk);
    REQUIRE(yow1 == yow);
}