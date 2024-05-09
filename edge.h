/*
 * @file: edge.h 
 * Definition and implementation of edge
 */
#pragma once

#include <string>
#include <limits.h>

typedef size_t Vertex;

class Edge {
    public:
        Vertex source;
        Vertex dest;
        double weight;
        std::string label;

    /* 
     * Constructor for an Edge of a graph where:
     * u - represents the first vertex connected
     * v - represents the second vertex connected
     * label - the edge label
     */
    Edge(Vertex u, Vertex v, double w, std::string label)
        : source(u), dest(v), weight(w), label(label) {}

    // Default Constructor
    Edge() : source(-1), dest(-1), weight(-1), label("") {}

    /* 
     * Compare two edges and return if this edge 
     * is weighted less than other edge
     */
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};