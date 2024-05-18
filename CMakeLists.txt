cmake_minimum_required(VERSION 3.10)
project(FlightPathOptimizer)



set(edgetests
    "./src/Airport.cpp"
)

set(airporttests
    "./src/Airport.cpp"
    "./tests/airportTest.cpp"
)

set(graphtests
    "./src/Airport.cpp"
    "./src/Graph.cpp"
    "./tests/graphTest.cpp"

)

# add_executable(edgeTest ${edgetests})
add_executable(airportTest ${airporttests})
add_executable(graphTest ${graphtests})