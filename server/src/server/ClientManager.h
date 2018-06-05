#ifndef LOCUS_CLIENTMANAGER_H
#define LOCUS_CLIENTMANAGER_H

#include <unordered_map>
#include "epoll/EPollEvent.h"
#include "message/Message.h"
#include "message/BlockingQueue.h"
#include "server/Server.h"
#include "server/Receiver.h"
#include "server/Sender.h"
#include "client/Client.h"

class ClientManager : public EPollEvent {
public:
    explicit ClientManager(Server &server, Receiver &receiver, Sender &sender);

    ~ClientManager() override;

    message::BlockingQueue<message::Message> &getBlockingQueue();

    void recv() override;

    void send() override {}

private:
    message::BlockingQueue<message::Message> blockingQueue;
    Server &server;
    Receiver &receiver;
    Sender &sender;
    std::unordered_map<int, std::unique_ptr<Client>> clientsMap;

    void addClient(const int &fileDescriptor, const sockaddr &sock_addr);

    void eraseClient(const int &fileDescriptor);

    void closeAllClients();
};


#endif //LOCUS_CLIENTMANAGER_H
