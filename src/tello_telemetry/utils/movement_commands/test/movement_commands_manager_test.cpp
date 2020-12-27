#include "movement_commands_manager_test.hpp"
// using namespace std::chrono_literals;


TEST(MovementCommandsManager, BasicTestUnsupportedValues)
{
    /*************************************************************
     * Test if this manager protects against un-supported values
     *************************************************************/
    
    auto manager = MovementCommandsManager();
    ASSERT_FALSE(manager.SetMovementCommand(Movements::BACKWARD,-10));
    // std::cout << "Done " << testing::UnitTest::GetInstance()->current_test_info()->name() << std::endl;
}

TEST(MovementCommandsManager, BasicTestMoveBackward)
{
    /*************************************************************
     * Test if MoveBackaward receive data from user
     *************************************************************/
    
    auto manager = MovementCommandsManager();
    ASSERT_TRUE(manager.SetMovementCommand(Movements::BACKWARD,10));
}
