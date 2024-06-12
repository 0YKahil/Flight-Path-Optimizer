/**
 * @file: utility_functions.h
 * @author: 0Ykahil
 * 
 * Declaration of Basic utility functions for console ui and prompts used in main program.
 */

#pragma once
#include <string>


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
