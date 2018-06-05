#ifndef LOCUS_PACKETTYPE_H
#define LOCUS_PACKETTYPE_H

#include <cstdint>
#include <unordered_map>
#include <string>

namespace packet {
    struct PacketType {
        enum header : uint8_t {
            OPEN = 0,
            CLOSE = 1,
            ACK_OK = 2,
            ACK_ERR = 3,
            ALIVE = 4,

            OPEN_ENCR = 8,
            PUBLIC_KEY = 9,
            SYMMETRIC_KEY = 10,
            TEST_KEY = 11,

            OPEN_PROT = 16,
            SET_NAME = 17,
            ADD_FOLLOWER = 18,
            NEW_FOLLOWED = 19,
            REMOVE_FOLLOWER = 20,
            REMOVE_FOLLOWED = 21,
            MY_LOCATION = 22,
            LOCATION = 23,

            UNDEFINED = 100
        };

        static uint8_t toUInt8(PacketType::header header);

        static PacketType::header toPacketType(uint8_t header);

        static std::string toString(PacketType::header header);
    };

}

#endif //LOCUS_PACKETTYPE_H
