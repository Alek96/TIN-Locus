#include "ClientData.h"
#include "buffer/Converter.h"

using namespace buffer;

ClientData::ClientData(const std::vector<unsigned char> &publicKey)
        : publicKey(publicKey) {
}

void ClientData::setName(const std::string &name) {
    this->name = name;
}

void ClientData::addPosition(const float &latitude, const float &longitude, const time_t &time) {
    positions.emplace_back(Position(latitude, longitude, time));
    
    if(positions.size() > 10) {
        positions.pop_front();
    }
}

void ClientData::addFollower(const short &id) {
    followers.push_back(id);
}

void ClientData::removeFollower(const short &id) {
    followers.remove(id);
}

void ClientData::startFollowing(const short &id) {
    following.emplace_back(Following(id));
}

void ClientData::stopFollowing(const short &id) {
    following.remove_if([&id](Following &w) { return w.id == id; });
}

std::vector<unsigned char> ClientData::getPublicKey() const {
    return publicKey;
}

bool ClientData::operator==(const ClientData &rhs) const {
    return publicKey == rhs.publicKey &&
           positions == rhs.positions &&
           followers == rhs.followers &&
           following == rhs.following;
}

bool ClientData::operator!=(const ClientData &rhs) const {
    return !(rhs == *this);
}

bool ClientData::operator<(const ClientData &rhs) const {
    if (publicKey < rhs.publicKey)
        return true;
    if (rhs.publicKey < publicKey)
        return false;
    if (name < rhs.name)
        return true;
    if (rhs.name < name)
        return false;
    if (positions < rhs.positions)
        return true;
    if (rhs.positions < positions)
        return false;
    if (followers < rhs.followers)
        return true;
    if (rhs.followers < followers)
        return false;
    return following < rhs.following;
}

bool ClientData::operator>(const ClientData &rhs) const {
    return rhs < *this;
}

bool ClientData::operator<=(const ClientData &rhs) const {
    return !(rhs < *this);
}

bool ClientData::operator>=(const ClientData &rhs) const {
    return !(*this < rhs);
}
