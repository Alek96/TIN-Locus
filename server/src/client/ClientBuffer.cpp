#include <cstring>
#include <cerrno>
#include <unistd.h>

#include "ClientBuffer.h"
#include "log/Logger.h"
#include "buffer/Converter.h"

using namespace Log;
using namespace packet;
using namespace buffer;
using namespace message;

ClientBuffer::ClientBuffer(const int &port) {
    setFileDescriptor(port);
}

const bool ClientBuffer::isClose() const {
    return close;
}

BlockingBuffer &ClientBuffer::getBufferIn() {
    return bufferIn;
}

BlockingBuffer &ClientBuffer::getBufferOut() {
    return bufferOut;
}

std::vector<unsigned char> ClientBuffer::recv(const size_t &n) const {
    Logger::getInstance().logDebug("ClientBuffer: try to receive " + std::to_string(n) + " bytes.");
    std::vector<unsigned char> buffer(n);
    size_t remaining = n;

    while (!isClose() && remaining) {
        buffer.resize(remaining);
        ssize_t count = ::recv(getFileDescriptor(), buffer.data() + n - remaining, sizeof(unsigned char) * remaining, 0);

        if (count < 0) {
            // if errno is EAGAIN or EWOULDBLOCK, that means we have receive all data.
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                close = true;
                Logger::getInstance().logError("ClientBuffer: Error while recv: " + std::string(std::strerror(errno)));
            }
            break;
        } else if (count == 0) {
            // End of file. The remote has close the connection.
            close = true;
            Logger::getInstance().logMessage("ClientBuffer: " + std::to_string(getFileDescriptor()) + 
                                             " has close the connection");
            break;
        }

        // receive count bytes
        remaining -= count;
    }

    if(!isClose()) {
        Logger::getInstance().logDebug("ClientBuffer: receive " + std::to_string(n - remaining) + " bytes.");
    }

    buffer.resize(n - remaining);
    return isClose() ? std::vector<unsigned char>() : buffer;
}

void ClientBuffer::recv() {

    //get size of upcoming packet
    if (!isClose() && bufferIn.getStage() == BlockingBuffer::Stage::Empty) {
        auto buffer = recv(2);
        if (!buffer.empty()) {
            uint16_t size = 0;
            from_bytes(buffer, size);
            Logger::getInstance().logDebug("ClientBuffer: new packet will have " + std::to_string(size) + " bytes.");
            bufferIn.setMaxSize(size);
            bufferIn.setStage(BlockingBuffer::Stage::Receiving);
        }
    }

    //Receiving data
    if (!isClose() && bufferIn.getStage() == BlockingBuffer::Stage::Receiving) {
        auto buffer = recv(bufferIn.remainingSize());
        if (!buffer.empty()) {
            bufferIn.push_back(std::move(buffer));
            if (bufferIn.remainingSize() == 0) {
                Logger::getInstance().logDebug("ClientBuffer: new packet have " + std::to_string(bufferIn.size()) + " bytes.");
                bufferIn.setStage(BlockingBuffer::Stage::Full);
                //notify client
                if(clientBlockingQueue) {
                    clientBlockingQueue->priorityPush(Message(Message::PacketReceive));
                }
            }
        }
    }

    //notify client if connection is closed
    if(isClose() && clientBlockingQueue) {
        clientBlockingQueue->priorityPush(Message(Message::Close));
    }
}

std::vector<unsigned char> ClientBuffer::send(std::vector<unsigned char> buffer) const {
    Logger::getInstance().logDebug("ClientBuffer: try to send " + std::to_string(buffer.size()) + " bytes.");

    const auto firstSize = buffer.size();
    if (!isClose() && !buffer.empty()) {
        ssize_t count = ::send(getFileDescriptor(), buffer.data(), sizeof(unsigned char) * buffer.size(), 0);
        if (count < 0) {
            // if errno is EAGAIN or EWOULDBLOCK, that means we have send all data.
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                close = true;
                Logger::getInstance().logError("ClientBuffer: Error while send:" + std::string(std::strerror(errno)));
            }
        } else if (count == 0) {
            Logger::getInstance().logDebug("ClientBuffer: send return 0, why?");
        } else {
            // receive count bytes
            buffer.erase(buffer.begin(), buffer.begin() + count);
        }
    }
    
    if(!isClose()) {
        Logger::getInstance().logDebug("ClientBuffer: send " + std::to_string(firstSize - buffer.size()) + " bytes.");
    }
    
    return buffer;
}

void ClientBuffer::send() {
    //set size of sending packet
    if (!isClose() && bufferOut.getStage() == BlockingBuffer::Stage::Full) {
        const auto size = static_cast<uint16_t>(bufferOut.size());
        bufferOut.push_front(to_bytes(size));
        bufferOut.setMaxSize(bufferOut.size());
        bufferOut.setStage(BlockingBuffer::Stage::Sending);
    }

    //Sending data
    while (!isClose() && bufferOut.getStage() == BlockingBuffer::Stage::Sending) {
        auto buffer = send(bufferOut.pop());
        if (!buffer.empty()) {
//            Logger::getInstance().logDebug("ClientBuffer: remained " + std::to_string(buffer.size()) + " bytes.");
            bufferOut.push_front(std::move(buffer));
            break;
        } else if (bufferOut.empty()) {
//            Logger::getInstance().logDebug("ClientBuffer: bufferOut is empty");
            bufferOut.setStage(BlockingBuffer::Stage::Empty);
            //notify client
            if(clientBlockingQueue) {
                clientBlockingQueue->priorityPush(Message(Message::PacketSend));
            }
            break;
        }
    }
}

void ClientBuffer::setClientBlockingQueue(BlockingQueue<Message> *clientBlockingQueue) {
    ClientBuffer::clientBlockingQueue = clientBlockingQueue;
}
