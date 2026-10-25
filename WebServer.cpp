/**
 * @file WebServer.cpp
 * @brief Implements the WebServer class.
 */
#include "WebServer.h"

WebServer::WebServer(int id) : id(id), current(), timeLeft(0), busy(false) {}

void WebServer::assign(const Request& r) {
    current = r;
    timeLeft = r.time;
    busy = true;
}

void WebServer::tick() {
    if (!busy) {
        return;
    }
    timeLeft--;
    if (timeLeft <= 0) {
        busy = false;
    }
}

bool WebServer::isBusy() const {
    return busy;
}

int WebServer::getId() const {
    return id;
}