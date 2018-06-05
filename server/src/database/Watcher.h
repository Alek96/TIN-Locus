#ifndef LOCUS_WATCHER_H
#define LOCUS_WATCHER_H

#include <cereal/types/string.hpp>
#include <ctime>

struct Watcher {
    Watcher() = default;
    explicit Watcher(short id);

    short id = 0;
    time_t time = 0;
    std::string name;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(id), CEREAL_NVP(time), CEREAL_NVP(name));
    }

    bool operator==(const Watcher &rhs) const;
    bool operator!=(const Watcher &rhs) const;

    bool operator<(const Watcher &rhs) const;
    bool operator>(const Watcher &rhs) const;
    bool operator<=(const Watcher &rhs) const;
    bool operator>=(const Watcher &rhs) const;
};


#endif //LOCUS_WATCHER_H
