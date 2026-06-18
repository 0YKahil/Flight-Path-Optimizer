cmake_minimum_required(VERSION 3.14)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are disabled. Use: cmake -S . -B build")
endif()

project(MyProject)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_Declare(
    Catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.10.tar.gz
)

FetchContent_MakeAvailable(nlohmann_json Catch2)

find_package(cpprestsdk REQUIRED)
find_package(OpenSSL REQUIRED)

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
    ${CMAKE_SOURCE_DIR}/src/utility_functions.cpp
    ${CMAKE_SOURCE_DIR}/tests/graphTest.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_main.cpp
)

set(utilitytests 
    ${CMAKE_SOURCE_DIR}/src/utility_functions.cpp
    ${CMAKE_SOURCE_DIR}/tests/utilityTest.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_main.cpp
)

set(main
    ${CMAKE_SOURCE_DIR}/src/Airport.cpp
    ${CMAKE_SOURCE_DIR}/src/Graph.cpp
    ${CMAKE_SOURCE_DIR}/src/utility_functions.cpp
    ${CMAKE_SOURCE_DIR}/src/FlightPathOptimizer.cpp
)

set(api_service
    ${CMAKE_SOURCE_DIR}/src/api_service.cpp
    ${CMAKE_SOURCE_DIR}/src/Airport.cpp
    ${CMAKE_SOURCE_DIR}/src/Graph.cpp
    ${CMAKE_SOURCE_DIR}/src/utility_functions.cpp
    ${CMAKE_SOURCE_DIR}/src/Logger.cpp
)

# Add executables
# add_executable(edgeTest ${edgetests})
# add_executable(airportTest ${airporttests})
add_executable(graphTest ${graphtests})
add_executable(flightPathOptimizer ${main})
add_executable(utilityTest ${utilitytests})

add_executable(api_service
    ${api_service}
)

target_link_libraries(api_service PRIVATE
    nlohmann_json::nlohmann_json
    cpprestsdk::cpprest
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_link_libraries(graphTest PRIVATE
    nlohmann_json::nlohmann_json
    Catch2::Catch2
)

target_link_libraries(flightPathOptimizer PRIVATE
    nlohmann_json::nlohmann_json
)

target_link_libraries(utilityTest PRIVATE
    nlohmann_json::nlohmann_json
    Catch2::Catch2
)
