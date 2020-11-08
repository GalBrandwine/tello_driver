#pragma once
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils/TelloSocket.hpp"
#include "TelloTelemetry.hpp"
#include "TelloCommander.hpp"
using namespace std::chrono_literals;

#define CONNECTION_TIMEOUT 50
class TelloDriver
{
public:
    TelloDriver(/* args */);
    ~TelloDriver();
    tello_protocol::TelloCommander& GetTelloCommander();
    tello_protocol::TelloTelemetry& GetTelloTelemetry();
    void Connect();
    bool WaitForConnection();
private:
    tello_protocol::TelloCommander m_TelloCommander;
    tello_protocol::TelloTelemetry m_TelloTelemetry;
    std::shared_ptr<spdlog::logger> m_BaseLogger;
};
