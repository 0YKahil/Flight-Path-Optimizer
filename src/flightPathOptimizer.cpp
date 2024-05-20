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
#include "../external/json.hpp" 
#include "../include/Graph.h"

/**
 * THRESHOLD will control the cutoff for the edge creation for the graph
 * and dictate when an edge will be made (distance between both locations <= THRESHOLD).
 *
 * NOTE: This will typically be what you will change depending on the aircraft's
 *       MAX range after calculating for fuel burning, wind, etc.
 */
const int THRESHOLD = 250;

/**
 * Generates an airport graph with airports as the vertices and distances as the edges 
 * represented by an adjacency list, ONLY creating edges where the **distance < THRESHOLD**.
 * 
 * @param jsonData The JsonData containing airport identifier, name, location, etc. data to be parsed
*/
Graph generateAirportGraph(nlohmann::json jsonData) {
    /* Parse airports from json to Airport objects and add them to graph*/ 

    Graph g((size_t)jsonData.size()); // numVertices = num airports in airports.json

    std::vector<Airport> airports; // array will hold our parsed airport objects

    for (const auto& item: jsonData) {
        Airport airport(
            item["ident"],
            item["name"],
            item["type"],
            // convert lon and lat from string to double
            std::stod(item["latitude"].get<std::string>()),
            std::stod(item["longitude"].get<std::string>())
        );

        airports.push_back(airport);

        // add the airport as a vertex to our graph
        g.addVertex(airport);
    }

    /* Iterating through pairs of airports and creating the
     * edges with distances as the weights.
     */ 
    for (size_t i = 0; i < airports.size(); ++i) {
        // start at i + 1 to prevent creating an edge to self
        for (size_t j = i + 1; j < airports.size(); ++j) {
            double distance = airports[i].distanceTo(airports[j]);

            // ensure distance is within THRESHOLD
            if (distance <= THRESHOLD) {
                g.addEdge(airports[i], airports[j]);
            }
        }
    }

    return g;
}


int main() {
    std::ifstream file("testairports.json");
    nlohmann::json jsonData;

    std::cout << "reading file... ";
    file >> jsonData; // read the airports file into json object
    std::cout << "done" << std::endl;

    // Creating AirportGraph

    std::cout << "generating airport graph... ";

    Graph g = generateAirportGraph(jsonData);

    std::cout << "done" << std::endl;

    g.printGraph();

}