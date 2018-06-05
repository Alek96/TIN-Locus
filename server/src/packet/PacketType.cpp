#include "PacketType.h"

namespace packet {

    uint8_t packet::PacketType::toUInt8(header header) {
        return static_cast<uint8_t>(header);
    }

    packet::PacketType::header packet::PacketType::toPacketType(uint8_t header) {
        static std::unordered_map<uint8_t, PacketType::header> packetTypeMap = {
                {static_cast<uint8_t>(PacketType::header::OPEN),            PacketType::header::OPEN},
                {static_cast<uint8_t>(PacketType::header::CLOSE),           PacketType::header::CLOSE},
                {static_cast<uint8_t>(PacketType::header::ACK_OK),          PacketType::header::ACK_OK},
                {static_cast<uint8_t>(PacketType::header::ACK_ERR),         PacketType::header::ACK_ERR},
                {static_cast<uint8_t>(PacketType::header::ALIVE),           PacketType::header::ALIVE},

                {static_cast<uint8_t>(PacketType::header::OPEN_ENCR),       PacketType::header::OPEN_ENCR},
                {static_cast<uint8_t>(PacketType::header::PUBLIC_KEY),      PacketType::header::PUBLIC_KEY},
                {static_cast<uint8_t>(PacketType::header::SYMMETRIC_KEY),   PacketType::header::SYMMETRIC_KEY},
                {static_cast<uint8_t>(PacketType::header::TEST_KEY),        PacketType::header::TEST_KEY},

                {static_cast<uint8_t>(PacketType::header::OPEN_PROT),       PacketType::header::OPEN_PROT},
                {static_cast<uint8_t>(PacketType::header::SET_NAME),        PacketType::header::SET_NAME},
                {static_cast<uint8_t>(PacketType::header::ADD_FOLLOWER),    PacketType::header::ADD_FOLLOWER},
                {static_cast<uint8_t>(PacketType::header::NEW_FOLLOWED),    PacketType::header::NEW_FOLLOWED},
                {static_cast<uint8_t>(PacketType::header::REMOVE_FOLLOWER), PacketType::header::REMOVE_FOLLOWER},
                {static_cast<uint8_t>(PacketType::header::REMOVE_FOLLOWED), PacketType::header::REMOVE_FOLLOWED},
                {static_cast<uint8_t>(PacketType::header::MY_LOCATION),     PacketType::header::MY_LOCATION},
                {static_cast<uint8_t>(PacketType::header::LOCATION),        PacketType::header::LOCATION},
        };

        return packetTypeMap.count(header) ? packetTypeMap.at(header) : PacketType::header::UNDEFINED;
    }

    std::string PacketType::toString(PacketType::header header) {
        static std::unordered_map<PacketType::header, std::string> packetTypeMap = {
                {PacketType::header::OPEN,            "OPEN"},
                {PacketType::header::CLOSE,           "CLOSE"},
                {PacketType::header::ACK_OK,          "ACK_OK"},
                {PacketType::header::ACK_ERR,         "ACK_ERR"},
                {PacketType::header::ALIVE,           "ALIVE"},

                {PacketType::header::OPEN_ENCR,       "OPEN_ENCR"},
                {PacketType::header::PUBLIC_KEY,      "PUBLIC_KEY"},
                {PacketType::header::SYMMETRIC_KEY,   "SYMMETRIC_KEY"},
                {PacketType::header::TEST_KEY,        "TEST_KEY"},

                {PacketType::header::OPEN_PROT,       "OPEN_PROT"},
                {PacketType::header::SET_NAME,        "SET_NAME"},
                {PacketType::header::ADD_FOLLOWER,    "ADD_FOLLOWER"},
                {PacketType::header::NEW_FOLLOWED,    "NEW_FOLLOWED"},
                {PacketType::header::REMOVE_FOLLOWER, "REMOVE_FOLLOWER"},
                {PacketType::header::REMOVE_FOLLOWED, "REMOVE_FOLLOWED"},
                {PacketType::header::MY_LOCATION,     "MY_LOCATION"},
                {PacketType::header::LOCATION,        "LOCATION"},
        };

        return packetTypeMap.count(header) ? packetTypeMap.at(header) : "UNDEFINED";
    }
}