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
     */
    void Update(const tello_protocol::PoseVelData &pos_vel) override;
    void Update(const std::vector<unsigned char> &message_from_subject) override;
    
    /**
     * @brief Scenario1 - No close loop operation. \n
     * 1. Move backward %1 for 1 sec \n
     * 2. Move forward %1 for 1 sec \n 
     * 3. Land
     * 
     */
    void StartScenario1();

    /**
     * @brief Scenario2 - No close loop operation. \n
     * 1. Takeoff \n 
     * 2. Turn Clockwise 30% \n
     * 3. Turn CounterClockwise 30% \n
     * 3. Land
     * 
     */
    void StartScenario2();

    /**
     * @brief Scenario3 - No close loop operation. \n
     * 1. Takeoff \n 
     * 2. Move Left 1% \n
     * 3. Move Right 1% \n
     * 3. Land
     * 
     */    
    void StartScenario3();
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

void Examples::StartScenario1()
{

    m_tello.Connect();
    m_tello.GetLogger()->info("Starting simple connection");
    if (!m_tello.WaitForConnection(10))
    {
        m_tello.GetLogger()->error("Connection error. exiting!");
        return;
    }
    int stick = 1;
    m_tello.GetLogger()->info("Sending Takeoff command!");
    m_tello.Takeoff();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    m_tello.GetLogger()->info("Sending Backward {}% command!", stick);
    m_tello.Backward(stick);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    stick = 0;
    m_tello.GetLogger()->info("Sending Backward {}stick% command!", stick);
    m_tello.Backward(stick);

    stick = 5;
    m_tello.GetLogger()->info("Sending Fowrard {}% command!", stick);
    m_tello.Forward(stick);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    stick = 0;
    m_tello.GetLogger()->info("Sending Fowrard {}% command!", stick);
    m_tello.Forward(stick);

    m_tello.GetLogger()->info("Sending Land command!");
    m_tello.Land();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    m_tello.GetLogger()->info("Scenario1 over!");
}

void Examples::StartScenario2()
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

    int stick = 30;
    m_tello.GetLogger()->info("Sending Clockwise {}% command!", stick);
    m_tello.Clockwise(stick);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    stick = 0;
    m_tello.GetLogger()->info("Sending Clockwise {}% command!", stick);
    m_tello.Clockwise(stick);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    stick = 30;
    m_tello.GetLogger()->info("Sending Counter-Clockwise {}% command!", stick);
    m_tello.CounterClockwise(stick);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    stick = 0;
    m_tello.GetLogger()->info("Sending Counter-Clockwise {}% command!", stick);
    m_tello.CounterClockwise(stick);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    m_tello.GetLogger()->info("Sending Land command!");
    m_tello.Land();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    m_tello.GetLogger()->info("Scenario2 over!");
}


void Examples::StartScenario3()
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

    int stick = 5;
    m_tello.GetLogger()->info("Sending Left {}% command!", stick);
    m_tello.Left(stick);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // stick = 0;
    // m_tello.GetLogger()->info("Sending Clockwise {}% command!", stick);
    // m_tello.Clockwise(stick);

    // std::this_thread::sleep_for(std::chrono::seconds(5));

    // stick = 30;
    m_tello.GetLogger()->info("Sending Right {}% command!", stick);
    m_tello.Right(stick);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    stick = 0;
    m_tello.GetLogger()->info("Sending Right {}% command!", stick);
    m_tello.Right(stick);
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    m_tello.GetLogger()->info("Sending Land command!");
    m_tello.Land();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    m_tello.GetLogger()->info("Scenario3 over!");
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
    // m_tello.GetLogger()->info("new_pos_vel {} {} {}", new_pos_vel.pose.x, new_pos_vel.pose.y, new_pos_vel.pose.z);
}

void Examples::Update(const std::vector<unsigned char> &message_from_subject)
{
    std::string s(message_from_subject.begin(), message_from_subject.end());
    m_tello.GetLogger()->info("received tello current log version: " + s);
}

int main()
{
    Examples examples;

    // examples.StartScenario1();

    // examples.StartScenario2();

    examples.StartScenario3();

    // examples.StartTakeoffAndLandScenario();
    // examples.StartTakeoffMoveBackwardAndLandScenario();
}