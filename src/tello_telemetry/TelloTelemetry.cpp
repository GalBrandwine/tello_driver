#include "TelloTelemetry.hpp"

namespace tello_protocol
{
    udp::socket &TelloInputDataSocket::GetSocket()
    {
        return m_tello_socket;
    }

    TelloInputDataSocket::TelloInputDataSocket(int port)
        : m_port(port),
          m_tello_socket(io_service_, udp::endpoint(udp::v4(), port))
    {
    }

    bool TelloTelemetry::process_data(const std::vector<unsigned char> &data)
    {
        auto received = tello_protocol::Packet(data);

        auto cmd = uint16(received.GetBuffer()[5], received.GetBuffer()[6]);
        if (cmd == tello_protocol::LOG_HEADER_MSG)
        {
            // LOG HEADER MSG is the biggest log message.
            // Drone will continue to send log_header connection requests with incremental ID.
            // Until sending back to drone 'ack_conn'.
            auto id = uint16(data[9], data[10]);

            SetBuildDate(received.GetBuffer().substr(28, 26));
            // DJI LOG VERSION something like this: DJI_LOG_V3I��Rc
            SetDJILogVersion(received.GetBuffer().substr(245, 6));

            // After sending back ack. the drone will not sent LOG_HEADER_MSG anymore.
            // send_ack_log(id);
            SetLogHeaderReceived();
        }
        else if (cmd == tello_protocol::LOG_DATA_MSG)
        {
            /* 
            Is cmd  is LOG_DATA_MSG.
            It means that a LOG_DATA connections has been already assured. 
            So this test is pseudo-passed.
            */
            // is_log_header_received = true;
            // keep_receiving = false;
        }

        std::fill(m_buffer.begin(), m_buffer.end(), 0);
        return true;
    }

    void TelloTelemetry::StartListening()
    {
        while (m_keep_receiving) // && !is_log_header_received
        {
            r = m_socket->GetSocket().receive(asio::buffer(m_buffer));

            std::vector<unsigned char> data(m_buffer.begin(), m_buffer.begin() + r);
            if (!process_data(data))
            {
                m_logger->error("Could not process data! Dumping:\n {DATA SUPPOSE TO BE HERE}");
                StopListening();
            }
        }
        m_socket->GetSocket().close();
    }

    void TelloTelemetry::SetLogHeaderReceived() { m_IsLogHeaderReceived = true; }
    bool TelloTelemetry::IsLogHeaderReceived() const { return m_IsLogHeaderReceived; }

    void TelloTelemetry::SetLogData(std::shared_ptr<LogData> log_data) { m_LogData = log_data; }
    std::shared_ptr<LogData> TelloTelemetry::GetLogData() const { return m_LogData; }

    void TelloTelemetry::SetFlightData(std::shared_ptr<FlightData> flight_data) { m_FlightData = flight_data; }
    std::shared_ptr<FlightData> TelloTelemetry::GetFlightData() const { return m_FlightData; }

    void TelloTelemetry::SetDJILogVersion(const std::string &log_version)
    {
        m_DJI_LOG_VERSION = log_version;
        m_logger->info("DJI LOG VERSION: {};", m_DJI_LOG_VERSION);
    }
    const std::string &TelloTelemetry::GetDJILogVersion() const { return m_DJI_LOG_VERSION; }

    void TelloTelemetry::SetBuildDate(const std::string &build_date)
    {
        m_BuildDate = build_date;
        m_logger->info("BUILD date: {};", m_BuildDate);
    }
    const std::string &TelloTelemetry::GetBuildDate() const { return m_BuildDate; }

    TelloTelemetry::TelloTelemetry(std::shared_ptr<spdlog::logger> logger)
        : m_logger(logger)

    {
    }
    TelloTelemetry::TelloTelemetry(std::shared_ptr<spdlog::logger> logger, int port)
        : m_logger(logger)

    {
        m_logger->info(m_logger->name() + " Initiated. Receiving port: {}", port);
        m_socket = std::make_shared<TelloInputDataSocket>(port);
        m_buffer.reserve(1024);
    }

    TelloTelemetry::~TelloTelemetry()
    {
    }
} // namespace tello_protocol
