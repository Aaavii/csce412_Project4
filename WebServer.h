/**
 * @file WebServer.h
 * @brief Declares the WebServer class.
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @brief A server that processes one request at a time.
 */
class WebServer {
public:
    /// @brief Creates an idle server with the given id.
    explicit WebServer(int id);

    /// @brief Gives the server a request and marks it busy.
    void assign(const Request& r);

    /// @brief Does one clock cycle of work. Becomes idle when timeLeft hits 0.
    void tick();

    /// @brief Returns true if the server is working on a request.
    bool isBusy() const;

    /// @brief Returns the server's id.
    int getId() const;

private:
    int id;           ///< Unique server number
    Request current;  ///< Request currently being processed
    int timeLeft;     ///< Cycles left on the current request
    bool busy;        ///< True while processing a request
};

#endif