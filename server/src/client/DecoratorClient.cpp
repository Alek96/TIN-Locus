#include "DecoratorClient.h"
#include "log/Logger.h"

using namespace Log;
using namespace message;

DecoratorClient::DecoratorClient(std::unique_ptr<Client> &&client)
        : client(std::move(client)) {}

const sockaddr &DecoratorClient::getClient_addr() const {
    return client->getClient_addr();
}

int DecoratorClient::getFileDescriptor() const {
    return client->getFileDescriptor();
}

const int DecoratorClient::getConnectionFD() const {
    return client->getConnectionFD();
}

message::BlockingQueue<message::Message> &DecoratorClient::getBlockingQueue() {
    return client->getBlockingQueue();
}

void DecoratorClient::closeConnection() {
    client->closeConnection();
}

message::BlockingQueue<message::Message> &DecoratorClient::getClientManagerBlockingQueue() {
    return client->getClientManagerBlockingQueue();
}
