/**
 * @file Colors.h
 * @brief ANSI color codes for colored console output.
 */
#ifndef COLORS_H
#define COLORS_H

#include <string>

/// @brief Text colors used to highlight different events in the console.
namespace Color {
    const std::string RESET  = "\033[0m";
    const std::string RED    = "\033[31m";
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE   = "\033[34m";
    const std::string CYAN   = "\033[36m";
}

#endif