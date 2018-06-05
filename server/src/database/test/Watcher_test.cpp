#include "gtest/gtest.h"
#include "../Watcher.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <iostream>

#define PRINT2

using namespace std;
using namespace cereal;

TEST(Watcher_Constructor, Initialize) {
    EXPECT_NO_THROW(Watcher());
    EXPECT_NO_THROW(Watcher(0));

    Watcher watcher(1);
    EXPECT_EQ(watcher, Watcher(1));
}


TEST(Watcher_Serialize, JSON) {
    Watcher watcher(1);

    stringstream ss;
    JSONOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(watcher));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Watcher_Deserialize, JSON) {
    stringstream ss;
    Watcher oWatcher(1);
    Watcher iWatcher;
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
    Watcher watcher(1);

    stringstream ss;
    BinaryOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(watcher));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Watcher_Deserialize, Binary) {
    stringstream ss;
    Watcher oWatcher(1);
    Watcher iWatcher;
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
    Watcher watcher(1);

    stringstream ss;
    PortableBinaryOutputArchive oarchive(ss);

    EXPECT_NO_THROW(oarchive(watcher));
#ifdef PRINT
    cout << ss.str() << endl;
#endif
}

TEST(Watcher_Deserialize, PortableBinary) {
    stringstream ss;
    Watcher oWatcher(1);
    Watcher iWatcher;
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
