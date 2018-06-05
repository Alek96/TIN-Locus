#include "BasicThread.h"
#include "log/Logger.h"

using namespace Log;
using namespace message;
using namespace std::chrono_literals;

BasicThread::BasicThread(const std::string& name) : name(name), blockingQueue(1000), ePollManager(name) {}

message::BlockingQueue<message::Message> &BasicThread::getBlockingQueue() {
    return blockingQueue;
}

void BasicThread::readMessage() {
    while (!exit) {
        auto msg = blockingQueue.pop_for(0ms);

        switch (msg.type) {
            case Message::Empty:
//            Logger::getInstance().logDebug("EPollManager: No message in queue");
                return;
            case Message::Close:
                Logger::getInstance().logMessage(name + ": Get Close message");
                exit = true;
                return;

            case Message::AddEPollEvent:
                if (!msg.ePollEvent) {
                    Logger::getInstance().logDebug(name + ": AddEPollEvent message doesn't contain ePollEvent");
                } else {
                    Logger::getInstance().logMessage(name + ": Get AddEPollEvent message");
                    ePollManager.addEPollEvent(*msg.ePollEvent);
                }
                break;

            case Message::ChangeEPollEvent:
                if (!msg.ePollEvent) {
                    Logger::getInstance().logDebug(name + ": ChangeEPollEvent message doesn't contain ePollEvent");
                } else {
                    Logger::getInstance().logMessage(name + ": Get ChangeEPollEvent message");
                    ePollManager.changeEPollEvent(*msg.ePollEvent);
                }
                break;

            case Message::EraseFileDescriptor:
                if (!msg.fileDescriptor) {
                    Logger::getInstance().logDebug(
                            name + ": EraseFileDescriptor message doesn't contain file descriptor");
                } else {
                    Logger::getInstance().logMessage(name + ": Get EraseFileDescriptor message");
                    ePollManager.eraseFileDescriptor(*msg.fileDescriptor);
                }
                break;

            default:
                Logger::getInstance().logDebug(name + ": Get unexpected message: " + msg.toString());
        }
    }
}

void BasicThread::operator()() {
    Logger::getInstance().logMessage(name + ": Starting");
    while (!exit) {
        readMessage();
        ePollManager.readEPoll();
    }
}
