#include "Position.h"

Position::Position(float latitude, float longitude, time_t time)
        : latitude(latitude), longitude(longitude), time(time) {}

bool Position::operator==(const Position &rhs) const {
    return latitude == rhs.latitude &&
           longitude == rhs.longitude &&
           time == rhs.time;
}

bool Position::operator!=(const Position &rhs) const {
    return !(rhs == *this);
}

bool Position::operator<(const Position &rhs) const {
    if (latitude < rhs.latitude)
        return true;
    if (rhs.latitude < latitude)
        return false;
    if (longitude < rhs.longitude)
        return true;
    if (rhs.longitude < longitude)
        return false;
    return time < rhs.time;
}

bool Position::operator>(const Position &rhs) const {
    return rhs < *this;
}

bool Position::operator<=(const Position &rhs) const {
    return !(rhs < *this);
}

bool Position::operator>=(const Position &rhs) const {
    return !(*this < rhs);
}
