#include "TelloDriver/TelloDriver.hpp"
#include <iostream>

/*******************************************
 * In this example I access some of the tello attributes that are available from the tello unofficial SDK. 
 * 
 * *******************************************/
int main()
{
    TelloDriver tello(spdlog::level::debug);
    tello.Connect();
    std::cout << "Starting simple connection\n";
    if (!tello.WaitForConnection(10))
    {
        std::cout << "Connection error. exiting!\n";
        exit(1);
    }

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

        // // Get battery percentage - WORKS WELL
        // auto battery_percentage = tello.GetTelloTelemetry().GetFlightData()->GetBatteryPrecentage();
        // std::cout << "battery_percentage: " << battery_percentage << '\n';

        // // Get battery left - Documented. But no data have seen there
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetBatteryLower();
        // std::cout << "battery_left: " << tmp << '\n';

        // Get fly time left -  WORKS WELL
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetDroneFLyTimeLeft();
        // std::cout << "fly time left: " << tmp << '\n';

        // // Get EmSky - IDK what that is. Filled with zeroes.
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetEmSky();
        // std::cout << "em_sky: " << tmp << '\n';

        // // EmGround - IDK what that is.
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetEmGround();
        // std::cout << "EmGround: " << tmp << '\n';

        // // GetEmOpen - IDK what that is. Filled with zeroes.
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetEmOpen();
        // std::cout << "GetEmOpen: " << tmp << '\n';

        // // GetDroneHover - IDK what that is. Filled with zeroes.
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetDroneHover();
        // std::cout << "GetDroneHover: " << tmp << '\n';

        // // GetOutageRecording - IDK what that is. Filled with zeroes.
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetOutageRecording();
        // std::cout << "GetOutageRecording: " << tmp << '\n';

        // // GetBatteryLow
        // // To receive data, one need first to send SetBatteryLow(foo)
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetBatteryLow();
        // std::cout << "GetBatteryLow: " << tmp << '\n';

        // // GetBatteryLower
        // // To receive data, one need first to send SetLowBattery(foo)
        // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetBatteryLower();
        // std::cout << "GetBatteryLower: " << tmp << '\n';

        // // GetTemperatureHeight - Not documented
        auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetTemperatureHeight();
        std::cout << "GetTemperatureHeight: " << tmp << '\n';

        std::this_thread::sleep_for(0.01s);
    }
}
