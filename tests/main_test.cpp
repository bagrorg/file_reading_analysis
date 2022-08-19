#include <gtest/gtest.h>
#include <filesystem>

#define MMAP_TEST_PATH "mmap_test/"
#define IFSTREAM_TEST_PATH "ifstream_test/"
#define READ_TEST_PATH "read_test/"

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    fs::create_directory(MMAP_TEST_PATH);
    fs::create_directory(IFSTREAM_TEST_PATH);
    fs::create_directory(READ_TEST_PATH);


    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}