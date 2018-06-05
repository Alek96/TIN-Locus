#include "BlockingBuffer.h"
#include <stdexcept>

//namespace {

std::vector<unsigned char> BlockingBuffer::pop(const size_t &n) {
    std::lock_guard<std::mutex> guard(accessMutex);
    return Buffer::pop(n);
}

std::vector<unsigned char> BlockingBuffer::pop() {
    std::lock_guard<std::mutex> guard(accessMutex);
    return Buffer::pop();
}

std::vector<unsigned char> BlockingBuffer::popAll() {
//    std::lock_guard<std::mutex> guard(accessMutex);
    return Buffer::popAll();
}

//}