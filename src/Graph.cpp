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