/**
 * @file Request.h
 * @brief Defines the Request struct.
 */
#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @brief A single web request waiting to be processed.
 */
struct Request {
    std::string ipIn;   ///< IP address the request came from
    std::string ipOut;  ///< IP address the result is sent to
    int time;           ///< Number of clock cycles needed to process it
    char jobType;       ///< 'P' = processing, 'S' = streaming
};

#endif