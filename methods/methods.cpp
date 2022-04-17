#include "methods.h"
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

Method::Method(const std::string &name, const fs::path &input,
               size_t BATCH_SIZE)
    : name(name), input(input), BATCH_SIZE(BATCH_SIZE) {
  fileSize = fs::file_size(input);
}

void Method::operator()() {
  auto host_start = std::chrono::steady_clock::now();
  run();
  auto host_end = std::chrono::steady_clock::now();
  dur = host_end - host_start;
}

void Method::report(const fs::path &output) {
  std::ofstream out(output);

  if (fs::is_empty(output)) {
    out << "method,duration,size\n";
  }

  out << name << "," << dur.count() << "," << fileSize << "\n";
}

MMapMethod::MMapMethod(const fs::path &input, size_t BATCH_SIZE)
    : Method("mmap", input, BATCH_SIZE) {
  size_t page_size = getpagesize();
  if (BATCH_SIZE % page_size != 0) {
    BATCH_SIZE = (BATCH_SIZE / page_size + 1) *
                 page_size; // или как то учитывать только в оффсете?
  }
}

void MMapMethod::run() {
  int fd = open(input.string().c_str(), O_RDONLY);
  size_t curSize = 0;

  while (curSize < fileSize) {
    errno = 0;
    void *file_data = mmap(NULL, BATCH_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);

    if (file_data == MAP_FAILED) {
      close(fd);
      throw std::runtime_error(strerror(errno));
    }

    const char *casted_data = reinterpret_cast<const char *>(file_data);
    stat.add_stats(casted_data, BATCH_SIZE);

    errno = 0;
    int retCode = munmap(file_data, BATCH_SIZE);

    if (retCode != 0) {
      close(fd);
      throw std::runtime_error(strerror(errno));
    }

    curSize += BATCH_SIZE;
  }

  close(fd);
}

ReadMethod::ReadMethod(const fs::path &input, size_t BATCH_SIZE)
    : Method("read", input, BATCH_SIZE) {}

void ReadMethod::run() {
  int fd = open(input.string().c_str(), O_RDONLY);
  size_t curSize = 0;
  std::vector<char> data(BATCH_SIZE);

  while (curSize < fileSize) {
    errno = 0;
    ssize_t readed = read(fd, data.data(), BATCH_SIZE);

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

    stat.add_stats(data.data(), BATCH_SIZE);
    curSize += BATCH_SIZE;
  }

  close(fd);
}

IfstreamMethod::IfstreamMethod(const fs::path &input, size_t BATCH_SIZE)
    : Method("ifstream", input, BATCH_SIZE) {}

void IfstreamMethod::run() {}
