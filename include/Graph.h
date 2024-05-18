/**
 * @file: graph.h
 * @author: 0Ykahil
 * 
 * Declaration of Graph
 */
#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include "Airport.h"
#include "Edge.h"

/**
 * @class Graph 
 * Represents a Graph of airports represented by an adjacency list of airports.
 */
class Graph {
    public:
        /**
         * Constructs an empty graph with given numVertices and if it is weighted.
         * 
         * @param numVertices the number of vertices.
         */
        Graph(size_t numVertices);

        /**
         * Adds an Airport vertex to the graph and maps it to an id in vertices
         * 
         * @param airport The airport vertex being added
         */
        void addVertex(const Airport& airport);

        /**
         * Adds an edge to the graph given a source airport and destination airport, and calculates its weight (distance)
         * 
         * @param source The source airport. 
         * @param dest The destination airport.
         */
        void addEdge(const Airport& source, const Airport& dest);

        /**
         * Prints out the graph in the format "Airport n (identifier) -> ..."
         * and displays it in the console.
         */
        void printGraph() const;

        // Print the graph to a given output stream
        void printGraph(std::ostream& os) const;
    private:
        size_t numVertices; // The current number of vertices in the graph.
        std::vector<std::list<Edge>> adjList; // the adjacency list containing the edges.
        std::vector<Airport> vertices; // The vertices (airports) in the graph.
        std::unordered_map<std::string, size_t> airportToIndex; // Maps airport id to an index
};
