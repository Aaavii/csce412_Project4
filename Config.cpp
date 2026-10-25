/**
 * @file Config.cpp
 * @brief Implements the Config class.
 */
#include "Config.h"
#include <fstream>
#include <iostream>

/// @brief Removes spaces and tabs from both ends of a string.
static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    return s.substr(start, end - start + 1);
}

bool Config::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue; // skip blank lines and comments
        }

        size_t eq = line.find('=');
        if (eq == std::string::npos) {
            continue;
        }
        std::string key = trim(line.substr(0, eq));
        std::string value = trim(line.substr(eq + 1));

        try {
            if (key == "servers")            numServers = std::stoi(value);
            else if (key == "runTime")       runTime = std::stoi(value);
            else if (key == "minTaskTime")   minTaskTime = std::stoi(value);
            else if (key == "maxTaskTime")   maxTaskTime = std::stoi(value);
            else if (key == "cooldown")      cooldown = std::stoi(value);
            else if (key == "arrivalChance") arrivalChance = std::stoi(value);
            else if (key == "minThreshold")  minThreshold = std::stoi(value);
            else if (key == "maxThreshold")  maxThreshold = std::stoi(value);
            else if (key == "verbose")       verbose = (value == "true" || value == "1");
            else if (key == "blocked")       blockedRanges.push_back(value);
            else std::cerr << "Unknown config key: " << key << "\n";
        } catch (...) {
            std::cerr << "Bad value for " << key << ": " << value << "\n";
        }
    }
    return true;
}