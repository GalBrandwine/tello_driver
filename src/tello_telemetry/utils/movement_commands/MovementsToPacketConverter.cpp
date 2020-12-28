#include "MovementsToPacketConverter.hpp"
namespace tello_protocol
{
    bool MovementsToPacketConverter::Convert(const std::unordered_map<Sticks, float> &stick_movements_dict, Packet &pktOut)
    {
        /**
         * Convert stick movements into a packet according to tello_protocol. 
         * Assumes pktOut is empty. 
        **/
        if (pktOut.GetData().size() > 0)
            return false;
        
        /* 
        axis1 = int(1024 + 660.0 * self.right_x) & 0x7ff
        axis2 = int(1024 + 660.0 * self.right_y) & 0x7ff
        axis3 = int(1024 + 660.0 * self.left_y) & 0x7ff
        axis4 = int(1024 + 660.0 * self.left_x) & 0x7ff
        axis5 = int(self.fast_mode) & 0x01        
        '''
        11 bits (-1024 ~ +1023) x 4 axis = 44 bits
        fast_mode takes 1 bit        
        44 bits will be packed in to 6 bytes (48 bits)

                    axis4      axis3      axis2      axis1
             |          |          |          |          |
                 4         3         2         1         0
        98765432109876543210987654321098765432109876543210
         |       |       |       |       |       |       |
             byte5   byte4   byte3   byte2   byte1   byte0
        '''
        log.info("stick command: fast=%d yaw=%4d thr=%4d pit=%4d rol=%4d" %
                  (axis5, axis4, axis3, axis2, axis1))
        log.info("stick command: fast=%04x yaw=%04x thr=%04x pit=%04x rol=%04x" %
                  (axis5, axis4, axis3, axis2, axis1))
        packed = axis1 | (axis2 << 11) | (axis3 << 22) | (axis4 << 33) | (axis5 << 44)
        packed_bytes = struct.pack('<Q', packed)
        pkt.add_byte(byte(packed_bytes[0]))
        pkt.add_byte(byte(packed_bytes[1]))
        pkt.add_byte(byte(packed_bytes[2]))
        pkt.add_byte(byte(packed_bytes[3]))
        pkt.add_byte(byte(packed_bytes[4]))
        pkt.add_byte(byte(packed_bytes[5]))
        pkt.add_time()
        pkt.fixup()
        log.info("stick command: %s" % byte_to_hexstring(pkt.get_buffer()))
         */
        return true;
    }
    MovementsToPacketConverter::MovementsToPacketConverter(/* args */)
    {
    }

    MovementsToPacketConverter::~MovementsToPacketConverter()
    {
    }
} // namespace tello_protocol
