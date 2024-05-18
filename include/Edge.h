/**
 * @file: edge.h 
 * @author: 0YKahil
 * 
 * Definition of edge
 */
#pragma once

#include <string>
#include "Airport.h"

/**
 * @class Edge 
 * Represents an edge in a graph of Airports, connecting two
 * vertices (airports).
 */
class Edge {
    public:
        Airport source; // The source vertex (airport) of the edge.
        Airport dest;   // The destination vertext (airport) of the edge.
        double weight; // The weight of the edge, representing the distance between source and dest.
        
        std::string label; // The label representing the edge 
                           //formatted as: "(source airport identifier, destination airport identifier)".

    /**
     * Constructs an edge using given vertices, weight, and label
     * 
     * @param u The source Airport vertex.
     * @param v The destination Airport vertex.
     * @param w The weight of the edge representing the distance between source and dest.
     * @param label The label of the edge formatted as "(source airport identifier, destination airport identifier)"
     *                                                  OR "(source airport identifier -> destination airport identifier)".
    */
    Edge(const Airport& u, const Airport& v, double w, std::string label);

    /**
     * Constructs an edge using given vertices, and label. Automatically calculates the weight 
     *                                                            using the distanceTo() function.
     * 
     * @param u The source Airport vertex.
     * @param v The destination Airport vertex.
     * @param label The label of the edge formatted as "(source airport identifier, destination airport identifier)"
     *                                                  OR "(source airport identifier -> destination airport identifier)".
    */
    Edge(const Airport& u, const Airport& v, std::string label);

    /**
     * Constructs an edge using given vertices, and automatically
     * calculates the weight, and provides the labels in the format: 
     *                 "(source identifier, destination identifier)"
     * 
     * @param u The source Airport vertex.
     * @param v The destination Airport vertex.
    */
    Edge(const Airport& u, const Airport& v);

    /**
     * Default constructor for Edge.
     * 
     * Constructs an edge with default values, weight = -1, and no label.
     */
    Edge();

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