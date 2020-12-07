#include "TelloDriver/TelloDriver.hpp"
#include <iostream>

/*******************************************
 * Perform simple takeoff, then land.
 * No stick stimulations.
 * No close-loop operations.
 * *******************************************/

int main()
{
    TelloDriver tello(spdlog::level::debug);
    tello.Connect();
    std::cout << "Starting simple Takeff\n";
    if (!tello.WaitForConnection(60))
    {
        std::cout << "Connection error. exiting!\n";
        exit(1);
    }
    tello.Takeoff();
    std::this_thread::sleep_for(10s);
    tello.Land();
    std::this_thread::sleep_for(10s);
}
