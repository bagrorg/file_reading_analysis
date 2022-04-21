#pragma once
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class TestFile {
public:
    TestFile(const fs::path &p, const std::vector<char> &bytes);
    ~TestFile();

    fs::path getPath() const;
private:
    fs::path p;
};
