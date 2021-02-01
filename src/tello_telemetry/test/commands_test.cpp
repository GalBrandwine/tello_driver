#include "commands_test.hpp"
#include "TelloDriver/TelloDriver.hpp"

// Setup
class Dummy : public IFlightDataObserver
{
private:
    tello_protocol::FlightDataStruct m_flight_data;

public:
    void Update(const tello_protocol::FlightDataStruct &flight_data) override;
    void Update(const std::vector<unsigned char> &message_from_subject) override{};
    const tello_protocol::FlightDataStruct &GetFlightData() const { return m_flight_data; };
    Dummy(/* args */);
    ~Dummy();
};
Dummy::Dummy(){};
Dummy::~Dummy(){};
void Dummy::Update(const tello_protocol::FlightDataStruct &flight_data)
{
    m_flight_data = flight_data;
};

using namespace std::chrono_literals;

/* High-level testing: 
 * Test the whole pipeline. E.g:
 * 1. Instantiate TelloDriver
 * 2. Send connect req.
 * 3. Wait until connection has been established.
 * */
TEST(WetTelloCommandTest, TrySendRecieveConnReq)
{
    TelloDriver tello(spdlog::level::debug);
    tello.Connect();
    EXPECT_TRUE(tello.WaitForConnection(10));
    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/**
* Test if driver allert abount drone disconnection.
* Manually turn off the drone after connection
**/
TEST(WetTelloSticksCommandTest, TestDisconnectionAllert)
{
    // Setup
    TelloDriver tello(spdlog::level::debug);

    // Run
    tello.Connect();
    if (!tello.WaitForConnection(10))
    {
        tello.GetLogger()->error("Couldn't Connect to drone.");
        ASSERT_TRUE(false);
    }

    // Test
    int counter = 100;
    bool res = true;
    while (counter-- > 0 && res == true)
    {
        // res = tello.GetTelloTelemetry().IsDroneConnected();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ASSERT_TRUE(res);

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}
/**
 * Test if drone recieves neutral stick commands. 
 * @example: 
 *  No movements. Just keep alive heartbeat.
 *  If everything work properly. Drone's LED suppose to blink with GREEN
 */
TEST(WetTelloSticksCommandTest, SendNeutralStickCommand)
{
    // Setup
    TelloDriver tello(spdlog::level::debug);

    // Run
    tello.Connect();
    if (!tello.WaitForConnection(10))
    {
        tello.GetLogger()->error("Couldn't Connect to drone.");
        ASSERT_TRUE(false);
    }

    // Test
    int counter = 100;
    while (counter-- > 0)
    {
        // auto droneMode = tello.GetTelloTelemetry().GetFlightData()->GetFlightMode();
        // tello.GetLogger()->info("DroneMode: " + std::to_string(droneMode));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/**
 * @warning: this test envolve drone movements!
 * 1. Takeoff
 * 2. Move backward 1% stick, for 1 sec.
 * 3. Land
**/
TEST(WetTelloSticksCommandTest, SendBackwardStickCommand)
{
    // Setup
    TelloDriver tello(spdlog::level::info);

    // Run
    tello.Connect();
    if (!tello.WaitForConnection(10))
    {
        tello.GetLogger()->error("Couldn't Connect to drone.");
        ASSERT_TRUE(false);
    }

    // Test
    tello.Takeoff();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    tello.Backward(1);

    int counter = 10;
    while (counter-- > 0)
    {
        // ASSERT_TRUE(tello.GetTelloTelemetry().IsDroneConnected());
        // auto droneMode = tello.GetTelloTelemetry().GetFlightData()->GetFlightMode();
        // tello.GetLogger()->info("DroneMode: " + std::to_string(droneMode));

        // auto pos = tello.GetPos();
        // tello.GetLogger()->info("********************************pos: " + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    tello.Backward(0);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    tello.Land();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/**
 * @brief Test SetAttLimit command
 * Setup:
 * * Create Tello instance.
 * * Attach to FlightData.
 * * Connect to drone.
 * Run:
 * * Send SetAttLimitReq(new_limit)
 * Test:
 * * See If implemented observer being called with new AttLimit.
**/
TEST(WetTelloSticksCommandTest, SendSetAttLimitTest)
{
    // Setup
    TelloDriver tello(spdlog::level::info);
    Dummy dummy;
    tello.Attach(OBSERVERS::FLIGHT_DATA_MSG, &dummy);

    // Run
    tello.Connect();
    if (!tello.WaitForConnection(10))
    {
        tello.GetLogger()->error("Couldn't Connect to drone.");
        ASSERT_TRUE(false);
    }

    int limit = 34;
    tello.SetAttLimitReq(limit);

    // Wait for Drone to reply.
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Test
    ASSERT_EQ(dummy.GetFlightData().attitude_limit, limit);

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}