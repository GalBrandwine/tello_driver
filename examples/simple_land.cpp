#include "TelloDriver/TelloDriver.hpp"
#include <iostream>

/*******************************************
 * @brief In this example I show how to create an object the contains TelloDriver, and attach itself to TelloDriver.
 * 
 * *******************************************/

class SimpleLandClient
{
public:
    /**
     * @brief Scenario1 - No close loop operation. \n
     * Land
     * 
     */
    void StartScenario1();
    SimpleLandClient(/* args */);
    ~SimpleLandClient();

private:
    TelloDriver m_tello;
};

SimpleLandClient::SimpleLandClient()
    : m_tello(spdlog::level::info)
{
}

void SimpleLandClient::StartScenario1()
{

    m_tello.Connect();
    m_tello.GetLogger()->info("Starting simple connection");
    if (!m_tello.WaitForConnection(10))
    {
        m_tello.GetLogger()->error("Connection error. exiting!");
        return;
    }

    m_tello.GetLogger()->info("Sending Land command!");
    m_tello.Land();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    m_tello.GetLogger()->info("Scenario1 over!");
}

SimpleLandClient::~SimpleLandClient()
{
}

int main()
{
    SimpleLandClient slc;

    slc.StartScenario1();
    return 0;
}