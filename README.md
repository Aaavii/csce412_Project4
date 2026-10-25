# CSCE 412 Project 4: Load Balancer

Avi Kumar, Fall 2026

This project simulates a load balancer in C++. A load balancer spreads incoming requests across a group of servers so none of them get overloaded.

The program starts with a full queue of random requests. Every clock cycle, new requests can show up, servers work on their current request, and free servers grab the next one from the queue. The load balancer adds a server when the queue gets too long (more than 80 requests per server) and removes one when it gets too short (less than 50 per server). It waits a few cycles between changes so it doesn't add and remove servers constantly.

It also has a firewall that blocks requests from certain IP ranges, a config file for settings, colored output, and verbose/quiet modes. At the end it prints a summary and saves everything to `loadbalancer_log.txt`.

## Classes
- **LoadBalancer**: runs the simulation and adds/removes servers
- **WebServer**: handles one request at a time
- **RequestQueue**: holds requests waiting for a server
- **Request**: stores a request's IPs, processing time, and job type
- **Firewall**: blocks requests from blocked IP ranges
- **Config**: reads settings from `config.txt`

## How to run
```
make
./loadbalancer
```
Enter the number of servers, the number of clock cycles, and `v` for verbose or `q` for quiet.

