#pragma once

#include <string>
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

#include "flight_data/FlightData.hpp"
#include "log_data/LogData.hpp"
#include "asio.hpp"
using asio::ip::udp;
namespace tello_protocol
{

    class TelloInputDataSocket
    {
    public:
        udp::socket &GetSocket();
        TelloInputDataSocket(int port);

    private:
        asio::io_service io_service_; // Manages IO for this socket
        unsigned short m_port = 9000;
        udp::socket m_tello_socket; //(io_service_, udp::endpoint(udp::v4(), m_port)); // The socket
    };

    class TelloTelemetry
    {
    public:
        TelloTelemetry(std::shared_ptr<spdlog::logger>, int port);
        TelloTelemetry(std::shared_ptr<spdlog::logger>);
        ~TelloTelemetry();

        void StartListening(); // Listener thread
        void StopListening() { m_keep_receiving = false; };
        void SetBuildDate(const std::string &);
        const std::string &GetBuildDate() const;
        void SetDJILogVersion(const std::string &);
        const std::string &GetDJILogVersion() const;

        std::shared_ptr<FlightData> GetFlightData() const;
        void SetFlightData(std::shared_ptr<FlightData>);

        std::shared_ptr<LogData> GetLogData() const;
        void SetLogData(std::shared_ptr<LogData>);

        bool IsLogHeaderReceived() const;
        void SetLogHeaderReceived();

    private:
        std::string m_BuildDate, m_DJI_LOG_VERSION;
        std::shared_ptr<spdlog::logger> m_logger;
        std::shared_ptr<FlightData> m_FlightData;
        std::shared_ptr<LogData> m_LogData;
        bool process_data(const std::vector<unsigned char> &);
        std::shared_ptr<TelloInputDataSocket> m_socket;
        bool m_IsLogHeaderReceived = false;
        bool m_keep_receiving = true;
        int r;
        std::vector<asio::detail::buffered_stream_storage::byte_type> m_buffer; //std::vector<unsigned char>
    };

} // namespace tello_protocol
