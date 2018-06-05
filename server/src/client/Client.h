#ifndef LOCUS_CLIENT_H
#define LOCUS_CLIENT_H

#include <sys/socket.h>
#include <vector>
#include "epoll/EPollEvent.h"
#include "packet/Packet.h"
#include "message/Message.h"
#include "message/BlockingQueue.h"

class Client : public EPollEvent {
public:
    void recv() override = 0;
    void send() override = 0;

    virtual void sendData(const std::vector<unsigned char> &bytes) = 0;
    virtual std::vector<unsigned char> recvData() = 0;

    virtual void sendPacket(const packet::Packet &packet);
    virtual packet::Packet recvPacket();

    virtual const sockaddr &getClient_addr() const = 0;

    virtual const int getConnectionFD() const = 0;

    virtual message::BlockingQueue<message::Message> &getBlockingQueue() = 0;

    virtual message::BlockingQueue<message::Message> &getClientManagerBlockingQueue() = 0;

    virtual void closeConnection() = 0;
};


#endif //LOCUS_CLIENT_H
