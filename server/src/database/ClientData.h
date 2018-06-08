#ifndef LOCUS_CLIENTDATA_H
#define LOCUS_CLIENTDATA_H

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <ctime>

#include "Position.h"
#include "Following.h"

struct ClientData {
    ClientData() = default;
    explicit ClientData(const std::vector<unsigned char> &publicKey);

    std::vector<unsigned char> publicKey;
    std::string name;
    std::list<Position> positions; //x, y, time
    std::list<short> followers; //i'm sending the location
    std::list<Following> following; //they are sending the location

//    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(publicKey), CEREAL_NVP(name), CEREAL_NVP(positions), CEREAL_NVP(followers), CEREAL_NVP(following));
    }

    void setName(const std::string &name);

    void addPosition(const float &latitude, const float &longitude, const time_t &time);

    void addFollower(const short &id);
    void removeFollower(const short &id);

    void startFollowing(const short &id);
    void stopFollowing(const short &id);

    std::vector<unsigned char> getPublicKey() const;

    bool operator==(const ClientData &rhs) const;
    bool operator!=(const ClientData &rhs) const;

    bool operator<(const ClientData &rhs) const;
    bool operator>(const ClientData &rhs) const;
    bool operator<=(const ClientData &rhs) const;
    bool operator>=(const ClientData &rhs) const;
};


#endif //LOCUS_CLIENTDATA_H
