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
        std::cout << "fly_mode: " << std::to_string(m_flight_data.fly_mode) << "\n";
        std::cout << "fly_speed: " << m_flight_data.fly_speed << "\n";
        std::cout << "east_speed: " << m_flight_data.east_speed << "\n";
        std::cout << "north_speed: " << m_flight_data.north_speed << "\n";
        std::cout << "ground_speed: " << m_flight_data.ground_speed << "\n";
        std::cout << "drone_hover: " << m_flight_data.flight_data_extras.drone_hover << "\n";
        std::cout << "down_visual_state: " << m_flight_data.flight_data_states.down_visual_state << "\n\n";
    };
    void Update(const std::vector<unsigned char> &message_from_subject) override{};
    const tello_protocol::FlightDataStruct &GetFlightData() const { return m_flight_data; };
    Dummy(/* args */){};
    ~Dummy(){};
};