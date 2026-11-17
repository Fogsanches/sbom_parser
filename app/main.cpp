#include "parser.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_CMakeLists.txt>\n";
        return 1;
    }
    try {
        auto result = sbom::parse_cmake_file(argv[1]);

        std::cout << "=== Found via find_package ===\n";
        for (const auto& p : result.find_packages) std::cout << "  - " << p << "\n";

        std::cout << "=== Found via FetchContent ===\n";
        for (const auto& f : result.fetch_contents) std::cout << "  - " << f << "\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}