#include <stdexcept>
#include <ctime>

#include "ProtocolClient.h"
#include "log/Logger.h"
#include "database/Database.h"
#include "buffer/Converter.h"

using namespace Log;
using namespace message;
using namespace packet;
using namespace crypto;
using namespace buffer;
using namespace std::chrono_literals;

ProtocolClient::ProtocolClient(std::unique_ptr<Client> &&client, crypto::CryptoModule &cryptoModule)
        : DecoratorClient(std::move(client)), cryptoModule(cryptoModule) {
    Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) + ": Created");

    sendPacket(Packet(PacketType::OPEN_ENCR));
}

ProtocolClient::~ProtocolClient() {
    //uregister
    if (id > 0) {
        Database::getInstance().getClientDataManager().unregister(id);
    }
}

void ProtocolClient::recv() {
    auto msg = getBlockingQueue().pop_for(0ms);

    switch (msg.type) {
        case Message::Empty:
            Logger::getInstance().logDebug(
                    "ProtocolClient " + std::to_string(getConnectionFD()) + ": No message in queue");
            break;

        case Message::Close:
            Logger::getInstance().logMessage(
                    "ProtocolClient " + std::to_string(getConnectionFD()) + ": Get Close message");
            closeConnection();
            break;

        case Message::PacketSend:
            Logger::getInstance().logDebug(
                    "ProtocolClient " + std::to_string(getConnectionFD()) + ": Get PacketSend message");
            sendRemainingData();
            break;

        case Message::PacketReceive:
            Logger::getInstance().logDebug(
                    "ProtocolClient " + std::to_string(getConnectionFD()) + ": Get PacketReceive message");
            receiveData();
            break;

        case Message::Update:
            Logger::getInstance().logMessage(
                    "ProtocolClient " + std::to_string(getConnectionFD()) + ": Get EraseObserver message");
            update();
            break;

        default:
            Logger::getInstance().logDebug("ProtocolClient " + std::to_string(getConnectionFD()) +
                                           ": Get unexpected message: " + msg.toString());
    }
}

void ProtocolClient::sendData(const std::vector<unsigned char> &bytes) {
    client->sendData(bytes);
}

std::vector<unsigned char> ProtocolClient::recvData() {
    auto bytes = client->recvData();
    Logger::getInstance().logDebug("ProtocolClient: Receive bytes: " + std::to_string(bytes.size()));

    return bytes;
}

void ProtocolClient::sendRemainingData() {
    if (!remainingPackets.empty()) {
        sendPacket(remainingPackets.front());
        remainingPackets.pop();
    }
}

void ProtocolClient::receiveData() {
    switch (stage) {
        case Stage::SetPublicKey:
            setPublicKey();
            break;
        case Stage::SetSymmetricKey:
            setSymmetricKey();
            break;
        case Stage::TestKey:
            testKey();
        case Stage::StableCommunication:
            stableCommunication();
        case Stage::Exit:
            break;
    }
}

void ProtocolClient::setPublicKey() {
    try {
        cryptoModule.use(CryptoModule::Algorithm::ServerRSA);
        Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) +
                                         ": Set ServerRSA encryption");
        auto packet = recvPacket();

        if (packet.getType() == PacketType::PUBLIC_KEY) {

            //set new key
            cryptoModule.setOuterRSAKey(packet.getBuffer().popAll());
            Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) +
                                             ": Set new public key");

            cryptoModule.use(CryptoModule::Algorithm::OuterRSA);
            Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) +
                                             ": Set OuterRSA encryption");

            //send symmetric key
            Packet newPacket(PacketType::SYMMETRIC_KEY);
            newPacket.getBuffer().push_back(cryptoModule.getSymmetricKey());
            sendPacket(newPacket);

            stage = Stage::SetSymmetricKey;
        } else {
            Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                           ": Get wrong packet in SetPublicKey stage. "
                                           "Expected " + PacketType::toString(PacketType::PUBLIC_KEY));
            closeConnection();
        }

    } catch (std::exception &e) {
        Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                       ": Error in SetPublicKey stage: " + e.what());
        closeConnection();
    }
}

void ProtocolClient::setSymmetricKey() {
    try {
        cryptoModule.use(CryptoModule::Algorithm::ServerRSA);
        Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) +
                                         ": Set ServerRSA encryption");
        auto packet = recvPacket();

        if (packet.getType() == PacketType::SYMMETRIC_KEY) {

            //check validation of test
            if (cryptoModule.getSymmetricKey() != packet.getBuffer().popAll()) {
                Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                               ": Get wrong returning symmetric key");
                closeConnection();
            } else {
                Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) +
                                                 ": Returning symmetric key is correct");

                //set AES encryption
                cryptoModule.use(CryptoModule::Algorithm::AES);
                Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) +
                                                 ": Set AES encryption");


                Packet newPacket(PacketType::TEST_KEY);
                newPacket.getBuffer().push_back(cryptoModule.getSymmetricKey());
                sendPacket(newPacket);

                stage = Stage::TestKey;
            }
        } else {
            Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                           ": Get wrong packet in SetSymmetricKey stage. "
                                           "Expected " + PacketType::toString(PacketType::SYMMETRIC_KEY));
            closeConnection();
        }
    } catch (std::exception &e) {
        Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                       ": Error in SetPublicKey stage: " + e.what());
        closeConnection();
    }
}

void ProtocolClient::testKey() {
    try {
        cryptoModule.use(CryptoModule::Algorithm::AES);
        Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) +
                                         ": Set AES encryption");
        auto packet = recvPacket();

        if (packet.getType() == PacketType::TEST_KEY) {

            Logger::getInstance().logMessage("ProtocolClient " + std::to_string(getConnectionFD()) +
                                             ": Test key for symmetric key was correct");

            sendPacket(Packet(PacketType::OPEN_PROT));

            stage = Stage::StableCommunication;

            selfRegister();

        } else {
            Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                           ": Get wrong packet in TestKey stage. "
                                           "Expected " + PacketType::toString(PacketType::TEST_KEY));
            closeConnection();
        }
    } catch (std::exception &e) {
        Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                       ": Error in TestKey stage: " + e.what());
        closeConnection();
    }
}

void ProtocolClient::selfRegister() {
    id = Database::getInstance().getClientDataManager().registerClient(
            cryptoModule.getOuterRSAKey(), &getBlockingQueue());
}

void ProtocolClient::stableCommunication() {
    try {
        auto packet = recvPacket();

        switch (packet.getType()) {
            case PacketType::CLOSE:
                closeConnection();
                break;
            case PacketType::SET_NAME:
                setName(packet);
                break;
            case PacketType::ADD_FOLLOWER:
                addFollower(packet);
                break;
            case PacketType::REMOVE_FOLLOWER:
                removeFollower(packet);
                break;
            case PacketType::REMOVE_FOLLOWED:
                removeFollowed(packet);
                break;
            case PacketType::MY_LOCATION:
                myLocation(packet);
                break;

            default: {
                Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                               ": Get wrong packet in StableCommunication stage: " +
                                               PacketType::toString(packet.getType()));
            }
        }
    } catch (std::exception &e) {
        Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                       ": Error in StableCommunication stage: " + e.what());
        closeConnection();
    }
}

void ProtocolClient::setName(packet::Packet &packet) {
    auto res = Database::getInstance().getClientDataManager().setName(id, toString(packet.getBuffer().popAll()));

    if (res) {
        sendPacket(Packet(PacketType::ACK_OK));
    } else {
        sendPacket(Packet(PacketType::ACK_ERR));
    }
}

void ProtocolClient::addFollower(packet::Packet &packet) {
    auto followerId = Database::getInstance().getClientDataManager().getNameId(toString(packet.getBuffer().popAll()));

    if (followerId > 0) {
        Database::getInstance().getClientDataManager().addFollower(id, followerId);
    }
}

void ProtocolClient::removeFollower(packet::Packet &packet) {
    auto followerId = Database::getInstance().getClientDataManager().getNameId(toString(packet.getBuffer().popAll()));

    if (followerId > 0) {
        Database::getInstance().getClientDataManager().removeFollower(id, followerId);
    }
}

void ProtocolClient::removeFollowed(packet::Packet &packet) {
    auto followerId = Database::getInstance().getClientDataManager().getNameId(toString(packet.getBuffer().popAll()));

    if (followerId > 0) {
        Database::getInstance().getClientDataManager().stopFollowing(id, followerId);
    }
}

void ProtocolClient::myLocation(packet::Packet &packet) {
    if (packet.getBuffer().size() != 16) {
        Logger::getInstance().logError("ProtocolClient " + std::to_string(getConnectionFD()) +
                                       ": Error in myLocation stage: Packet does not contain 16 bytes");
        closeConnection();
    } else {
        float latitude;
        float longitude;
        time_t deltaTime;

        from_bytes(packet.getBuffer().pop(4), latitude);
        from_bytes(packet.getBuffer().pop(4), longitude);
        from_bytes(packet.getBuffer().pop(4), deltaTime);
        auto time = std::time(nullptr) - deltaTime;

        Database::getInstance().getClientDataManager().addPosition(id, latitude, longitude, time);
    }
}

void ProtocolClient::update() {
    if (stage == Stage::StableCommunication) {
        auto names = Database::getInstance().getClientDataManager().getNewNames(id);
        for (auto &name : names) {
            Packet newPacket(PacketType::NEW_FOLLOWED);
            newPacket.getBuffer().push_back(to_bytes(name.first));
            newPacket.getBuffer().push_back(to_bytes(name.second));

            remainingPackets.push(newPacket);
        }

        auto positions = Database::getInstance().getClientDataManager().getNewPositions(id);
        for (auto &position : positions) {
            Packet newPacket(PacketType::LOCATION);
            newPacket.getBuffer().push_back(to_bytes(position.first));
            newPacket.getBuffer().push_back(to_bytes(position.second.latitude));
            newPacket.getBuffer().push_back(to_bytes(position.second.longitude));
            newPacket.getBuffer().push_back(to_bytes(position.second.time));

            remainingPackets.push(newPacket);
        }
    }
    sendRemainingData();
}
