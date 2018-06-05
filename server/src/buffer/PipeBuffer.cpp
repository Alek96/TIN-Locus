#include "PipeBuffer.h"
#include <fcntl.h>  // O_NONBLOCK flag for pipe
#include <cerrno>
#include <system_error>

PipeBuffer::PipeBuffer() {
    // open pipe, NOTE: pipe2 is Linux-specific
    if (pipe2(pipeFds, O_NONBLOCK | O_CLOEXEC) != 0) {
        throw std::system_error(errno, std::generic_category());
    }

    // close pipe fds in destructor? O_CLOEXEC flag?
}

void PipeBuffer::pushByteToPipe() const {
    // unsigned char ?
    if (write(pipeWriteFd, "1", sizeof "1"[0]) == -1) {
        throw std::system_error(errno, std::generic_category());
    }

    // something more?
}

void PipeBuffer::popByteFromPipe() const {
    unsigned char byte;
    if (read(pipeReadFd, (void*)&byte, sizeof byte) == -1) {
        throw std::system_error(errno, std::generic_category());
    }

    // something more?
}

int PipeBuffer::getReadFd() const {
    return pipeReadFd;
}

int PipeBuffer::getWriteFd() const {
    return pipeWriteFd;
}
