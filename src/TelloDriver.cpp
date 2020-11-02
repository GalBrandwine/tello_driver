
#include "TelloDriver/TelloDriver.hpp"



TelloDriver::TelloDriver(/* args */)
: m_BaseLogger(spdlog::stdout_color_mt("TelloDriver"))
{
    m_BaseLogger->info("TelloDriver created!");
}

TelloDriver::~TelloDriver()
{
}