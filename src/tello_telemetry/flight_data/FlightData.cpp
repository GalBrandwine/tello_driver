#include "flight_data/FlightData.hpp"

namespace tello_protocol
{
    /**
     * @brief Set alt_low_bat as received back from drone.
     * This message is sent back after a SET_LOW_BAT command
     * 
     * @todo Move this into DataManager, and attach an observer to TelloTelemetr.
     * So if User call SET_LOW_BAT command,
     * the observer will catch the incomming reponse and insert the data into DataManager.
     * 
     * @param lowbatthreshold 
     */
    void FlightData::SetLowBatThreshold(const std::string &lowbatthreshold)
    {
        std::memcpy(&m_low_battery_threhold, &lowbatthreshold[1], sizeof(short));
        m_logger->info("Setting LowBatThreshold: {}%", m_low_battery_threhold);
    }

    /**
     * @brief Set attlimit as received back from drone.
     * This message is sent back after a SET_ATT_LIMIT command
     * 
     * @todo Move this into DataManager, and attach an observer to TelloTelemetr.
     * So if User call SET_ATT_LIMIT command,
     * the observer will catch the incomming reponse and insert the data into DataManager.
     * 
     * @param attlimit 
     */
    void FlightData::SetAttLimit(const std::string &attlimit)
    {
        std::memcpy(&m_attitude_limit, &attlimit[1], sizeof(float));
        m_logger->info("Setting Att: {}[deg]", m_attitude_limit);
    }

    /**
     * @brief Set alt_limit as received back from drone.
     * This message is sent back after a SET_ALT_LIMIT command
     * 
     * @todo Move this into DataManager, and attach an observer to TelloTelemetr.
     * So if User call SET_ALT_LIMIT command,
     * the observer will catch the incomming reponse and insert the data into DataManager.
     * 
     * @param altlimit 
     */
    void FlightData::SetAltLimit(const std::string &altlimit)
    {
        std::memcpy(&m_alt_limit, &altlimit[1], sizeof(short));
        m_logger->info("Setting AltLimit: {}[m]", m_alt_limit);
    }

    const FlightDataStruct &FlightData::GetFlightData()
    {
        return m_flight_data;
    }

    bool FlightData::Update(const std::vector<unsigned char> &data)
    {
        if (data.size() < 24)
        {
            if (m_logger)
                m_logger->warn("data length didnt match.");
            return false;
        }

        short temp;
        std::memcpy(&m_flight_data.height, &data[0], sizeof(short));
        // m_logger->warn("data {}", temp);
        // m_flight_data.height = temp;

        // m_height = int16(data[0], data[1]);

        // NED speeds
        std::memcpy(&m_flight_data.north_speed, &data[2], sizeof(short));
        // m_flight_data.north_speed = temp;
        // m_north_speed = int16(data[2], data[3]);

        std::memcpy(&m_flight_data.east_speed, &data[4], sizeof(short));
        // m_flight_data.east_speed = temp;
        // m_east_speed = int16(data[4], data[5]);
        // m_logger->warn("east_speed {}", m_east_speed);

        // Other method for storing data from buffer to reference:
        // short temp;
        // std::memcpy(&temp, &data[6], sizeof(short));

        std::memcpy(&m_flight_data.ground_speed, &data[6], sizeof(short));
        // m_flight_data.ground_speed = temp;
        // m_ground_speed = int16(data[6], data[7]);
        // m_logger->warn("ground_speed {}", m_ground_speed);

        std::memcpy(&m_flight_data.fly_time, &data[8], sizeof(short));
        // m_logger->warn("fly_time {}", temp);

        // m_fly_time = int16(data[8], data[9]);
        // m_logger->warn("fly_time {}", temp);

        // std::memcpy(&temp, &data[10], sizeof(char));
        // m_logger->warn("fly_time {}", temp);
        // m_flight_data.fly_time = temp;

        // m_imu_state = ((data[10] >> 0) & 0x1);
        m_flight_data.flight_data_states.imu_state = bool((data[10] >> 0) & 0x1);
        // m_logger->warn("imu_state {}", m_flight_data.flight_data_states.imu_state);
        // m_logger->warn("m_imu_state {}", m_imu_state);
        // m_logger->warn("");

        // m_pressure_state = ((data[10] >> 1) & 0x1);
        m_flight_data.flight_data_states.pressure_state = bool((data[10] >> 1) & 0x1);
        // m_logger->warn("pressure_state {}", m_flight_data.flight_data_states.pressure_state);
        // m_logger->warn("m_pressure_state {}", m_imu_state);
        // m_logger->warn("");

        // m_down_visual_state = ((data[10] >> 2) & 0x1);
        m_flight_data.flight_data_states.down_visual_state = bool((data[10] >> 2) & 0x1);
        // m_logger->warn("down_visual_state {}", m_flight_data.flight_data_states.down_visual_state);
        // m_logger->warn("m_down_visual_state {}", m_down_visual_state);
        // m_logger->warn("");

        // m_power_state = ((data[10] >> 3) & 0x1);
        m_flight_data.flight_data_states.power_state = bool((data[10] >> 3) & 0x1);
        // m_logger->warn("power_state {}", m_flight_data.flight_data_states.power_state);
        // m_logger->warn("m_power_state {}", m_power_state);
        // m_logger->warn("");

        // m_battery_state = ((data[10] >> 4) & 0x1);
        m_flight_data.flight_data_states.battery_state = bool((data[10] >> 4) & 0x1);
        // m_logger->warn("battery_state {}", m_flight_data.flight_data_states.battery_state);
        // m_logger->warn("m_battery_state {}", m_battery_state);
        // m_logger->warn("");

        // m_gravity_state = ((data[10] >> 5) & 0x1);
        m_flight_data.flight_data_states.gravity_state = bool((data[10] >> 5) & 0x1);
        // m_logger->warn("gravity_state {}", m_flight_data.flight_data_states.gravity_state);
        // m_logger->warn("m_gravity_state {}", m_gravity_state);
        // m_logger->warn("");

        // m_wind_state = ((data[10] >> 7) & 0x1);
        m_flight_data.flight_data_states.wind_state = bool((data[10] >> 7) & 0x1);
        // m_logger->warn("wind_state {}", m_flight_data.flight_data_states.wind_state);
        // m_logger->warn("m_wind_state {}", m_wind_state);
        // m_logger->warn("");

        // m_imu_calibration_state = data[11];
        m_flight_data.camera_state = data[11];
        // m_battery_percentage = data[12];
        m_flight_data.battery_percentage = data[12];

        // m_drone_battery_left = int16(data[13], data[14]);
        std::memcpy(&m_flight_data.drone_battery_left, &data[13], sizeof(short));
        // m_drone_fly_time_left = int16(data[15], data[16]);
        std::memcpy(&m_flight_data.drone_fly_time_left, &data[15], sizeof(short));

        // m_em_sky = ((data[17] >> 0) & 0x1);
        m_flight_data.flight_data_extras.em_sky = bool((data[17] >> 0) & 0x1);
        // m_em_ground = ((data[17] >> 1) & 0x1);
        m_flight_data.flight_data_extras.em_ground = bool((data[17] >> 1) & 0x1);
        // m_em_open = ((data[17] >> 2) & 0x1);
        m_flight_data.flight_data_extras.em_open = bool((data[17] >> 2) & 0x1);
        // m_drone_hover = ((data[17] >> 3) & 0x1);
        m_flight_data.flight_data_extras.drone_hover = bool((data[17] >> 3) & 0x1);
        // m_outage_recording = ((data[17] >> 4) & 0x1);
        m_flight_data.flight_data_extras.outage_recording = bool((data[17] >> 4) & 0x1);
        // m_battery_low = ((data[17] >> 5) & 0x1);
        m_flight_data.flight_data_extras.battery_low = bool((data[17] >> 5) & 0x1);
        // m_battery_lower = ((data[17] >> 6) & 0x1);
        m_flight_data.flight_data_extras.battery_lower = bool((data[17] >> 6) & 0x1);
        // m_factory_mode = ((data[17] >> 7) & 0x1);
        m_flight_data.flight_data_extras.factory_mode = bool((data[17] >> 7) & 0x1);

        // m_fly_mode = data[18];
        m_flight_data.fly_mode = data[18];
        m_logger->warn("fly_mode: {}", std::to_string(m_flight_data.fly_mode));
        // m_throw_fly_timer = data[19];
        m_flight_data.throw_fly_timer = data[19];
        // m_camera_state = data[20];
        m_flight_data.camera_state = data[20];
        // m_electrical_machinery_state = data[21];
        m_flight_data.electrical_machinery_state = data[21];
        // m_front_in = ((data[22] >> 0) & 0x1);
        m_flight_data.front_info.front_in = bool((data[22] >> 0) & 0x1);
        // m_front_out = ((data[22] >> 1) & 0x1);
        m_flight_data.front_info.front_out = bool((data[22] >> 1) & 0x1);
        // m_front_lsc = ((data[22] >> 2) & 0x1);
        m_flight_data.front_info.front_lsc = bool((data[22] >> 3) & 0x1);
        m_flight_data.temperature_height = bool((data[23] >> 0) & 0x1);
        // m_temperature_height = ((data[23] >> 0) & 0x1);

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
    FlightData::~FlightData()
    {
        m_logger->info(m_logger->name() + " Destructing.");
    };
} // namespace tello_protocol