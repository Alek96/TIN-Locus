#ifndef LOCUS_BLOCKINGBUFFER_H
#define LOCUS_BLOCKINGBUFFER_H

#include <mutex>
#include "Buffer.h"

//namespace {
class BlockingBuffer : public Buffer {
public:
    explicit BlockingBuffer() = default;

    template<typename T>
    void push_front(T &&bytes);

    template<typename T>
    void push_back(T &&bytes);

    std::vector<unsigned char> pop() override;

    std::vector<unsigned char> pop(const size_t &n) override;

    std::vector<unsigned char> popAll() override;

private:
    std::mutex accessMutex;
};

template<typename T>
void BlockingBuffer::push_front(T &&bytes) {
    static_assert(std::is_same<std::vector<unsigned char>, typename std::remove_reference<T>::type>::value,
                  "Parameter in push_front must be std::vector<unsigned char> type");

    std::lock_guard<std::mutex> guard(accessMutex);
    Buffer::push_front(bytes);
}

template<typename T>
void BlockingBuffer::push_back(T &&bytes) {
//    static_assert(std::is_same<std::vector<unsigned char>, typename std::remove_const<std::remove_reference<T>>::type>::value,
    static_assert(std::is_same<std::vector<unsigned char>, typename std::remove_reference<T>::type>::value,
                  "Parameter in push_back must be std::vector<unsigned char> type");

    std::lock_guard<std::mutex> guard(accessMutex);
    Buffer::push_back(bytes);
}
//}


#endif //LOCUS_BLOCKINGBUFFER_H
