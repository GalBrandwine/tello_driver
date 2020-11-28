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
    const float GetAttLimit();
    const short GetAltLimit();
    const short GetWifiStrength();
    const tello_protocol::Vec3 GetPos();
    TelloDriver(spdlog::level::level_enum lvl = spdlog::level::info);
    ~TelloDriver();
    tello_protocol::TelloCommander &GetTelloCommander();
    tello_protocol::TelloTelemetry &GetTelloTelemetry();
    void Takeoff();
    void Land();
    void Connect();
    bool WaitForConnection(int);

private:
    /// Work for the private io_context to perform. If we do not give the
    /// io_context some work to do then the io_context::run() function will exit
    /// immediately.
    // asio::executor_work_guard<asio::io_context::executor_type> *work_;
    /// Thread used for running the work io_context's run loop.
    // std::thread work_thread_;
    // Private io_context used for performing socket operations.
    // asio::io_context work_io_context_;

    tello_protocol::TelloCommander m_TelloCommander;
    tello_protocol::TelloTelemetry m_TelloTelemetry;
    std::shared_ptr<spdlog::logger> m_BaseLogger;
};
