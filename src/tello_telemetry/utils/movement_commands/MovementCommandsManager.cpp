#include "MovementCommandsManager.hpp"

namespace tello_protocol
{
    const std::unordered_map<Sticks, float> &MovementCommandsManager::GetStickMovements() const
    {
        return m_SticksDict;
    }

    bool MovementCommandsManager::SetMovementCommand(Movements movement, float amount)
    {
        /**
         * These movements imitate stick movements, hence  'amount' is how much stick was given to the desired direction
         * This function accept only values between 0 to 100.
        **/

        if (amount < 0 || amount > 100)
            return false;

        std::cout << __PRETTY_FUNCTION__ << movement_to_string(movement).c_str() << "\n";
        /* 
       def up(self, val):
        """Up tells the drone to ascend. Pass in an int from 0-100."""
        log.info('up(val=%d)' % val)
        self.left_y = val / 100.0

    def down(self, val):
        """Down tells the drone to descend. Pass in an int from 0-100."""
        log.info('down(val=%d)' % val)
        self.left_y = val / 100.0 * -1

    def forward(self, val):
        """Forward tells the drone to go forward. Pass in an int from 0-100."""
        log.info('forward(val=%d)' % val)
        self.right_y = val / 100.0

    def right(self, val):
        """Right tells the drone to go right. Pass in an int from 0-100."""
        log.info('right(val=%d)' % val)
        self.right_x = val / 100.0

    def left(self, val):
        """Left tells the drone to go left. Pass in an int from 0-100."""
        log.info('left(val=%d)' % val)
        self.right_x = val / 100.0 * -1

    def clockwise(self, val):
        """
        Clockwise tells the drone to rotate in a clockwise direction.
        Pass in an int from 0-100.
        """
        log.info('clockwise(val=%d)' % val)
        self.left_x = val / 100.0

    def counter_clockwise(self, val):
        """
        CounterClockwise tells the drone to rotate in a counter-clockwise direction.
        Pass in an int from 0-100.
        """
        log.info('counter_clockwise(val=%d)' % val)
        self.left_x = val / 100.0 * -1
         */
        switch (movement)
        {
        case Movements::BACKWARD:
            // Backward tells the drone to go in reverse. Pass in an int from 0-100.
            set_backward(amount);
            return true;
            break;

        default:
            return false;
            break;
        }
    }

    void MovementCommandsManager::set_backward(float amount)
    {
        /* 
    Backward movements interpreted via right stick, in Y axes (Movements are stick-wise) 
    */

        /* 
        def backward(self, val):
        """Backward tells the drone to go in reverse. Pass in an int from 0-100."""
        log.info('backward(val=%d)' % val)
        self.right_y = val / 100.0 * -1
         */
        std::cout << "Setting backward(" << amount << ")\n";
        m_SticksDict[Sticks::RIGHT_Y] = amount / 100 * -1;
    }

    MovementCommandsManager::MovementCommandsManager(/* args */)
    {
        std::cout << __PRETTY_FUNCTION__ << "::" << __LINE__ << "::Initiated";
        for (int possible_stick_movement = Sticks::RIGHT_Y; possible_stick_movement != Sticks::LEFT_X; possible_stick_movement++)
        {
            Sticks movement = static_cast<Sticks>(possible_stick_movement);
            m_SticksDict[movement] = STICK_NEUTRAL_VALUE;
        }
    }

    MovementCommandsManager::~MovementCommandsManager()
    {
    }
} // namespace tello_protocol
