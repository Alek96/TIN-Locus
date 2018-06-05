#ifndef LOCUS_BLOCKBUFFER_H
#define LOCUS_BLOCKBUFFER_H

#include <cstddef>
#include <list>
#include <vector>
#include <stdexcept>
#include "Converter.h"

namespace buffer {
    class BlockBuffer {
    public:
        BlockBuffer() = default;

        const size_t size() const;
        const size_t frontSize() const;

        void push(const std::vector<unsigned char> &bytes);
        void push(std::vector<unsigned char> &&bytes);

        template<typename T>
        void push(const T &object);

        std::vector<unsigned char> pop();

        template<typename T>
        T &pop(T &object);

        std::list< std::vector<unsigned char> > &operator*();
        const std::list< std::vector<unsigned char> > &operator*() const;

        bool operator==(const BlockBuffer &rhs) const;
        bool operator!=(const BlockBuffer &rhs) const;

        bool operator<(const BlockBuffer &rhs) const;
        bool operator>(const BlockBuffer &rhs) const;
        bool operator<=(const BlockBuffer &rhs) const;
        bool operator>=(const BlockBuffer &rhs) const;
        
    private:
        std::list< std::vector<unsigned char> > blockBuffer;
    };

    template<typename T>
    void BlockBuffer::push(const T &object) {
        push(to_bytes(object));
    }

    template<typename T>
    T &BlockBuffer::pop(T &object) {
        if(blockBuffer.empty()) {
            throw std::runtime_error("BlockBuffer is Empty");

        }
        if (sizeof(T) != frontSize()) {
            throw std::runtime_error("Object has wrong number of bytes");
        }

        from_bytes(pop(), object);

        return object;
    }
}

#endif //LOCUS_BLOCKBUFFER_H
