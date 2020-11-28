#include "socket_test.hpp"
using namespace std::chrono_literals;

//
// async_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include "asio.hpp"
#include <thread>
// #include <asio/io_context.hpp>
// #include <asio/ip/udp.hpp>
// #include <asio/use_future.hpp>
using asio::ip::udp;

class server
{
public:
    server(asio::io_context &io_context, short port)
        : socket_(io_context, udp::endpoint(udp::v4(), port)),
          sender_endpoint_(asio::ip::address_v4::from_string("192.168.10.1"), 8889)
    {
        std::string conn = "conn_req:\x96\x17";
        int index = 0;
        for (auto &letter : conn)
            data_[index++] = letter;
        do_send(conn.length());
        do_receive();
    }

    void do_receive()
    {
        socket_.async_receive_from(
            asio::buffer(data_, max_length), sender_endpoint_,
            [this](std::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0)
                {
                    do_send(bytes_recvd);
                    std::cout << "bytes_recvd: " << bytes_recvd << "\n";
                    is_bytes_recvd = true;
                }
                else
                {
                    do_receive();
                }
            });
    }

    void do_send(std::size_t length)
    {
        socket_.async_send_to(
            asio::buffer(data_, length), sender_endpoint_,
            [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {
                do_receive();
            });
    }
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

// Lowest level testing: test aisio liobrary async capabilities
TEST(AsioSocketAsyncOperations, TrySendConnReq)
{

    // We run the io_context off in its own thread so that it operates
    // completely asynchronously with respect to the rest of the program.
    asio::io_context io_context;
    asio::executor_work_guard<asio::io_context::executor_type> work_(asio::make_work_guard(io_context));
    std::thread thread(std::thread([&io_context]() { io_context.run(); std::cout << "work_io_context_ finished!\n"; }));

    server s(io_context, std::atoi("9000"));

    // io_context.run(); // <--- BLOKING

    int timeout_ticks = 500;
    while (!s.is_bytes_recvd)
    {
        std::this_thread::sleep_for(20ms);
        if (timeout_ticks-- < 0)
        {
            break;
        }
    }
    EXPECT_TRUE(s.is_bytes_recvd);
    io_context.stop();
    thread.join();
    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

// Mid-level testing: test if 'connReq' being sent correctly
TEST(TelloSocketAsync, TrySendConnReq)
{
    // asio::io_context io_context;

    // We run the io_context off in its own thread so that it operates
    // completely asynchronously with respect to the rest of the program.
    asio::io_context io_context;
    asio::executor_work_guard<asio::io_context::executor_type> work_(asio::make_work_guard(io_context));
    std::thread thread(std::thread([&io_context]() { io_context.run(); std::cout << "work_io_context_ finished!\n"; }));

    TelloSocket tello_socket(io_context, "192.168.10.1", 8889, 9000);

    tello_socket.Send("conn_req:\x96\x17");

    int timeout_ticks = 500;
    while (!tello_socket.IsBytesReceived())
    {
        std::this_thread::sleep_for(20ms);
        if (timeout_ticks-- < 0)
        {
            break;
        }
    }
    EXPECT_TRUE(tello_socket.IsBytesReceived());

    io_context.stop();
    thread.join();
    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/* Mid-level testing:
 * Test if 'connAck' being received correctly.
 * If so. this means a connection has been established,
 *  */
TEST(TelloSocketAsync, TrySendRecieveConnReq)
{
    // asio::io_context io_context;

    // We run the io_context off in its own thread so that it operates
    // completely asynchronously with respect to the rest of the program.
    asio::io_context io_context;
    auto work = asio::require(io_context.get_executor(),
                              asio::execution::outstanding_work.tracked);
    std::thread thread([&io_context]() { io_context.run(); });

    TelloSocket tello_socket(io_context, "192.168.10.1", 8889, 9000);
    std::string msg("conn_req:\x96\x17");
    tello_socket.Send(msg);
    int timeout_ticks = 500;

    std::vector<unsigned char> received_data(1024);
    int bytes_recieve = 0;
    // Run
    while (bytes_recieve != 11)
    {
        tello_socket.Send(msg);
        std::this_thread::sleep_for(20ms);
        bytes_recieve = tello_socket.AsyncReceive(received_data);
        if (timeout_ticks-- < 0)
        {
            break;
        }
    }

    // Test
    std::vector<unsigned char> rec(received_data.begin(), received_data.begin() + 4);
    std::vector<unsigned char> sended(msg.begin(), msg.begin() + 4);
    EXPECT_EQ(sended, rec);

    io_context.stop();
    thread.join();
    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/* High-level testing: 
 * Initiate io_context within TelloSocket.
 * */
// TEST(FullTelloSocketAsync, TrySendRecieveConnReq)
// {
//     // asio::io_context io_context;

//     TelloSocket tello_socket("192.168.10.1", 8889, 9000);
//     std::string msg("conn_req:\x96\x17");
//     tello_socket.Send(msg);
//     int timeout_ticks = 500;

//     std::vector<unsigned char> received_data(1024);
//     int bytes_recieve = 0;
//     // Run
//     while (bytes_recieve != 11)
//     {
//         tello_socket.Send(msg);
//         std::this_thread::sleep_for(20ms);
//         bytes_recieve = tello_socket.AsyncReceive(received_data);
//         if (timeout_ticks-- < 0)
//         {
//             break;
//         }
//     }

//     // Test
//     std::vector<unsigned char> rec(received_data.begin(), received_data.begin() + 4);
//     std::vector<unsigned char> sended(msg.begin(), msg.begin() + 4);
//     EXPECT_EQ(sended, rec);

//     io_context.stop();
//     thread.join();
//     std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
// }
