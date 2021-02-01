#include "TelloDriver/TelloDriver.hpp"
#include <iostream>

/**
 * @brief This is an example for creating user observer and attaching it to TelloDriver.
 * This class can be attached to:
 * * a speficic IPositionVelocityObserver, for getting new Pose and velocity
 * * a speficic IImuAttitudeObserver, for getting new Imu gyre attitude ans such.
 * * a speficic IFlightDataObserver, for getting new flight data like battery %, and some flags like "flight_data_extras.battery_low".
 * * and to a generic IObserver, for getting standart std::vector<unsignd char> and interprating it the user way.
 */
class PosObserver : public IPositionVelocityObserver,
                    public IFlightDataObserver,
                    public IImuAttitudeObserver
{
public:
    /**
     * @brief Must be overriden.
     * 
     * @param[in] message_from_subject - data filled with information relenavt to this observer.
     */
    void Update(const tello_protocol::ImuAttitudeData &imu_attitude) override;
    void Update(const tello_protocol::FlightDataStruct &flight_data) override;
    void Update(const tello_protocol::PoseVelData &pos_vel) override;
    void Update(const std::vector<unsigned char> &message_from_subject) override;
    PosObserver(/* args */);
    ~PosObserver();

    const tello_protocol::FlightDataStruct &GetFlightData() const { return m_flight_data; };
    const tello_protocol::ImuAttitudeData &GetImuAttitudeData() const { return m_imu_attitude_data; };
    const tello_protocol::PoseVelData &GetPosVelData() const { return m_pos_vel_data; };

private:
    tello_protocol::FlightDataStruct m_flight_data;
    tello_protocol::ImuAttitudeData m_imu_attitude_data;
    tello_protocol::PoseVelData m_pos_vel_data;
};

PosObserver::PosObserver(/* args */)
{
}

PosObserver::~PosObserver()
{
}

void PosObserver::Update(const tello_protocol::ImuAttitudeData &imu_attitude)
{
    m_imu_attitude_data = imu_attitude;
}

void PosObserver::Update(const tello_protocol::FlightDataStruct &flight_data)
{
    m_flight_data = flight_data;
}

void PosObserver::Update(const tello_protocol::PoseVelData &pos_vel)
{
    m_pos_vel_data = pos_vel;
}

void PosObserver::Update(const std::vector<unsigned char> &message_from_subject)
{
    std::string s(message_from_subject.begin(), message_from_subject.end());
    std::cout << "received tello current log version: " << s << "\n";
}

int main()
{

    TelloDriver tello(spdlog::level::info);
    tello.Connect();
    
    std::cout << "Attaching user observer\n";
    PosObserver pos_vel_obs;

    tello.Attach(OBSERVERS::POSITION_VELOCITY_LOG, &pos_vel_obs);
    tello.Attach(OBSERVERS::DJI_LOG_VERSION, &pos_vel_obs);
    tello.Attach(OBSERVERS::FLIGHT_DATA_MSG, &pos_vel_obs);

    std::cout << "Starting simple connection\n";
    if (!tello.WaitForConnection(10))
    {
        tello.GetLogger()->error("Connection error. exiting!");
        exit(1);
    }

    while (1)
    {
        if (!pos_vel_obs.GetFlightData().flight_data_extras.battery_low)
        {
            // tello.GetLogger()->info("Battery: {}", std::to_string(pos_vel_obs.GetFlightData().battery_percentage));
        }
        else if (!pos_vel_obs.GetFlightData().flight_data_extras.battery_lower)
        {
            tello.GetLogger()->warn("Battery too low!");
        }
        else
        {
            tello.GetLogger()->error("Battery too lowest!");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// std::cout << "Spinned\n";

    // Get WifiStrength
    // auto wifi_strength = tello.GetWifiStrength();
    // std::cout << "Wifi: " << wifi_strength << "\n";

    /**
         * @brief Get CurrentPosition.
         * These are the RAW values.
         * 
         * @note Some software filtering is required.
         */
    // auto temp_pos = tello.GetPos();
    // if (pos.z != temp_pos.z) //
    // {
    //     pos = temp_pos;
    //     tello.GetLogger()->info("pos.x " + std::to_string(pos.x) + " pos.y " + std::to_string(pos.y) + " pos.z " + std::to_string(pos.z) + '\n');
    // }

    // pos = tello.GetPos();
    // tello.GetLogger()->info("pos.x " + std::to_string(pos.x) + " pos.y " + std::to_string(pos.y) + " pos.z " + std::to_string(pos.z) + '\n');

    // Get FlightMode
    // MODE 1: NOT OK
    // MODE 6: OK?
    // MODE 11: MOVING?
    // MODE 12: LANDING??
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
    // auto tmp = tello.GetTelloTelemetry().GetFlightData()->GetTemperatureHeight();
    // std::cout << "GetTemperatureHeight: " << tmp << '\n';

    // }