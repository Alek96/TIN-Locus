#include "SimpleClient.h"
#include "log/Logger.h"

using namespace Log;
using namespace message;
using namespace std::chrono_literals;

SimpleClient::SimpleClient(const sockaddr &client_addr, std::unique_ptr<ClientBuffer> clientBuffer,
                         message::BlockingQueue<message::Message> *clientManagerBlockingQueue)
        : client_addr(client_addr), clientBuffer(std::move(clientBuffer)), blockingQueue(1000),
          clientManagerBlockingQueue(clientManagerBlockingQueue) {

    setFileDescriptor(blockingQueue.getReadFd());

//  set clientBuffer blockingQueue for return
    this->clientBuffer->setClientBlockingQueue(&getBlockingQueue());

    Logger::getInstance().logDebug("SimpleClient " + std::to_string(getConnectionFD()) + ": Created");
}

SimpleClient::~SimpleClient() {
    setFileDescriptor(0);
}

void SimpleClient::recv() {
    auto msg = blockingQueue.pop_for(0ms);

    switch (msg.type) {
        case Message::Empty:
            Logger::getInstance().logDebug(
                    "SimpleClient " + std::to_string(getConnectionFD()) + ": No message in queue");
            break;

        case Message::Close:
            Logger::getInstance().logMessage(
                    "SimpleClient " + std::to_string(getConnectionFD()) + ": Get Close message");
            closeConnection();
            break;

        default:
            Logger::getInstance().logDebug("SimpleClient: Get unexpected message: " + msg.toString());
    }
}

void SimpleClient::sendData(const std::vector<unsigned char> &bytes) {
    clientBuffer->getBufferOut().push_back(const_cast<std::vector<unsigned char> &>(bytes));
    clientBuffer->getBufferOut().setStage(BlockingBuffer::Stage::Full);
    //notify sender
    clientBuffer->send();
}

std::vector<unsigned char> SimpleClient::recvData() {
    Logger::getInstance().logDebug("SimpleClient: Try pop clientBuffer: " +
                                   std::to_string(clientBuffer->getBufferIn().size()));

    auto bytes = clientBuffer->getBufferIn().pop(clientBuffer->getBufferIn().size());

    Logger::getInstance().logDebug("SimpleClient: Receive bytes: " + std::to_string(bytes.size()));

    clientBuffer->getBufferIn().setStage(BlockingBuffer::Stage::Empty);
    return bytes;
}

const sockaddr &SimpleClient::getClient_addr() const {
    return client_addr;
}

const int SimpleClient::getConnectionFD() const {
    return clientBuffer->getFileDescriptor();
}

message::BlockingQueue<message::Message> &SimpleClient::getBlockingQueue() {
    return blockingQueue;
}

void SimpleClient::closeConnection() {
    Logger::getInstance().logError("SimpleClient " + std::to_string(getConnectionFD()) +
                                   ": Closing connection");

    Message msg(Message::EraseClient);
    msg.fileDescriptor = std::make_unique<int>(clientBuffer->getFileDescriptor());
    clientManagerBlockingQueue->priorityPush(std::move(msg));
}

message::BlockingQueue<message::Message> &SimpleClient::getClientManagerBlockingQueue() {
    return *clientManagerBlockingQueue;
}
