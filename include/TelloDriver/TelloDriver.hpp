#pragma once
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class TelloDriver
{
public:
    TelloDriver(/* args */);
    ~TelloDriver();
private:
    std::shared_ptr<spdlog::logger> m_BaseLogger;
};
