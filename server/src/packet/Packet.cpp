#include "Packet.h"
#include "buffer/Converter.h"

using namespace buffer;

namespace packet {

    Packet::Packet(PacketType::header type) : type(type) {}

    PacketType::header Packet::getType() const {
        return type;
    }

    Buffer &Packet::getBuffer() {
        return buffer;
    }

    const Buffer &Packet::getBuffer() const {
        return buffer;
    }

    void Packet::addTypeToBuffer() {
        buffer.push_front(to_bytes(PacketType::toUInt8(type)));
    }

    void Packet::parse() {
        auto headerBuffer = buffer.pop(1);
        uint8_t headerId;
        from_bytes(headerBuffer, headerId);
        type = PacketType::toPacketType(headerId);
    }
}