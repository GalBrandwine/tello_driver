#pragma once
#include "IReciever.hpp"
#include "ISender.hpp"
#include <mutex>
#include <iostream>
#include <functional>
#include <thread>
#include <cstdlib>

#include "asio.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/udp.hpp"
#include "asio/use_future.hpp"
using asio::ip::udp;

class TelloSocket : public ISender, public IReciever
{
public:
    int AsyncReceive(std::vector<unsigned char> &) override;
    int Receive(std::vector<unsigned char> &) override;
    void Send(const std::string &) override;

    void do_send(std::size_t length);
    bool IsBytesReceived() { return m_is_bytes_received; };
    void do_receive();
    TelloSocket(asio::io_context &io_context, const std::string &droneIp, const short droneCommandPort, const short droneDataPort);
    ~TelloSocket();

private:
    std::mutex m_sendM;
    bool m_is_bytes_received = false;
    unsigned short m_port = 8889;
    unsigned short droneDataPort = 9000;
    udp::socket m_tello_socket;
    udp::endpoint m_sender_endpoint;
    size_t m_bytes_recvd;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];
};