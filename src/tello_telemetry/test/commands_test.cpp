#include "commands_test.hpp"
#include "TelloDriver/TelloDriver.hpp"

using namespace std::chrono_literals;

/* High-level testing: 
 * Test the whole pipeline. E.g:
 * 1. Instantiate TelloDriver
 * 2. Send connect req.
 * 3. Wait until connection has been established.
 * */
TEST(WetTelloCommandTest, TrySendRecieveConnReq)
{
    TelloDriver tello(spdlog::level::debug);
    tello.Connect();
    EXPECT_TRUE(tello.WaitForConnection(10));
    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

TEST(WetTelloSticksCommandTest, TakeoffAndMoveForward)
{
    TelloDriver tello(spdlog::level::debug);
    tello.Connect();
    EXPECT_TRUE(tello.WaitForConnection(10));
    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}
// tello.GetTelloTelemetry().StopListening();
// TearDownTestCase();

// using asio::ip::udp;

// asio::io_service io_service_; // Manages IO for this socket
// unsigned short port = 9000;
// /*
// Packets from the drone suppose to arrive at this (address,port): ('',9000)
// Packets are being send from client to drone to this (address,port) = ('192.168.10.1', 8889)
// */
// unsigned short drone_port = 8889;
// std::string drone_ip = "192.168.10.1";
// udp::endpoint remote_endpoint_(asio::ip::address_v4::from_string(drone_ip), drone_port);

// udp::socket tello_socket(io_service_, udp::endpoint(udp::v4(), port)); // The socket

// //Run
// auto pkt = tello_protocol::Packet("conn_req:\x96\x17");
// std::cout << pkt << std::endl;
// tello_socket.send_to(asio::buffer(pkt.GetBuffer()), remote_endpoint_);

// // Test
// using namespace std::chrono_literals;
// bool keep_sending = true;
// auto buffer_ = std::vector<unsigned char>(1024);
// // It is known that conn_req answer is in size 11 bytes
// size_t r = 0;

// tello_socket.send_to(asio::buffer(pkt.GetBuffer()), remote_endpoint_);
// std::this_thread::sleep_for(0.5s);

// while (keep_sending)
// {
//     r = tello_socket.receive(asio::buffer(buffer_));
//     if (r == 11)
//         keep_sending = false;
// }

// // "conn_ack:\x96\x17" (Sniffed using wireshark)
// std::vector<unsigned char> conn_ack_from_drone = {0x63, 0x6f, 0x6e, 0x6e, 0x5f, 0x61, 0x63, 0x6b, 0x3a, 0x96, 0x17};

// int index = 0;
// for (auto &letter : conn_ack_from_drone)
// {
//     ASSERT_EQ(buffer_[index], letter);
//     index++;
// }
