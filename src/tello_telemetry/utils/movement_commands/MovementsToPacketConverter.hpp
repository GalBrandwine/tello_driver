#pragma once
#include <unordered_map>

#include "MovementCommandsManager.hpp"
#include "protocol.hpp"
/** Some initial thoughts:
 * This class could be wraped with IConverter interface . 
 * But since its not likely to have more packages that need to be converted. An interface is overkill.
 */

namespace tello_protocol
{
    class MovementsToPacketConverter
    {
    public:
        bool Convert(const std::unordered_map<Sticks, float> &, Packet &);
        MovementsToPacketConverter(/* args */);
        ~MovementsToPacketConverter();

    private:
        /* data */
    };
} // namespace tello_protocol
