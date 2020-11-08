#include "TelloSocket.hpp"

int TelloSocket::Recieve(std::vector<unsigned char> &data)
{
    size_t rec = m_tello_socket.receive(asio::buffer(data));
    return rec;
}

void TelloSocket::Send(const std::string &cmd)
{   
    std::lock_guard<std::mutex> lock(m_sendM);
    m_tello_socket.send_to(asio::buffer(cmd), m_remote_endpoint);
}

TelloSocket::~TelloSocket()
{
    m_tello_socket.close();
}
TelloSocket::TelloSocket(const std::string &droneIp, const unsigned short droneCommandPort, const unsigned short droneDataPort)
    : m_port(droneCommandPort),
      m_remote_endpoint(asio::ip::address_v4::from_string(droneIp), droneCommandPort),
      m_tello_socket(io_service_, udp::endpoint(udp::v4(), droneDataPort)) // The socket
{
}