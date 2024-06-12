cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files for different executables
set(edgetests
    ${CMAKE_SOURCE_DIR}/src/Airport.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_main.cpp
)

set(airporttests
    ${CMAKE_SOURCE_DIR}/src/Airport.cpp
    ${CMAKE_SOURCE_DIR}/tests/airportTest.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_main.cpp
)

set(graphtests
    ${CMAKE_SOURCE_DIR}/src/Airport.cpp
    ${CMAKE_SOURCE_DIR}/src/Graph.cpp
    ${CMAKE_SOURCE_DIR}/tests/graphTest.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_main.cpp
)

set(main
    ${CMAKE_SOURCE_DIR}/src/Airport.cpp
    ${CMAKE_SOURCE_DIR}/src/Graph.cpp
    ${CMAKE_SOURCE_DIR}/src/utility_functions.cpp
    ${CMAKE_SOURCE_DIR}/src/FlightPathOptimizer.cpp
)

# Add executables
# add_executable(edgeTest ${edgetests})
add_executable(airportTest ${airporttests})
add_executable(graphTest ${graphtests})
add_executable(flightPathOptimizer ${main})
