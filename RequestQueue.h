/**
 * @file RequestQueue.h
 * @brief Declares the RequestQueue class.
 */
#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>
#include "Request.h"

/**
 * @brief First-in, first-out line of requests waiting for a server.
 */
class RequestQueue {
public:
    /// @brief Adds a request to the back of the queue.
    void push(const Request& r);

    /// @brief Removes and returns the request at the front. Only call if not empty.
    Request pop();

    /// @brief Returns how many requests are waiting.
    int size() const;

    /// @brief Returns true if no requests are waiting.
    bool isEmpty() const;

private:
    std::queue<Request> requests; ///< The underlying queue
};

#endif