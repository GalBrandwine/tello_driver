#pragma once
#include <unordered_map>
#include <atomic>
#include <iostream>

namespace tello_protocol
{

    enum Sticks
    {
        RIGHT_Y = 0,
        RIGHT_X,
        LEFT_Y,
        LEFT_X,
        FAST_MODE
    };

    enum Movements
    {
        FORWARD = 0,
        BACKWARD,
        UP,
        DOWN,
        RIGHT,
        LEFT,
        CLOCKWISE,
        COUNTER_CLOCKWISE

    };

    static short int STICK_NEUTRAL_VALUE = 0;
    static short int STICK_NEUTRAL_CONVERTION_VALUE = 1024;
    static const std::string movement_to_string(Movements movement)
    {
        switch (movement)
        {
        case Movements::FORWARD:
            return "FORWARD";
            break;
        case Movements::BACKWARD:
            return "BACKWARD";
            break;
        case Movements::UP:
            return "UP";
            break;
        case Movements::DOWN:
            return "DOWN";
            break;
        case Movements::RIGHT:
            return "RIGHT";
            break;
        case Movements::LEFT:
            return "LEFT";
            break;
        case Movements::CLOCKWISE:
            return "CLOCKWISE";
            break;
        case Movements::COUNTER_CLOCKWISE:
            return "COUNTER_CLOCKWISE";
            break;
        default:
            return "NO SUCH MOVEMENT";
            break;
        }
    }

    class MovementCommandsManager
    {
    public:
        const std::unordered_map<Sticks, float> &GetStickMovements() const;
        /**
         * @brief Set the Movement Command.
         * These movements imitate stick movements, hence  'amount' is how much stick was given to the desired direction
         * This function accept only values between 0 to 100.
         * 
         * @param amount - Amount of stick movement [0,100]
         * @return true - If command set succesfully.
         * @return false - If (amount  > 100 or amount < 100)
         */
        bool SetMovementCommand(Movements, float amount);
        bool SetFastMode(bool);

        MovementCommandsManager(/* args */);
        ~MovementCommandsManager();

    private:
        /**
         * @brief Forward movements interpreted via right stick, in Y axes (Movements are stick-wise).
         * 
         * @param amount - Amount of stick movement [0,100]
         */
        void set_forward(float amount);

        /**
         * @brief Backward movements interpreted via right stick, in Y axes (Movements are stick-wise).
         * @param amount - Amount of stick movement [0,100]
         */
        void set_backward(float amount);

        /**
         * @brief Rotate clockwise movements interpreted via left stick, in X axes (Movements are stick-wise).
         * @param amount - Amount of stick movement [0,100]
         */
        void set_clockwise(float amount);

        /**
         * @brief Tells the drone to rotate in a counter-clockwise direction
         * Movements interpreted via left stick, in X axes (Movements are stick-wise).
         * 
         * @param amount - Amount of stick movement [0,100]
         */
        void set_counter_clockwise(float amount);

        /**
         * @brief Left tells the drone to go left
         * Movements interpreted via right stick, in X axes (Movements are stick-wise).
         * 
         * @param amount - Amount of stick movement [0,100]
        */
        void set_left(float amount);

        /**
         * @brief Right tells the drone to go right
         * Movements interpreted via right stick, in X axes (Movements are stick-wise).
         * 
         * @param amount - Amount of stick movement [0,100]
         */
        void set_right(float amount);

        /**
         * @brief Up tells the drone to ascend
         * Movements interpreted via left stick, in Y axes (Movements are stick-wise).
         * 
         * @param amount - Amount of stick movement [0,100]
         */
        void set_up(float amount);

        /**
         * @brief Up tells the drone to descend
         * Movements interpreted via left stick, in Y axes (Movements are stick-wise).
         * 
         * @param amount - Amount of stick movement [0,100]
         */
        void set_down(float amount);
        std::unordered_map<Sticks, float> m_SticksDict;
    };
} // namespace tello_protocol
