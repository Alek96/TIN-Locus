#include "gtest/gtest.h"
#include "database/ClientData.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <iostream>

#include "buffer/Converter.h"

#define PRINT2

using namespace std;
using namespace cereal;
using namespace buffer;

void fill(ClientData &clientInfo) {
    clientInfo.setName("name");

    clientInfo.addPosition(0,0,0);
    clientInfo.addPosition(0,0,1);
    clientInfo.addPosition(0,0,2);
    clientInfo.addPosition(0,0,3);

    clientInfo.addFollower(1);
    clientInfo.addFollower(2);

    clientInfo.startFollowing(1);
    clientInfo.startFollowing(2);
}

TEST(ClientInfo_Constructor, Initialize) {
    EXPECT_NO_THROW(ClientData());
    EXPECT_NO_THROW(ClientData(std::vector<unsigned char>(4, 0)));
    EXPECT_NO_THROW(ClientData(toByteVector("0000")));
}

TEST(ClientInfo_Serialize, JSON) {
    ClientData clientInfo(toByteVector("0000"));
    fill(clientInfo);

    stringstream ss;
    JSONOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(clientInfo));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(ClientInfo_Deserialize, JSON) {
    stringstream ss;
    ClientData oclientInfo(toByteVector("0000"));
    fill(oclientInfo);

    ClientData iclientInfo;
    {
        JSONOutputArchive oarchive(ss);
        oarchive(oclientInfo);
    }
    {
        JSONInputArchive iarchive(ss);
        iarchive(iclientInfo);
    }
    EXPECT_EQ(oclientInfo, iclientInfo);
}

TEST(ClientInfo_Serialize, Binary) {
    ClientData clientInfo(toByteVector("0000"));
    fill(clientInfo);

    stringstream ss;
    BinaryOutputArchive archive(ss);

    EXPECT_NO_THROW(archive(clientInfo));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(ClientInfo_Deserialize, Binary) {
    stringstream ss;
    ClientData oclientInfo(toByteVector("0000"));
    fill(oclientInfo);

    ClientData iclientInfo;
    {
        BinaryOutputArchive oarchive(ss);
        oarchive(oclientInfo);
    }
    {
        BinaryInputArchive iarchive(ss);
        iarchive(iclientInfo);
    }
    EXPECT_EQ(oclientInfo, iclientInfo);
}

TEST(ClientInfo_Serialize, PortableBinary) {
    ClientData clientInfo(toByteVector("0000"));
    fill(clientInfo);

    stringstream ss;
    PortableBinaryOutputArchive archive(ss);

    EXPECT_NO_THROW(archive(clientInfo));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(ClientInfo_Deserialize, PortableBinary) {
    stringstream ss;
    ClientData oclientInfo(toByteVector("0000"));
    fill(oclientInfo);

    ClientData iclientInfo;
    {
        PortableBinaryOutputArchive oarchive(ss);
        oarchive(oclientInfo);
    }
    {
        PortableBinaryInputArchive iarchive(ss);
        iarchive(iclientInfo);
    }
    EXPECT_EQ(oclientInfo, iclientInfo);
}

