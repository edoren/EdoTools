#include <catch2/catch.hpp>

#include <edoren/String.hpp>
#include <edoren/system/FileSystem.hpp>

using namespace edoren;

namespace {

#if PLATFORM_IS(PLATFORM_WINDOWS)
String ABS_START = "C:\\";
String SEP = "\\";
#else
String ABS_START = "/";
String SEP = "/";
#endif

}  // namespace

TEST_CASE("FileSystem::IsAbsolutePath", "[FileSystem]") {
    SECTION("true if the path is absolute, false otherwise") {
        String absolute = ABS_START + "hello";
        String relative = "hello" + SEP + "world";
        REQUIRE(filesystem::IsAbsolutePath(absolute) == true);
        REQUIRE(filesystem::IsAbsolutePath(relative) == false);
    }

#if PLATFORM_IS(PLATFORM_WINDOWS)
    SECTION("On windows having a \\ at the start does not mean an absolute path") {
        String relative = SEP + "hello" + SEP + "world";
        REQUIRE(filesystem::IsAbsolutePath(relative) == false);
    }
#endif
}

TEST_CASE("FileSystem::NormalizePath", "[FileSystem]") {
    SECTION("must resolve all the .. directories") {
        String path1 = "hello" + SEP + "world" + SEP + "..";
        String path2 = "hello" + SEP + ".." + SEP + "world" + SEP + "..";
        String path3 = "hello" + SEP + ".." + SEP + "world" + SEP + ".." + SEP + "..";
        String path4 = "hello" + SEP + ".." + SEP + "world" + SEP + ".." + SEP + ".." + SEP + "..";
        String path1Norm = filesystem::NormalizePath(path1);
        String path2Norm = filesystem::NormalizePath(path2);
        String path3Norm = filesystem::NormalizePath(path3);
        String path4Norm = filesystem::NormalizePath(path4);
        REQUIRE(path1Norm == "hello");
        REQUIRE(path2Norm == ".");
        REQUIRE(path3Norm == "..");
        REQUIRE(path4Norm == ".." + SEP + "..");
    }

    SECTION("must resolve all the . directories") {
        String path = "hello" + SEP + "." + SEP + "world" + SEP + "." + SEP + "." + SEP + ".";
        String pathNorm = filesystem::NormalizePath(path);
        REQUIRE(pathNorm == "hello" + SEP + "world");
    }

    SECTION("must remove all the leftover separators") {
        String path1 = "hello" + SEP + SEP + "1234" + SEP + SEP + "world" + SEP + SEP + "." + SEP + SEP + SEP;
        String path1Norm = filesystem::NormalizePath(path1);
        REQUIRE(path1Norm == "hello" + SEP + "1234" + SEP + "world");
    }

    SECTION("if absolute must keep the root component") {
        String path1 = ABS_START + "hello";
        String path2 = ABS_START + "hello" + SEP + ".." + SEP + "..";
        String path3 = ABS_START + ".." + SEP + "..";
        String path1Norm = filesystem::NormalizePath(path1);
        String path2Norm = filesystem::NormalizePath(path2);
        String path3Norm = filesystem::NormalizePath(path3);
        REQUIRE(path1Norm == ABS_START + "hello");
        REQUIRE(path2Norm == ABS_START + "");
        REQUIRE(path3Norm == ABS_START + "");
    }

#if PLATFORM_IS(PLATFORM_WINDOWS)
    SECTION("on Windows this should change any '/' separators to '\\'") {
        String path = ABS_START + "hello/world/1234";
        String path_norm = filesystem::NormalizePath(path);
        REQUIRE(path_norm == ABS_START + "hello" + SEP + "world" + SEP + "1234");
    }
#endif
}

TEST_CASE("FileSystem::Join", "[FileSystem]") {
    SECTION("must be able to join any number of paths") {
        String joined = filesystem::Join("hello", "world", "1234");
        REQUIRE(joined == "hello" + SEP + "world" + SEP + "1234");
    }

    SECTION("if any of the provided paths is empty it must ignore it") {
        String joined = filesystem::Join("hello", "", "1234");
        REQUIRE(joined.getSize() == 10);
        REQUIRE(joined == "hello" + SEP + "1234");
    }

    SECTION("if the right element is an absolute path it must return it") {
        String left = "hello";
        String right = ABS_START + "world" + SEP + "1234";
        String joined = filesystem::Join(left, right);
        REQUIRE(joined == ABS_START + "world" + SEP + "1234");
    }

    SECTION("must add a separator if the left argument does not finish in one") {
        String left1 = "hello";
        String left2 = "hello" + SEP;
        String right = "world";
        String joined1 = filesystem::Join(left1, right);
        String joined2 = filesystem::Join(left2, right);
        REQUIRE(joined1 == joined2);
    }
}
