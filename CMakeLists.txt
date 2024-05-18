cmake_minimum_required(VERSION 3.10)
project(FlightPathOptimizer)



set(edgetests
    "./src/Edge.cpp"
    "./tests/edgeTest.cpp"
    "./src/Airport.cpp"
)

set(airporttests
    "./src/Airport.cpp"
    "./tests/airportTest.cpp"
)

add_executable(edgeTest ${edgetests})
add_executable(airportTest ${airporttests})