
#include "TelloDriver/TelloDriver.hpp"

void TelloDriver::Connect()
{
}
TelloDriver::TelloDriver()
    : m_BaseLogger(spdlog::stdout_color_mt("TelloDriver")),
    m_TelloTelemetry(tello_protocol::TelloTelemetry(spdlog::stdout_color_mt("tello_telemetry"),9000))
{
    m_BaseLogger->info("TelloDriver created!");
    
    auto flight_data = std::make_shared<tello_protocol::FlightData>(spdlog::stdout_color_mt("flight_data"));
    m_TelloTelemetry.SetFlightData(std::move(flight_data));
    
    auto log_data = std::make_shared<tello_protocol::LogData>(spdlog::stdout_color_mt("log_data"));
    m_TelloTelemetry.SetLogData(std::move(log_data));
}

TelloDriver::~TelloDriver()
{
}