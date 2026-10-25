#include <iostream>
#include <string>

#include "Config.h"
#include "LoadBalancer.h"

using namespace std;

int main() {
    Config config;
    bool loaded = config.load("config.txt");
    if (!loaded) {
        cout << "Could not open config.txt, using default settings." << endl;

    }
    cout << "Enter number of servers: ";
    cin >> config.numServers;
    cout << "Enter number of clock cycles to run: ";
    cin >> config.runTime;

    cout << "verbose or quiet mode (v/q): ";
    string mode;
    cin >> mode;

    config.verbose = (mode == "v");

    LoadBalancer lb(config);
    lb.run(config.runTime);

    cout << "Log saved to loadbalancer_log.txt" << endl;

    return 0;
}