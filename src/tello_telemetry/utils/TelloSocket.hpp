#pragma once
#include "IReciever.hpp"
#include "ISender.hpp"
#include <mutex>
#include <iostream>
#include <functional>
#include <thread>
#include <cstdlib>
#include "boost/asio.hpp"

class TelloSocket : public ISender, public IReciever
{
public:
    int Receive(std::vector<unsigned char> &) override;
    void Send(const std::string &) override;

    void do_send(std::size_t length);
    bool IsBytesReceived() { return m_is_bytes_received; };
    void do_receive();
    TelloSocket(const std::string &droneIp, const short droneCommandPort, const short droneDataPort);
    ~TelloSocket();

private:
    std::mutex m_sendM;
    bool m_is_bytes_received = false;
    unsigned short m_port = 8889;
    unsigned short droneDataPort = 9000;
    boost::asio::ip::udp::socket *m_tello_socket;
    boost::asio::ip::udp::endpoint m_sender_endpoint;

    size_t m_bytes_recvd;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];

    /// Private io_context used for performing logging operations.
    boost::asio::io_context work_io_context_;

    /// Work for the private io_context to perform. If we do not give the
    /// io_context some work to do then the io_context::run() function will exit
    /// immediately.
    boost::asio::executor_work_guard<
        boost::asio::io_context::executor_type>
        work_;

    /// Thread used for running the work io_context's run loop.
    std::thread work_thread_;
};