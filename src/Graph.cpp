/**
 * @file: graph.cpp
 * @author: 0Ykahil
 * 
 * Implementation of graph ADT
 */
#include "Graph.h"

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


std::pair<std::vector<int>, double> Graph::findShortestPath(const Airport& start, const Airport& destination) {
    int total_distance = 0;

    // Check for a direct flight first to avoid unneeded landings
    int srcIdx = airportToIndex.at(start.id);
    int destIdx = airportToIndex.at(destination.id);
    for (const auto& edge : adjList[srcIdx]) {
        if (edge.dest == destIdx) {
            total_distance = edge.weight;
            return {{srcIdx, destIdx}, total_distance};
        }
    }

    // Creating our minheap priority queue
    std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>> pq;

    std::vector<int> dist(numVertices, INT32_MAX); // dist will hold our distances set
    
    // array holding visited status of vertices
    std::vector<bool> visited(numVertices, false);

    // Hold previous vertices to current vertex
    std::vector<int> prev(numVertices, -1);

    // Dijkstra's Algorithm begins here (https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)

    // Initialize source distance and push to heap
    pq.push(std::make_pair(0, srcIdx));
    dist[srcIdx] = 0;

    // Loop until heap is empty and distances are finalized
    while (!pq.empty()) {
        // Extract top
        int u = pq.top().second;
        pq.pop();

        // Ensure we haven't visited current vertex already
        if (visited[u]) {
            continue;
        }
        visited[u] = true;

        // Get all adjacent vertices to current vertex
        for (const auto& edge : adjList[u]) {
            int v = edge.dest;
            int weight = edge.weight;

            // Check for shorter path to v through u and update it if there is
            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.push(std::make_pair(dist[v], v));
                prev[v] = u;
            }
        }
    }
 
    // If heap was finished, check if there was a path to destination
    if (dist[destIdx] == INT32_MAX) {
        return {}; // return empty vector if there was no path
    }

    total_distance = dist[destIdx];

    // Construct the path from start to destination
    std::vector<int> path;

    // iterate through prevs until beginning of path is reached
    for (int at = destIdx; at != -1; at = prev[at]) {
        path.push_back(at);
    }


    return {path, total_distance};
}


void Graph::printShortestPath(const std::string startID, const std::string destID) {
    Airport startAirport;
    Airport destAirport;

    // ENSURE AIRPORT IDs are valid
    try
    {
        startAirport = vertices[airportToIndex.at(startID)];
    }
    catch(const std::exception& e)
    {
        std::cout << "\033[31m" << "'" << startID << "' IS NOT A VALID ID (did you forget to include the regional letter? e.g. 'K'JFK or 'C'YYZ)" << '\n';
        return; // exit the function if startID does not exist
    }

    try
    {
        destAirport = vertices[airportToIndex.at(destID)];
    }
    catch(const std::exception& e)
    {
        std::cout << "\033[31m" << "'" << destID << "' IS NOT A VALID ID (did you forget to include the regional letter? e.g. 'K'JFK or 'C'YYZ)" << '\n';
        return; 
    }
    
    

    std::pair<std::vector<int>, double> res = findShortestPath(startAirport, destAirport);

    // if there was no path, res should be an empty array
    if (res.first.empty()) {
    std::cout << "NO REACHABLE PATH FROM " << startID << " to " << destID << " BY AN AIRCRAFT WITH THE CURRENT RANGE" << std::endl;
    return;
    }

    // print the array in reverse to get start -> dest path
    std::cout << "\nPATH FROM " << startID << " to " << destID << ":" << "\033[37m" << std::endl;
    for (int i = res.first.size() - 1; i >= 0; i--) {
        if (i != res.first.size() - 1) {
            std::cout << " -> ";
        }
        std::cout << vertices[res.first[i]].id;
    }
    std::cout << "\033[33m" << "\n\nTotal distance: ~" << res.second << "nm" << std::endl;
}

// Ostream version
void Graph::printShortestPath(const std::string startID, const std::string destID, std::ostream& os) {
    Airport startAirport = vertices[airportToIndex.at(startID)];
    Airport destAirport = vertices[airportToIndex.at(destID)];
    std::pair<std::vector<int>, double> res = findShortestPath(startAirport, destAirport);

    // if there was no path, res should be an empty array
    if (res.first.empty()) {
    os << "NO REACHABLE PATH FROM " << startID << " to " << destID << " BY AN AIRCRAFT WITH THE CURRENT RANGE" << std::endl;
    return;
    }
    // print the array in reverse to get start -> dest path
    os << "\nPATH FROM " << startID << " to " << destID << ":\n" << "\033[37m" << std::endl;
    for (int i = res.first.size() - 1; i >= 0; i--) {
        if (i != res.first.size() - 1) {
            os << " -> ";
        }
        os << vertices[res.first[i]].id;
    }
    os << "\033[33m" << "\nTotal distance: ~" << res.second << "nm" << std::endl;
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

std::vector<Airport> Graph::getAirports() const {
    return this->vertices;
}