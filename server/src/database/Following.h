#ifndef LOCUS_FOLLOWING_H
#define LOCUS_FOLLOWING_H

#include <cereal/types/string.hpp>
#include <ctime>

struct Following {
    Following() = default;
    explicit Following(short id);

    short id = 0;
    time_t time = 0;
    std::string name;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(id), CEREAL_NVP(time), CEREAL_NVP(name));
    }

    bool operator==(const Following &rhs) const;
    bool operator!=(const Following &rhs) const;

    bool operator<(const Following &rhs) const;
    bool operator>(const Following &rhs) const;
    bool operator<=(const Following &rhs) const;
    bool operator>=(const Following &rhs) const;
};


#endif //LOCUS_FOLLOWING_H
