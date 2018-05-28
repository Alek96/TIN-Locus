#include "ClientManager.h"
#include "log/Logger.h"
#include "client/ClientBuffer.h"
#include "client/SimpleClient.h"
#include "client/EncryptClient.h"
#include "client/ProtocolClient.h"

#include <iostream>

using namespace log;
using namespace message;
using namespace std::chrono_literals;


ClientManager::ClientManager(Server &server, Receiver &receiver, Sender &sender)
        : blockingQueue(1000), server(server), receiver(receiver), sender(sender) {
    setFileDescriptor(blockingQueue.getReadFd());
    Logger::getInstance().logMessage("ClientManager: My fd is " + std::to_string(getFileDescriptor()));
}

ClientManager::~ClientManager() {
    setFileDescriptor(0);
}

BlockingQueue<Message> &ClientManager::getBlockingQueue() {
    return blockingQueue;
}

void ClientManager::recv() {
    auto msg = blockingQueue.pop_for(0ms);

    switch (msg.type) {
        case Message::Empty:
            Logger::getInstance().logDebug("ClientManager: No message in queue");
            break;
        case Message::Close:
            Logger::getInstance().logMessage("ClientManager: Get Close message");
            closeAllClients();
            break;

        case Message::AddClient:
            if (!msg.fileDescriptor) {
                Logger::getInstance().logDebug(
                        "ClientManager: AddClient message doesn't contain fileDescriptor");
            } else if (!msg.sock_addr) {
                Logger::getInstance().logDebug(
                        "ClientManager: AddClient message doesn't contain sock_addr");
            } else {
                Logger::getInstance().logMessage("ClientManager: Get AddClient message");
                addClient(*msg.fileDescriptor, *msg.sock_addr);
            }
            break;

        case Message::EraseClient:
            if (!msg.fileDescriptor) {
                Logger::getInstance().logDebug(
                        "ClientManager: EraseClient message doesn't contain fileDescriptor");
            } else {
                Logger::getInstance().logMessage("ClientManager: Get EraseClient message");
                eraseClient(*msg.fileDescriptor);
            }
            break;

        case Message::UpgradeClientWithEncryption:
            if (!msg.fileDescriptor) {
                Logger::getInstance().logDebug(
                        "ClientManager: UpgradeClientWithEncryption message doesn't contain fileDescriptor");
            } else {
                Logger::getInstance().logMessage("ClientManager: Get UpgradeClientWithEncryption message");
                upgradeClientWithEncryption(*msg.fileDescriptor);
            }
            break;

        case Message::UpgradeClientWithProtocol:
            if (!msg.fileDescriptor) {
                Logger::getInstance().logDebug(
                        "ClientManager: UpgradeClientWithProtocol message doesn't contain fileDescriptor");
            } else {
                Logger::getInstance().logMessage("ClientManager: Get UpgradeClientWithProtocol message");
                upgradeClientWithProtocol(*msg.fileDescriptor);
            }
            break;

        default:
            Logger::getInstance().logDebug("ClientManager: Get unexpected message: " + msg.toString());
    }
}

void ClientManager::addClient(const int &fileDescriptor, const sockaddr &sock_addr) {
    //Create ClientBuffer
    std::unique_ptr<ClientBuffer> clientBuffer = std::make_unique<ClientBuffer>(fileDescriptor);


    {//Add buffer to receiver
        Message msg = Message(Message::AddEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(clientBuffer.get());;
        receiver.getBlockingQueue().push(std::move(msg));
    }

    {//Add buffer to sender
        Message msg = Message(Message::AddEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(clientBuffer.get());
        sender.getBlockingQueue().push(std::move(msg));
    }

    //Create SimpleClient
    clientsMap.emplace(fileDescriptor,
                       std::make_unique<SimpleClient>(sock_addr, std::move(clientBuffer), &getBlockingQueue()));

    {//Add SimpleClient to server
        Message msg = Message(Message::AddEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(&*clientsMap.at(fileDescriptor));
        server.getBlockingQueue().push(std::move(msg));
    }
}

void ClientManager::eraseClient(const int &fileDescriptor) {
    auto it = clientsMap.find(fileDescriptor);
    if (it == clientsMap.end()) {
        Logger::getInstance().logDebug("ClientManager: fileDescriptor not register");
        return;
    }

    Logger::getInstance().logMessage("ClientManager: Erase client with fd: " + std::to_string(fileDescriptor));
    clientsMap.erase(fileDescriptor);
}

void ClientManager::closeAllClients() {}

void ClientManager::upgradeClientWithEncryption(int &fileDescriptor) {
    //Create EncryptClient
    auto client = std::make_unique<EncryptClient>(std::move(clientsMap.at(fileDescriptor)));
    clientsMap.erase(fileDescriptor);
    clientsMap.emplace(fileDescriptor, std::move(client));

    {//Change to EncryptClient in server
        Message msg = Message(Message::ChangeEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(&*clientsMap.at(fileDescriptor));
        server.getBlockingQueue().push(std::move(msg));
    }
}

void ClientManager::upgradeClientWithProtocol(int &fileDescriptor) {
    //Create ProtocolClient
    auto client = std::make_unique<ProtocolClient>(std::move(clientsMap.at(fileDescriptor)));
    clientsMap.erase(fileDescriptor);
    clientsMap.emplace(fileDescriptor, std::move(client));

    {//Change to ProtocolClient in server
        Message msg = Message(Message::ChangeEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(&*clientsMap.at(fileDescriptor));
        server.getBlockingQueue().push(std::move(msg));
    }
}