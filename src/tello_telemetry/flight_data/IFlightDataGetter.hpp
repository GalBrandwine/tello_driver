#pragma once
#include <memory>
#include "utils/telemetry_data/TelemetryData.hpp"
class IFlightDataGetter
{
public:
    virtual const tello_protocol::FlightDataStruct &GetFlightData() = 0;
    virtual ~IFlightDataGetter(){};
};
