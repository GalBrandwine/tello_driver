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

/**
 * @brief Test the whole pipeline. 
 * E.g:
 * 1. Instantiate TelloDriver
 * 2. Send connect req.
 * 3. Wait until connection has been established.
 * */
TEST(WetTelloCommandTest, TrySendReceiveConnReq)
{
    TelloDriver tello(spdlog::level::debug);
    tello.Connect();
    EXPECT_TRUE(tello.WaitForConnection(10));
    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/**
 * @test Test if ThrowAndGoCommand works.
 * @warning This test require drone movements!
 * 
 * Setup:
 * * Create Tello instance.
 * * Attach to FlightData.
 * Run:
 * * Connect to drone.
 * * Send GetLowBatThresh() command
 * Test:
 * * See if Received LowBatThresh and stored corresclty.
 */
TEST(WetTelloCommandTest, TryThrowAndGoTest)
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

    // int new_thresh = 50;
    // tello.SetBatThreshReq(new_thresh);

    // Test
    tello.ThrowAndGo();
    tello.GetLogger()->info("Now the user should throw the drone away..\nWaiting for 5 secs.");
    std::this_thread::sleep_for(std::chrono::seconds(5));

    bool condition_met = false;
    int num_of_tries = 1000;
    while (num_of_tries >= 0)
    {
        auto fly_mode = dummy.GetFlightData().fly_mode;
        tello.GetLogger()->info("Current fly mode: {}", std::to_string(fly_mode));
        tello.GetLogger()->info("bat: {}", std::to_string(dummy.GetFlightData().battery_percentage));
        tello.GetLogger()->info("drone_battery_left: {}", std::to_string(dummy.GetFlightData().drone_battery_left));
        tello.GetLogger()->info("fly_time: {}", std::to_string(dummy.GetFlightData().fly_time));
        tello.GetLogger()->info("drone_fly_time_left: {}\n", std::to_string(dummy.GetFlightData().drone_fly_time_left));
        num_of_tries--;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    tello.GetLogger()->info("*************** Landing ***************");
    tello.Land();

    num_of_tries = 1000;
    while (num_of_tries >= 0)
    {
        auto fly_mode = dummy.GetFlightData().fly_mode;
        tello.GetLogger()->info("Current fly mode: {}", fly_mode);
        num_of_tries--;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    ASSERT_TRUE(condition_met);

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/**
* @test Test if driver allert about drone disconnection.
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
 * @test Test if drone receives neutral stick commands. 
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

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/**
 * @test Movements
 * 
 * @warning: this test evolve drone movements!
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
 * @test Test SetAttLimit command
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
    bool condition_met = false;
    int num_of_tries = 100;

    while (!condition_met and num_of_tries >= 0)
    {
        condition_met = dummy.GetFlightData().attitude_limit == limit;
        num_of_tries--;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Test
    ASSERT_TRUE(condition_met);

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/**
 * @test Test If LowBatThreshMsg received and stored correctly.
 * Setup:
 * * Create Tello instance.
 * * Attach to FlightData.
 * * Connect to drone.
 * Run:
 * * Send GetLowBatThresh() command
 * Test:
 * * See if Received LowBatThresh and stored corresclty.
**/
TEST(WetTelloSticksCommandTest, GetLowBatThreshMsgTest)
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

    // Test
    bool condition_met = false;
    int num_of_tries = 100;
    while (!condition_met and num_of_tries >= 0)
    {
        condition_met = dummy.GetFlightData().low_battery_threshold != -1;
        num_of_tries--;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ASSERT_TRUE(condition_met);

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

/**
 * @test Test If LOW_BAT_THRESHOLD_CMD command working properly.
 * Setup:
 * * Create Tello instance.
 * * Attach to FlightData.
 * * Connect to drone.
 * Run:
 * * Send SetLowBatThresh(new_thresh) command
 * Test:
 * * See if received LowBatThresh has desired new thresh.
**/
TEST(WetTelloSticksCommandTest, SetLowBatThreshMsgTest)
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

    int new_thresh = 50;
    tello.SetBatThreshReq(new_thresh);

    // Test
    bool condition_met = false;
    int num_of_tries = 100;
    while (!condition_met and num_of_tries >= 0)
    {
        condition_met = dummy.GetFlightData().low_battery_threshold == new_thresh;
        num_of_tries--;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ASSERT_TRUE(condition_met);

    std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}