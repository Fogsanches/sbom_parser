cmake_minimum_required(VERSION 3.14)

# Включаем поддержку современных политик CMake
cmake_policy(SET CMP0091 NEW)

project(sbom_parser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Принудительно задаем единый тип системной библиотеки (динамический) ---
# Это лечит конфликт LNK2038 / LNK2005 между твоим кодом и gtest
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# --- 1. Библиотека парсера ---
add_library(sbom_parser_lib
    src/parser.cpp
)
target_include_directories(sbom_parser_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- 2. Консольное приложение ---
add_executable(sbom_parser_app app/main.cpp)
target_link_libraries(sbom_parser_app PRIVATE sbom_parser_lib)

# --- 3. Подключение Google Test ---
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.14.0
    # Отключаем встроенные тесты gtest, чтобы они не собирались вместе с твоими
    GIT_SHALLOW TRUE
)

# Чтобы старый CMake внутри gtest не ругался
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

# Скачиваем и добавляем gtest в проект
FetchContent_MakeAvailable(googletest)

# --- 4. Создание и настройка тестового исполняемого файла ---
enable_testing()
add_executable(sbom_parser_tests tests/test_parser.cpp)

# Линкуем наши тесты с парсером и gtest
target_link_libraries(sbom_parser_tests 
    PRIVATE 
        sbom_parser_lib 
        gtest 
        gtest_main
)

# Автоматическая регистрация тестов в Visual Studio
include(GoogleTest)
gtest_discover_tests(sbom_parser_tests)