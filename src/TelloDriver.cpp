
#include "TelloDriver/TelloDriver.hpp"

tello_protocol::TelloCommander &TelloDriver::GetTelloCommander()
{
    return m_TelloCommander;
}

tello_protocol::TelloTelemetry &TelloDriver::GetTelloTelemetry()
{
    return m_TelloTelemetry;
}

void TelloDriver::Connect()
{
    m_TelloCommander.SendConnReq();
    m_TelloTelemetry.StartListening();
}

bool TelloDriver::WaitForConnection()
{
    int timeout_counter = 0;
    while (!m_TelloTelemetry.IsLogHeaderReceived())
    {
        m_BaseLogger->info("Waiting for connection... {}", CONNECTION_TIMEOUT - timeout_counter);
        if (timeout_counter++ > CONNECTION_TIMEOUT)
            return false;
        std::this_thread::sleep_for(0.5s);
    }
    return true;
}

TelloDriver::TelloDriver()
    : m_BaseLogger(spdlog::stdout_color_mt("TelloDriver")),
      m_TelloTelemetry(spdlog::stdout_color_mt("tello_telemetry")),
      m_TelloCommander(spdlog::stdout_color_mt("tello_commander"))
{
    m_BaseLogger->info("TelloDriver created!");

    auto flight_data = std::make_shared<tello_protocol::FlightData>(spdlog::stdout_color_mt("flight_data"));
    m_TelloTelemetry.SetFlightData(std::move(flight_data));

    auto log_data = std::make_shared<tello_protocol::LogData>(spdlog::stdout_color_mt("log_data"));
    m_TelloTelemetry.SetLogData(std::move(log_data));
    
    auto shared_socket = std::make_shared<TelloSocket>("192.168.10.1", 8889, 9000);
    m_TelloTelemetry.SetSocket(shared_socket);
    m_TelloCommander.SetSocket(shared_socket);

    // Inject TelloTelemtry the TelloCommander, so upon special messages TelloTelemetry could respond with ACK.
    m_TelloTelemetry.SetTelloCommander(std::make_shared<tello_protocol::TelloCommander>(m_TelloCommander));
}

TelloDriver::~TelloDriver()
{
}