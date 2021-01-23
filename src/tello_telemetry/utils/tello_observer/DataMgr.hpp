#pragma once
#include <string>
enum OBSERVERS
{
    ACK_LOG_HEADER,
    DJI_LOG_VERSION //< Notify all attached that Log build data and version, has been received.
};

static const std::string observer_name(OBSERVERS observer_type)
{
    switch (observer_type)
    {
    case OBSERVERS::ACK_LOG_HEADER:
        return "ACK_LOG_HEADER";
    case OBSERVERS::DJI_LOG_VERSION:
        return "DJI_LOG_VERSION";
    default:
        return "NOT IMPLEMENTED";
    }
}