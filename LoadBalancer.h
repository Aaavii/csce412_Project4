/**
 * @file LoadBalancer.h
 * @brief Declares the LoadBalancer class.
 */
#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <fstream>
#include <random>
#include <string>
#include <vector>
#include "Config.h"
#include "Firewall.h"
#include "Request.h"
#include "RequestQueue.h"
#include "WebServer.h"

/**
 * @brief Manages the request queue and web servers, and scales servers up or down.
 */
class LoadBalancer {
public:
    /// @brief Sets up servers, firewall, and settings from the config.
    explicit LoadBalancer(const Config& config);

    /// @brief Deletes all servers and closes the log file.
    ~LoadBalancer();

    /// @brief Runs the simulation for the given number of clock cycles.
    void run(int cycles);

    /// @brief Makes a request with random IPs, time, and job type.
    Request generateRequest();

    /// @brief Checks a request against the firewall, then queues it or blocks it.
    void enqueueRequest(const Request& r);

    /// @brief Ticks every server once. Returns how many finished a request this cycle.
    int tickServers();

    /// @brief Gives a request to every idle server while the queue has requests.
    void assignRequests();

    /// @brief Adds or removes a server based on the queue size and cooldown.
    void checkScaling();

    /// @brief Creates a new server.
    void addServer();

    /// @brief Removes one idle server (keeps at least one server).
    void removeServer();

    /// @brief Prints the starting status.
    void printStartSummary();

    /// @brief Prints the ending status and statistics.
    void printSummary();

private:
    // ---- From the design ----
    RequestQueue queue;                ///< Requests waiting for a server
    std::vector<WebServer*> servers;   ///< Active servers
    int clock;                         ///< Current clock cycle
    int minThreshold;                  ///< Remove a server below minThreshold * servers
    int maxThreshold;                  ///< Add a server above maxThreshold * servers
    int cooldown;                      ///< Cycles to wait between scaling actions
    int lastScaleTime;                 ///< Clock cycle of the last add/remove

    // ---- Settings ----
    Firewall firewall;                 ///< IP range blocker
    int minTaskTime;                   ///< Shortest request time
    int maxTaskTime;                   ///< Longest request time
    int arrivalChance;                 ///< Percent chance of a new request each cycle
    bool verbose;                      ///< Print events as they happen
    int nextServerId;                  ///< Id to give the next new server

    // ---- Statistics for the summary ----
    int startingQueueSize;             ///< Queue size before the first cycle
    int startingServers;               ///< Server count before the first cycle
    int totalGenerated;                ///< Requests created (including blocked)
    int totalProcessed;                ///< Requests finished by servers
    int totalBlocked;                  ///< Requests rejected by the firewall
    int serversAdded;                  ///< Times a server was added
    int serversRemoved;                ///< Times a server was removed
    int peakServers;                   ///< Most servers at one time

    std::mt19937 rng;                  ///< Random number generator
    std::ofstream logFile;             ///< Plain-text copy of all output

    /// @brief Returns a random int from low to high (inclusive).
    int randomInt(int low, int high);

    /// @brief Returns a random IP like "192.168.4.21".
    std::string randomIP();

    /// @brief Prints a message in color to the console and plain to the log file.
    /// @param always If true, prints even in quiet mode (used for summaries).
    void log(const std::string& message, const std::string& color, bool always = false);
};

#endif