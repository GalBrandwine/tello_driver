#pragma once

#include <string>
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include "utils/IReciever.hpp"
#include "utils/TelloSocket.hpp"
#include "flight_data/FlightData.hpp"
#include "log_data/LogData.hpp"
#include "asio.hpp"
#include <thread>
#include "TelloCommander.hpp"
#include "utils/telemetry_data/TelemetryData.hpp"
using namespace std::chrono_literals;
namespace tello_protocol
{
    class TelloTelemetry
    {
    public:
        const int GetLogHeaderId() const;
        // void SetTelloCommander(std::shared_ptr<tello_protocol::TelloCommander>);
        void SetSocket(std::shared_ptr<IReciever>);

        TelloTelemetry(std::shared_ptr<spdlog::logger>, spdlog::level::level_enum lvl = spdlog::level::info);
        ~TelloTelemetry();

        void StopListening();
        void StartListening();
        void Listener(); // Listener thread
        void SetBuildDate(const std::string &);
        const std::string &GetBuildDate() const;
        void SetDJILogVersion(const std::string &);
        const std::string &GetDJILogVersion() const;

        std::shared_ptr<FlightData> GetFlightData() const;
        void SetFlightData(std::shared_ptr<FlightData>);

        std::shared_ptr<LogData> GetLogData() const;
        void SetLogData(std::shared_ptr<LogData>);

        bool IsConnReqAckReceived() const;
        bool IsLogHeaderReceived() const;
        void SetLogHeaderReceived();

    private:
        std::shared_ptr<tello_protocol::TelloCommander> m_TelloCommander;
        std::string m_BuildDate, m_DJI_LOG_VERSION;
        std::shared_ptr<spdlog::logger> m_logger;
        std::shared_ptr<FlightData> m_FlightData;
        std::shared_ptr<LogData> m_LogData;
        bool process_data(const std::vector<unsigned char> &);
        std::shared_ptr<IReciever> m_socket;
        bool m_IsLogHeaderReceived = false;
        bool m_keep_receiving = true;
        bool m_anyDataReceived = false;
        bool m_connReqAckRecieved = false;
        int m_BytesReceived, m_IsLogHeaderReceivedId;
        std::thread m_Listener;
        std::vector<unsigned char> m_buffer; // = std::vector<unsigned char>(1024); //std::vector<unsigned char>
    };

} // namespace tello_protocol
