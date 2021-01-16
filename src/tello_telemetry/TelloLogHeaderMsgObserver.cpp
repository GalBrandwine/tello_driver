#include "TelloLogHeaderMsgObserver.hpp"

namespace tello_protocol
{
    void TelloLogHeaderMsgObserver::Update(const std::vector<unsigned char> &message_from_subject)
    {
        tello_protocol::Packet received(message_from_subject);

        unsigned short cmd;
        std::memcpy(&cmd, &message_from_subject[5], sizeof(unsigned short));

        if (cmd == tello_protocol::LOG_HEADER_MSG)
        {
            m_logger->debug("LOG_HEADER_MSG received");

            unsigned short log_id;
            std::memcpy(&log_id, &message_from_subject[9], sizeof(unsigned short));
            
            m_commander.SendAckLog(log_id);
            
            // m_telemetry.SetLogHeaderReceived();
            m_telemetry.SetBuildDate(received.GetBuffer().substr(28, 26));

            // DJI LOG VERSION something like this: DJI_LOG_V3I��Rc
            auto f = received.GetBuffer().find("DJI");
            m_telemetry.SetDJILogVersion(received.GetBuffer().substr(f, 15));
        }
    }

    TelloLogHeaderMsgObserver::TelloLogHeaderMsgObserver(TelloTelemetry &telemetry, TelloCommander &commander, std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum lvl)
        : m_telemetry(telemetry),
          m_commander(commander),
          m_logger(logger)
    {
        m_logger->set_level(lvl);
        m_logger->info(m_logger->name() + " Initiated.");

        this->m_telemetry.Attach(this);
    }

    TelloLogHeaderMsgObserver::~TelloLogHeaderMsgObserver()
    {
    }
} // namespace tello_protocol
