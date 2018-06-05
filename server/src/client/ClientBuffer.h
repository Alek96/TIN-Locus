#ifndef LOCUS_CLIENTBUFFER_H
#define LOCUS_CLIENTBUFFER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "epoll/EPollEvent.h"
#include "buffer/BlockingBuffer.h"
#include "message/Message.h"
#include "message/BlockingQueue.h"

class ClientBuffer : public EPollEvent {
public:
    explicit ClientBuffer(const int &port);
    ClientBuffer(const ClientBuffer&) = delete;

    ~ClientBuffer() override = default;

    const bool isClose() const;

    BlockingBuffer &getBufferIn();

    BlockingBuffer &getBufferOut();

    void setClientBlockingQueue(message::BlockingQueue<message::Message> *clientBlockingQueue);

    void recv() override;

    void send() override;

private:
    mutable bool close = false;
    BlockingBuffer bufferIn;
    BlockingBuffer bufferOut;
    message::BlockingQueue<message::Message> *clientBlockingQueue = nullptr;

    std::vector<unsigned char> recv(const size_t &n) const;
    std::vector<unsigned char>  send(std::vector<unsigned char> buffer) const;
};


#endif //LOCUS_CLIENTBUFFER_H
