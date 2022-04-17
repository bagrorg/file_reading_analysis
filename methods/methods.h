#pragma once

#include <array>
#include <filesystem>

namespace fs = std::filesystem;
using Duration = std::chrono::duration<double, std::micro>;

struct Statistic {
    std::array<size_t, 256> bytes_stat;

    void add_stats(const char *data, size_t length);
};

class Method {
public:
    Method(const std::string &name, const fs::path &input, size_t BATCH_SIZE);
    virtual ~Method() = default;

    void operator()();
    void report(const fs::path &output);

protected:
    virtual void run() = 0;

    Statistic stat;
    fs::path input;
    size_t fileSize;
    std::string name;
    Duration dur;
    size_t BATCH_SIZE;
};

class MMapMethod : public Method {
public:    
    MMapMethod(const fs::path &input, size_t BATCH_SIZE);
private:    
    void run() override;
};

class ReadMethod : public Method {
public:    
    ReadMethod(const fs::path &input, size_t BATCH_SIZE);
private:    
    void run() override;
};

class IfstreamMethod : public Method {
public:    
    IfstreamMethod(const fs::path &input, size_t BATCH_SIZE);
private:    
    void run() override;
};

