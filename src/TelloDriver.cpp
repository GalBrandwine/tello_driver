
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

        std::this_thread::sleep_for(50ms);
    }

    // After sending back ack. the drone will not sent LOG_HEADER_MSG anymore.
    m_TelloCommander.SendAckLog(m_TelloTelemetry.GetLogHeaderId());
    return true;
}

TelloDriver::TelloDriver(spdlog::level::level_enum lvl)
    : m_BaseLogger(spdlog::stdout_color_mt("TelloDriver")),
      m_TelloTelemetry(spdlog::stdout_color_mt("tello_telemetry"), lvl),
      m_TelloCommander(spdlog::stdout_color_mt("tello_commander"), lvl)
//   work_io_context_(),
//   work_(asio::make_work_guard(work_io_context_))
{
    m_BaseLogger->info("TelloDriver created! version: {}", TelloDriverVersion);
    m_BaseLogger->set_level(lvl);

    auto flight_data = std::make_shared<tello_protocol::FlightData>(spdlog::stdout_color_mt("flight_data"));
    m_TelloTelemetry.SetFlightData(std::move(flight_data));

    auto log_data = std::make_shared<tello_protocol::LogData>(spdlog::stdout_color_mt("log_data"));
    m_TelloTelemetry.SetLogData(std::move(log_data));

    // We run the io_context off in its own thread so that it operates
    // completely asynchronously with respect to the rest of the program.
    // asio::io_context io_context;
    // auto work = asio::require(io_context.get_executor(),
    //                           asio::execution::outstanding_work.tracked);

    // auto ext = io_context.get_executor();
    // auto work = ext.require(ext.outstanding_work_tracked);
    // m_io_context_thread = std::thread([this]() { io_context.run(); });

    /* 
    Stopping the io_context from running out of work
    Some applications may need to prevent an io_context object's run() call from returning when there is no more work to do. 
    For example, the io_context may be being run in a background thread that is launched prior to the application's asynchronous operations. 
    The run() call may be kept running by creating an object of type boost::asio::executor_work_guard<io_context::executor_type>: 
    */

    // m_io_context_thread = std::thread([this]() { io_context.run(); });
    // // asio::executor_work_guard<asio::io_context::executor_type> g = asio::make_work_guard(io_context);
    asio::io_context work_io_context_;
    asio::executor_work_guard<asio::io_context::executor_type> work_(asio::make_work_guard(work_io_context_));
    std::thread work_thread_(std::thread([&work_io_context_]() {work_io_context_.run(); std::cout << "work_io_context_ finished!\n"; }));

    auto shared_socket = std::make_shared<TelloSocket>(work_io_context_, "192.168.10.1", 8889, 9000);
    m_TelloTelemetry.SetSocket(shared_socket);
    m_TelloCommander.SetSocket(shared_socket);
    // io_context.run();
}

TelloDriver::~TelloDriver()
{
    /// Indicate that we have finished with the private io_context. Its
    /// io_context::run() function will exit once all other work has completed.
    // if (work_ != nullptr)
    //     work_->reset();
    // if (work_thread_.joinable())
    //     work_thread_.join();
    m_BaseLogger->info(m_BaseLogger->name() + " Destructing!");
}