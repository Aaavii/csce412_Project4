/**
 * @file Config.h
 * @brief Declares the Config class.
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

/**
 * @brief Settings for the simulation, loaded from a key=value text file.
 *
 * Any setting missing from the file keeps its default value.
 */
class Config {
public:
    int numServers = 10;      ///< Starting number of servers
    int runTime = 10000;      ///< Number of clock cycles to run
    int minTaskTime = 2;      ///< Shortest request time (cycles)
    int maxTaskTime = 20;     ///< Longest request time (cycles)
    int cooldown = 20;        ///< Cycles to wait after adding/removing a server
    int arrivalChance = 10;   ///< Percent chance a new request arrives each cycle
    int minThreshold = 50;    ///< Remove a server below this many requests per server
    int maxThreshold = 80;    ///< Add a server above this many requests per server
    bool verbose = true;      ///< true = print events, false = summaries only
    std::vector<std::string> blockedRanges; ///< IP ranges like "10.0.0.0-10.255.255.255"

    /// @brief Reads settings from a file. Returns false if the file can't be opened.
    bool load(const std::string& filename);
};

#endif