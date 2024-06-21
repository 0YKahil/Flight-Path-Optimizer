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
int THRESHOLD = 0;

/*
 * MODE controls the mode parameter of Graph::findshortestPath(). 
 * mode=0 (default), will print the paths using their ICAO codes (AAAA -> BBBB -> CCCC)
 * mode=1 (full name mode), will print the paths using their airport names (Airport A -> Airport B -> Airport B)
 */
int MODE = 0;

// Config will contain all the persisting settings set by the user
Config config("settings", "config.json"); // Initialize config.json or Open it

std::string dataFilepath = "./datasets/airports.json";
std::string fetchScriptpath = "./scripts/fetchAirportData.py";

// This will hold our list of all airports with their corresponding codes
json j = parseJSON(dataFilepath);

std::unordered_map<std::string, std::string> airports_map = createAirportMapFromJson(j);


// Initializes config values
void initConfig() {
    if (config.read("path.mode", "-1") == "-1") {
        config.write("path.mode", "0");
    }
    else {
        MODE = std::stoi(config.read("path.mode", "-1"));
    }

    if (config.read("user.range", "-1") == "-1") {
        config.write("user.range", "0");
    }
    else {
        THRESHOLD = std::stoi(config.read("user.range", "0"));
    }
}

// Moves the cursor to the specified position on the console
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

    std::cout << BOLD << CYAN << "\n" << menuItems[0] << RESET << std::endl; // Title printed separately 
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
    std::cout << BOLD << YELLOW << "\n" << " RANGE: " + std::to_string(THRESHOLD) + "NM | MODE: MODE " + std::to_string(MODE + 1) << RESET << std::endl;
}

// Executes the find path action and Prompts the user for starting and destination codes then outputs the best path
void findPathAction(int mode) {
    // Read the json file into jsonData
    std::cout << "reading file... ";
    std::ifstream file(dataFilepath);
    nlohmann::json jsonData;
    file >> jsonData; // read the airports file into json object
    std::cout << "done" << std::endl;
    system("cls");

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
        std::cout << GREEN << "Enter the STARTING airport's FULL identifier (e.g. CYOW not YOW) (or 'exit' to quit): \n> " << RESET;
        std::cin >> lstartID;

        std::string startID = toUpperCase(lstartID);
        if (startID == "EXIT") break;

        // Ensure id exists in graph
        if (!g.isValidAirport(startID)) {
            system("cls");
            std::cout << RED << "'" << startID << "' IS NOT A VALID ID (did you forget to include the regional letter? e.g. 'K'JFK or 'C'YYZ)\n" << RESET;
            continue;
        }

        std::cout << GREEN << "Enter the DESTINATION airport's FULL identifier: \n> " << RESET;
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

        g.printShortestPath(startID, destID, MODE);
        std::cout << RESET << std::endl;

        auto t2 = std::chrono::high_resolution_clock::now();

        // Calculating time taken to finish
        std::chrono::duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "\033[35m" << "\ntime taken: " << ms_double.count() << "ms" << RESET << std::endl;
        break;
    }

}

// Performs the configure range action and allows the user to enter a new range to replace "user.range" in config.json
void configureRangeAction() {
    system("cls");
    int newRange;

    std::cout << GREEN << "CURRENT RANGE: " << THRESHOLD << RESET << std::endl;
    std::cout << WHITE << "\nPlease enter the NEW aircraft range: \n> " << RESET;
    std::cin >> newRange;

    // Ensure input is an integer
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // skip invalid input
        system("cls");
        std::cout << "\033[31m" << "Not a valid input. Please enter a Number: \n> " << "\033[0m";
        std::cin >> newRange;
    }

    THRESHOLD = newRange;

    config.write("user.range", std::to_string(newRange)); // Write th updated range to the config file

    system("cls");
    std::cout << YELLOW << BOLD << "\n User Range successfully updated to " << THRESHOLD << "nm" << RESET << std::endl;
}

int configureModeAction(int selection) {
    system("cls"); // Clear the console screen

    switch (selection) {
    case 1: // Mode 0
        if (MODE == 0) {
            std::cout << BOLD << YELLOW << "\n Already Using Mode 1. " << RESET << std::endl;
            break;
        }
        MODE = 0;
        config.write("path.mode", "0");
        std::cout << BOLD << YELLOW << "\n Switched to Mode 1" << RESET << std::endl;
        break;
    case 2: // Mode 1
        if (MODE == 1) {
            std::cout << BOLD << YELLOW << "\n Already Using Mode 2. " << RESET << std::endl;
            break;
        }
        MODE = 1;
        config.write("path.mode", "1");
        std::cout << BOLD << YELLOW << "\n Switched to Mode 2." << RESET << std::endl;
        break;
    case 3: // Return to configuration menu
        return 0;
    default:
        break;
    }
    std::cout << "\n Press any key to return to Configuration Menu...";  // Prompt user to press any key to return to the menu
    _getch(); // Wait for a key press
    return 1;
}

// Display the select mode menu to allow cycling between the two modes in Graph::printShortestPath();
void modeMenu() {
    std::vector<std::string> modeItems = {
        " Select An Output Path Mode",
        " > Mode 1: Output ICAO Codes (AAAA -> AAAB -> AAAC) <",
        " > Mode 2: Output Airport Names (A International Airport -> B International Airport -> C International Airport) <",
        " > Return To Configuration Menu <"
    };

    int currentSelection = 1;

    while (true) {
        printMenu(modeItems, currentSelection);
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
                if (currentSelection < modeItems.size() - 1) {
                    ++currentSelection; // Move down
                }
                break;
            default:
                break;
            }
        }
        else if (ch == 13) { // Enter pressed
            if(configureModeAction(currentSelection)) return; // skip confirmation
            break;
        }
    }

}

// Performs the selected configuration action and returns 0; or returns 1 if exiting to main menu
int configureAction(int selection) {
    system("cls"); // Clear the console screen
    switch (selection) {
    case 1:
        configureRangeAction();
        break;
    case 2:
        modeMenu();
        return 0;
    case 3:
        std::cout << "Returning to menu ..." << std::endl;
        return 1;
    default:
        break;
    }
    std::cout << "\n Press any key to return to configuration...";  // Prompt user to press any key to return to the menu
    _getch(); // Wait for a key press
    return 0;
}

// Opens the configuration Menu and corresponding actions
void configureMenu() {
    std::vector<std::string> configureItems = {
        " Configuration Menu",
        " > Configure Range <",
        " > Configure Path Output Mode <",
        " > Return To Main Menu <"
    };
    // Initialize our current selection to be the the first item that is not the title
    int currentSelection = 1;

    // MAIN LOOP
    while (true) {
        printMenu(configureItems, currentSelection);
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
                if (currentSelection < configureItems.size() - 1) {
                    ++currentSelection; // Move down
                }
                break;
            default:
                break;
            }
        }
        else if (ch == 13) { // Enter pressed
            if (configureAction(currentSelection)) return;  // Perform the specific menu action
        }
    }
}

// Opens the search airport screen an commences the search action
void searchAction() {
    system("cls");
    std::string phrase;
    std::cout << GREEN << "Enter the name or part of the name of the desired airport \n> " << RESET;
    std::getline(std::cin >> std::ws, phrase);

    std::vector<std::string> matching_airports = getAirportsFromPhrase(airports_map, phrase);
    if (matching_airports.size() > 0) {
        for (auto& airport : matching_airports) {
            std::cout << airport << std::endl;
        }
    }
    else {
        std::cout << "No Results Matching '" << RED << phrase << RESET << "'" << std::endl;
    }

}


/**
 * Performs an action based on the selected menu item and confirms return to menu after it is done.
 * If configuration menu case is done executing, skips the confirmation.
 */
void menuAction(int selection) {
    system("cls"); // Clear the console screen
    switch (selection) {
    case 1:
        findPathAction(0);
        break;
    case 2:
        searchAction();
        break;
    case 3:
        std::cout << "Rerunning the fetch script ..." << std::endl;  // Placeholder to re fetch the airport data from the API
        break;
    case 4:
        configureMenu();
    case 5:
        std::cout << "Exiting..." << std::endl;  // Print exit message
        exit(0);  // Exit the program
    default:
        break;
    }
    std::cout << "\nPress any key to return to the menu...";  // Prompt user to press any key to return to the menu
    _getch(); // Wait for a key press
}


int main() {
    // Initialize our config file if it doesn't exist or load its information if it does
    initConfig();
    runScript(dataFilepath);
    std::cout << "HERE\n";
    THRESHOLD = promptRange(config); // Ask for aircraft Range

    config.write("user.range", std::to_string(THRESHOLD));
    std::cout << "\nRange saved to config.json." << std::endl;

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

