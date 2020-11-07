#pragma once
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "TelloTelemetry.hpp"
class TelloDriver
{
public:
    TelloDriver(/* args */);
    ~TelloDriver();
    void Connect();
private:
    tello_protocol::TelloTelemetry m_TelloTelemetry;
    std::shared_ptr<spdlog::logger> m_BaseLogger;
};
