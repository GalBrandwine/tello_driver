#include "TelloDriver/TelloDriver.hpp"
#include <iostream>
int main()
{
    TelloDriver tello;
    tello.Connect();
    std::cout << "Starting simple connection\n";
    if (tello.WaitForConnection())
    {
        while (1)
        {
            std::cout << "Spinned\n";
            std::this_thread::sleep_for(0.5s);
        }
    }
    else
    {
        std::cout << "Connection error. exiting!\n";
    }
    exit(0);
}