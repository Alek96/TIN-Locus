#ifndef LOCUS_POSITION_H
#define LOCUS_POSITION_H

#include <cereal/cereal.hpp>
#include <ctime>

struct Position {
    Position() = default;
    Position(float latitude, float longitude, time_t time);

    float latitude = 0;
    float longitude = 0;
    time_t time = 0;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(latitude), CEREAL_NVP(longitude), CEREAL_NVP(time));
    }

    bool operator==(const Position &rhs) const;
    bool operator!=(const Position &rhs) const;

    bool operator<(const Position &rhs) const;
    bool operator>(const Position &rhs) const;
    bool operator<=(const Position &rhs) const;
    bool operator>=(const Position &rhs) const;
};


#endif //LOCUS_POSITION_H
