/**
 * @file: edge.h 
 * @author: 0YKahil
 * 
 * Definition of edge
 */
#pragma once

#include <string>
#include "Airport.h"

typedef Airport Vertex;


/**
 * @class Edge 
 * @brief Represents an edge in a graph of Airports, connecting two
 *        vertices (airports).
 */
class Edge {
    public:
        Vertex source; // The source vertex (airport) of the edge.
        Vertex dest;   // The destination vertext (airport) of the edge.
        double weight; // The weight of the edge, representing the distance between source and dest.
        
        std::string label; // The label representing the edge 
                           //formatted as: "(source airport identifier, destination airport identifier)".

    /**
     * @brief Constructs an edge using given vertices, weight, and label
     * 
     * @param u The source Airport vertex.
     * @param v The destination Airport vertex.
     * @param w The weight of the edge representing the distance between source and dest.
     * @param label The label of the edge formatted as "(source airport identifier, destination airport identifier)"
     *                                                  OR "(source airport identifier -> destination airport identifier)".
    */
    Edge(Vertex u, Vertex v, double w, std::string label);

    /**
     * @brief Constructs an edge using given vertices, and label. Automatically calculates the weight 
     *                                                            using the distanceTo() function.
     * 
     * @param u The source Airport vertex.
     * @param v The destination Airport vertex.
     * @param label The label of the edge formatted as "(source airport identifier, destination airport identifier)"
     *                                                  OR "(source airport identifier -> destination airport identifier)".
    */
    Edge(Vertex u, Vertex v, std::string label);

    /**
     * @brief Constructs an edge using given vertices, and automatically
     *        calculates the weight, and provides the labels in the format: 
     *                         "(source identifier, destination identifier)"
     * 
     * @param u The source Airport vertex.
     * @param v The destination Airport vertex.
    */
    Edge(Vertex u, Vertex v);

    /**
     * @brief Default constructor for Edge.
     * 
     * Constructs an edge with default values, weight = -1, and no label.
     */
    Edge();

    /**
     * @brief Compares the weights of two edges.
     * 
     * @param other The other edge to compare this edge with.
     * @return true if this edge's weight is less than the other edge's weight; false otherwise
     */
    bool operator<(const Edge& other) const;
};