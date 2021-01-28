#include "TelloDriver/TelloDriver.hpp"
#include <iostream>

/*******************************************
 * Perform simple takeoff, then land.
 * No stick stimulations.
 * No close-loop operations.
 * *******************************************/

#include "TelloDriver/TelloDriver.hpp"
#include <iostream>

/*******************************************
 * @brief In this example I show how to create an object the contains TelloDriver, and attach itself to TelloDriver.
 * 
 * *******************************************/

class Examples : public IPositionVelocityObserver
{
public:
    /**
     * @brief Must be overriden.
     * 
     * @param[in] message_from_subject - data filled with information relenavt to this observer.
     */
    void Update(const tello_protocol::PoseVelData &pos_vel) override;
    void Update(const std::vector<unsigned char> &message_from_subject) override;
    void StartTakeoffAndLandScenario();
    void StartTakeoffMoveBackwardAndLandScenario();
    Examples(/* args */);
    ~Examples();

private:
    TelloDriver m_tello;
};

Examples::Examples()
    : m_tello(spdlog::level::info)
{
    m_tello.Attach(OBSERVERS::POSITION_VELOCITY_LOG, this);
    m_tello.Attach(OBSERVERS::DJI_LOG_VERSION, this);
}

void Examples::StartTakeoffMoveBackwardAndLandScenario()
{
    m_tello.Connect();
    m_tello.GetLogger()->info("Starting simple connection");
    if (!m_tello.WaitForConnection(10))
    {
        m_tello.GetLogger()->error("Connection error. exiting!");
        return;
    }

    m_tello.GetLogger()->info("Sending Takeoff command!");
    m_tello.Takeoff();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    m_tello.Backward(1);
    m_tello.GetLogger()->info("Sending Backward 1% command!");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_tello.GetLogger()->info("Sending Backward 0% command!");
    m_tello.Backward(0);
    m_tello.GetLogger()->info("Sending Land command!");
    m_tello.Land();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    m_tello.GetLogger()->info("Scenario over!");
}
void Examples::StartTakeoffAndLandScenario()
{
    m_tello.Connect();
    m_tello.GetLogger()->info("Starting simple connection");
    if (!m_tello.WaitForConnection(10))
    {
        m_tello.GetLogger()->error("Connection error. exiting!");
        return;
    }

    m_tello.GetLogger()->info("Sending Takeoff command!");
    m_tello.Takeoff();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    m_tello.GetLogger()->info("Sending Land command!");
    m_tello.Land();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    m_tello.GetLogger()->info("Scenario over!");
}

Examples::~Examples()
{
}

void Examples::Update(const tello_protocol::PoseVelData &pos_vel)
{
    auto new_pos_vel = pos_vel;
    m_tello.GetLogger()->info("new_pos_vel {} {} {}", new_pos_vel.pose.x, new_pos_vel.pose.y, new_pos_vel.pose.z);
}

void Examples::Update(const std::vector<unsigned char> &message_from_subject)
{
    std::string s(message_from_subject.begin(), message_from_subject.end());
    m_tello.GetLogger()->info("received tello current log version: " + s);
}

int main()
{
    Examples examples;
    examples.StartTakeoffAndLandScenario();
    // examples.StartTakeoffMoveBackwardAndLandScenario();
}