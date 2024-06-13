/**
 * @file: flightPathOptimizer.cpp
 * @author: 0Ykahil
 *
 * Handles JSON data parsing, user inputs, outputs, and utilizes
 * Dijkstra's algorithm to find the shortest path between a given start
 * point and destination airport.
 */
#include "../include/Graph.h"
#include "../include/utility_functions.h"


 /**
  * THRESHOLD will control the cutoff for the edge creation for the graph
  * and dictate when an edge will be made (distance between both locations <= THRESHOLD).
  *
  * NOTE: This will typically be what you will change depending on the aircraft's
  *       MAX range after calculating for fuel burning, wind, etc.
  */
int THRESHOLD;

std::string dataFilepath = "./datasets/airports.json";
std::string fetchScriptpath = "./scripts/fetchAirportData.py";


// Moves the cursor to the specified (x, y) position on the console
void gotoXY(int x, int y) {
    COORD coord = { (SHORT)(x), (SHORT)(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Set the cursor position to coord
}

// Sets the console text color
void setConsoleColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Prints the menu items and highlights the currently selected item
void printMenu(const std::vector<std::string>& menuItems, int currentSelection) {
    system("cls"); // Clear the console every time menu is printed

    std::cout << BOLD << CYAN << menuItems[0] << RESET << std::endl; // Title printed separately 
    for (int i = 1; i < menuItems.size(); ++i) {  // Display all the menu items
        if (i == currentSelection) {
            setConsoleColor(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED); // Highlight selected item
            std::cout << "\n" << menuItems[i] << std::endl;  // Print the highlighted item
            setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // Reset to normal color
        }
        else {
            std::cout << GREEN << "\n" << menuItems[i] << RESET << std::endl;  // Print the item normally if it is not selected
        }
    }
}

void findPathAction(int mode) {
    THRESHOLD = 0;

    // Read the json file into jsonData
    std::cout << "reading file... ";
    std::ifstream file(dataFilepath);
    nlohmann::json jsonData;
    file >> jsonData; // read the airports file into json object
    std::cout << "done" << std::endl;
    system("cls");
    THRESHOLD = promptRange(); // Ask for aircraft Range

    // Creating AirportGraph
    std::cout << "generating airport graph... ";
    Graph g(jsonData.size());
    g.generateAirportGraph(jsonData, THRESHOLD, true);
    std::cout << "done" << std::endl;
    system("cls");
    // User input for starting and destination airport ids
    std::string lstartID, ldestID;

    // MAIN LOOP
    while (true) {
        std::cout << YELLOW << "Enter the STARTING airport's FULL identifier (e.g. CYOW not YOW) (or 'exit' to quit): \n> " << RESET;
        std::cin >> lstartID;

        std::string startID = toUpperCase(lstartID);
        if (startID == "EXIT") break;

        // Ensure id exists in graph
        if (!g.isValidAirport(startID)) {
            system("cls");
            std::cout << RED << "'" << startID << "' IS NOT A VALID ID (did you forget to include the regional letter? e.g. 'K'JFK or 'C'YYZ)\n" << RESET;
            continue;
        }

        std::cout << YELLOW << "Enter the DESTINATION airport's FULL identifier: \n> " << RESET;
        std::cin >> ldestID;
        std::string destID = toUpperCase(ldestID);
        if (destID == "EXIT") break;

        if (!g.isValidAirport(destID)) {
            system("cls");
            std::cout << "\033[31m" << "'" << destID << "' IS NOT A VALID ID (did you forget to include the regional letter? e.g. 'K'JFK or 'C'YYZ)\n" << RESET;
            continue;
        }


        auto t1 = std::chrono::high_resolution_clock::now();
        // Display the Path
        system("cls");
        std::cout << YELLOW << "Aircraft Range: " << THRESHOLD << "nm \n" << RESET;

        g.printShortestPath(startID, destID, 0);
        std::cout << RESET << std::endl;

        auto t2 = std::chrono::high_resolution_clock::now();

        // Calculating time taken to finish
        std::chrono::duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "\033[35m" << "\ntime taken: " << ms_double.count() << "ms" << RESET << std::endl;
        break;
    }

}

// Performs an action based on the selected menu item
void menuAction(int selection) {
    system("cls"); // Clear the console screen
    switch (selection) {
    case 1:
        findPathAction(0); // Placeholder for the main program of finding a path
        break;
    case 2:
        std::cout << "Searching for an airport ..." << std::endl;  // Placeholder to search for an airport by its code or name
        break;
    case 3:
        std::cout << "Rerunning the fetch script ..." << std::endl;  // Placeholder to re fetch the airport data from the API
        break;
    case 4:
        std::cout << "Configuring ..." << std::endl;  // Placeholder to configure the print mode for printshortestpath
        break;
    case 5:
        std::cout << "Exiting..." << std::endl;  // Print exit message
        exit(0);  // Exit the program
    default:
        break;
    }
    std::cout << "Press any key to return to the menu...";  // Prompt user to press any key to return to the menu
    _getch(); // Wait for a key press
}


int main() {
    runScript(dataFilepath);
    std::cout << "Press any key to return to the menu...";  // Prompt user to press any key to return to the menu
    _getch();

    std::vector<std::string> menuItems = {
        " Flight Path Optimizer (use arrow keys for selection) ",
        " > Plan Optimal Route <",
        " > Search For Airport Identifier <",
        " > Update Airport Data <",
        " > Configure Program <",
        " > Exit"
    };
    
    // Initialize our current selection to be the the first item that is not the title
    int currentSelection = 1;

    // MAIN LOOP
    while (true) {
        printMenu(menuItems, currentSelection);
        int ch = _getch();  // Get key press

        // If key is special arrow key
        if (ch == 224) {
            switch (_getch()) {
            case 72: // Up arrow pressed
                // Don't select the title
                if (currentSelection > 1 && currentSelection != 0) {
                    --currentSelection;  // Move up the list
                }
                break;
            case 80: // Down arrow pressed
                // If not already at the bottom
                if (currentSelection < menuItems.size() - 1) {
                    ++currentSelection; // Move down
                }
                break;
            default:
                break;
            }
        }
        else if (ch == 13) { // Enter pressed
            menuAction(currentSelection);  // Perform the specific menu action
        }
    }

    return 0;
}

