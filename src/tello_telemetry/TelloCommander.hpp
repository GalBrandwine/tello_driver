#pragma once
#include "utils/movement_commands/MovementCommandsManager.hpp"
#include "utils/movement_commands/MovementsToPacketConverter.hpp"
#include "protocol.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils/ISender.hpp"
#include "utils/ISendAckLog.hpp"
#include "utils/ISendStickCommands.hpp"
namespace tello_protocol
{
    class TelloCommander : public ISendAckLog, public ISendStickCommands
    {
    public:
        /**
         * \section Movements-section
         * \brief This section expose movements commands, as implemented in [TelloPy](https://github.com/hanyazou/TelloPy/blob/develop-0.7.0/tellopy/_internal/tello.py#L328)
         * 
        **/
        void Down(int);
        void Up(int);
        void Clockwise(int);
        void CounterClockwise(int);
        void Left(int);
        void Right(int);
        void Backward(int);
        void Forward(int);
        bool SetFastMode(bool);
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
        bool SendStickCommands() override;

        void SendLandReq();

        /**
         * @brief 
         * 
         *     # def set_att_limit(self, limit):
         * self.log.info('set attitude limit=%s (cmd=0x%02x seq=0x%04x)' % (
         * int(limit), ATT_LIMIT_CMD, self.pkt_seq_num))
         * pkt = Packet(ATT_LIMIT_CMD)
         * pkt.add_byte(0x00)        
         * pkt.add_byte(0x00)
         * pkt.add_byte( int(float_to_hex(float(limit))[4:6], 16) ) # 'attitude limit' formatted in float of 4 bytes
         * pkt.add_byte(0x41)
         * pkt.fixup()
         * self.send_packet(pkt)
         * self.get_att_limit()

         * 
         */
        void SetAttLimitReq(int limit);

        /**
         * @brief Send ATT_LIMIT_MSG to drone.
         * In response the drone will send back ATT_LIMIT_MSG with att_limit data.
         * 
         *     # def get_att_limit(self):
         * ''' ... '''
         * self.log.debug('get attitude limit (cmd=0x%02x seq=0x%04x)' % (
         * ATT_LIMIT_MSG, self.pkt_seq_num))
         * pkt = Packet(ATT_LIMIT_MSG)
         * pkt.fixup()
         * return self.send_packet(pkt)
         */
        void GetAttLimitReq();

        /**
         * @brief Sen SetAltLimit command, with new limit.
         * After sending the packed, call GetAltLimitReq().
         * So drone will send back as acknowledeg the new limit.
         * 
         * @param limit - new altitude limit.
         */
        void SetAltLimitReq(int limit);

        /**
         * @brief Send GetAltLimit request.
         * This will result with an incomming data from drone with id tello_protocol::ALT_LIMIT_MSG.
         * 
         * @todo Add observer on this incomming data to TelloTelemetry.
         * 
         */
        void GetAltLimitReq();

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
        void SendAckLog(const int id) override;

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
