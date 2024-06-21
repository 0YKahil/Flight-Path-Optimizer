/**
 * @file: utility_functions.cpp
 * @author: 0Ykahil
 * 
 * Implementation of the utility functions
 */
#include "../include/utility_functions.h"   

namespace fs = std::filesystem;

Config::Config(const std::string& directory, const std::string& filename)
{
    // Ensure the directory exists
    if (!fs::exists(directory)) {
        if (!fs::create_directories(directory)) {
            std::cerr << "Failed to create directory: " << directory << std::endl;
            return;
        }
    }

    // Set the file path
    filepath = directory + "/" + filename;

    // Load existing JSON data if the file exists
    std::ifstream file(filepath);
    if (file.is_open() && file.peek() != std::ifstream::traits_type::eof()) {
        file >> jsonData;
        file.close();
    }
    else {
        // Initialize jsonData to an empty object if file is not open or empty
        jsonData = nlohmann::json::object();
    }
}


std::string Config::read(const std::string& key, const std::string& defaultValue) {
    if (jsonData.is_null()) {
        return defaultValue;
    }
    return jsonData.value(key, defaultValue);
}

void Config::write(const std::string& key, const std::string& value) {
    jsonData[key] = value;
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << jsonData.dump(4);
        file.close();
    }
}

bool isInteger(const std::string& string) {
    return !string.empty() && std::all_of(string.begin(), string.end(), ::isdigit);
}

int toInteger(const std::string& string) {
    int out = -1;
    if (isInteger(string)) {
        out = std::stoi(string);
    }
    return out;
}


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
    char input;
    while (true) {
        std::cout << "Would you like to run the fetch airports script for possible updates? (y/n): \n> ";
        std::cin >> input;


        if (input == 'y' || input == 'Y') {
            std::string runCommand = "python3 " + scriptPath;
            std::cout << runCommand << std::endl;
            int result = system(runCommand.c_str());

            if (result == 0) {
                std::cout << "script executed successfully." << std::endl;
            } else {
                std::cerr << "Failed to execute script." << std::endl;
            }
            break; // break from the loop after running script
        } else if (input == 'n' || input == 'N') {
            std::cout << "Proceeding with existing file..." << std::endl;
            break; // break from the loop
        } else {
            std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
        }
    }
    
}

std::string getExecutableDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

void runScript(const std::string& jsonFilePath) {
    // Check if the file already exists, automatically run script if it doesn't
    if (!fileExists(jsonFilePath)) {
        std::string dir = getExecutableDirectory(); // get current directory

        std::cout << jsonFilePath << " does not exist. Fetching airport data automatically." << std::endl;

        // Construct path to script batch file
        std::string runCommand = dir + "\\scripts\\fetchAirportData.exe";
        int result = system(runCommand.c_str()); // run the script

        if (result == 0) {
            std::cout << "Script executed successfully." << std::endl;
        }
        else {
            std::cerr << "Failed to execute script." << std::endl;
        }
        return;
    }
}

bool prompt(const std::string& question) {
    char input;
    while(true) {
        std::cout << question << " (y/n): ";
        std::cin >> input;

        if (input == 'y' || input == 'Y') {
            return true;
        } else if (input == 'n' || input == 'N') {
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


int promptRange(Config config) {
    std::string input; // Will hold the string input OR the value from read()
    int out = 0; // Will carry the integer value of input and be returned

    // Open the config file if it exists or create it if it doesn't
    // Checking if user.range already exists from before and returning it if it does
    out = toInteger(config.read("user.range", "-1"));

    if (out > 0) {
        std::cout << "\nUsing saved range: " << out << "nm" << std::endl;
        return out;
    }

    // Allow user to input their range.
    std::cout << "\nIt seems this is your first time running the program" << std::endl;
    std::cout << "\n\nPlease enter the range of your aircraft (in nautical miles): \n> ";
    std::cin >> input;

    // Ensure input is an integer
    while (!isInteger(input)) {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // skip invalid input
        system("cls");
        std::cout << "\033[31m" << "Not a valid input. Please enter a Number: \n> " << "\033[0m";
        std::cin >> input;
    }

    system("cls");
    return toInteger(input);
}


json parseJSON(const std::string& filename) {
    std::ifstream file(filename);
    json j;
    file >> j;
    return j;
}



std::unordered_map<std::string, std::string> createAirportMapFromJson(const json& j) {
    std::unordered_map<std::string, std::string> airportMap;

    for (const auto& item : j) {
        std::string id = item["ident"];
        std::string name = item["name"];
        airportMap[id] = name;
    }

    return airportMap;
}

std::vector<std::string> getAirportsFromPhrase(std::unordered_map<std::string, std::string> airports_map, std::string phrase) {
    std::vector<std::string> matching_airports = {}; // Initialize the matching airports as airports found so far

    // Convert all phrases to upper case
    std::string upperPhrase = toUpperCase(phrase);

    for (auto it = airports_map.begin(); it != airports_map.end(); ++it) {
        // Convert the airport name to uppercase
        std::string upperValue = toUpperCase(it->second);
        size_t found = upperValue.find(upperPhrase);
        if (found != std::string::npos) {
            matching_airports.push_back(it->first + ": " + it->second);
        }
    }

    return matching_airports;
}