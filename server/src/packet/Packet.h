#ifndef LOCUS_PACKET_H
#define LOCUS_PACKET_H

#include "PacketType.h"
#include "buffer/Buffer.h"

namespace packet {
    class Packet {
    public:
        Packet() = default;
        explicit Packet(PacketType::header type);

        PacketType::header getType() const;

        Buffer &getBuffer();
        const Buffer &getBuffer() const;

        void addTypeToBuffer();
        void parse();

    private:
        PacketType::header type = PacketType::UNDEFINED;
        Buffer buffer;
    };
}


#endif //LOCUS_PACKET_H
