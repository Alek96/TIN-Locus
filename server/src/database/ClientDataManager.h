#ifndef LOCUS_CLIENTPOSITIONMANAGER_H
#define LOCUS_CLIENTPOSITIONMANAGER_H

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/unordered_map.hpp>

#include "ClientData.h"
#include "message/Message.h"
#include "message/BlockingQueue.h"

class ClientDataManager {
public:
    ClientDataManager() = default;

    short registerClient(const std::vector<unsigned char> &publicKey,
                         message::BlockingQueue<message::Message> *blockingQueue);
    void unregister(const short &id);

    void addFollower(const short &myId, const short &followerId);
    void removeFollower(const short &myId, const short &followerId);

    void stopFollowing(const short &myId, const short &followingId);

    void addPosition(const short &myId, const float &latitude, const float &longitude, const time_t &time);
    std::list<std::pair<short, Position>> getNewPositions(const short &id);

    short getNameId(const std::string &name);
    bool setName(const short &myId, const std::string &name);
    std::list<std::pair<short, std::string>> getNewNames(const short &id);


    bool operator==(const ClientDataManager &rhs) const;
    bool operator!=(const ClientDataManager &rhs) const;

    friend class cereal::access;

protected:
    short nextId = 1;
    std::map<std::vector<unsigned char>, short> clientsId;
    std::unordered_map<std::string, short> clientsName;
    std::unordered_map<short, ClientData> clientsData;
    std::unordered_map<short, message::BlockingQueue<message::Message> *> clientsQueue;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(nextId), CEREAL_NVP(clientsId), CEREAL_NVP(clientsName), CEREAL_NVP(clientsData));
    }

    short addClient(const std::vector<unsigned char> &publicKey);
    void notifyFollowers(const short &id);
};


#endif //LOCUS_CLIENTPOSITIONMANAGER_H
