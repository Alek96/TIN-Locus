#include "Message.h"
#include <unistd.h>
#include "log/Logger.h"

using namespace packet;

namespace message {
    Message::Message(const Message::Type &type) : type(type) {}

    bool Message::operator==(const Message &rhs) const {
        if(errorType && rhs.errorType) {
            return type == rhs.type &&
                   *errorType == *rhs.errorType;
        } else {
            return type == rhs.type;
        }
    }

    bool Message::operator!=(const Message &rhs) const {
        return !(rhs == *this);
    }

    bool Message::operator<(const Message &rhs) const {
        if (type < rhs.type)
            return true;

        if(errorType && rhs.errorType) {
            if (rhs.type < type)
                return false;
            return *errorType < *rhs.errorType;
        } else {
            return false;
        }
    }

    bool Message::operator>(const Message &rhs) const {
        return rhs < *this;
    }

    bool Message::operator<=(const Message &rhs) const {
        return !(rhs < *this);
    }

    bool Message::operator>=(const Message &rhs) const {
        return !(*this < rhs);
    }

    const std::string Message::toString() const {
        switch (type) {
            case Empty:
                return std::string("Empty");
            case Error:
                return std::string("Error");
            case Close:
                return std::string("Close");
            case AddClient:
                return std::string("AddClient");
            case EraseClient:
                return std::string("EraseClient");
            case AddEPollEvent:
                return std::string("AddEPollEvent");
            case ChangeEPollEvent:
                return std::string("ChangeEPollEvent");
            case EraseFileDescriptor:
                return std::string("EraseFileDescriptor");
            case PacketSend:
                return std::string("PacketSend");
            case PacketReceive:
                return std::string("PacketReceive");
        }
    }
}