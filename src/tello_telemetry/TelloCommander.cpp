#include "TelloCommander.hpp"
namespace tello_protocol
{
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
    TelloCommander::TelloCommander(std::shared_ptr<spdlog::logger> logger)
        : m_logger(logger)
    {
        m_logger->info(m_logger->name() + " Initiated!.");
    }

    TelloCommander::~TelloCommander()
    {
    }
} // namespace tello_protocol
