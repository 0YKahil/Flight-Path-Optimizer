cmake_minimum_required(VERSION 3.10)
project(FlightPathOptimizer)

set(headers 
    "edge.h"
    "graph.h"
)

set(sources 
    "graph.cpp"
    "flightPathOptimizer.cpp"
)

set(edgetests
    "edge.h"
    "edgeTest.cpp"
    "Airport.cpp"
)

set(airporttests
    "Airport.cpp"
    "airportTest.cpp"
)

add_executable(edgeTest ${edgetests})
add_executable(airportTest ${airporttests})
