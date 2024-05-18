#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/Graph.h"
#include "../include/Airport.h"

TEST_CASE("Graph, vertices, and edges") {
    size_t numAirports = 3;

    Airport a1("YOW", "Ottawa", "medium_airport", 45.3225, -75.6692);
    Airport a2("JFK", "New York", "large_airport", 40.6413, -73.7781);
    Airport a3("LAX", "Los Angeles", "large_airport", 33.9416, -118.4085);

    Graph g(numAirports);
    g.addVertex(a1);
    g.addVertex(a2);
    g.addVertex(a3);

    g.addEdge(a1, a2);
    g.addEdge(a2, a3);
    g.addEdge(a1, a3);

    std::ostringstream outstream;
    g.printGraph(outstream);

    std::string expected =
        "Airport YOW (Ottawa) -> JFK (293), LAX (2063), \n"
        "Airport JFK (New York) -> YOW (293), LAX (2145), \n"
        "Airport LAX (Los Angeles) -> JFK (2145), YOW (2063), \n";

    REQUIRE(outstream.str() == expected);
}