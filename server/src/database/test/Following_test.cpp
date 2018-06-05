#include "gtest/gtest.h"
#include "database/Following.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <iostream>

#define PRINT2

using namespace std;
using namespace cereal;

TEST(Watcher_Constructor, Initialize) {
    EXPECT_NO_THROW(Following());
    EXPECT_NO_THROW(Following(0));

    Following watcher(1);
    EXPECT_EQ(watcher, Following(1));
}


TEST(Watcher_Serialize, JSON) {
    Following watcher(1);

    stringstream ss;
    JSONOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(watcher));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Watcher_Deserialize, JSON) {
    stringstream ss;
    Following oWatcher(1);
    Following iWatcher;
    {
        JSONOutputArchive oarchive(ss);
        oarchive(oWatcher);
    }
    {
        JSONInputArchive iarchive(ss);
        iarchive(iWatcher);
    }
    EXPECT_EQ(oWatcher, iWatcher);
}

TEST(Watcher_Serialize, Binary) {
    Following watcher(1);

    stringstream ss;
    BinaryOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(watcher));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Watcher_Deserialize, Binary) {
    stringstream ss;
    Following oWatcher(1);
    Following iWatcher;
    {
        BinaryOutputArchive oarchive(ss);
        oarchive(oWatcher);
    }
    {
        BinaryInputArchive iarchive(ss);
        iarchive(iWatcher);
    }
    EXPECT_EQ(oWatcher, iWatcher);
}

TEST(Watcher_Serialize, PortableBinary) {
    Following watcher(1);

    stringstream ss;
    PortableBinaryOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(watcher));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Watcher_Deserialize, PortableBinary) {
    stringstream ss;
    Following oWatcher(1);
    Following iWatcher;
    {
        PortableBinaryOutputArchive oarchive(ss);
        oarchive(oWatcher);
    }
    {
        PortableBinaryInputArchive iarchive(ss);
        iarchive(iWatcher);
    }
    EXPECT_EQ(oWatcher, iWatcher);
}
