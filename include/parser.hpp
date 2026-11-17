#pragma once
#include <string>
#include <vector>

namespace sbom {

    // Результат разбора одного CMakeLists.txt
    struct ParseResult {
        std::vector<std::string> find_packages;   // из find_package(...)
        std::vector<std::string> fetch_contents;  // из FetchContent_Declare(...)
    };

    // Разбирает содержимое CMakeLists.txt и возвращает результат
    ParseResult parse_cmake_text(const std::string& text);

    // Читает файл и разбирает его. Бросает std::runtime_error, если файл не открылся
    ParseResult parse_cmake_file(const std::string& path);

} // namespace sbom