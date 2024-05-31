/**
 * @file: graph.cpp
 * @author: 0Ykahil
 * 
 * Implementation of graph ADT
 */
#include "../include/Graph.h"


Graph::Graph(size_t numVertices)
    : numVertices(numVertices), adjList(numVertices) {}

void Graph::addVertex(const Airport& airport) {
    vertices.push_back(airport);

    // Map the vertex's id to the current index in vertices
    airportToIndex[airport.id] = vertices.size() - 1;
}

void Graph::addEdge(const Airport& source, const Airport& dest) {
    std::lock_guard<std::mutex> lock(mtx);

    // get source and destination indices
    size_t srcIdx = airportToIndex[source.id];
    size_t destIdx = airportToIndex[dest.id];

    // Create edge from source to dest
    int weight = source.distanceTo(dest);
    Edge edge(srcIdx, destIdx, weight);

    // add edge to the adjacency list mapped at its sources index
    adjList[srcIdx].push_back(edge);
    
    // adding the edges that go from dest to source (two-way)
    Edge reverseEdge(destIdx, srcIdx, weight);
    adjList[destIdx].push_back(reverseEdge);
}

void Graph::printGraph() const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        // output current airport id and name
        std::cout << "Airport " << vertices[i].id << " (" << vertices[i].name << ") -> ";
        // iterate over edges for the current airport
        for (const auto& edge : adjList[i]) {
            // output destination airport id and name as well as the edge
            std::cout << vertices[edge.dest].id << " (" << edge.weight << "), ";
        }
        std::cout << std::endl;
    }
    
}


void Graph::printGraph(std::ostream& os) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << "Airport " << vertices[i].id << " (" << vertices[i].name << ") -> ";
        for (const auto& edge : adjList[i]) {
            os << vertices[edge.dest].id << " (" << edge.weight << "), ";
        }
        os << std::endl;
    }
}

void Graph::generateAirportGraph(const nlohmann::json& jsonData, const int threshold, bool useMultithreading) {
    /* Parse airports from json to Airport objects and add them to graph*/ 
    this->numVertices = jsonData.size(); // numVertices = num airports in airports.json
    adjList.resize(numVertices); // create enough space for all our airports

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
        this->addVertex(airport);
    }

    /* Multithreaded version (STILL IN TESTING BUT SHOULD WORK) */
    if (useMultithreading == true) {
        auto addEdges = [&](size_t start, size_t end) {
            for (size_t i = start; i < end; ++i) {
                for (size_t j = i + 1; j < airports.size(); ++j) {
                    double distance = airports[i].distanceTo(airports[j]);

                    // ensure within THRESHOLD
                    if (distance <= threshold) {
                        this->addEdge(airports[i], airports[j]);
                    }
                }
            }
        };

        // Get number of available threads
        size_t numThreads = std::thread::hardware_concurrency(); 
        std::vector<std::thread> threads;
        size_t chunkSize = airports.size() / numThreads; // range of vertices each thread will be handling
        
        // Start the threads
        for (size_t t = 0; t < numThreads; ++t) {
            size_t start = t * chunkSize;
            size_t end = (t = numThreads - 1) ? airports.size() : start + chunkSize;
            threads.emplace_back(addEdges, start, end); // create thread
        }
        
        for (auto& thread: threads) {
            thread.join();
        }
    } else  {
        /** 
         * Iterating through pairs of airports and creating the
         * edges with distances as the weights.
         * 
         * SINGLE THREADED VERSION
         */ 
        for (size_t i = 0; i < airports.size(); ++i) {
            // start at i + 1 to prevent creating an edge to self
            for (size_t j = i + 1; j < airports.size(); ++j) {
                double distance = airports[i].distanceTo(airports[j]);

                // ensure distance is within THRESHOLD
                if (distance <= threshold) {
                    this->addEdge(airports[i], airports[j]);
                }
            }
        }
    }

}


std::pair<bool, std::vector<Airport>> Graph::findShortestPath(const Airport& start, const Airport& destination) const {
    return {false, {}};// stub
}


void Graph::toDOT(const std::string& filename) const {
    std::ofstream file(filename);
    file << "graph G {\n";
    for (size_t i = 0; i < adjList.size(); ++i) {
        for (const auto& edge : adjList[i]) {
            if (i < edge.dest) {
                file << "    \"" << vertices[i].id << "\" -- \""
                     << vertices[edge.dest].id << "\" [label=\"" << edge.weight << "\"];\n";
            }
        }
    }
    file << "}\n";
    file.close();
}