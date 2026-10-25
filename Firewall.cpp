/**
 * @file Firewall.cpp
 * @brief Implements the Firewall class.
 */
#include "Firewall.h"
#include <sstream>

uint32_t Firewall::ipToInt(const std::string& ip) {
    std::stringstream ss(ip);
    std::string part;
    uint32_t result = 0;
    while (std::getline(ss, part, '.')) {
        result = (result << 8) | static_cast<uint32_t>(std::stoi(part));
    }
    return result;
}

void Firewall::addRange(const std::string& range) {
    size_t dash = range.find('-');
    if (dash == std::string::npos) {
        return;
    }
    uint32_t start = ipToInt(range.substr(0, dash));
    uint32_t end = ipToInt(range.substr(dash + 1));
    ranges.push_back({start, end});
}

bool Firewall::isBlocked(const std::string& ip) const {
    uint32_t value = ipToInt(ip);
    for (const auto& r : ranges) {
        if (value >= r.first && value <= r.second) {
            return true;
        }
    }
    return false;
}