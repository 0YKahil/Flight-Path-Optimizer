/*
 * @file: edgeTest.cpp
 * @author: 0Ykahil
 * 
 * Tests for Edge class
 */
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "edge.h"

double yowLat = 45.3225;
double yowLon = -75.6692;
double jfkLat = 40.6413;
double jfkLon = -73.7781;
Airport yow = Airport("YOW", "Ottawa international", "large_airport", yowLat, yowLon);
Airport jfk = Airport("JFK", "John F Kennedy international", "large_airport", jfkLat, jfkLon);
Edge e1 = Edge(Vertex(yow), Vertex(jfk), yow.distanceTo(jfk), "edge1");

TEST_CASE("Test Constructor") {
    REQUIRE(e1.weight == 10);
}
