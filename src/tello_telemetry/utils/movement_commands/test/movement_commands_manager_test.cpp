#include "movement_commands_manager_test.hpp"
// using namespace std::chrono_literals;

TEST(MovementCommandsManager, BasicTestUnsupportedValues)
{
    /*************************************************************
     * Test if this manager protects against un-supported values
     *************************************************************/

    auto manager = tello_protocol::MovementCommandsManager();
    ASSERT_FALSE(manager.SetMovementCommand(tello_protocol::Movements::BACKWARD, -10));
    // std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

TEST(MovementCommandsManager, BasicTestMoveBackward)
{
    /*************************************************************
     * Test if MoveBackaward receive data from user
     *************************************************************/

    auto manager = tello_protocol::MovementCommandsManager();
    ASSERT_TRUE(manager.SetMovementCommand(tello_protocol::Movements::BACKWARD, 10));
}

TEST(StickMovementsConverter, PacketEmptynessTest)
{
    /*************************************************************
     * Test converter return FALSE if entered non-empty packet
     *************************************************************/

    // Setup
    auto manager = tello_protocol::MovementCommandsManager();
    auto converter = tello_protocol::MovementsToPacketConverter();

    // Test
    auto pkt = tello_protocol::Packet(tello_protocol::STICK_CMD, 0x60);
    pkt.AddByte('G');
    pkt.AddByte('A');
    pkt.AddByte('L');
    ASSERT_FALSE(converter.Convert(manager.GetStickMovements(), pkt));
}

TEST(StickMovementsConverter, PacketEmptynessTest)
{
    /*************************************************************
     * Test converted stick movements, if the packet looks like It 
     * suppose to.
     *************************************************************/

    // Setup
    auto manager = tello_protocol::MovementCommandsManager();
    auto converter = tello_protocol::MovementsToPacketConverter();

    manager.SetMovementCommand(tello_protocol::Movements::BACKWARD, 10);

    // Test
    auto pkt = tello_protocol::Packet(tello_protocol::STICK_CMD, 0x60);
    converter.Convert(manager.GetStickMovements(), pkt);

    ASSERT_FALSE();
}
