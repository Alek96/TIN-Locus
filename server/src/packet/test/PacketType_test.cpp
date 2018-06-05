#include "gtest/gtest.h"
#include "../PacketType.h"

using namespace packet;

TEST(PacketType_toUint8, Open) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::OPEN), 0);
}
TEST(PacketType_toUint8, Close) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::CLOSE), 1);
}
TEST(PacketType_toUint8, AckOK) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::ACK_OK), 2);
}
TEST(PacketType_toUint8, AckErr) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::ACK_ERR), 3);
}
TEST(PacketType_toUint8, Alive) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::ALIVE), 4);
}


TEST(PacketType_toUint8, OpenEncr) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::OPEN_ENCR), 8);
}
TEST(PacketType_toUint8, PublicKey) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::PUBLIC_KEY), 9);
}
TEST(PacketType_toUint8, SymmetriKey) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::SYMMETRIC_KEY), 10);
}
TEST(PacketType_toUint8, TestKey) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::TEST_KEY), 11);
}


TEST(PacketType_toUint8, OpenProt) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::OPEN_PROT), 16);
}
TEST(PacketType_toUint8, SetName) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::SET_NAME), 17);
}
TEST(PacketType_toUint8, AddFollower) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::ADD_FOLLOWER), 18);
}
TEST(PacketType_toUint8, NewFollowed) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::NEW_FOLLOWED), 19);
}
TEST(PacketType_toUint8, RemoveFollower) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::REMOVE_FOLLOWER), 20);
}
TEST(PacketType_toUint8, RemoveFollowed) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::REMOVE_FOLLOWED), 21);
}
TEST(PacketType_toUint8, MyLocation) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::MY_LOCATION), 22);
}
TEST(PacketType_toUint8, Location) {
    EXPECT_EQ(PacketType::toUInt8(PacketType::LOCATION), 23);
}



TEST(PacketType_toPackeType, Open) {
    EXPECT_EQ(PacketType::toPacketType(0), PacketType::OPEN);
}
TEST(PacketType_toPackeType, Close) {
    EXPECT_EQ(PacketType::toPacketType(1), PacketType::CLOSE);
}
TEST(PacketType_toPackeType, AckOk) {
    EXPECT_EQ(PacketType::toPacketType(2), PacketType::ACK_OK);
}
TEST(PacketType_toPackeType, AckErr) {
    EXPECT_EQ(PacketType::toPacketType(3), PacketType::ACK_ERR);
}
TEST(PacketType_toPackeType, Alive) {
    EXPECT_EQ(PacketType::toPacketType(4), PacketType::ALIVE);
}


TEST(PacketType_toPackeType, OpenEncr) {
    EXPECT_EQ(PacketType::toPacketType(8), PacketType::OPEN_ENCR);
}
TEST(PacketType_toPackeType, PublicKey) {
    EXPECT_EQ(PacketType::toPacketType(9), PacketType::PUBLIC_KEY);
}
TEST(PacketType_toPackeType, SymmetricKey) {
    EXPECT_EQ(PacketType::toPacketType(10), PacketType::SYMMETRIC_KEY);
}
TEST(PacketType_toPackeType, testKey) {
    EXPECT_EQ(PacketType::toPacketType(11), PacketType::TEST_KEY);
}


TEST(PacketType_toPackeType, OpenProt) {
    EXPECT_EQ(PacketType::toPacketType(16), PacketType::OPEN_PROT);
}
TEST(PacketType_toPackeType, SetName) {
    EXPECT_EQ(PacketType::toPacketType(17), PacketType::SET_NAME);
}
TEST(PacketType_toPackeType, AddFollower) {
    EXPECT_EQ(PacketType::toPacketType(18), PacketType::ADD_FOLLOWER);
}
TEST(PacketType_toPackeType, NewFollowed) {
    EXPECT_EQ(PacketType::toPacketType(19), PacketType::NEW_FOLLOWED);
}
TEST(PacketType_toPackeType, RemoveFollower) {
    EXPECT_EQ(PacketType::toPacketType(20), PacketType::REMOVE_FOLLOWER);
}
TEST(PacketType_toPackeType, RemoveFollowed) {
    EXPECT_EQ(PacketType::toPacketType(21), PacketType::REMOVE_FOLLOWED);
}
TEST(PacketType_toPackeType, MyLocation) {
    EXPECT_EQ(PacketType::toPacketType(22), PacketType::MY_LOCATION);
}
TEST(PacketType_toPackeType, Location) {
    EXPECT_EQ(PacketType::toPacketType(23), PacketType::LOCATION);
}


TEST(PacketType_toPackeType, UndefinedType) {
    EXPECT_EQ(PacketType::toPacketType(50), PacketType::UNDEFINED);
}


TEST(PacketType_toString, Open) {
    EXPECT_EQ(PacketType::toString(PacketType::OPEN), "OPEN");
}
TEST(PacketType_toString, Close) {
    EXPECT_EQ(PacketType::toString(PacketType::CLOSE), "CLOSE");
}
TEST(PacketType_toString, AckOK) {
    EXPECT_EQ(PacketType::toString(PacketType::ACK_OK), "ACK_OK");
}
TEST(PacketType_toString, AckErr) {
    EXPECT_EQ(PacketType::toString(PacketType::ACK_ERR), "ACK_ERR");
}
TEST(PacketType_toString, Alive) {
    EXPECT_EQ(PacketType::toString(PacketType::ALIVE), "ALIVE");
}


TEST(PacketType_toString, OpenEncr) {
    EXPECT_EQ(PacketType::toString(PacketType::OPEN_ENCR), "OPEN_ENCR");
}
TEST(PacketType_toString, PublicKey) {
    EXPECT_EQ(PacketType::toString(PacketType::PUBLIC_KEY), "PUBLIC_KEY");
}
TEST(PacketType_toString, SymmetriKey) {
    EXPECT_EQ(PacketType::toString(PacketType::SYMMETRIC_KEY), "SYMMETRIC_KEY");
}
TEST(PacketType_toString, TestKey) {
    EXPECT_EQ(PacketType::toString(PacketType::TEST_KEY), "TEST_KEY");
}


TEST(PacketType_toString, OpenProt) {
    EXPECT_EQ(PacketType::toString(PacketType::OPEN_PROT), "OPEN_PROT");
}
TEST(PacketType_toString, SetName) {
    EXPECT_EQ(PacketType::toString(PacketType::SET_NAME), "SET_NAME");
}
TEST(PacketType_toString, AddFollower) {
    EXPECT_EQ(PacketType::toString(PacketType::ADD_FOLLOWER), "ADD_FOLLOWER");
}
TEST(PacketType_toString, NewFollowed) {
    EXPECT_EQ(PacketType::toString(PacketType::NEW_FOLLOWED), "NEW_FOLLOWED");
}
TEST(PacketType_toString, RemoveFollower) {
    EXPECT_EQ(PacketType::toString(PacketType::REMOVE_FOLLOWER), "REMOVE_FOLLOWER");
}
TEST(PacketType_toString, RemoveFollowed) {
    EXPECT_EQ(PacketType::toString(PacketType::REMOVE_FOLLOWED), "REMOVE_FOLLOWED");
}
TEST(PacketType_toString, MyLocation) {
    EXPECT_EQ(PacketType::toString(PacketType::MY_LOCATION), "MY_LOCATION");
}
TEST(PacketType_toString, Location) {
    EXPECT_EQ(PacketType::toString(PacketType::LOCATION), "LOCATION");
}
