#pragma once
#include "IReciever.hpp"
#include "ISender.hpp"
#include <mutex>
using asio::ip::udp;

class TelloSocket : public ISender, public IReciever
{
public:
    int Recieve(std::vector<unsigned char> &) override;
    void Send(const std::string &) override;

    TelloSocket(const std::string &droneIp, const unsigned short droneCommandPort, const unsigned short droneDataPort);
    ~TelloSocket();

private:
    std::mutex m_sendM;
    asio::io_service io_service_;
    unsigned short m_port = 8889;
    unsigned short droneDataPort = 9000;
    udp::socket m_tello_socket;
    udp::endpoint m_remote_endpoint;
};