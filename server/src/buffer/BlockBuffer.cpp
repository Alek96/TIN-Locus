#include "BlockBuffer.h"


namespace buffer {

    const size_t BlockBuffer::size() const {
        return blockBuffer.size();
    }

    const size_t BlockBuffer::frontSize() const {
        return blockBuffer.front().size();
    }

    void BlockBuffer::push(const std::vector<unsigned char> &bytes) {
        blockBuffer.push_back(bytes);
    }

    void BlockBuffer::push(std::vector<unsigned char> &&bytes) {
        blockBuffer.push_back(std::move(bytes));
    }

    std::vector<unsigned char> BlockBuffer::pop() {
        if(blockBuffer.empty()) {
            throw std::runtime_error("BlockBuffer is Empty");

        }
        auto result = blockBuffer.front();
        blockBuffer.pop_front();
        return result;
    }

    std::list< std::vector<unsigned char> > &BlockBuffer::operator*() {
        return blockBuffer;
    }

    const std::list<std::vector<unsigned char> > &BlockBuffer::operator*() const {
        return blockBuffer;
    }

    bool BlockBuffer::operator==(const BlockBuffer &rhs) const {
        return blockBuffer == rhs.blockBuffer;
    }

    bool BlockBuffer::operator!=(const BlockBuffer &rhs) const {
        return !(rhs == *this);
    }

    bool BlockBuffer::operator<(const BlockBuffer &rhs) const {
        return blockBuffer < rhs.blockBuffer;
    }

    bool BlockBuffer::operator>(const BlockBuffer &rhs) const {
        return rhs < *this;
    }

    bool BlockBuffer::operator<=(const BlockBuffer &rhs) const {
        return !(rhs < *this);
    }

    bool BlockBuffer::operator>=(const BlockBuffer &rhs) const {
        return !(*this < rhs);
    }
}