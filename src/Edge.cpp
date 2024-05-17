/**
 * @file: Edge.cpp
 * @author: 0YKahil
 * 
 * Implementation of Edge
 */

#include "Edge.h"

// Constructor for Edge made up of source and dest airport
Edge::Edge(Vertex u, Vertex v, double w, std::string label)
    : source(u), dest(v), weight(w), label(label) {}

// Constructor for Edge with automatic weight calculation
Edge::Edge(Vertex u, Vertex v, std::string label)
    : source(u), dest(v), weight(u.distanceTo(v)), label(label) {}

// Constructor for Edge with automatic weight and label calculation
Edge::Edge(Vertex u, Vertex v)
    : source(u), dest(v), weight(u.distanceTo(v)), label("(" + u.code + ", " + v.code) {}

// Default Constructor with default/empty values
Edge::Edge() : source(Airport()), dest(Airport()), weight(-1), label("") {}

// return true if weight of this edge is less than weight of other edge
bool Edge::operator<(const Edge& other) const {
    return weight < other.weight;
}

