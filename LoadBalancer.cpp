/**
 * @file LoadBalancer.cpp
 * @brief Implements the LoadBalancer class.
 */
#include "LoadBalancer.h"
#include "Colors.h"
#include <iostream>

LoadBalancer::LoadBalancer(const Config& config)
    : clock(0),
      minThreshold(config.minThreshold),
      maxThreshold(config.maxThreshold),
      cooldown(config.cooldown),
      lastScaleTime(-config.cooldown),  // allows a scaling check on cycle 0
      minTaskTime(config.minTaskTime),
      maxTaskTime(config.maxTaskTime),
      arrivalChance(config.arrivalChance),
      verbose(config.verbose),
      nextServerId(1),
      startingQueueSize(0),
      startingServers(config.numServers),
      totalGenerated(0),
      totalProcessed(0),
      totalBlocked(0),
      serversAdded(0),
      serversRemoved(0),
      peakServers(config.numServers),
      rng(std::random_device{}()),
      logFile("loadbalancer_log.txt") {
    for (int i = 0; i < config.numServers; i++) {
        servers.push_back(new WebServer(nextServerId++));
    }
    for (const std::string& range : config.blockedRanges) {
        firewall.addRange(range);
    }
}

LoadBalancer::~LoadBalancer() {
    for (WebServer* s : servers) {
        delete s;
    }
    servers.clear();
    logFile.close();
}

int LoadBalancer::randomInt(int low, int high) {
    std::uniform_int_distribution<int> dist(low, high);
    return dist(rng);
}

std::string LoadBalancer::randomIP() {
    return std::to_string(randomInt(1, 255)) + "." +
           std::to_string(randomInt(0, 255)) + "." +
           std::to_string(randomInt(0, 255)) + "." +
           std::to_string(randomInt(0, 255));
}

Request LoadBalancer::generateRequest() {
    Request r;
    r.ipIn = randomIP();
    r.ipOut = randomIP();
    r.time = randomInt(minTaskTime, maxTaskTime);
    r.jobType = (randomInt(0, 1) == 0) ? 'P' : 'S';
    return r;
}

void LoadBalancer::enqueueRequest(const Request& r) {
    totalGenerated++;
    if (firewall.isBlocked(r.ipIn)) {
        totalBlocked++;
        log("[cycle " + std::to_string(clock) + "] x Blocked request from " + r.ipIn,
            Color::RED);
        return;
    }
    queue.push(r);
}

int LoadBalancer::tickServers() {
    int finished = 0;
    for (WebServer* s : servers) {
        bool wasBusy = s->isBusy();
        s->tick();
        if (wasBusy && !s->isBusy()) {
            finished++;
        }
    }
    return finished;
}

void LoadBalancer::assignRequests() {
    for (WebServer* s : servers) {
        if (queue.isEmpty()) {
            return;
        }
        if (!s->isBusy()) {
            s->assign(queue.pop());
        }
    }
}

void LoadBalancer::addServer() {
    servers.push_back(new WebServer(nextServerId++));
    serversAdded++;
    lastScaleTime = clock;
    if (static_cast<int>(servers.size()) > peakServers) {
        peakServers = static_cast<int>(servers.size());
    }
    log("[cycle " + std::to_string(clock) + "] + Added server (now " +
            std::to_string(servers.size()) + ", queue " + std::to_string(queue.size()) + ")",
        Color::GREEN);
}

void LoadBalancer::removeServer() {
    if (servers.size() <= 1) {
        return; // always keep at least one server
    }
    // Remove an idle server so no request in progress is lost.
    for (int i = static_cast<int>(servers.size()) - 1; i >= 0; i--) {
        if (!servers[i]->isBusy()) {
            delete servers[i];
            servers.erase(servers.begin() + i);
            serversRemoved++;
            lastScaleTime = clock;
            log("[cycle " + std::to_string(clock) + "] - Removed server (now " +
                    std::to_string(servers.size()) + ", queue " + std::to_string(queue.size()) + ")",
                Color::YELLOW);
            return;
        }
    }
    // No idle server right now; try again on a later cycle.
}

void LoadBalancer::log(const std::string& message, const std::string& color, bool always) {
    if (verbose || always) {
        std::cout << color << message << Color::RESET << "\n";
    }
    logFile << message << "\n";
}

void LoadBalancer::printStartSummary() {
    log("========== Starting Load Balancer ==========", Color::CYAN, true);
    log("Starting servers:   " + std::to_string(servers.size()), Color::CYAN, true);
    log("Starting queue:     " + std::to_string(startingQueueSize), Color::CYAN, true);
    log("Task time range:    " + std::to_string(minTaskTime) + " - " +
            std::to_string(maxTaskTime) + " cycles", Color::CYAN, true);
    log("Arrival chance:     " + std::to_string(arrivalChance) + "% per cycle", Color::CYAN, true);
    log("Scaling range:      " + std::to_string(minThreshold) + " - " +
            std::to_string(maxThreshold) + " requests per server", Color::CYAN, true);
    log("Cooldown:           " + std::to_string(cooldown) + " cycles", Color::CYAN, true);
    log("Mode:               " + std::string(verbose ? "verbose" : "quiet"), Color::CYAN, true);
    log("=============================================", Color::CYAN, true);
}

void LoadBalancer::printSummary() {
    int busy = 0;
    for (WebServer* s : servers) {
        if (s->isBusy()) {
            busy++;
        }
    }
    int idle = static_cast<int>(servers.size()) - busy;

    log("", Color::RESET, true);
    log("============== Final Summary ==============", Color::BLUE, true);
    log("Clock cycles run:       " + std::to_string(clock), Color::BLUE, true);
    log("Starting queue size:    " + std::to_string(startingQueueSize), Color::BLUE, true);
    log("Ending queue size:      " + std::to_string(queue.size()), Color::BLUE, true);
    log("Task time range:        " + std::to_string(minTaskTime) + " - " +
            std::to_string(maxTaskTime) + " cycles", Color::BLUE, true);
    log("Requests generated:     " + std::to_string(totalGenerated), Color::BLUE, true);
    log("Requests processed:     " + std::to_string(totalProcessed), Color::BLUE, true);
    log("Requests in progress:   " + std::to_string(busy), Color::BLUE, true);
    log("Requests blocked:       " + std::to_string(totalBlocked), Color::BLUE, true);
    log("Starting servers:       " + std::to_string(startingServers), Color::BLUE, true);
    log("Ending servers:         " + std::to_string(servers.size()), Color::BLUE, true);
    log("  Active (busy):        " + std::to_string(busy), Color::BLUE, true);
    log("  Inactive (idle):      " + std::to_string(idle), Color::BLUE, true);
    log("Peak servers:           " + std::to_string(peakServers), Color::BLUE, true);
    log("Servers added:          " + std::to_string(serversAdded), Color::BLUE, true);
    log("Servers removed:        " + std::to_string(serversRemoved), Color::BLUE, true);
    log("=============================================", Color::BLUE, true);
}



void LoadBalancer::run(int cycles) {

    // ---------- SETUP ----------
    int startCount = static_cast<int>(servers.size()) * 100;

    for (int i = 0; i < startCount; i++) {
        queue.push(generateRequest());
        totalGenerated++;
    }

    startingQueueSize = queue.size();
    printStartSummary();

    // ---------- MAIN LOOP ----------
    for (int i = 0; i < cycles; i++) {

        // 1. Maybe a new request arrives
        if (randomInt(1, 100) <= arrivalChance) {
            enqueueRequest(generateRequest());
        }

        // 2. Servers do one cycle of work
        totalProcessed += tickServers();

        // 3. Free servers take new work
        assignRequests();

        // 4. Add/remove servers if needed
        checkScaling();

        // 5. Time moves forward
        clock++;
    }

    // ---------- END ----------
    printSummary();
}

void LoadBalancer::checkScaling() {

    // 1. Respect the cooldown
    if ((clock - lastScaleTime) < cooldown) {
        return;
    }

    // 2. Gather numbers
    int queueSize = static_cast<int>(queue.size());
    int serverCount = static_cast<int>(servers.size());

    // 3. Decide whether to scale
    if (queueSize > maxThreshold * serverCount) {
        addServer();
    }
    else if (queueSize < minThreshold * serverCount) {
        removeServer();
    }
}