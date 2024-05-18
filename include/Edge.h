/**
 * @file: edge.h 
 * @author: 0YKahil
 * 
 * Declaration of Graph
 */
#pragma once

#include <string>
#include "Airport.h"

typedef size_t Vertex;

/**
 * @class Edge 
 * Represents an edge in a graph of Airports, connecting two
 * vertices (airports).
 */
class Edge {
    public:
        Vertex source; // The source vertex (airport) of the edge.
        Vertex dest;   // The destination vertex (airport) of the edge.
        int weight; // The weight of the edge, representing the distance between source and dest.

    /**
     * Constructs an edge using given vertices, weight, and label
     * 
     * @param u The source Airport vertex.
     * @param v The destination Airport vertex.
     * @param w The weight of the edge representing the distance between source and dest.
    */
    Edge(const Vertex& u, const Vertex& v, int w)
        : source(u), dest(v), weight(w) {}


    /**
     * Default constructor for Edge.
     * 
     * Constructs an edge with default values, weight = -1, and no label.
     */
    Edge() : source(-1), dest(-1), weight(-1) {}

    /**
     * Compares the weights of two edges.
     * 
     * @param other The other edge to compare this edge with.
     * @return true if this edge's weight is less than the other edge's weight; false otherwise
     */
    bool operator<(const Edge& other) const;

    /**
     * Checks if the two edges are the same edge.
     * 
     * @param other The other edge to compare this edge with.
     * @return true if this edge is the same as the other edge (locations are the same if flipped); false otherwise
     */
    bool operator==(const Edge& other) const;


};