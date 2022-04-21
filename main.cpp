#include <filesystem>
#include <iostream>
#include "methods/methods.h"
#include <unistd.h>



using MethodPtr = std::unique_ptr<Method>;

int main(int argc, char *argv[]) {
    int c;
    std::string method_name;
    std::string input_path;
    std::string output_path;

    size_t batch_size = 0;
    size_t iterations = 1;
    bool verbose = false;

    while ((c = getopt(argc, argv, "m:i:o:b:vr:")) != -1) {
        switch (c) {
            case 'm':
                method_name = optarg;
                break;
            case 'i':
                input_path = optarg;
                break;
            case 'o':
                output_path = optarg;
                break;
            case 'b':
                batch_size = std::stoul(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case 'r':
                iterations = std::stoul(optarg);
                break;
            default:
                std::cerr << "Unknown argument: " << c << std::endl;
                return EXIT_FAILURE;
        }
    }
    if (method_name.empty()) {
        std::cerr << "No method name set" << std::endl;
        return EXIT_FAILURE;
    }

    if (input_path.empty()) {
        std::cerr << "Input path is empty" << std::endl;
        return EXIT_FAILURE;
    }

    if (output_path.empty()) {
        std::cerr << "Output path is empty" << std::endl;
        return EXIT_FAILURE;
    }

    if (batch_size == 0) {
        batch_size = fs::file_size(input_path);
    }

    MethodPtr m;

    // TODO: FACTORY
    if (method_name == "mmap") {
        m = std::make_unique<MMapMethod>(fs::path(input_path));
    } else if (method_name == "ifstream") {
        m = std::make_unique<IfstreamMethod>(input_path, batch_size);
    } else if (method_name == "read") {
        m = std::make_unique<ReadMethod>(input_path, batch_size);
    } else {
        std::cerr << "Unknown method" << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 0; i < iterations; i++) {
        m->run();
        m->report(output_path);
        if (verbose) {
            m->print(std::cout);
        }
        m->refresh();
    }

    return EXIT_SUCCESS;
}