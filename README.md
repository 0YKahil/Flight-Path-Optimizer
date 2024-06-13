# Please refer to main branch README for information on the console RELEASE version

##This version is a converted version of the main project from a Linux Based GNU and CMake Environment to a MSVC visual studio environment
**The Biggest none source file change is the main function will become a simple console ui style application instead of the old terminal typing based version**
#### Summary of changes in src and scripts:
- Some return types of functions (specifically the findShortestPath() functions) were modified from pair<vector, double> to pair<vector, int> so the compiler doesn't freak out when dealing with the cases where the code converts a double to an int without casting (I chose to go for int as I wanted the total distance rounded)
- For the same reason above, MSVC seems to not allow converting of data types, so in a lot of parts of the Graph.cpp functions, data type casting had to be added to allow the program to smoothly run

**Although those things were changed, it was only for the strictness of the MSVC compiler, and the program should still run and behave the same in all cases**
