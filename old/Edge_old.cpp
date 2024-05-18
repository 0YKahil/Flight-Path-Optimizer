/**
 * @file: Edge.cpp
 * @author: 0YKahil
 * 
 * Implementation of Edge
 */

#include "Edge_old.h"

// Constructor for Edge made up of source and dest airport
Edge::Edge(const Airport& u, const Airport& v, double w, std::string label)
    : source(u), dest(v), weight(w), label(label) {}

// Constructor for Edge with automatic weight calculation
Edge::Edge(const Airport& u, const Airport& v, std::string label)
    : source(u), dest(v), weight(u.distanceTo(v)), label(label) {}

// Constructor for Edge with automatic weight and label calculation
Edge::Edge(const Airport& u, const Airport& v)
    : source(u), dest(v), weight(u.distanceTo(v)), label("(" + u.id + ", " + v.id) {}

// Default Constructor with default/empty values
Edge::Edge() : source(Airport()), dest(Airport()), weight(-1), label("") {}


bool Edge::operator<(const Edge& other) const {
    return weight < other.weight;
}

bool Edge::operator==(const Edge& other) const {
    // same exact edge
    if (this->source == other.source && this->dest == other.dest) {
        return true;
    }
    // same edge but source and destination are flipped
    if (this->source == other.dest && this->dest == other.source) {
        return true;
    }
    return false;
}


