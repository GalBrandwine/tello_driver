#include "flight_data/FlightData.hpp"

namespace tello_protocol
{
    void FlightData::SetLowBatThreshold(const std::string &lowbatthreshold)
    {
        std::memcpy(&m_low_battery_threhold, &lowbatthreshold[1], sizeof(short));
        m_logger->info("Setting LowBatThreshold: {}%", m_low_battery_threhold);
    }
    void FlightData::SetAttLimit(const std::string &attlimit)
    {
        std::memcpy(&m_attitude_limit, &attlimit[1], sizeof(float));
        m_logger->info("Setting Att: {}[deg]", m_attitude_limit);
    }
    void FlightData::SetAltLimit(const std::string &altlimit)
    {
        std::memcpy(&m_alt_limit, &altlimit[1], sizeof(short));
        m_logger->info("Setting AltLimit: {}[m]", m_alt_limit);
    }
    const double FlightData::GetFlightMode() const
    {
        return m_fly_mode;
    }
    const double FlightData::GetHeight() const
    {
        return m_height;
    }
    const double FlightData::GetNorthSpeed() const
    {
        return m_north_speed;
    }
    const double FlightData::GetEastSpeed() const
    {
        return m_east_speed;
    }
    const double FlightData::GetGroundSpeed() const
    {
        return m_ground_speed;
    }
    const tello_protocol::Vec3 FlightData::GetNEDSpeeds() const
    {
        return tello_protocol::Vec3{m_north_speed, m_east_speed, m_ground_speed};
    }
    const double FlightData::GetFlyTime() const
    {
        return m_fly_time;
    }
    const double FlightData::GetIMUState() const
    {
        return m_imu_state;
    }
    const double FlightData::GetPressureState() const
    {
        return m_pressure_state;
    }
    const double FlightData::GetDownVisualState() const
    {
        return m_down_visual_state;
    }
    const double FlightData::GetPowerState() const
    {
        return m_power_state;
    }
    const double FlightData::GetBatteryState() const
    {
        return m_battery_state;
    }
    const double FlightData::GetGravityState() const
    {
        return m_gravity_state;
    }
    const double FlightData::GetWindState() const
    {
        return m_wind_state;
    }
    const double FlightData::GetIMUCalibrationState() const
    {
        return m_imu_calibration_state;
    }
    const double FlightData::GetBatteryPrecentage() const
    {
        return m_battery_percentage;
    }
    const double FlightData::GetDroneBatteryLeft() const
    {
        return m_drone_battery_left;
    }
    const double FlightData::GetDroneFLyTime() const
    {
        return m_drone_fly_time_left;
    }
    const double FlightData::GetEmSky() const
    {
        return m_em_sky;
    }
    const double FlightData::GetEmGround() const
    {
        return m_em_ground;
    }
    const double FlightData::GetEmOpen() const
    {
        return m_em_open;
    }
    const double FlightData::GetDroneHover() const
    {
        return m_drone_hover;
    }
    const double FlightData::GetOutageRecording() const
    {
        return m_outage_recording;
    }
    const double FlightData::GetBatteryLow() const
    {
        return m_battery_low;
    }
    const double FlightData::GetBatteryLower() const
    {
        return m_battery_lower;
    }
    const double FlightData::GetFactoryMode() const
    {
        return m_factory_mode;
    }
    const double FlightData::GetThrowFlyTimer() const
    {
        return m_throw_fly_timer;
    }
    const double FlightData::GetCameraState() const
    {
        return m_camera_state;
    }
    const double FlightData::GetElectricalMachineryState() const
    {
        return m_electrical_machinery_state;
    }
    const double FlightData::GetFrontIn() const
    {
        return m_front_in;
    }
    const double FlightData::GetFrontOut() const
    {
        return m_front_out;
    }
    const double FlightData::GetFrontLsc() const
    {
        return m_front_lsc;
    }
    const double FlightData::GetTemperatureHeight() const
    {
        return m_temperature_height;
    }

    bool FlightData::SetData(const std::string &data)
    {
        if (data.length() < 24)
        {
            if (m_logger)
                m_logger->warn("data length didnt match.");
            return false;
        }

        m_height = int16(data[0], data[1]);
        // NED speeds
        m_north_speed = int16(data[2], data[3]);
        m_east_speed = int16(data[4], data[5]);

        // Other method for storing data from buffer to reference:
        // short temp;
        // std::memcpy(&temp, &data[6], sizeof(short));

        m_ground_speed = int16(data[6], data[7]);

        m_fly_time = int16(data[8], data[9]);
        m_imu_state = ((data[10] >> 0) & 0x1);
        m_pressure_state = ((data[10] >> 1) & 0x1);
        m_down_visual_state = ((data[10] >> 2) & 0x1);
        m_power_state = ((data[10] >> 3) & 0x1);
        m_battery_state = ((data[10] >> 4) & 0x1);
        m_gravity_state = ((data[10] >> 5) & 0x1);
        m_wind_state = ((data[10] >> 7) & 0x1);
        m_imu_calibration_state = data[11];
        m_battery_percentage = data[12];
        m_drone_battery_left = int16(data[13], data[14]);
        m_drone_fly_time_left = int16(data[15], data[16]);
        m_em_sky = ((data[17] >> 0) & 0x1);
        m_em_ground = ((data[17] >> 1) & 0x1);
        m_em_open = ((data[17] >> 2) & 0x1);
        m_drone_hover = ((data[17] >> 3) & 0x1);
        m_outage_recording = ((data[17] >> 4) & 0x1);
        m_battery_low = ((data[17] >> 5) & 0x1);
        m_battery_lower = ((data[17] >> 6) & 0x1);
        m_factory_mode = ((data[17] >> 7) & 0x1);
        m_fly_mode = data[18];
        m_throw_fly_timer = data[19];
        m_camera_state = data[20];
        m_electrical_machinery_state = data[21];
        m_front_in = ((data[22] >> 0) & 0x1);
        m_front_out = ((data[22] >> 1) & 0x1);
        m_front_lsc = ((data[22] >> 2) & 0x1);
        m_temperature_height = ((data[23] >> 0) & 0x1);

        // if (m_logger)
        // {
        //     m_logger->info("Binary : {}", spdlog::to_hex(data));
        // }

        return true;
    };
    FlightData::FlightData(std::shared_ptr<spdlog::logger> logger)
        : m_logger(logger)
    {
    }
    FlightData::~FlightData(){};
} // namespace tello_protocol