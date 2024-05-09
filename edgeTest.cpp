/*
 * @file: edgeTest.cpp
 * @author: 0Ykahil
 * 
 * Tests for Edge class
 */
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "edge.h"

Edge e1 = Edge(Vertex(1), Vertex(2), 10, "edge1");

TEST_CASE("Test Constructor") {
    REQUIRE(e1.weight == 10);
}
