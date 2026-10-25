/**
 * @file Firewall.h
 * @brief Declares the Firewall class (IP range blocker).
 */
#ifndef FIREWALL_H
#define FIREWALL_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Blocks requests whose IP falls inside any blocked range.
 */
class Firewall {
public:
    /// @brief Adds a range written as "start-end", e.g. "10.0.0.0-10.255.255.255".
    void addRange(const std::string& range);

    /// @brief Returns true if the IP is inside any blocked range.
    bool isBlocked(const std::string& ip) const;

private:
    std::vector<std::pair<uint32_t, uint32_t>> ranges; ///< Blocked ranges as numbers

    /// @brief Converts "a.b.c.d" into one 32-bit number so ranges are easy to compare.
    static uint32_t ipToInt(const std::string& ip);
};

#endif