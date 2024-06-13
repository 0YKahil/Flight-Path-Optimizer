# Flight Path Optimizer

Flight Path Optimizer calculates the shortest route for a limited plane's range using Dijkstra's algorithm. The program fetches airport data from OurAirports API, calculates the great circle distances (orthodromic distance) between all pairs of airports that fit the given needs, and creates a graph with airports as nodes and requirement matching distances as edges, then outputs the shortest path to the destination of choice.

This Project was made **with the advising of a Licensed Pilot** and the routes are tested and ensured to be the most optimal routes without unnecessary stops or landings that waste time


This project uses a modifiable **Threshold** ([see here](#Threshold)) when creating paths representing **the realistic range of the aircraft** to remove any unreachable paths.

For example, while the max range is 650nm, a realistic range for the Cesna 172 is around 400nm , so the program will set a 400nm **threshold** which can be modified if needed.

~~**Change THRESHOLD VARIABLE in flightpathOptimizer.cpp to change the max range** (TO BE CHANGED)~~<br>

**THRESHOLD can be changed at the beginning of the program by inputting the aircrafts range when prompted**

## Features

- Fetches Airport location data from OurAirports api
- parses and filters data using a python script to only include
small, medium, and large airports (no heliports or seaports) and restrict to region of choice
- Calculates the circle distance between all the pairs of airports
using the Haversine formula https://en.wikipedia.org/wiki/Haversine_formula
- Converts calculations into an adjacency list representing a graph with airports as nodes and **Reachable** distances as the edges
- Using Dijkstra's Algorithm (https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm), finds the shortest path from a given starting airport and destination and outputs it to the user

# Threshold
### The Threshold sets the maximum distance (in nautical miles) for creating edges between airports in the network graph. This ensures only direct, feasible routes are included.
### Its purpose is to:
- **Limit Edge Creation:** Only connect airports within the specified distance.
- **Optimize Performance:** Reduce graph density for faster pathfinding.
- **Realistic pathing:** Reflect a realistic route that the specified aircraft range can reach.
### For example if your range is 280nm, a graph from sample data would look like this:
![Alt text](planning_and_test_documents/generateGraph_multi_280_expected.png)
### Whereas if your range is 250nm, the path to reach MDW would no longer be possible, resulting in this graph:
![Alt text](planning_and_test_documents/generateGraph_multi_250_expected.png)
### And if for some reason your range is only 100nm...
![Alt text](planning_and_test_documents/generateGraph_multi_100_expected.png)



## Requirements

- Visual C++ Redistributable


- **For source code version compiling** Python3.X, Cmake3.10+, and a C++ compiler (GNU was used for this project but others should work).

## Usage
### **NON-UI Source Code Version (Usage through console/terminal)**
It is **not recommended** to use this version if you do not know what you are doing as it is mainly run using a terminal or command prompt (need GNUWin32 on windows)
1. Clone the repository into the desired directory

2. Navigate to the directory and enter "cmake ." into your command prompt or terminal to automatically configure and build the project based on the premade CMakeLists.txt
3. Run the program using "./flightPathOptimizer" and proceed with the instructions"

4. Allow the fetching script to run automatically to create the data file, then **enter the range of your aircraft after flight conditions considerations**.

5. Enter the **full ICAO** or Identifier code (Which mean include the region identifying letter 'C' for canadian airports, 'K' for US airports, etc.)
![Alt text](img/enter_airports.png)

6. The code should now run and give you the **optimal and most efficient path** to reach your inputted destination.
![alt text](img/result.png)



### **Non-Graphic UI version**
- To be added

### **Graphic UI version**
- To be added


<br><br>

# Changelog
### v1.0.0 - 11 June 2024
- Initial Working Program

### v1.0.1 - 12 June 2024
- **Important Change**: Updated the algorithm to check for a direct flight from the current airport after every landing