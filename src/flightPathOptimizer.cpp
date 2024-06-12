/**
 * @file: flightPathOptimizer.cpp
 * @author: 0Ykahil
 * 
 * Handles JSON data parsing, implements edge calculations, and utilizes 
 * Dijkstra's algorithm to find the shortest path between a given start 
 * point and destination airport.
 */
#include <fstream>
#include <iostream>
#include <chrono>
#include "../include/Graph.h"


// Color Escape codes
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

/**
 * THRESHOLD will control the cutoff for the edge creation for the graph
 * and dictate when an edge will be made (distance between both locations <= THRESHOLD).
 *
 * NOTE: This will typically be what you will change depending on the aircraft's
 *       MAX range after calculating for fuel burning, wind, etc.
 */
const int THRESHOLD = 450;

int main() {
    // Read the json file into jsonData
    std::cout << "reading file... ";
    std::ifstream file("./datasets/airports.json");
    nlohmann::json jsonData;
    file >> jsonData; // read the airports file into json object
    std::cout << "done" << std::endl;

    // Creating AirportGraph

    std::cout << "generating airport graph... ";

    Graph g(jsonData.size());
    g.generateAirportGraph(jsonData, THRESHOLD, true);
    std::cout << "done" << std::endl;

    // Creating visual dot file of graph found in ./dot_files/
    g.toDOT("./dot_files/airports.dot");


    // g.printGraph();

    // std::vector<Airport> airports = g.getAirports();

    // std::cout << "\n\nAirport IDs available: \n";
    // for (auto ap : airports) {
    //     std::cout << ap.id << ", ";
    // }

    // Allow user to input through the console
    std::string id1;
    std::string id2;
    std::cout << "Enter the FULL identifier code for the STARTING airport (e.g. CYOW not YOW): \n> ";
    std::cin >> id1;
    std::cout << "Enter the FULL identifier code for the DESTINATION airport (e.g. CYOW not YOW): \n> ";
    std::cin >> id2;



    auto t1 = std::chrono::high_resolution_clock::now();

    // Display the Path
    std::cout << "\n\n" << BOLD << GREEN;
    std::cout << "RANGE: " << THRESHOLD << "nm \n";
    g.printShortestPath(id1, id2);   
    std::cout << RESET << std::endl; 


    auto t2 = std::chrono::high_resolution_clock::now();

    // Calculating time taken to finish
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << GREEN << "\ntime taken: " << ms_double.count() << "ms" << RESET << std::endl;
}