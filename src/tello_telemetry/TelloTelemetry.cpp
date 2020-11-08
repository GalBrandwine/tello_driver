#include "TelloTelemetry.hpp"

namespace tello_protocol
{

    bool TelloTelemetry::process_data(const std::vector<unsigned char> &data)
    {
        auto received = tello_protocol::Packet(data);

        auto cmd = uint16(received.GetBuffer()[5], received.GetBuffer()[6]);
        m_logger->info("cmd: {}", cmd);
        if (cmd == tello_protocol::LOG_HEADER_MSG)
        {
            m_logger->info("LOG_HEADER_MSG received");
            // LOG HEADER MSG is the biggest log message.
            // Drone will continue to send log_header connection requests with incremental ID.
            // Until sending back to drone 'ack_conn'.
            auto id = uint16(data[9], data[10]);

            SetBuildDate(received.GetBuffer().substr(28, 26));
            // DJI LOG VERSION something like this: DJI_LOG_V3I��Rc
            auto f = received.GetBuffer().find("DJI");
            SetDJILogVersion(received.GetBuffer().substr(f, 15));

            // After sending back ack. the drone will not sent LOG_HEADER_MSG anymore.
            if (m_TelloCommander)
                m_TelloCommander->SendAckLog(id);

            SetLogHeaderReceived();
        }
        else if (cmd == tello_protocol::LOG_DATA_MSG)
        {
            m_logger->info("LOG_DATA_MSG received");
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

    void TelloTelemetry::Listener()
    {
        while (m_keep_receiving) // && !is_log_header_received
        {
            m_BytesReceived = m_socket->Recieve(m_buffer);
            m_logger->info("Bytes received: {}", m_BytesReceived);
            std::vector<unsigned char> data(m_buffer.begin(), m_buffer.begin() + m_BytesReceived);
            if (!process_data(data))
            {
                m_logger->error("Could not process data! Dumping:\n {DATA SUPPOSE TO BE HERE}");
                // StopListening();
            }
        }
    }

    void TelloTelemetry::SetTelloCommander(std::shared_ptr<tello_protocol::TelloCommander> telloCommander)
    {
        m_TelloCommander = telloCommander;
    };

    void TelloTelemetry::SetLogHeaderReceived()
    {
        m_IsLogHeaderReceived = true;
    };
    bool TelloTelemetry::IsLogHeaderReceived() const
    {
        return m_IsLogHeaderReceived;
    }

    void TelloTelemetry::SetLogData(std::shared_ptr<LogData> log_data)
    {
        m_LogData = log_data;
    }
    std::shared_ptr<LogData> TelloTelemetry::GetLogData() const
    {
        return m_LogData;
    }

    void TelloTelemetry::SetFlightData(std::shared_ptr<FlightData> flight_data)
    {
        m_FlightData = flight_data;
    }
    std::shared_ptr<FlightData> TelloTelemetry::GetFlightData() const
    {
        return m_FlightData;
    }

    void TelloTelemetry::SetDJILogVersion(const std::string &log_version)
    {
        m_DJI_LOG_VERSION = log_version;
        m_logger->info("DJI LOG VERSION: {};", m_DJI_LOG_VERSION);
    }
    const std::string &TelloTelemetry::GetDJILogVersion() const
    {
        return m_DJI_LOG_VERSION;
    }

    void TelloTelemetry::SetBuildDate(const std::string &build_date)
    {
        m_BuildDate = build_date;
        m_logger->info("BUILD date: {};", m_BuildDate);
    }
    const std::string &TelloTelemetry::GetBuildDate() const { return m_BuildDate; }

    void TelloTelemetry::StartListening()
    {
        m_keep_receiving = true;
        m_Listener = std::thread(&TelloTelemetry::Listener, this);
        m_logger->info(m_logger->name() + " Listener thread started");
    }

    void TelloTelemetry::SetSocket(std::shared_ptr<IReciever> socket)
    {
        m_socket = socket;
    }

    TelloTelemetry::TelloTelemetry(std::shared_ptr<spdlog::logger> logger)
        : m_logger(logger)
    {
        m_logger->info(m_logger->name() + " Initiated.");
        // m_buffer.reserve(1024);
        m_buffer = std::vector<unsigned char>(1024);
    }

    TelloTelemetry::~TelloTelemetry()
    {
        m_logger->info(m_logger->name() + " Destructing.");
        m_keep_receiving = false;
        if (m_Listener.joinable())
            m_Listener.join();
    }

} // namespace tello_protocol
