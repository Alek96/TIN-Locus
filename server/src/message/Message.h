#ifndef LOCUS_MESSAGE_H
#define LOCUS_MESSAGE_H

#include <memory>
#include <string>
#include <sys/socket.h>
#include "ErrorType.h"
#include "BlockingQueue.h"
#include "packet/Packet.h"
#include "buffer/BlockingBuffer.h"
#include "epoll/EPollEvent.h"

namespace message {
    struct Message {
        enum Type : unsigned int {
            Empty,
            Error,
            Close,
            AddClient,
            EraseClient,
            AddEPollEvent,
            ChangeEPollEvent,
            EraseFileDescriptor,
            PacketSend,
            PacketReceive,
            Update,
            EraseObserver,
        };

        Message() = default;
        explicit Message(const Type &type);

        virtual ~Message() = default;

        Message(const Message&) = delete;
        Message(Message&&) = default;
        Message &operator=(const Message&) = delete;
        Message &operator=(Message&&) = default;

        bool operator==(const Message &rhs) const;
        bool operator!=(const Message &rhs) const;

        bool operator<(const Message &rhs) const;
        bool operator>(const Message &rhs) const;
        bool operator<=(const Message &rhs) const;
        bool operator>=(const Message &rhs) const;

        const std::string toString() const;

        Type type = Empty;

        std::unique_ptr<ErrorType> errorType;
        std::unique_ptr<int> fileDescriptor;
        std::unique_ptr<sockaddr> sock_addr;
        std::unique_ptr<EPollEvent *> ePollEvent;
        std::unique_ptr<BlockingQueue<Message> *> blockingQueue;
        std::unique_ptr<short> id;
    };
}


#endif //LOCUS_MESSAGE_H
