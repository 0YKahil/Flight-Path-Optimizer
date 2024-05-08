# Airport Route Planner

Airport Route planner calculates the shortest route for a limited plane's range using Dijkstra's algorithm. The program fetches airport data from OurAirports API, calculates the distances between all pairs of airports that fit the given needs, and creates a graph with airports as nodes and requirement matching distances as edges, then outputs the shortest path to the destination of choice.


This Project Uses the Cessna 172 as the main plane of choice but can be easily modified to accommodate other planes

## Features

- Fetches Airport location data from OurAirports api
- parses and filters data using a python script to only include
small, medium, and large airports (no heliports or seaports)
- Calculates the circle distance between all the pairs of airports
using the Haversine formula https://en.wikipedia.org/wiki/Haversine_formula
- Converts calculations into an adjacency list representing a graph with airports as nodes and **Reachable** distances as the edges
- Using Dijkstra's Algorithm (https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm), finds the shortest path from a given starting airport and destination and outputs it to the user


## Requirements

- To be added

## Usage

- To be added


