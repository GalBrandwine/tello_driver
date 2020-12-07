#pragma once
#include <cstdlib>
#include <iostream>
#include "asio.hpp"
#include <thread>
using asio::ip::udp;

class server
{
public:
    server(asio::io_context &io_context, short port);
    void do_receive();
    void do_send(char *data, std::size_t length);
    bool is_bytes_recvd = false;

private:
    udp::socket socket_;
    udp::endpoint sender_endpoint_;

    enum
    {
        max_length = 1024
    };
    char data_[max_length];
};