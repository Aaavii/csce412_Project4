/**
 * @file RequestQueue.cpp
 * @brief Implements the RequestQueue class.
 */
#include "RequestQueue.h"

void RequestQueue::push(const Request& r) {
    requests.push(r);
}

Request RequestQueue::pop() {
    Request r = requests.front();
    requests.pop();
    return r;
}

int RequestQueue::size() const {
    return static_cast<int>(requests.size());
}

bool RequestQueue::isEmpty() const {
    return requests.empty();
}