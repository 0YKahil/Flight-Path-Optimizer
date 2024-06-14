/**
 * @file: utility_functions.h
 * @author: 0Ykahil
 * 
 * Declaration of Basic utility functions for console ui and prompts used in main program.
 */

#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <vector>
#include "../external/json.hpp"

// Config is a simple class to create and parse config files for persistence of configuration
class Config {
    public:
        /**
         * Constructor for a Config with data to be persisted.
         * @param directory The directory of the config file.
         * @param filename The name of the config file
         */
        Config(const std::string& directory, const std::string& filename);

        /**
         * Reads an item in the JSON config file using the provided and returns the value associated with it.
         * If there is no value for a given key, returns the defaultValue provided.
         * 
         * @param key The key of the item of interest (e.g. 'userRange').
         * @param defaultValue The default value that is returned if the key is not found.
         */
        std::string read(const std::string& key, const std::string& defaultValue = "");

        /**
         * Writes an item to the JSON config file with a provided key and associated value.
         * 
         * @param key The key of the item being written (e.g. "userRange").
         * @param value The associated value written to the provided key (e.g. "550").
         */
        void write(const std::string& key, const std::string& value);

    private:
        std::string filepath; // File path to config file
        nlohmann::json jsonData; // The json Data provided to the object from the given file

};

// returns true if the form of string is an integer (e.g. "500"); false otherwise
bool isInteger(const std::string& string);

/**
 * Returns the given string as an integer; or -1 if string is not an integer
 * 
 * REQUIRES: string has to represent an integer
 */
int toInteger(const std::string& string);

// Returns true if a file already exists in the director; false otherwise.
bool fileExists(const std::string& filename);

/** 
 * Prompts the user if they would like to run a specific json file python script
 * if the json file does not exist, then it automatically runs the script.
 * 
 * @param scriptPath The path to the desired python script.
 * @param jsonFilePath The desired path for the json file (this should be the data folder for the program).
 */
void askRunScript(const std::string& scriptPath, const std::string& jsonFilePath);

/**
 * Simple Yes no prompt that asks the user a yes no question.
 * Return true if yes was answered; false otherwise.
 * 
 * @param question The question prompt for the user.
 */
bool prompt(const std::string& question);


/**
 * Converts a string to uppercase and returns it
 * 
 * @param str The string to be converted to uppercase
 */
std::string toUpperCase(std::string str);


/**
 * Prompts for user entered aircraft range
 * and returns it if it is valid
 */
int promptRange();

/**
 * Saves a specific item to the file path ini file with the given key and value
 * e.g. userRange = 50
 * 
 * @param filepath file path to the ini file.
 * @param key The key associated with the item being saved (e.g. "userRange")
 * @param value The value being saved to that specific key
 */
template <typename T>
void writeToIni(const std::string& filepath, const std::string& key, const T value);

/**
 * Loads the data from the ini 
 */
void loadIni(const std::string& filepath, std::vector<int> items);
