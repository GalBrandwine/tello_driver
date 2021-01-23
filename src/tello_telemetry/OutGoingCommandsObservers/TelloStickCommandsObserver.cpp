#include "TelloStickCommandsObserver.hpp"

namespace tello_protocol
{
    void TelloStickCommandsObserver::Update(const std::vector<unsigned char> &message_from_subject)
    {
        m_commander.SendStickCommands();
    }

    TelloStickCommandsObserver::TelloStickCommandsObserver(TelloTelemetry &telemetry, ISendStickCommands &commander, std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum lvl)
        : m_telemetry(telemetry),
          m_commander(commander),
          m_logger(logger)
    {
        m_logger->set_level(lvl);
        m_logger->info(m_logger->name() + " Initiated.");

        this->m_telemetry.Attach(this);
    }

    TelloStickCommandsObserver::~TelloStickCommandsObserver()
    {
    }
} // namespace tello_protocol
