#ifndef LOCUS_BUFFER_H
#define LOCUS_BUFFER_H

#include <cstddef>
#include <vector>
#include <list>
#include <utility>

class Buffer {
public:
    enum class Stage {
        Empty,
        Full,
        Receiving,
        Sending
    };

    explicit Buffer() = default;

    void setMaxSize(const size_t &bufferMaxSize);

    const size_t size() const;

    const size_t remainingSize() const;

    const bool empty() const;

    const bool full() const;

    Stage getStage() const;

    void setStage(const Stage &stage);

    template<typename T>
    void push_front(T &&bytes);

    template<typename T>
    void push_back(T &&bytes);

    virtual std::vector<unsigned char> pop();

    virtual std::vector<unsigned char> pop(const size_t &n);
    
    virtual std::vector<unsigned char> popAll();

private:
    std::list<std::vector<unsigned char> > buffer;
    size_t bufferMaxSize = 0;
    size_t bufferSize = 0;
    Stage stage = Stage::Empty;

    const size_t frontSize() const;
};

template<typename T>
void Buffer::push_front(T &&bytes) {
    static_assert(std::is_same<std::vector<unsigned char>, typename std::remove_reference<T>::type>::value,
                  "Parameter in push_front must be std::vector<unsigned char> type");

    bufferSize += bytes.size();
    buffer.push_front(std::forward<std::vector<unsigned char> >(bytes));
}

template<typename T>
void Buffer::push_back(T &&bytes) {
    static_assert(std::is_same<std::vector<unsigned char>, typename std::remove_reference<T>::type>::value,
                  "Parameter in push_back must be std::vector<unsigned char> type");

    bufferSize += bytes.size();
    buffer.push_back(std::forward<T>(bytes));
}

#endif //LOCUS_BUFFER_H
