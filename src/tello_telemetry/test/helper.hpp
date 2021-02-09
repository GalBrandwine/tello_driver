#pragma once
#include "TelloDriver/TelloDriver.hpp"

class Dummy : public IFlightDataObserver
{
private:
    tello_protocol::FlightDataStruct m_flight_data;

public:
    void Update(const tello_protocol::FlightDataStruct &flight_data) override
    {
        m_flight_data = flight_data;
    };
    void Update(const std::vector<unsigned char> &message_from_subject) override{};
    const tello_protocol::FlightDataStruct &GetFlightData() const { return m_flight_data; };
    Dummy(/* args */){};
    ~Dummy(){};
};
// Dummy::Dummy(){};
// Dummy::~Dummy(){};
// void Dummy::Update(const tello_protocol::FlightDataStruct &flight_data)
// {
//     m_flight_data = flight_data;
// };