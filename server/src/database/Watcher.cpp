#include "Watcher.h"

Watcher::Watcher(short id)
        : id(id) {}

bool Watcher::operator==(const Watcher &rhs) const {
    return id == rhs.id &&
           time == rhs.time &&
           name == rhs.name;
}

bool Watcher::operator!=(const Watcher &rhs) const {
    return !(rhs == *this);
}

bool Watcher::operator<(const Watcher &rhs) const {
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

bool Watcher::operator>(const Watcher &rhs) const {
    return rhs < *this;
}

bool Watcher::operator<=(const Watcher &rhs) const {
    return !(rhs < *this);
}

bool Watcher::operator>=(const Watcher &rhs) const {
    return !(*this < rhs);
}

