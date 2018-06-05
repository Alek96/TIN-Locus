#include "Buffer.h"
#include <stdexcept>


void Buffer::setMaxSize(const size_t &bufferMaxSize) {
    Buffer::bufferMaxSize = bufferMaxSize;
}

const size_t Buffer::size() const {
    return bufferSize;
}

const size_t Buffer::remainingSize() const {
    return bufferMaxSize - bufferSize;
}

const bool Buffer::empty() const {
    return !static_cast<bool>(bufferSize);
}

const bool Buffer::full() const {
    return bufferSize >= bufferMaxSize;
}

Buffer::Stage Buffer::getStage() const {
    return stage;
}

void Buffer::setStage(const Buffer::Stage &stage) {
    Buffer::stage = stage;
}

std::vector<unsigned char> Buffer::pop() {
    if (!size()) {
        throw std::runtime_error("The BlockingBuffer is Empty");
    }

    std::vector<unsigned char> bytes = std::move(buffer.front());
    buffer.pop_front();
    bufferSize -= bytes.size();
    return std::move(bytes);
}

std::vector<unsigned char> Buffer::pop(const size_t &n) {
    if (n > size()) {
        throw std::runtime_error("The BlockingBuffer has too small number of bytes");
    }

    std::vector<unsigned char> returnBuffer(n);
    auto itRetBuffer = returnBuffer.begin();
    size_t left = n;


    while (left) {
        auto itBuffer = buffer.front().begin();
        if (frontSize() <= left) {
            for (int i = 0; i < frontSize(); ++i) {
                *itRetBuffer++ = *itBuffer++;
            }
            left -= frontSize();
            buffer.pop_front();
        } else {
            for (int i = 0; i < left; ++i) {
                *itRetBuffer++ = *itBuffer++;
            }
            left = 0;
            buffer.front().erase(buffer.front().begin(), itBuffer);
        }
    }

    bufferSize -= n;
    return returnBuffer;
}

const size_t Buffer::frontSize() const {
    return buffer.front().size();
}

std::vector<unsigned char> Buffer::popAll() {
    return pop(size());
}
