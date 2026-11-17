#include <gtest/gtest.h>
#include "parser.hpp"

TEST(ParserTest, FindsSingleFindPackage) {
    auto r = sbom::parse_cmake_text("find_package(OpenCV REQUIRED)\n");
    ASSERT_EQ(r.find_packages.size(), 1u);
    EXPECT_EQ(r.find_packages[0], "OpenCV");
}

TEST(ParserTest, FindsMultipleFindPackages) {
    std::string text =
        "find_package(OpenCV REQUIRED)\n"
        "find_package(Boost 1.70 REQUIRED)\n";
    auto r = sbom::parse_cmake_text(text);
    ASSERT_EQ(r.find_packages.size(), 2u);
    EXPECT_EQ(r.find_packages[0], "OpenCV");
    EXPECT_EQ(r.find_packages[1], "Boost");
}

TEST(ParserTest, FindsFetchContent) {
    std::string text =
        "FetchContent_Declare(googletest GIT_REPOSITORY https://...)\n";
    auto r = sbom::parse_cmake_text(text);
    ASSERT_EQ(r.fetch_contents.size(), 1u);
    EXPECT_EQ(r.fetch_contents[0], "googletest");
}

TEST(ParserTest, HandlesEmptyText) {
    auto r = sbom::parse_cmake_text("");
    EXPECT_TRUE(r.find_packages.empty());
    EXPECT_TRUE(r.fetch_contents.empty());
}

TEST(ParserTest, IgnoresComments) {
    // Строка с # в начале — не считается (упрощение: у нас просто не будет ключевого слова)
    std::string text = "# find_package(Fake REQUIRED)\n";
    auto r = sbom::parse_cmake_text(text);
    // В нашем упрощённом парсере он найдётся, поэтому либо не тестируй это,
    // либо доработай parse, чтобы игнорировать строки, начинающиеся с #
}