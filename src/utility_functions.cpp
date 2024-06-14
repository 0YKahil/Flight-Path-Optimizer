/**
 * @file: utility_functions.cpp
 * @author: 0Ykahil
 * 
 * Implementation of the utility functions
 */
#include "utility_functions.h"

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void askRunScript(const std::string& scriptPath, const std::string& jsonFilePath) {
    // Check if the file already exists, automatically run script if it doesn't
    if (!fileExists(jsonFilePath)) {
        std::cout << jsonFilePath << " does not exist. Running script automatically." << std::endl;
        std::string runCommand = "python3 " + scriptPath;
        int result = system(runCommand.c_str()); // run the script

        if (result == 0) {
            std::cout << "Script executed successfully." << std::endl;
        } else {
            std::cerr << "Failed to execute script." << std::endl;
        }
        return;
    }
    

    // If file already exists, prompt to ask to update
    char userInput;
    while (true) {
        std::cout << "Would you like to run the fetch airports script for possible updates? (y/n): \n> ";
        std::cin >> userInput;


        if (userInput == 'y' || userInput == 'Y') {
            std::string runCommand = "python3 " + scriptPath;
            std::cout << runCommand << std::endl;
            int result = system(runCommand.c_str());

            if (result == 0) {
                std::cout << "script executed successfully." << std::endl;
            } else {
                std::cerr << "Failed to execute script." << std::endl;
            }
            break; // break from the loop after running script
        } else if (userInput == 'n' || userInput == 'N') {
            std::cout << "Proceeding with existing file..." << std::endl;
            break; // break from the loop
        } else {
            std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
        }
    }
    
}


bool prompt(const std::string& question) {
    char userInput;
    while(true) {
        std::cout << question << " (y/n): ";
        std::cin >> userInput;

        if (userInput == 'y' || userInput == 'Y') {
            return true;
        } else if (userInput == 'n' || userInput == 'N') {
            return false;
        } else {
            std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
        }
    }
}

std::string toUpperCase(std::string str) {
    std::string out;
    for (auto& c : str) out += toupper(c);
    return out;
}

int promptRange() {
    int input;

    std::cout << "Please enter the range of your aircraft (in nautical miles): \n> ";
    std::cin >> input;

    // Ensure input is an integer
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // skip invalid input
        system("cls");
        std::cout << "\033[31m" << "Not a valid input. Please enter a Number: \n> " << "\033[0m";
        std::cin >> input;
    }

    return input;
}
