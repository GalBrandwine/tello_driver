#include "TelloCommander.hpp"
namespace tello_protocol
{
    void TelloCommander::Backward(int amount)
    {
        assert(amount >= 0 && amount <= 100);
        if (!m_MovementCommandsManager.SetMovementCommand(tello_protocol::BACKWARD, amount))
        {
            m_logger->warn("Could not set " + std::string(__FUNCTION__) + "(" + std::to_string(amount) + ")");
        }
    }
    void TelloCommander::Forward(int amount)
    {
        assert(amount >= 0 && amount <= 100);
        if (!m_MovementCommandsManager.SetMovementCommand(tello_protocol::FORWARD, amount))
        {
            m_logger->warn("Could not set " + std::string(__FUNCTION__) + "(" + std::to_string(amount) + ")");
        }
    }

    bool TelloCommander::SendStickCommands()
    {
        auto pkt = tello_protocol::Packet(tello_protocol::STICK_CMD, 0x60);
        auto &mvmnts = m_MovementCommandsManager.GetStickMovements();
        if (!m_MovementsToPacketConverter.Convert(mvmnts, pkt))
        {
            m_logger->error("Could not convert stick movements! Something went wrong.");
            return false;
        }
        m_socket->Send(pkt.GetBuffer());
        return true;
    }

    void TelloCommander::SetAltLimitReq(int limit)
    {
        m_logger->debug("Set altitude limit={} (cmd=0x{:x} seq=0x{:x})", limit, tello_protocol::SET_ALT_LIMIT_CMD, 0x01e4);

        auto pkt = tello_protocol::Packet(tello_protocol::SET_ALT_LIMIT_CMD);
        pkt.AddByte(limit);
        pkt.AddByte(0x00);
        m_socket->Send(pkt.GetBuffer());
    }

    void TelloCommander::SendTakeoffReq()
    {
        m_logger->debug("Sending takeoff");
        SetAltLimitReq(30);

        auto pkt = tello_protocol::Packet(tello_protocol::TAKEOFF_CMD);
        pkt.Fixup();
        m_socket->Send(pkt.GetBuffer());
    }

    void TelloCommander::SendLandReq()
    {
        m_logger->debug("Sending land");

        auto pkt = tello_protocol::Packet(tello_protocol::LAND_CMD);
        pkt.AddByte(0x00);
        pkt.Fixup();
        m_socket->Send(pkt.GetBuffer());
    }

    void TelloCommander::SendAckLog(const int id)
    {
        auto pkt = tello_protocol::Packet(tello_protocol::LOG_HEADER_MSG, 0x50);
        pkt.AddByte(0x00);
        Byte byte = le16(id);
        pkt.AddByte(byte.LeftNibble);
        pkt.AddByte(byte.RightNibble);
        pkt.Fixup();
        m_logger->info("Sending conn_ack msg: {}", spdlog::to_hex(pkt.GetBuffer()));
        m_socket->Send(pkt.GetBuffer());
    }

    void TelloCommander::SendConnReq()
    {
        auto conn_req = tello_protocol::Packet("conn_req:\x96\x17");
        m_socket->Send(conn_req.GetBuffer());
    }
    
    void TelloCommander::SetSocket(std::shared_ptr<ISender> socket)
    {
        m_socket = socket;
    }
    TelloCommander::TelloCommander(std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum lvl)
        : m_logger(logger)
    {
        m_logger->info(m_logger->name() + " Initiated.");
        m_logger->set_level(lvl);
    }

    TelloCommander::~TelloCommander()
    {
        m_logger->info(m_logger->name() + " Destructing.");
    }
} // namespace tello_protocol
