#include "TelloCommander.hpp"
namespace tello_protocol
{
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

    /* 
        def land(self):
        """Land tells the drone to come in for landing."""
        log.info('land (cmd=0x%02x seq=0x%04x)' % (LAND_CMD, self.pkt_seq_num))
        pkt = Packet(LAND_CMD)
        pkt.add_byte(0x00)
        pkt.fixup()
        return self.send_packet(pkt)


        def takeoff(self):
        """Takeoff tells the drones to liftoff and start flying."""
        log.info('set altitude limit 30m')
        pkt = Packet(SET_ALT_LIMIT_CMD)
        log.info(pkt.get_buffer())
        pkt.add_byte(0x1e)  # 30m
        log.info(pkt.get_buffer())
        pkt.add_byte(0x00)
        log.info(pkt.get_buffer())
        self.send_packet(pkt)
        log.info('takeoff (cmd=0x%02x seq=0x%04x)' % (TAKEOFF_CMD, self.pkt_seq_num))
        pkt = Packet(TAKEOFF_CMD)
        log.info(pkt.get_buffer())
        pkt.fixup()
        log.info(pkt.get_buffer())
        return self.send_packet(pkt)
     */
    void TelloCommander::SendAckLog(const int id)
    {
        auto pkt = tello_protocol::Packet(tello_protocol::LOG_HEADER_MSG, 0x50);
        pkt.AddByte(0x00);
        Byte byte = le16(id);
        pkt.AddByte(byte.LeftNibble);
        pkt.AddByte(byte.RightNibble);
        pkt.Fixup();
        m_logger->info("Senging conn_ack msg: {}", spdlog::to_hex(pkt.GetBuffer()));
        m_socket->Send(pkt.GetBuffer());
        // tello_socket.send_to(asio::buffer(pkt.GetBuffer()), remote_endpoint_);
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
    TelloCommander::TelloCommander(std::shared_ptr<spdlog::logger> logger,spdlog::level::level_enum lvl)
        : m_logger(logger)
    {
        m_logger->info(m_logger->name() + " Initiated!.");
        m_logger->set_level(lvl);
    }

    TelloCommander::~TelloCommander()
    {
    }
} // namespace tello_protocol
