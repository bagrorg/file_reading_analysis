#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include "methods/methods.h"

namespace fs = std::filesystem;



int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Not enough arguments" << std::endl;
        return EXIT_FAILURE;
    }

    std::string method = argv[1];
    fs::path input_path = argv[2];
    fs::path output_path = argv[3];
    size_t BATCH_SIZE = fs::file_size(input_path);
    
    if (argc == 5) {
        BATCH_SIZE = std::stoul(argv[5]);           // check argument correctness??
    }



    return EXIT_SUCCESS;
}