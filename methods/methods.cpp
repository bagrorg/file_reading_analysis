#include "methods.h"
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

void Statistic::add_stats(const char *data, size_t length) {
    for (int i = 0; i < length; i++) {
        bytes_stat[(unsigned char) data[i]] += 1;
    }
}

void Statistic::print(std::ostream &out) {
    for (int i = 0; i < bytes_stat.size(); i++) {
        if (bytes_stat[i] == 0) continue;
        out << '\t' << i << ' ' << bytes_stat[i] << std::endl;
    }
}

Method::Method(const std::string &name, const fs::path &input)
        : name(name), input(input) {
    fileSize = fs::file_size(input);
}

void Method::run() {
    auto host_start = std::chrono::steady_clock::now();
    run_();
    auto host_end = std::chrono::steady_clock::now();
    dur = host_end - host_start;
}

void Method::report(const fs::path &output) {
    std::ofstream out(output, std::fstream::app);

    if (fs::is_empty(output)) {
        out << "method,duration,size,batch\n";
    }

    out << name << "," << dur.count() << "," << fileSize << ",";
    if (withBatch) {
        out << BATCH_SIZE;
    } else {
        out << -1;
    }
    out << "\n";
}

void Method::print(std::ostream &out) {
    out << "Method: " << name << std::endl;
    out << "\tSize: " << fileSize << std::endl;
    out << "\tTime: " << dur.count() << std::endl;
    stat.print(out);
}

Statistic Method::getStats() const {
    return stat;
}

size_t Method::getFileSize() const {
    return fileSize;
}

void Method::refresh() {
    stat.bytes_stat = {};
}

MMapMethod::MMapMethod(const fs::path &input)
        : Method("mmap", input) {
    withBatch = false;
}

void MMapMethod::run_() {
    int fd = open(input.string().c_str(), O_RDONLY);

    errno = 0;
    void *file_data = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        close(fd);
        throw std::runtime_error(strerror(errno));
    }

    const char *casted_data = reinterpret_cast<const char *>(file_data);
    stat.add_stats(casted_data, fileSize);

    errno = 0;
    int retCode = munmap(file_data, fileSize);
    if (retCode != 0) {
        close(fd);
        throw std::runtime_error(strerror(errno));
    }

    close(fd);
}

ReadMethod::ReadMethod(const fs::path &input, size_t BATCH_SIZE_)
        : Method("read", input) {
    BATCH_SIZE = BATCH_SIZE_;
}

void ReadMethod::run_() {
    int fd = open(input.string().c_str(), O_RDONLY);
    size_t curSize = 0;
    std::vector<char> data(BATCH_SIZE);

    while (curSize < fileSize) {
        size_t wantToRead = std::min(BATCH_SIZE, fileSize - curSize);
        errno = 0;
        ssize_t readed = read(fd, data.data(), wantToRead);

        if (readed == -1) {
            switch (errno) {
                case EAGAIN:
                case EINTR:
                    continue;

                default:
                    throw std::runtime_error("Something wrong with file (READ): " +
                                             std::string(strerror(errno)));
            }
        } else if (readed == 0) {
            throw std::runtime_error(
                    "File trunkated! Current size: " + std::to_string(curSize) +
                    ", expected size: " + std::to_string(fileSize));
        }

        stat.add_stats(data.data(), wantToRead);
        curSize += wantToRead;
    }

    close(fd);
}

IfstreamMethod::IfstreamMethod(const fs::path &input, size_t BATCH_SIZE_)
        : Method("ifstream", input) {
    BATCH_SIZE = BATCH_SIZE_;
}

void IfstreamMethod::run_() {
    std::ifstream fin(input);
    std::vector<char> buffer (BATCH_SIZE,0);
    size_t curSize = 0;

    while(curSize < fileSize) {
        size_t wantToRead = std::min(buffer.size(), fileSize - curSize);
        fin.read(buffer.data(), wantToRead);
        size_t readed = fin.gcount();
        curSize += readed;

        stat.add_stats(buffer.data(), readed);
    }
    fin.close();
}

