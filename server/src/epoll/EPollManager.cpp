#include "EPollManager.h"

#include "log/Logger.h"
#include <string>
#include <chrono>
#include <unistd.h>
#include <cstring>
#include <cerrno>

using namespace Log;
using namespace message;
using namespace std::chrono_literals;

//namespace {

EPollManager::EPollManager(const std::string &name)
        : name(name), events(static_cast<unsigned long>(maxSizeEvents)) {
    Logger::getInstance().logDebug(name + ": Initialize");
    ePollFD = epoll_create1(0);
    if (ePollFD < 0) {
        throw std::runtime_error(name + ": Error while creating epoll: " + std::string(std::strerror(errno)));
    }
}

EPollManager::~EPollManager() {
    Logger::getInstance().logMessage(name + ": Closing");
    close(ePollFD);
}

void EPollManager::addEPollEvent(EPollEvent *newEvent) {
    event.data.ptr = newEvent;
    event.events = flags; //EPOLLIN EPOLLOUT

    if (epoll_ctl(ePollFD, EPOLL_CTL_ADD, newEvent->getFileDescriptor(), &event) < 0) {
        throw std::runtime_error(name + ": Error while adding to epoll_ctl" + std::string(std::strerror(errno)));
    }

    Logger::getInstance().logMessage(
            name + ": The EPollEvent is added with fd: " + std::to_string(newEvent->getFileDescriptor()));
}

void EPollManager::changeEPollEvent(EPollEvent *newEvent) {
    event.data.ptr = newEvent;
    event.events = flags; //EPOLLIN EPOLLOUT

    if (epoll_ctl(ePollFD, EPOLL_CTL_MOD, newEvent->getFileDescriptor(), &event) < 0) {
        throw std::runtime_error(name + ": Error while changing in epoll_ctl" + std::string(std::strerror(errno)));
    }

    Logger::getInstance().logMessage(
            name + ": The EPollEvent is change with fd: " + std::to_string(newEvent->getFileDescriptor()));
}

void EPollManager::eraseFileDescriptor(const int &socketID) {
    epoll_ctl(ePollFD, EPOLL_CTL_DEL, socketID, nullptr);

    Logger::getInstance().logMessage(name + ": The file descriptor is erased: " + std::to_string(socketID));
}

void EPollManager::readEPoll() {
    int nfds = epoll_wait(ePollFD, events.data(), maxSizeEvents, 0);

    if (nfds < 0) {
        Logger::getInstance().logError(name + ": epoll_wait error: " + std::string(std::strerror(errno)));
    } else if (nfds > 0) {
        Logger::getInstance().logDebug(name + ": epoll_wait return: " + std::to_string(nfds));
    }

    for (int i = 0; i < nfds; ++i) {
        try {

            if (events[i].events & EPOLLIN) {
                Logger::getInstance().logDebug(name + ": Run recv for " + std::to_string(
                        static_cast<EPollEvent *>(events[i].data.ptr)->getFileDescriptor()));
                static_cast<EPollEvent *>(events[i].data.ptr)->recv();
            } else if (events[i].events & EPOLLOUT) {
                Logger::getInstance().logDebug(name + ": Run send for " + std::to_string(
                        static_cast<EPollEvent *>(events[i].data.ptr)->getFileDescriptor()));
                static_cast<EPollEvent *>(events[i].data.ptr)->send();
            } else {
                Logger::getInstance().logError(
                        name + ": Error on file descriptor " + std::to_string(events[i].events)
                        + " with error: " + std::string(std::strerror(errno)));
            }
        } catch (std::exception &e) {
            Logger::getInstance().logError(name + ": Error in readEPoll: " + e.what());
        }
    }
}

uint32_t EPollManager::getFlags() const {
    return flags;
}

void EPollManager::setFlags(const uint32_t &flags) {
    EPollManager::flags = flags;
}

const std::string &EPollManager::getName() const {
    return name;
}
//}