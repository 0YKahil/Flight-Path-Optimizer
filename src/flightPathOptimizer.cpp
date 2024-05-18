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


int main() {
    std::ifstream file("testairports.json");
    nlohmann::json jsonData;

    std::cout << "reading file... ";
    file >> jsonData; // read the airports file into json object
    std::cout << "done" << std::endl;

    /**
     * CREATING AIRPORT GRAPH
     * using the json data provided from the given file
     */
    std::cout << "generating airport graph... ";

    Graph g((size_t)jsonData.size()); // numVertices here is the number of airports in airports.json

    /* Parse airports from json to Airport objects and add them to graph*/ 

    std::vector<Airport> airports; // array will hold our parsed airport objects

    for (const auto& item: jsonData) {
        // Ensuring that if the airport does not have a provided code, use the alternate,
        // and if also not provided, use the identifier
        // std::string identifier = item["icao"];
        // if (item["icao"] == "") {
        //     if (item["iata"] != "") identifier = item["iata"];
        //     else identifier = item["ident"];
        // }
        // create airport object using the data
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

    std::cout << "done" << std::endl;

    g.printGraph();

}