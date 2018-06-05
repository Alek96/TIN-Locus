#include "ClientManager.h"
#include "log/Logger.h"
#include "client/ClientBuffer.h"
#include "client/SimpleClient.h"
#include "client/EncryptClient.h"
#include "client/ProtocolClient.h"

#include <iostream>

using namespace Log;
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

        default:
            Logger::getInstance().logDebug("ClientManager: Get unexpected message: " + msg.toString());
    }
}

void ClientManager::addClient(const int &fileDescriptor, const sockaddr &sock_addr) {
    //Create ClientBuffer
    std::unique_ptr<ClientBuffer> clientBuffer = std::make_unique<ClientBuffer>(fileDescriptor);
    auto clientBufferPtr = clientBuffer.get();

    //Create SimpleClient
    auto simpleClient = std::make_unique<SimpleClient>(sock_addr, std::move(clientBuffer), &getBlockingQueue());

    //Create EncryptClient
    auto encryptClient = std::make_unique<EncryptClient>(std::move(simpleClient));
    
    //Create ProtocolClient
            auto cryptoModule = &encryptClient->getCryptoModule();
    auto protocolClient = std::make_unique<ProtocolClient>(std::move(encryptClient), *cryptoModule);
    
    //Add client to map
    clientsMap.emplace(fileDescriptor, std::move(protocolClient));
    
    {//Add ProtocolClient to server
        Message msg = Message(Message::AddEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(&*clientsMap.at(fileDescriptor));
        server.getBlockingQueue().push(std::move(msg));
    }
    

    {//Add buffer to receiver
        Message msg = Message(Message::AddEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(clientBufferPtr);;
        receiver.getBlockingQueue().push(std::move(msg));
    }

    {//Add buffer to sender
        Message msg = Message(Message::AddEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(clientBufferPtr);
        sender.getBlockingQueue().push(std::move(msg));
    }
}

void ClientManager::eraseClient(const int &fileDescriptor) {
    auto it = clientsMap.find(fileDescriptor);
    if (it == clientsMap.end()) {
        Logger::getInstance().logDebug("ClientManager: fileDescriptor is not registered");
        return;
    }

    Logger::getInstance().logMessage("ClientManager: Erase client with fd: " + std::to_string(fileDescriptor));
    clientsMap.erase(fileDescriptor);
}

void ClientManager::closeAllClients() {}
