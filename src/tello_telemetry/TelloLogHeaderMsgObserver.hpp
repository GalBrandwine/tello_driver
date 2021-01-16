#pragma once
#include "utils/tello_observer/IObserver.hpp"
#include "TelloTelemetry.hpp"
#include "TelloCommander.hpp"

namespace tello_protocol
{
    class TelloLogHeaderMsgObserver : public IObserver
    {
    public:
        void Update(const std::vector<unsigned char> &message_from_subject) override;
        TelloLogHeaderMsgObserver(TelloTelemetry &telemetry, TelloCommander &commander, std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum lvl = spdlog::level::info);
        virtual ~TelloLogHeaderMsgObserver();

    private:
        TelloTelemetry &m_telemetry;
        TelloCommander &m_commander;
        std::shared_ptr<spdlog::logger> m_logger;
    };
} // namespace tello_protocol
