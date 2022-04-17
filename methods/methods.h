#pragma once

#include <array>
#include <filesystem>

namespace fs = std::filesystem;
using Duration = std::chrono::duration<double, std::micro>;

struct Statistic {
    std::array<size_t, 256> bytes_stat;

    void add_stats(const char *data, size_t length);
    void print(std::ostream &out);
};

class Method {
public:
    Method(const std::string &name, const fs::path &input);
    virtual ~Method() = default;

    void run();
    void report(const fs::path &output);
    void print(std::ostream &out);

protected:
    virtual void run_() = 0;

    Statistic stat;
    fs::path input;
    size_t fileSize;
    std::string name;
    Duration dur;
};

class MMapMethod : public Method {
public:    
    explicit MMapMethod(const fs::path &input);
private:    
    void run_() override;
};

class ReadMethod : public Method {
public:    
    ReadMethod(const fs::path &input, size_t BATCH_SIZE);
private:    
    void run_() override;
    size_t BATCH_SIZE;
};

class IfstreamMethod : public Method {
public:    
    IfstreamMethod(const fs::path &input, size_t BATCH_SIZE);
private:    
    void run_() override;
    size_t BATCH_SIZE;
};

