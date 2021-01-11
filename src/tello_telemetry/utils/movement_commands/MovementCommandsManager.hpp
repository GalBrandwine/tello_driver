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
        bool SetMovementCommand(Movements, float);
        bool SetFastMode(bool);

        MovementCommandsManager(/* args */);
        ~MovementCommandsManager();

    private:
        void set_backward(float);
        std::unordered_map<Sticks, float> m_SticksDict;
    };
} // namespace tello_protocol
