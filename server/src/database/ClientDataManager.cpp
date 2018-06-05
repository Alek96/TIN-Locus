#include "ClientDataManager.h"

#include <stdexcept>
#include "buffer/Converter.h"

using namespace buffer;
using namespace message;

short ClientDataManager::addClient(const std::vector<unsigned char> &publicKey) {
    clientsId.insert({publicKey, nextId});
    clientsData.insert({nextId, ClientData(publicKey)});
    return nextId++;
}

short ClientDataManager::registerClient(const std::vector<unsigned char> &publicKey,
                                        message::BlockingQueue<message::Message> *blockingQueue) {
    short id = 0;

    auto clientId = clientsId.find(publicKey);
    if (clientId != clientsId.end()) {
        id = clientId->second;
    } else {
        id = addClient(publicKey);
    }

    clientsQueue.insert({id, blockingQueue});
    return id;
}

void ClientDataManager::unregister(const short &id) {
    clientsQueue.erase(id);
}

void ClientDataManager::addFollower(const short &myId, const short &followerId) {
    auto myIdIt = clientsData.find(myId);
    if (myIdIt == clientsData.end()) {
        std::runtime_error("Error in addFollower method. myId does not exist");
    }
    auto followerIdIt = clientsData.find(followerId);
    if (followerIdIt == clientsData.end()) {
        std::runtime_error("Error in addFollower method. followerId does not exist");
    }

    myIdIt->second.addFollower(followerId);
    followerIdIt->second.startFollowing(myId);

    auto followersQueue = clientsQueue.find(followerId);
    if (followersQueue != clientsQueue.end()) {
        followersQueue->second->priorityPush(Message(Message::Type::Update));
    }
}

void ClientDataManager::removeFollower(const short &myId, const short &followerId) {
    auto myIdIt = clientsData.find(myId);
    if (myIdIt == clientsData.end()) {
        std::runtime_error("Error in removeFollower method. myId does not exist");
    }
    auto followerIdIt = clientsData.find(followerId);
    if (followerIdIt == clientsData.end()) {
        std::runtime_error("Error in removeFollower method. followerId does not exist");
    }

    myIdIt->second.removeFollower(followerId);
    followerIdIt->second.stopFollowing(myId);

//    auto observerQueue = clientsQueue.find(followerId);
//    if (observerQueue != clientsQueue.end()) {
//        Message message(Message::Type::EraseObserver);
//        message.id = std::make_unique<short>(myId);
//        observerQueue->second->priorityPush(std::move(message));
//    }
}

void ClientDataManager::stopFollowing(const short &myId, const short &followingId) {
    auto myIdIt = clientsData.find(myId);
    if (myIdIt == clientsData.end()) {
        std::runtime_error("Error in stopFollowing method. myId does not exist");
    }
    auto followingIdIt = clientsData.find(followingId);
    if (followingIdIt == clientsData.end()) {
        std::runtime_error("Error in stopFollowing method. followingId does not exist");
    }

    myIdIt->second.stopFollowing(followingId);
    followingIdIt->second.removeFollower(myId);
}

void ClientDataManager::notifyFollowers(const short &id) {
    auto clientDataIt = clientsData.find(id);
    if (clientDataIt == clientsData.end()) {
        std::runtime_error("Error in notifyFollowers method. id does not exist");
    }

    for (auto &follower : clientDataIt->second.followers) {
        auto followerQueue = clientsQueue.find(follower);
        if (followerQueue != clientsQueue.end()) {
            followerQueue->second->priorityPush(Message(Message::Type::Update));
        }
    }
}

void
ClientDataManager::addPosition(const short &myId, const float &latitude, const float &longitude, const time_t &time) {
    auto myIdIt = clientsData.find(myId);
    if (myIdIt == clientsData.end()) {
        std::runtime_error("Error in addPosition method. myId does not exist");
    }

    myIdIt->second.addPosition(latitude, longitude, time);
    notifyFollowers(myId);
}

std::list<std::pair<short, Position>> ClientDataManager::getNewPositions(const short &id) {
    std::list<std::pair<short, Position>> positions;
    int maxPositionsPerFollowingId = 3;

    auto itId = clientsData.find(id);
    if (itId == clientsData.end()) {
        std::runtime_error("Error in getNewPositions method. id does not exist");
    }

    for (auto &followingIt : itId->second.following) {
        auto followingItData = clientsData.at(followingIt.id);

        if (!followingItData.positions.empty() && !followingItData.name.empty()) {
            std::list<std::pair<short, Position>> newPositions;
            time_t newTime = 0;

            auto FollowingItPosition = --followingItData.positions.end();
            while (newPositions.size() < maxPositionsPerFollowingId) {
                if (FollowingItPosition->time > followingIt.time) {
                    newPositions.emplace_back(followingIt.id, *FollowingItPosition);
                    newTime = std::max(newTime, FollowingItPosition->time);
                } else {
                    break;
                }
                if (FollowingItPosition == followingItData.positions.begin()) {
                    break;
                }
            }
            followingIt.time = std::max(followingIt.time, newTime);
            positions.insert(positions.end(), newPositions.begin(), newPositions.end());
        }
    }

    return positions;
}

short ClientDataManager::getNameId(const std::string &name) {
    auto nameId = clientsName.find(name);
    return nameId != clientsName.end() ? nameId->second : static_cast<short>(0);
}

bool ClientDataManager::setName(const short &myId, const std::string &name) {
    auto myIdIt = clientsData.find(myId);
    if (myIdIt == clientsData.end()) {
        std::runtime_error("Error in addPosition method. myId does not exist");
    }

    auto nameCount = clientsName.count(name);
    if (nameCount == 0) {
        if (!myIdIt->second.name.empty()) {
            clientsName.erase(myIdIt->second.name);
        }
        clientsName.insert({name, myId});
        myIdIt->second.setName(name);
        notifyFollowers(myId);
        return true;
    } else {
        return nameCount == 1 && myIdIt->second.name == name;
    }
}

std::list<std::pair<short, std::string>> ClientDataManager::getNewNames(const short &id) {
    std::list<std::pair<short, std::string>> newNames;

    auto itId = clientsData.find(id);
    if (itId == clientsData.end()) {
        std::runtime_error("Error in getNewNames method. id does not exist");
    }

    for (auto &followingIt : itId->second.following) {
        auto followingItData = clientsData.at(followingIt.id);

        if (!followingItData.name.empty() && followingItData.name != followingIt.name) {
            followingIt.name = followingItData.name;
            newNames.emplace_back(followingIt.id, followingItData.name);
        }
    }

    return newNames;
}

bool ClientDataManager::operator==(const ClientDataManager &rhs) const {
    return nextId == rhs.nextId &&
           clientsId == rhs.clientsId &&
           clientsData == rhs.clientsData;
}

bool ClientDataManager::operator!=(const ClientDataManager &rhs) const {
    return !(rhs == *this);
}
