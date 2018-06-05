#include "EncryptClient.h"
#include "log/Logger.h"

using namespace Log;
using namespace message;
using namespace std::chrono_literals;

EncryptClient::EncryptClient(std::unique_ptr<Client> &&client)
        : DecoratorClient(std::move(client)) {

    Logger::getInstance().logDebug("EncryptClient " + std::to_string(getConnectionFD()) + ": Created");
}

void EncryptClient::recv() {
    auto msg = getBlockingQueue().pop_for(0ms);

    switch (msg.type) {
        case Message::Empty:
            Logger::getInstance().logDebug(
                    "EncryptClient " + std::to_string(getConnectionFD()) + ": No message in queue");
            break;

        case Message::Close:
            Logger::getInstance().logMessage(
                    "EncryptClient " + std::to_string(getConnectionFD()) + ": Get Close message");
            closeConnection();
            break;

        default:
            Logger::getInstance().logDebug("EncryptClient: Get unexpected message: " + msg.toString());
    }
}

void EncryptClient::sendData(const std::vector<unsigned char> &bytes) {
    client->sendData(cryptoModule.encrypt(bytes));
}

std::vector<unsigned char> EncryptClient::recvData() {
    auto bytes = client->recvData();
    Logger::getInstance().logDebug("EncryptClient: Receive bytes: " + std::to_string(bytes.size()));

    return cryptoModule.decrypt(bytes);
}

crypto::CryptoModule &EncryptClient::getCryptoModule() {
    return cryptoModule;
}
