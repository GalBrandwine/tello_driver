#pragma once
#include <iostream>
#include "protocol.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils/telemetry_data/TelemetryData.hpp"
#include "IFlightDataGetter.hpp"
namespace tello_protocol
{

    class FlightData : public IFlightDataGetter
    {
    public:
        /**
         * @brief Fill the Flight Data struct
         * 
         * @param[out] flight_data_out 
         */
        void GetFlightData(tello_protocol::FlightDataStruct &flight_data_out) override;

        bool Update(const std::vector<unsigned char> &data);

        FlightData(std::shared_ptr<spdlog::logger>);
        ~FlightData();

    private:
        std::shared_ptr<spdlog::logger> m_logger;
       
        double m_imu_calibration_state = -1;
        double m_imu_state = -1;
        double m_light_strength = -1;
        double m_outage_recording = -1;
        double m_power_state = -1;
        double m_pressure_state = -1;
        double m_smart_video_exit_mode = -1;
        double m_temperature_height = -1;
        double m_throw_fly_timer = -1;
        double m_wifi_disturb = -1;
        short m_wifi_strength = -1;
        // short m_alt_limit = -1;
        double m_wind_state = -1;
        float m_attitude_limit = -1;
        short m_low_battery_threshold = -1;
        FlightDataStruct m_flight_data;
    };
} // namespace tello_protocol