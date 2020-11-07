#pragma once
#include "packet_parser/protocol.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "asio.hpp"
using asio::ip::udp;
namespace tello_protocol
{
    class TelloOutCommandSocket
    {
    public:
        udp::socket &GetSocket();
        void Send(const std::string &);
        // TelloOutCommandSocket(const std::string &ip, const unsigned short port);
        TelloOutCommandSocket(const std::string &droneIp, const unsigned short droneCommandPort, const unsigned short droneDataPort);

    private:
        asio::io_service io_service_;
        unsigned short m_port = 8889;
        unsigned short droneDataPort = 9000;
        udp::socket m_tello_socket;
        //(asio::ip::address_v4::from_string(drone_ip), drone_port);
        udp::endpoint m_remote_endpoint;
    };

    class TelloCommander
    {
    public:
        void SendConnReq();
        void SendAckLog(const int id);
        TelloCommander(std::shared_ptr<spdlog::logger>, const std::string &, const unsigned short, const unsigned short);

    private:
        void sendCommand(const tello_protocol::Packet &);
        std::shared_ptr<spdlog::logger> m_logger;
        std::shared_ptr<TelloOutCommandSocket> m_socket;
    };

} // namespace tello_protocol
