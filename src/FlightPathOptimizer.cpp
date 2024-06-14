/**
 * @file: flightPathOptimizer.cpp
 * @author: 0Ykahil
 * 
 * Handles JSON data parsing, user inputs, outputs, and utilizes 
 * Dijkstra's algorithm to find the shortest path between a given start 
 * point and destination airport.
 */
#include <fstream>
#include <iostream>
#include <chrono>
#include "Graph.h"
#include "utility_functions.h"


/**
 * THRESHOLD will control the cutoff for the edge creation for the graph
 * and dictate when an edge will be made (distance between both locations <= THRESHOLD).
 *
 * NOTE: This will typically be what you will change depending on the aircraft's
 *       MAX range after calculating for fuel burning, wind, etc.
 */
int THRESHOLD = 0;


int main() {
    std::string filepath = "./datasets/airports.json";
    std::string fetchScriptpath = "./scripts/fetchAirportData.py";

    // Ask to update the json dataset
    askRunScript(fetchScriptpath, filepath);

    // Read the json file into jsonData
    std::cout << "reading file... ";
    std::ifstream file(filepath);
    nlohmann::json jsonData;
    file >> jsonData; // read the airports file into json object
    std::cout << "done\n" << std::endl;

    // // Prompt for aircraft range
    THRESHOLD = promptRange();

    /**
     * WRITE TO CONFIG STARTS HERE
     */
    Config config("Settings", "config.json");

    // writing
    config.write("user.range", "412");
    THRESHOLD = toInteger(config.read("user.range", "0"));

    std::cout << ++THRESHOLD << std::endl; 

    /**
     * **WRITE TO CONFIG ENDS**
     */



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

    // User input for starting and destination airport ids
    std::string lstartID, ldestID;

    // MAIN LOOP
    while (true) {

        std::cout << YELLOW << "\n\nEnter the FULL identifier code for the STARTING airport (e.g. CYOW not YOW) (or 'exit' to quit): \n> " << RESET;
        std::cin >> lstartID;

        std::string startID = toUpperCase(lstartID);
        if (startID == "EXIT") break;
        
        // Ensure id exists in graph
        if (!g.isValidAirport(startID)) {
            std::cout << RED << "'" << startID << "' IS NOT A VALID ID (did you forget to include the regional letter? e.g. 'K'JFK or 'C'YYZ)\n" << RESET;
            continue;
        }

        std::cout << YELLOW << "Enter the FULL identifier code for the DESTINATION airport: \n> " << RESET;
        std::cin >> ldestID;
        std::string destID = toUpperCase(ldestID);
        if (destID == "EXIT") break;

        if (!g.isValidAirport(destID)) {
            std::cout << "\033[31m" << "'" << destID << "' IS NOT A VALID ID (did you forget to include the regional letter? e.g. 'K'JFK or 'C'YYZ)\n" << RESET;
            continue;
        }


        auto t1 = std::chrono::high_resolution_clock::now();
        // Display the Path
        std::cout << "\n\n" << GREEN << "AIRCRAFT RANGE: " << THRESHOLD << "nm \n" << RESET;

        g.printShortestPath(startID, destID);
        std::cout << RESET << std::endl; 

        auto t2 = std::chrono::high_resolution_clock::now();

        // Calculating time taken to finish
        std::chrono::duration<double, std::milli> ms_double = t2 - t1;
        std::cout << GREEN << "\ntime taken: " << ms_double.count() << "ms" << RESET << std::endl;
        break;
    }


}
