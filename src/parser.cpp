#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace sbom {

    // Убирает пробелы с начала и конца строки
    static std::string trim(const std::string& s) {
        size_t start = 0;
        while (start < s.size() && std::isspace((unsigned char)s[start])) ++start;
        size_t end = s.size();
        while (end > start && std::isspace((unsigned char)s[end - 1])) --end;
        return s.substr(start, end - start);
    }

    // Ищет в строке ключевое слово и возвращает первый "токен" после него
    // Например, для "find_package(OpenCV REQUIRED)" и keyword "find_package("
    // вернёт "OpenCV"
    static std::string extract_first_arg(const std::string& line, const std::string& keyword) {
        size_t pos = line.find(keyword);
        if (pos == std::string::npos) return "";
        pos += keyword.size();
        std::string rest = line.substr(pos);

        // читаем до пробела, ')' или конца строки
        std::string arg;
        for (char c : rest) {
            if (std::isspace((unsigned char)c) || c == ')' || c == '(') break;
            arg += c;
        }
        return arg;
    }

    ParseResult parse_cmake_text(const std::string& text) {
        ParseResult result;
        std::istringstream stream(text);
        std::string line;

        while (std::getline(stream, line)) {
            std::string trimmed = trim(line);

            // ищем find_package(...)
            if (trimmed.find("find_package(") != std::string::npos) {
                std::string pkg = extract_first_arg(trimmed, "find_package(");
                if (!pkg.empty()) result.find_packages.push_back(pkg);
            }

            // ищем FetchContent_Declare(...)
            if (trimmed.find("FetchContent_Declare(") != std::string::npos) {
                std::string name = extract_first_arg(trimmed, "FetchContent_Declare(");
                if (!name.empty()) result.fetch_contents.push_back(name);
            }
        }
        return result;
    }

    ParseResult parse_cmake_file(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + path);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return parse_cmake_text(buffer.str());
    }

} // namespace sbom