
#include "TelloDriver/TelloDriver.hpp"

/**
 * \section Movements section
**/
void TelloDriver::CounterClockwise(int amount)
{
    assert(amount >= 0 && amount <= 100);
    m_TelloCommander.CounterClockwise(amount);
}
void TelloDriver::Clockwise(int amount)
{
    assert(amount >= 0 && amount <= 100);
    m_TelloCommander.Clockwise(amount);
}
void TelloDriver::Right(int amount)
{
    assert(amount >= 0 && amount <= 100);
    m_TelloCommander.Right(amount);
}
void TelloDriver::Left(int amount)
{
    assert(amount >= 0 && amount <= 100);
    m_TelloCommander.Left(amount);
}
void TelloDriver::Down(int amount)
{
    assert(amount >= 0 && amount <= 100);
    m_TelloCommander.Down(amount);
}
void TelloDriver::Up(int amount)
{
    assert(amount >= 0 && amount <= 100);
    m_TelloCommander.Up(amount);
}
void TelloDriver::Backward(int amount)
{
    assert(amount >= 0 && amount <= 100);
    m_TelloCommander.Backward(amount);
}
void TelloDriver::Forward(int amount)
{
    assert(amount >= 0 && amount <= 100);
    m_TelloCommander.Forward(amount);
}

void TelloDriver::SetAltLimitReq(int alt_limit)
{
    m_TelloCommander.SetAltLimitReq(alt_limit);
}

// const float TelloDriver::GetAttLimit()
// {
//     // return m_TelloTelemetry.GetFlightData()->GetAttLimit();
// }

// const short TelloDriver::GetAltLimit()
// {
//     // return m_TelloTelemetry.GetFlightData()->GetAltLimit();
// }
// const short TelloDriver::GetWifiStrength()
// {
//     // return m_TelloTelemetry.GetFlightData()->GetWifiStrength();
// }

// const tello_protocol::Vec3 TelloDriver::GetPos()
// {
//     // return m_TelloTelemetry.GetLogData()->GetLogMvo().GetPos();
// }

// tello_protocol::TelloCommander &TelloDriver::GetTelloCommander()
// {
//     return m_TelloCommander;
// }

// tello_protocol::TelloTelemetry &TelloDriver::GetTelloTelemetry()
// {
//     return m_TelloTelemetry;
// }

void TelloDriver::Land()
{
    m_TelloCommander.SendLandReq();
}
void TelloDriver::Takeoff()
{
    m_TelloCommander.SendTakeoffReq();
}

void TelloDriver::Attach(OBSERVERS oberver_type, IObserver *observer)
{
    m_BaseLogger->info("Attaching " + observer_name(oberver_type));
    m_DataManager.Attach(oberver_type, observer);
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

    m_TelloCommander.SendConnReq();
    while (!m_DataManager.IsConnReqAckReceived())
    {
        m_TelloCommander.SendConnReq();
        auto t1 = Time::now();
        fs = t1 - t0;
        auto passed = fs.count();
        if (passed == (int)passed) // Print once a  second
            m_BaseLogger->info("Waiting for connection... {}", timeout - fs.count());
        if (fs.count() > timeout)
            return false;

        std::this_thread::sleep_for(50ms);
    }

    return true;
}

TelloDriver::TelloDriver(spdlog::level::level_enum lvl)
    : m_BaseLogger(spdlog::stdout_color_mt("TelloDriver")),
      m_TelloTelemetry(spdlog::stdout_color_mt("tello_telemetry"), lvl),
      m_TelloCommander(spdlog::stdout_color_mt("tello_commander"), lvl),
      m_DataManager(spdlog::stdout_color_mt("data_manager"), spdlog::level::debug)
{
    m_BaseLogger->info("TelloDriver created! version: {}", TelloDriverVersion);
    m_BaseLogger->set_level(lvl);

    auto shared_socket = std::make_shared<TelloSocket>("192.168.10.1", 8889, 9000);
    m_TelloTelemetry.SetSocket(shared_socket);
    m_TelloCommander.SetSocket(shared_socket);

    /**
     * @brief Observers initiation.
     * 
     */

    // m_TelloConnAckMsgObserver = std::make_shared<tello_protocol::TelloConnAckMsgObserver>(m_TelloTelemetry, m_DataManager, spdlog::stdout_color_mt("TelloConnAckMsgObserver"), lvl);
    m_TelloAckLogHeaderIdSenderObserver = std::make_shared<tello_protocol::TelloAckLogHeaderIdSenderObserver>(m_TelloCommander, m_DataManager, spdlog::stdout_color_mt("TelloAckLogHeaderIdSenderObserver"), lvl);
    m_TelloWifiMsgObserver = std::make_shared<tello_protocol::TelloWifiMsgObserver>(m_TelloTelemetry, m_DataManager, spdlog::stdout_color_mt("TelloWifiMsgObserver"), lvl);
    m_TelloLogDataMsgObserver = std::make_shared<tello_protocol::TelloLogDataMsgObserver>(m_TelloTelemetry, m_DataManager, spdlog::stdout_color_mt("TelloLogDataMsgObserver"), lvl);
    m_TelloFlightDataMsgObserver = std::make_shared<tello_protocol::TelloFlightDataMsgObserver>(m_TelloTelemetry, m_DataManager, spdlog::stdout_color_mt("TelloFlightDataMsgObserver"), lvl);
    m_TelloLogHeaderMsgObserver = std::make_shared<tello_protocol::TelloLogHeaderMsgObserver>(m_TelloTelemetry, m_DataManager, spdlog::stdout_color_mt("TelloLogHeaderMsgObserver"), lvl);
    m_TelloStickCommandsObserver = std::make_shared<tello_protocol::TelloStickCommandsObserver>(m_TelloTelemetry, m_TelloCommander, m_DataManager, spdlog::stdout_color_mt("StickCommandsObserver"), lvl);
}

TelloDriver::~TelloDriver()
{
    m_BaseLogger->info(m_BaseLogger->name() + " Destructed!");
}