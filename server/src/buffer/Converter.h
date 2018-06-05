#ifndef LOCUS_CONVERTER_H
#define LOCUS_CONVERTER_H

#include <vector>
#include <memory>
#include <type_traits>
#include <iomanip>
#include <cstddef>

namespace buffer {

//    using byte = unsigned char;
    using byte = unsigned char;

    template<typename T>
    std::vector<byte> to_bytes(const T &object) {
        std::vector<byte> bytes(sizeof(T));

        const byte *begin = reinterpret_cast< const byte * >( std::addressof(object));
        const byte *end = begin + sizeof(T);
        std::copy(begin, end, std::begin(bytes));

        return bytes;
    }

    template<typename T>
    T &from_bytes(const std::vector<byte> &bytes, T &object) {
        static_assert(std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

        byte *begin_object = reinterpret_cast< byte * >( std::addressof(object));
        std::copy(std::begin(bytes), std::end(bytes), begin_object);

        return object;
    }

    inline std::vector<byte> toByteVector(const std::string &string) {
        return std::vector<byte>(string.begin(), string.end());
    }

    inline std::string toString(const std::vector<byte> &vector) {
        return std::string(vector.begin(), vector.end());
    }
}

#endif //LOCUS_CONVERTER_H
