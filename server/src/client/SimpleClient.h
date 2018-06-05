#ifndef LOCUS_SIMPLECLIENT_H
#define LOCUS_SIMPLECLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include "Client.h"
#include "message/Message.h"
#include "message/BlockingQueue.h"
#include "ClientBuffer.h"

class SimpleClient : public Client {
public:
    SimpleClient(const sockaddr &client_addr, std::unique_ptr<ClientBuffer> clientBuffer,
           message::BlockingQueue<message::Message> *clientManagerBlockingQueue);

    SimpleClient(SimpleClient&&) noexcept = default;

    ~SimpleClient() override;

    void recv() override;
    void send() override {}

    void sendData(const std::vector<unsigned char> &bytes) override;
    std::vector<unsigned char> recvData() override;

    const sockaddr &getClient_addr() const override;

    const int getConnectionFD() const override;

    message::BlockingQueue<message::Message> &getBlockingQueue() override;

    message::BlockingQueue<message::Message> &getClientManagerBlockingQueue() override;

    void closeConnection() override;

private:
    struct sockaddr client_addr;
    std::unique_ptr<ClientBuffer> clientBuffer;
    message::BlockingQueue<message::Message> blockingQueue;
    message::BlockingQueue<message::Message> *clientManagerBlockingQueue;
};


#endif //LOCUS_SIMPLECLIENT_H
