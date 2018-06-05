#include "gtest/gtest.h"
#include "../Position.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <iostream>

#define PRINT2

using namespace std;
using namespace cereal;

TEST(Position_Constructor, Initialize) {
    EXPECT_NO_THROW(Position());
    EXPECT_NO_THROW(Position(0, 0, 0));

    Position position(1, 2, 3);
    EXPECT_EQ(position, Position(1, 2, 3));
}


TEST(Position_Serialize, JSON) {
    Position position(1, 2, 3);

    stringstream ss;
    JSONOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(position));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Position_Deserialize, JSON) {
    stringstream ss;
    Position oPosition(1, 2, 3);
    Position iPosition;
    {
        JSONOutputArchive oarchive(ss);
        oarchive(oPosition);
    }
    {
        JSONInputArchive iarchive(ss);
        iarchive(iPosition);
    }
    EXPECT_EQ(oPosition, iPosition);
}

TEST(Position_Serialize, Binary) {
    Position position(1, 2, 3);

    stringstream ss;
    BinaryOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(position));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Position_Deserialize, Binary) {
    stringstream ss;
    Position oPosition(1, 2, 3);
    Position iPosition;
    {
        BinaryOutputArchive oarchive(ss);
        oarchive(oPosition);
    }
    {
        BinaryInputArchive iarchive(ss);
        iarchive(iPosition);
    }
    EXPECT_EQ(oPosition, iPosition);
}

TEST(Position_Serialize, PortableBinary) {
    Position position(1, 2, 3);

    stringstream ss;
    PortableBinaryOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(position));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Position_Deserialize, PortableBinary) {
    stringstream ss;
    Position oPosition(1, 2, 3);
    Position iPosition;
    {
        PortableBinaryOutputArchive oarchive(ss);
        oarchive(oPosition);
    }
    {
        PortableBinaryInputArchive iarchive(ss);
        iarchive(iPosition);
    }
    EXPECT_EQ(oPosition, iPosition);
}

