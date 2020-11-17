#include "TelloDriver/TelloDriver.hpp"
#include <iostream>
int main()
{
    TelloDriver tello(spdlog::level::debug);
    tello.Connect();
    std::cout << "Starting simple connection\n";
    if (tello.WaitForConnection())
    {
        while (1)
        {
            // std::cout << "Spinned\n";

            // Get WifiStrength
            // auto wifi_strength = tello.GetWifiStrength();
            // std::cout << "Wifi: " << wifi_strength << "\n";

            // Get CurrentPosition.
            // These are the RAW values, some software filtering is required.
            //auto pos = tello.GetPos();
            // std::cout << "pos.x " << pos.x << " pos.y " << pos.y << " pos.z " << pos.z << '\n';

            // Get FlightMode
            // So far I've seen 2 modes: 1, and 6.
            // MODE 1: NOT OK
            // MODE 6: OK?
            // auto flightMode = tello.GetTelloTelemetry().GetFlightData()->GetFlightMode();
            // std::cout << "flightMode: " << flightMode << '\n';

            // Get Height
            // This is undocumented member, I think its height change speed (aka acceleration in Z).
            // auto height = tello.GetTelloTelemetry().GetFlightData()->GetHeight();
            // std::cout << "height: " << height << '\n';

            // Get NED speeds
            // It seems that North points to rear.
            // It seems that East points to the right.
            // auto nedSpeeds = tello.GetTelloTelemetry().GetFlightData()->GetNEDSpeeds();
            // std::cout << "nedSpeeds.north " << nedSpeeds.x << " nedSpeeds.east " << nedSpeeds.y << " nedSpeeds.down " << nedSpeeds.z << '\n';

            // Get fly time
            // auto flyTime = tello.GetTelloTelemetry().GetFlightData()->GetFlyTime();
            // std::cout << "flyTime: " << flyTime << '\n';
            
            // Get battery percentage
            auto battery_percentage = tello.GetTelloTelemetry().GetFlightData()->GetBatteryPrecentage();
            std::cout << "battery_percentage: " << battery_percentage << '\n';
            
            std::this_thread::sleep_for(0.01s);
        }
    }
    else
    {
        std::cout << "Connection error. exiting!\n";
    }
    exit(0);
}