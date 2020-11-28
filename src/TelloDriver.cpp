
#include "TelloDriver/TelloDriver.hpp"

const float TelloDriver::GetAttLimit()
{
    return m_TelloTelemetry.GetFlightData()->GetAttLimit();
}

const short TelloDriver::GetAltLimit()
{
    return m_TelloTelemetry.GetFlightData()->GetAltLimit();
}
const short TelloDriver::GetWifiStrength()
{

    return m_TelloTelemetry.GetFlightData()->GetWifiStrength();
}
const tello_protocol::Vec3 TelloDriver::GetPos()
{
    return m_TelloTelemetry.GetLogData()->GetLogMvo().GetPos();
}
tello_protocol::TelloCommander &TelloDriver::GetTelloCommander()
{
    return m_TelloCommander;
}

tello_protocol::TelloTelemetry &TelloDriver::GetTelloTelemetry()
{
    return m_TelloTelemetry;
}

void TelloDriver::Land()
{
    m_TelloCommander.SendLandReq();
}
void TelloDriver::Takeoff()
{
    m_TelloCommander.SendTakeoffReq();
}
void TelloDriver::Connect()
{
    m_TelloCommander.SendConnReq();
    m_TelloTelemetry.StartListening();
}

bool TelloDriver::WaitForConnection(int timeout)
{

    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    fsec fs;
    auto t0 = Time::now();

    while (!m_TelloTelemetry.IsLogHeaderReceived())
    {

        auto t1 = Time::now();
        fs = t1 - t0;
        auto passed = fs.count();
        if (passed == (int)passed) // Print once a  second
            m_BaseLogger->info("Waiting for connection... {}", timeout - fs.count());
        if (fs.count() > timeout)
            return false;
    }

    // After sending back ack. the drone will not sent LOG_HEADER_MSG anymore.
    m_TelloCommander.SendAckLog(m_TelloTelemetry.GetLogHeaderId());
    return true;
}

TelloDriver::TelloDriver(spdlog::level::level_enum lvl)
    : m_BaseLogger(spdlog::stdout_color_mt("TelloDriver")),
      m_TelloTelemetry(spdlog::stdout_color_mt("tello_telemetry"), lvl),
      m_TelloCommander(spdlog::stdout_color_mt("tello_commander"), lvl)
{
    m_BaseLogger->info("TelloDriver created! version: {}", TelloDriverVersion);
    m_BaseLogger->set_level(lvl);
    
    auto flight_data = std::make_shared<tello_protocol::FlightData>(spdlog::stdout_color_mt("flight_data"));
    m_TelloTelemetry.SetFlightData(std::move(flight_data));

    auto log_data = std::make_shared<tello_protocol::LogData>(spdlog::stdout_color_mt("log_data"));
    m_TelloTelemetry.SetLogData(std::move(log_data));

    auto shared_socket = std::make_shared<TelloSocket>("192.168.10.1", 8889, 9000);
    m_TelloTelemetry.SetSocket(shared_socket);
    m_TelloCommander.SetSocket(shared_socket);
}

TelloDriver::~TelloDriver()
{
    m_BaseLogger->info(m_BaseLogger->name() + " Destructing!");
}