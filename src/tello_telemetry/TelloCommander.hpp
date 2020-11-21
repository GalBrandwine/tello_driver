#pragma once
#include "protocol.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils/ISender.hpp"
namespace tello_protocol
{
    class TelloCommander
    {
    public:
        void SendLandReq();
        void SetAltLimitReq(int);
        void SendTakeoffReq();
        void SendConnReq();
        void SendAckLog(const int id);
        void SetSocket(std::shared_ptr<ISender>);
        TelloCommander(std::shared_ptr<spdlog::logger>, spdlog::level::level_enum lvl = spdlog::level::info);
        ~TelloCommander();

    private:
        void sendCommand(const tello_protocol::Packet &);
        std::shared_ptr<spdlog::logger> m_logger;
        std::shared_ptr<ISender> m_socket;
    };

} // namespace tello_protocol
