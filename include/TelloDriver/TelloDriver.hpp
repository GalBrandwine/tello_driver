#pragma once
#include <memory>
#include <chrono>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "utils/TelloSocket.hpp"
#include "TelloTelemetry.hpp"
#include "TelloCommander.hpp"
#include "TelloDriverConfig.hpp"

#include <cstdlib>
#include <iostream>
#include "asio.hpp"
#include <thread>

using namespace std::chrono_literals;
using asio::ip::udp;

#define CONNECTION_TIMEOUT 50
class TelloDriver
{
public:
    /**
    * Movements section
    **/
    void Backward(int);
    void Forward(int);
    const float GetAttLimit();
    const short GetAltLimit();
    const short GetWifiStrength();
    const tello_protocol::Vec3 GetPos();
    std::shared_ptr<spdlog::logger> GetLogger() { return m_BaseLogger; };
    TelloDriver(spdlog::level::level_enum lvl = spdlog::level::info);
    ~TelloDriver();
    tello_protocol::TelloCommander &GetTelloCommander();
    tello_protocol::TelloTelemetry &GetTelloTelemetry();
    void Takeoff();
    void Land();
    void Connect();
    bool WaitForConnection(int);

private:
    tello_protocol::TelloCommander m_TelloCommander;
    tello_protocol::TelloTelemetry m_TelloTelemetry;
    std::shared_ptr<spdlog::logger> m_BaseLogger;
    void setStickCommandsThread();
    bool m_KeepRunning = true;
    std::thread m_StickCommandsSendingThread;
};
