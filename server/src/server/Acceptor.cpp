#include "Acceptor.h"

#include <string>
#include <stdexcept>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include "log/Logger.h"

using namespace Log;
using namespace message;

//namespace  {
Acceptor::Acceptor(const uint16_t &port, message::BlockingQueue<message::Message> *blockingQueue)
        : blockingQueue(blockingQueue) {
    //tworzenie gniazda
    int socketfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
//        int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        throw std::runtime_error("Acceptor: Error " + std::string(std::strerror(static_cast<int>(socketfd))) +
                                 " when opening socket: " + std::to_string(socketfd));
    }
    setFileDescriptor(socketfd);

    //ustawienie adresu gniazda
    bzero((char *) &server_addr, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int setOpt = 1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &setOpt, sizeof(int))) {
        throw std::runtime_error("Acceptor: Error " + std::string(std::strerror(static_cast<int>(socketfd))) +
                                 " when setting the SO_REUSEADDR socket option: " + std::to_string(socketfd));
    }

    //przypisanie adresu serwera do gniazda
    if (bind(socketfd, (struct sockaddr *) &server_addr, sizeof server_addr) < 0) {
        throw std::runtime_error("Acceptor: Error " + std::string(std::strerror(static_cast<int>(socketfd))) +
                                 " when bind socket: " + std::to_string(socketfd));
    }

    //nasluchiwanie na gniezdzie
    listen(socketfd, 5);

    Logger::getInstance().logMessage("Acceptor: My fd is " + std::to_string(socketfd));
}

bool Acceptor::setNonBlock(int &sfd) {
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags < 0) {
        Logger::getInstance().logError("Acceptor: Error " + std::string(std::strerror(static_cast<int>(flags))) +
                                       " when setting non blocking fd: " + std::to_string(sfd));
        return false;
    }

    if (fcntl(sfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        Logger::getInstance().logError("Acceptor: Error " + std::string(std::strerror(static_cast<int>(flags))) +
                                       " when setting non blocking fd: " + std::to_string(sfd));
        return false;
    }
    return true;
}

void Acceptor::recv() {
    struct sockaddr in_addr{};
    socklen_t in_len = sizeof in_addr;
    int in_fd = accept(getFileDescriptor(), &in_addr, &in_len);

    if (in_fd < 0) {
        Logger::getInstance().logDebug("Acceptor: Non connection");
        return;
    }
    if(!setNonBlock(in_fd)) {
        close(in_fd);
        return;;
    }
    Logger::getInstance().logMessage("Acceptor: New connection: " + std::to_string(in_fd));

    Message msg(Message::AddClient);
    msg.fileDescriptor = std::make_unique<int>(in_fd);
    msg.sock_addr = std::make_unique<sockaddr>(in_addr);
    blockingQueue->push(std::move(msg));
}
//}