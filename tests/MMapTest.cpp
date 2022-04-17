#include <gtest/gtest.h>
#include "../methods/methods.h" //TODO relative path
#include "utils/utils.h"
#include <random>

#define MMAP_TEST_PATH "mmap_test/"
#define BIG_TEST_SIZE 100

TEST(MMapMethodTests, fileSize) {
    std::vector<char> testBytes = {
            123,
            123,
            123,
            123,
            123,
            123
    };

    fs::path inp = fs::path(MMAP_TEST_PATH) / fs::path("fileSizeTest");
    TestFile tf(inp, testBytes);
    MMapMethod m(inp);

    ASSERT_EQ(m.getFileSize(), testBytes.size());
}

TEST(MMapMethodTests, statisticsSame) {
    std::vector<char> testBytes = {
            123,
            123,
            123,
            123,
            123,
            123
    };

    fs::path inp = fs::path(MMAP_TEST_PATH) / fs::path("statisticsSameTest");
    TestFile tf(inp, testBytes);
    MMapMethod m(inp);
    m.run();
    auto stats = m.getStats();

    ASSERT_EQ(stats.bytes_stat[123], testBytes.size());
}

TEST(MMapMethodTests, statisticsDifferent) {
    std::vector<char> testBytes = {
            123,
            123,
            111,
            111,
            0,
            123
    };

    fs::path inp = fs::path(MMAP_TEST_PATH) / fs::path("statisticsDifferentTest");
    TestFile tf(inp, testBytes);
    MMapMethod m(inp);
    m.run();
    auto stats = m.getStats();

    ASSERT_EQ(stats.bytes_stat[123], 3);
    ASSERT_EQ(stats.bytes_stat[111], 2);
    ASSERT_EQ(stats.bytes_stat[0], 1);
}

TEST(MMapMethodTests, bigStatsTest) {
    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_int_distribution<char> dist {-127, 127};

    auto gen = [&dist, &mersenne_engine](){
        return dist(mersenne_engine);
    };

    std::vector<char> testBytes(BIG_TEST_SIZE);
    std::generate(testBytes.begin(), testBytes.end(), gen);

    std::array<size_t, 256> expected = {};
    std::for_each(testBytes.begin(), testBytes.end(), [&expected](char b) {
        expected[b]++;
    });

    fs::path inp = fs::path(MMAP_TEST_PATH) / fs::path("bigStatsTest");
    TestFile tf(inp, testBytes);
    MMapMethod m(inp);
    m.run();
    auto stats = m.getStats();

    ASSERT_EQ(stats.bytes_stat, expected);
}


