#include "Following.h"

Following::Following(short id)
        : id(id) {}

bool Following::operator==(const Following &rhs) const {
    return id == rhs.id &&
           time == rhs.time &&
           name == rhs.name;
}

bool Following::operator!=(const Following &rhs) const {
    return !(rhs == *this);
}

bool Following::operator<(const Following &rhs) const {
    if (id < rhs.id)
        return true;
    if (rhs.id < id)
        return false;
    if (time < rhs.time)
        return true;
    if (rhs.time < time)
        return false;
    return name < rhs.name;
}

bool Following::operator>(const Following &rhs) const {
    return rhs < *this;
}

bool Following::operator<=(const Following &rhs) const {
    return !(rhs < *this);
}

bool Following::operator>=(const Following &rhs) const {
    return !(*this < rhs);
}

