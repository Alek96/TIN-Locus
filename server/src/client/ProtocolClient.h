#ifndef LOCUS_PROTOCOLCLIENT_H
#define LOCUS_PROTOCOLCLIENT_H

#include <memory>
#include <queue>
#include "DecoratorClient.h"
#include "crypto/CryptoModule.h"

class ProtocolClient : public DecoratorClient {
    enum class Stage : int {
        SetPublicKey,
        SetSymmetricKey,
        TestKey,
        StableCommunication,
        Exit
    };
public:
    explicit ProtocolClient(std::unique_ptr<Client> &&client, crypto::CryptoModule &cryptoModule);

    ProtocolClient(ProtocolClient&&) noexcept = default;

    ~ProtocolClient() override;

    void recv() override;
    void send() override {}

    void sendData(const std::vector<unsigned char> &bytes) override;
    std::vector<unsigned char> recvData() override;

private:
    crypto::CryptoModule& cryptoModule;
    Stage stage = Stage::SetPublicKey;
    std::queue<packet::Packet> remainingPackets;
    short id = 0;

    void sendRemainingData();
    void receiveData();

    void setPublicKey();
    void setSymmetricKey();
    void testKey();
    void stableCommunication();

    void setName(packet::Packet &packet);

    void selfRegister();

    void addFollower(packet::Packet &packet);

    void removeFollower(packet::Packet &packet);

    void removeFollowed(packet::Packet &packet);

    void myLocation(packet::Packet &packet);

    void update();

    void eraseObserver(short &id);
};


#endif //LOCUS_PROTOCOLCLIENT_H
