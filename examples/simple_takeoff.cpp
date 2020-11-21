#include "TelloDriver/TelloDriver.hpp"
#include <iostream>

/*******************************************
 * Perform simple takeoff, then land.
 * No close loop operations.
 * *******************************************/

int main()
{
    TelloDriver tello(spdlog::level::debug);
    tello.Connect();
    std::cout << "Starting simple Takeff\n";
    if (!tello.WaitForConnection(10))
    {
        std::cout << "Connection error. exiting!\n";
        exit(1);
    }
    tello.Takeoff();
    std::this_thread::sleep_for(5s);
    tello.Land();
    // while (1)
    // {

    //     std::this_thread::sleep_for(0.01s);
    // }
}
