#pragma once
#include "utils/movement_commands/MovementCommandsManager.hpp"
#include "utils/movement_commands/MovementsToPacketConverter.hpp"
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
        /**
         * \section Movements-section
         * \brief This section expose movements commands, as implemented in [TelloPy](https://github.com/hanyazou/TelloPy/blob/develop-0.7.0/tellopy/_internal/tello.py#L328)
         * 
        **/
        void Backward(int);
        void Forward(int);

        /**
         * @brief Send stick commands.
         * This works like a *keep-alive*, to the drone.
         * If not sent for too much time. The TelloDrone will assume that companion (E.G remote, other controller) lost connection.
         * Thus drone will stand put.
         * 
         * 1. Create empty packet with STICK_CMD header.
         * 2. Get movements from MovementCommandsManager
         * 3. Convert them stickCommandt into packet data.
         * 4. Send to drone.
         * 
         * @return true if packet converter successfully
         * @return false otherwise
         */
        bool SendStickCommands();

        void SendLandReq();
        void SetAltLimitReq(int);
        void SendTakeoffReq();

        /**
         * @brief Send connection request
         * With request to port 0x9617
         * 
         */
        void SendConnReq();

        /**
         * @brief LOG HEADER MSG is the biggest log message.
         * Drone will continue to send log_header connection requests with incremental ID.
         * Until sending back to drone 'ack_conn'.
         * 
         * @param id - same ID that drone sent to client. We must return *ack* with this ID ASAP,
         * in order to start LOGGING session.
         */
        void SendAckLog(const int id);

        /**
         * @brief Set the Socket object
         * 
         */
        void SetSocket(std::shared_ptr<ISender>);

        TelloCommander(std::shared_ptr<spdlog::logger>, spdlog::level::level_enum lvl = spdlog::level::info);
        ~TelloCommander();

    private:
        std::shared_ptr<spdlog::logger> m_logger;
        std::shared_ptr<ISender> m_socket;
        MovementCommandsManager m_MovementCommandsManager;
        MovementsToPacketConverter m_MovementsToPacketConverter;
    };

} // namespace tello_protocol
