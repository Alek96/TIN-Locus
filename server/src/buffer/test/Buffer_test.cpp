#include "gtest/gtest.h"
#include "buffer/BlockBuffer.h"

#include "buffer/Converter.h"

using namespace buffer;

TEST(BlockBuffer_getSize, Empty) {
    BlockBuffer buffer;
    EXPECT_EQ(buffer.size(), 0);
}

TEST(BlockBuffer_push, fewElem) {
    BlockBuffer buffer;

    std::vector<unsigned char> bytes(5, 1);
    buffer.push(bytes);
    EXPECT_EQ(buffer.size(), 1);

    buffer.push(std::vector<unsigned char>(5, 2));
    EXPECT_EQ(buffer.size(), 2);

    buffer.push(2);
    EXPECT_EQ(buffer.size(), 3);
}

TEST(BlockBuffer_pop, Empty) {
    BlockBuffer buffer;

    ASSERT_THROW(buffer.pop(), std::runtime_error);

    int x;
    ASSERT_THROW(buffer.pop(x), std::runtime_error);
}

TEST(BlockBuffer_pop, NonEmpty1) {
    BlockBuffer buffer;

    std::vector<unsigned char> bytes(5, 1);
    std::vector<unsigned char> bytes2(bytes);
    buffer.push(bytes);

    EXPECT_EQ(buffer.pop(), bytes2);
    EXPECT_EQ(buffer.size(), 0);
}

TEST(BlockBuffer_pop, NonEmpty2) {
    BlockBuffer buffer;

    std::vector<unsigned char> bytes(5, 1);
    std::vector<unsigned char> bytes2(bytes);
    buffer.push(std::move(bytes));

    auto out = buffer.pop();
    EXPECT_NE(out, bytes);
    EXPECT_EQ(out, bytes2);
    EXPECT_EQ(buffer.size(), 0);
}

TEST(BlockBuffer_pop, NonEmpty3) {
    BlockBuffer buffer;

    int x;
    int pre = x;
    buffer.push(to_bytes(x));

    auto post = buffer.pop();
    EXPECT_EQ(post, to_bytes(pre));
    EXPECT_EQ(buffer.size(), 0);
}

TEST(BlockBuffer_pop, NonEmpty4) {
    BlockBuffer buffer;

    std::vector<unsigned char> bytes(4, 0);
    buffer.push(bytes);

    int result;
    EXPECT_EQ(buffer.pop(result), 0);
    EXPECT_EQ(buffer.size(), 0);
}

TEST(BlockBuffer_pop, NonEmpty5) {
    BlockBuffer buffer;

    std::vector<unsigned char> bytes(4, 0);
    buffer.push(std::move(bytes));

    int result;
    EXPECT_EQ(buffer.pop(result), 0);
    EXPECT_EQ(buffer.size(), 0);
}

TEST(BlockBuffer_pop, NonEmpty6) {
    BlockBuffer buffer;

    int x;
    int pre = x;
    int post;
    buffer.push(x);

    EXPECT_EQ(buffer.pop(post), pre);
    EXPECT_EQ(buffer.size(), 0);
}