#include "EPollEvent.h"

#include <unistd.h>
#include "log/Logger.h"

//namespace {

EPollEvent::EPollEvent(EPollEvent &&ePollEvent) noexcept {
    fileDescriptor = ePollEvent.fileDescriptor;
    ePollEvent.fileDescriptor = 0;
}

int EPollEvent::getFileDescriptor() const {
    return fileDescriptor;
}

void EPollEvent::setFileDescriptor(const int &fileDescriptor) {
    EPollEvent::fileDescriptor = fileDescriptor;
}

EPollEvent::~EPollEvent() {
    if (getFileDescriptor() > 0) {
        close(getFileDescriptor());
        log::Logger::getInstance().logMessage(
                "EPollEvent: Close file descriptor: " + std::to_string(getFileDescriptor()));
    }
}

//}