#include "gtest/gtest.h"
#include "../ClientDataManager.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <iostream>

#include "buffer/Converter.h"

#define PRINT2

using namespace std;
using namespace cereal;
using namespace buffer;
using namespace message;

class ClientDataManagerTest : public ClientDataManager {
public:
    int addClient(const std::vector<unsigned char> &publicKey) { ClientDataManager::addClient(publicKey); }

    void notifyObservers(const int &id) { ClientDataManager::notifyFollowers(id); }
};

TEST(ClientPositionManager_Constructor, Initialize) {
    ClientDataManager clientDataManager;

//    EXPECT_EQ(clientDataManager.nextId, 0);
//    EXPECT_TRUE(clientDataManager.clientsId.empty());
//    EXPECT_TRUE(clientDataManager.clientsData.empty());
//    EXPECT_TRUE(clientDataManager.clientsQueue.empty());
}

TEST(Basedata_AddClient, Test) {
    ClientDataManagerTest clientDataManager;
    auto publicKey = toByteVector("0000");

    auto id = clientDataManager.addClient(publicKey);

    EXPECT_EQ(id, 1);
//    EXPECT_EQ(clientDataManager.nextId, 1);
//    EXPECT_EQ(clientDataManager.clientsData.at(id), ClientData(publicKey));
//    EXPECT_TRUE(clientDataManager.clientsQueue.empty());
}


TEST(Basedata_RegisterClient, AddOne) {
    ClientDataManagerTest clientDataManager;
    auto publicKey = toByteVector("0000");
    BlockingQueue<Message> blockingQueue(10);

    auto id = clientDataManager.registerClient(publicKey, &blockingQueue);

    EXPECT_EQ(id, 1);
//    EXPECT_EQ(clientDataManager.nextId, 1);
//    EXPECT_EQ(clientDataManager.clientsData.at(id), ClientData(publicKey));
//    EXPECT_EQ(clientDataManager.clientsQueue.at(id), &blockingQueue);
}

TEST(Basedata_RegisterClient, AddTheSameOne) {
    ClientDataManager clientDataManager;
    auto publicKey = toByteVector("0000");
    BlockingQueue<Message> blockingQueue(10);

    auto id1 = clientDataManager.registerClient(publicKey, &blockingQueue);
    auto id2 = clientDataManager.registerClient(publicKey, &blockingQueue);

    EXPECT_EQ(id1, id2);
//    EXPECT_EQ(clientDataManager.nextId, 1);
//    EXPECT_EQ(clientDataManager.clientsData.at(id2), ClientData(publicKey));
//    EXPECT_EQ(clientDataManager.clientsQueue.at(id2), &blockingQueue);
}

TEST(Basedata_RegisterClient, AddTwoDiffrent) {
    ClientDataManager clientDataManager;
    BlockingQueue<Message> blockingQueue(10);
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue);

    EXPECT_NE(id1, id2);
    EXPECT_EQ(id2, 2);
//    EXPECT_EQ(clientDataManager.nextId, 2);
//    EXPECT_EQ(clientDataManager.clientsData.at(id2), ClientData(publicKey2));
//    EXPECT_EQ(clientDataManager.clientsQueue.at(id2), &blockingQueue);
}

TEST(Basedata_UnregisterClient, Test) {
    ClientDataManager clientDataManager;
    auto publicKey = toByteVector("0000");
    BlockingQueue<Message> blockingQueue(10);

    auto id = clientDataManager.registerClient(publicKey, &blockingQueue);
    clientDataManager.unregister(id);

    EXPECT_EQ(id, 1);
//    EXPECT_EQ(clientDataManager.nextId, 1);
//    EXPECT_EQ(clientDataManager.clientsData.at(id), ClientData(publicKey));
//    EXPECT_EQ(clientDataManager.clientsQueue.find(id), clientDataManager.clientsQueue.end());
}

TEST(Basedata_AddObserver, Test) {
    ClientDataManager clientDataManager;
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");
    BlockingQueue<Message> blockingQueue1(10);
    BlockingQueue<Message> blockingQueue2(10);

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue1);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue2);

    EXPECT_EQ(blockingQueue2.size(), 0);
    clientDataManager.addFollower(id1, id2);
    EXPECT_EQ(blockingQueue2.size(), 1);
    EXPECT_EQ(blockingQueue2.pop().type, Message::Update);
}

TEST(Basedata_EraseObserver, Test) {
    ClientDataManager clientDataManager;
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");
    BlockingQueue<Message> blockingQueue1(10);
    BlockingQueue<Message> blockingQueue2(10);

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue1);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue2);

    clientDataManager.addFollower(id1, id2);
    blockingQueue2.pop();

    EXPECT_EQ(blockingQueue1.size(), 0);
    EXPECT_EQ(blockingQueue2.size(), 0);
    clientDataManager.removeFollower(id1, id2);
    EXPECT_EQ(blockingQueue1.size(), 0);
    EXPECT_EQ(blockingQueue2.size(), 0);
    
//    EXPECT_EQ(blockingQueue2.size(), 1);
//    auto msg = blockingQueue2.pop();
//    EXPECT_EQ(msg.type, Message::EraseObserver);
//    EXPECT_EQ(*msg.id, id1);
}

TEST(Basedata_NotifyObservers, Test) {
    ClientDataManagerTest clientDataManager;
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");
    BlockingQueue<Message> blockingQueue1(10);
    BlockingQueue<Message> blockingQueue2(10);

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue1);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue2);

    clientDataManager.addFollower(id1, id2);
    blockingQueue2.pop();

    EXPECT_EQ(blockingQueue2.size(), 0);
    clientDataManager.notifyObservers(id1);
    EXPECT_EQ(blockingQueue2.size(), 1);
    EXPECT_EQ(blockingQueue2.pop().type, Message::Update);
}

TEST(Basedata_SetName, Test) {
    ClientDataManager clientDataManager;
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");
    BlockingQueue<Message> blockingQueue1(10);
    BlockingQueue<Message> blockingQueue2(10);

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue1);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue2);

    clientDataManager.addFollower(id1, id2);
    blockingQueue2.pop();

    EXPECT_EQ(blockingQueue2.size(), 0);
    clientDataManager.setName(id1, "name");
    EXPECT_EQ(blockingQueue2.size(), 1);
    EXPECT_EQ(blockingQueue2.pop().type, Message::Update);
}

TEST(Basedata_GetNewNames, Test) {
    ClientDataManager clientDataManager;
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");
    BlockingQueue<Message> blockingQueue1(10);
    BlockingQueue<Message> blockingQueue2(10);

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue1);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue2);

    //no new names
    EXPECT_EQ(clientDataManager.getNewNames(id2).size(), 0);

    clientDataManager.addFollower(id1, id2);
    clientDataManager.setName(id1, "name");

    //one new names
    auto newNames = clientDataManager.getNewNames(id2);
    ASSERT_FALSE(newNames.empty());
    EXPECT_EQ(newNames.size(), 1);
    EXPECT_EQ(newNames.front().second, "name");

    //no new names
    EXPECT_EQ(clientDataManager.getNewNames(id2).size(), 0);
}

TEST(Basedata_AddPosition, Test) {
    ClientDataManager clientDataManager;
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");
    BlockingQueue<Message> blockingQueue1(10);
    BlockingQueue<Message> blockingQueue2(10);

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue1);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue2);

    clientDataManager.addFollower(id1, id2);
    blockingQueue2.pop();

    EXPECT_EQ(blockingQueue2.size(), 0);
    clientDataManager.addPosition(id1, 0, 0, 0);
    EXPECT_EQ(blockingQueue2.size(), 1);
    EXPECT_EQ(blockingQueue2.pop().type, Message::Update);
}

TEST(Basedata_GetNewPositions, Test) {
    ClientDataManager clientDataManager;
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");
    BlockingQueue<Message> blockingQueue1(10);
    BlockingQueue<Message> blockingQueue2(10);

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue1);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue2);
    clientDataManager.setName(id1, "name");

    //no new positions
    EXPECT_EQ(clientDataManager.getNewPositions(id2).size(), 0);

    clientDataManager.addFollower(id1, id2);
    clientDataManager.addPosition(id1, 0, 0, 1);

    //one new position
    auto newPositions = clientDataManager.getNewPositions(id2);
    ASSERT_FALSE(newPositions.empty());
    EXPECT_EQ(newPositions.size(), 1);
    EXPECT_EQ(newPositions.front().second, Position(0, 0, 1));

    //no new positions
    EXPECT_EQ(clientDataManager.getNewPositions(id2).size(), 0);
}

TEST(Basedata_EraseWatcher, Test) {
    ClientDataManager clientDataManager;
    auto publicKey1 = toByteVector("0000");
    auto publicKey2 = toByteVector("0001");
    BlockingQueue<Message> blockingQueue1(10);
    BlockingQueue<Message> blockingQueue2(10);

    auto id1 = clientDataManager.registerClient(publicKey1, &blockingQueue1);
    auto id2 = clientDataManager.registerClient(publicKey2, &blockingQueue2);

    clientDataManager.addFollower(id1, id2);
    blockingQueue2.pop();

    EXPECT_EQ(blockingQueue1.size(), 0);
    EXPECT_EQ(blockingQueue2.size(), 0);
    clientDataManager.stopFollowing(id2, id1);
    EXPECT_EQ(blockingQueue1.size(), 0);
    EXPECT_EQ(blockingQueue2.size(), 0);

    clientDataManager.addPosition(id1, 0, 0, 1);
    EXPECT_EQ(blockingQueue2.size(), 0);
}


TEST(ClientPositionManager_Serialize, JSON) {
    ClientDataManager clientDataManager;

    stringstream ss;
    JSONOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(clientDataManager));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(ClientPositionManager_Deserialize, JSON) {
    stringstream ss;
    ClientDataManager oclientDataManager;
    ClientDataManager iclientDataManager;
    {
        JSONOutputArchive oarchive(ss);
        oarchive(oclientDataManager);
    }
    {
        JSONInputArchive iarchive(ss);
        iarchive(iclientDataManager);
    }
    EXPECT_EQ(oclientDataManager, iclientDataManager);
}

TEST(ClientPositionManager_Serialize, Binary) {
    ClientDataManager clientDataManager;

    stringstream ss;
    BinaryOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(clientDataManager));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(ClientPositionManager_Deserialize, Binary) {
    stringstream ss;
    ClientDataManager oclientDataManager;
    ClientDataManager iclientDataManager;
    {
        BinaryOutputArchive oarchive(ss);
        oarchive(oclientDataManager);
    }
    {
        BinaryInputArchive iarchive(ss);
        iarchive(iclientDataManager);
    }
    EXPECT_EQ(oclientDataManager, iclientDataManager);
}

TEST(ClientPositionManager_Serialize, PortableBinary) {
    ClientDataManager clientDataManager;

    stringstream ss;
    PortableBinaryOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(clientDataManager));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(ClientPositionManager_Deserialize, PortableBinary) {
    stringstream ss;
    ClientDataManager oclientDataManager;
    ClientDataManager iclientDataManager;
    {
        PortableBinaryOutputArchive oarchive(ss);
        oarchive(oclientDataManager);
    }
    {
        PortableBinaryInputArchive iarchive(ss);
        iarchive(iclientDataManager);
    }
    EXPECT_EQ(oclientDataManager, iclientDataManager);
}
