#include "Client.h"
#include "log/Logger.h"

using namespace Log;
using namespace packet;

void Client::sendPacket(const Packet &packet) {
    auto newPacket = packet;
    newPacket.addTypeToBuffer();
    sendData(newPacket.getBuffer().popAll());

    Logger::getInstance().logMessage("Client " + std::to_string(getConnectionFD()) +
                                     ": Send packet: " + PacketType::toString(packet.getType()));
}

packet::Packet Client::recvPacket() {
    packet::Packet packet;
    packet.getBuffer().push_back(recvData());
    Logger::getInstance().logDebug("Client " + std::to_string(getConnectionFD()) +
                                   ": Get packet with bytes: " + std::to_string(packet.getBuffer().size()));
    packet.parse();

    Logger::getInstance().logMessage("Client " + std::to_string(getConnectionFD()) +
                                     ": Get packet: " + PacketType::toString(packet.getType()));
    return packet;
}
