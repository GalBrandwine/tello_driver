#include "TelloCommander.hpp"
namespace tello_protocol
{
    void TelloOutCommandSocket::Send(const std::string &cmd)
    {
        m_tello_socket.send_to(asio::buffer(cmd), m_remote_endpoint);
    }
    udp::socket &TelloOutCommandSocket::GetSocket() { return m_tello_socket; }
    TelloOutCommandSocket::TelloOutCommandSocket(const std::string &droneIp, const unsigned short droneCommandPort, const unsigned short droneDataPort)
        : m_port(droneCommandPort),
          m_remote_endpoint(asio::ip::address_v4::from_string(droneIp), droneCommandPort),
          m_tello_socket(io_service_, udp::endpoint(udp::v4(), droneDataPort)) // The socket
    {
    }

    void TelloCommander::SendConnReq()
    {
        auto conn_req = tello_protocol::Packet("conn_req:\x96\x17");
        m_socket->Send(conn_req.GetBuffer());
    }
    TelloCommander::TelloCommander(std::shared_ptr<spdlog::logger> logger, const std::string &droneIp, const unsigned short droneCommandPort, const unsigned short droneDataPort)
        : m_logger(logger)
    {
        m_socket = std::make_shared<TelloOutCommandSocket>(droneIp, droneCommandPort, droneDataPort);
    }

    TelloCommander::~TelloCommander()
    {
    }
} // namespace tello_protocol
