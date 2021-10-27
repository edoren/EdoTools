// #include <catch2/catch.hpp>

// #include <System/FileSystem.hpp>
// #include <System/String.hpp>

// using namespace engine;

// static FileSystem fileSystem;

// TEST_CASE("FileSystem::IsAbsolutePath", "[FileSystem]") {
//     SECTION("true if the path is absolute, false otherwise") {
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         String absolute = "C:\\hello";
//         String relative = "\\hello\\world";
// #else
//         String absolute = "/hello";
//         String relative = "hello/world";
// #endif
//         REQUIRE(fileSystem.isAbsolutePath(absolute) == true);
//         REQUIRE(fileSystem.isAbsolutePath(relative) == false);
//     }
// }

// TEST_CASE("FileSystem::NormalizePath", "[FileSystem]") {
//     SECTION("must resolve all the .. directories") {
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         String path1 = "hello\\world\\..";
//         String path2 = "hello\\..\\world\\..";
//         String path3 = "hello\\..\\world\\..\\..";
//         String path4 = "hello\\..\\world\\..\\..\\..";
// #else
//         String path1 = "hello/world/..";
//         String path2 = "hello/../world/..";
//         String path3 = "hello/../world/../..";
//         String path4 = "hello/../world/../../..";
// #endif
//         String path1Norm = fileSystem.normalizePath(path1);
//         String path2Norm = fileSystem.normalizePath(path2);
//         String path3Norm = fileSystem.normalizePath(path3);
//         String path4Norm = fileSystem.normalizePath(path4);
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         REQUIRE(path1Norm == "hello");
//         REQUIRE(path2Norm == ".");
//         REQUIRE(path3Norm == "..");
//         REQUIRE(path4Norm == "..\\..");
// #else
//         REQUIRE(path1Norm == "hello");
//         REQUIRE(path2Norm == ".");
//         REQUIRE(path3Norm == "..");
//         REQUIRE(path4Norm == "../..");
// #endif
//     }
//     SECTION("must resolve all the . directories") {
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         String path = "hello\\.\\world\\.\\.\\.";
// #else
//         String path = "hello/./world/././.";
// #endif
//         String pathNorm = fileSystem.normalizePath(path);
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         REQUIRE(pathNorm == "hello\\world");
// #else
//         REQUIRE(pathNorm == "hello/world");
// #endif
//     }
//     SECTION("must remove all the leftover separators") {
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         String path1 = "hello\\\\1234\\\\world\\\\.\\\\\\";
// #else
//         String path1 = "hello//1234//world//.///";
// #endif
//         String path1Norm = fileSystem.normalizePath(path1);
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         REQUIRE(path1Norm == "hello\\1234\\world");
// #else
//         REQUIRE(path1Norm == "hello/1234/world");
// #endif
//     }
//     SECTION("if absolute must keep the root component") {
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         String path1 = "C:\\hello";
//         String path2 = "C:\\hello\\..\\..";
//         String path3 = "C:\\..\\..";
// #else
//         String path1 = "/hello";
//         String path2 = "/hello/../..";
//         String path3 = "/../..";
// #endif
//         String path1Norm = fileSystem.normalizePath(path1);
//         String path2Norm = fileSystem.normalizePath(path2);
//         String path3Norm = fileSystem.normalizePath(path3);
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         REQUIRE(path1Norm == "C:\\hello");
//         REQUIRE(path2Norm == "C:\\");
//         REQUIRE(path3Norm == "C:\\");
// #else
//         REQUIRE(path1Norm == "/hello");
//         REQUIRE(path2Norm == "/");
//         REQUIRE(path3Norm == "/");
// #endif
//     }
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//     SECTION("on Windows this should change any '/' separators to '\\'") {
//         String path = "C:\\hello/world/1234";
//         String path_norm = fileSystem.normalizePath(path);
//         REQUIRE(path_norm == "C:\\hello\\world\\1234");
//     }
// #endif
// }

// TEST_CASE("FileSystem::Join", "[FileSystem]") {
//     SECTION("must be able to join any number of paths") {
//         String joined = fileSystem.join("hello", "world", "1234");
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         REQUIRE(joined == "hello\\world\\1234");
// #else
//         REQUIRE(joined == "hello/world/1234");
// #endif
//     }
//     SECTION("if any of the provided paths is empty it must ignore it") {
//         String joined = fileSystem.join("hello", "", "1234");
//         REQUIRE(joined.getSize() == 10);
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         REQUIRE(joined == "hello\\1234");
// #else
//         REQUIRE(joined == "hello/1234");
// #endif
//     }
//     SECTION("if the right element is an absolute path it must return it") {
//         String left = "hello";
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         String right = "C:\\world\\1234";
// #else
//         String right = "/world/1234";
// #endif
//         String joined = fileSystem.join(left, right);
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         REQUIRE(joined == "C:\\world\\1234");
// #else
//         REQUIRE(joined == "/world/1234");
// #endif
//     }
//     SECTION(
//         "must add a separator if the left argument does not finish in one") {
//         String left1 = "hello";
// #if PLATFORM_IS(PLATFORM_WINDOWS)
//         String left2 = "hello\\";
// #else
//         String left2 = "hello/";
// #endif
//         String right = "world";
//         String joined1 = fileSystem.join(left1, right);
//         String joined2 = fileSystem.join(left2, right);
//         REQUIRE(joined1 == joined2);
//     }
// }
