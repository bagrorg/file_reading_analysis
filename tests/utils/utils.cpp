#include <fstream>
#include "utils.h"

TestFile::TestFile(const fs::path &p, const std::vector<char> &bytes) : p(p) {
    std::ofstream out(p, std::ios::binary);
    out.write(bytes.data(), bytes.size());
    if (!out.good()) {   //TODO check
        throw std::runtime_error("Something wrong with file creation");
    }
    if (!fs::exists(p)) {
        throw std::runtime_error("File wasn't created");
    }
    out.close();
}

TestFile::~TestFile() {
    if (fs::exists(p)) {
        fs::remove(p);
    }
}

fs::path TestFile::getPath() const {
    return p;
}
