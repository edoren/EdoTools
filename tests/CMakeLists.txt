cmake_minimum_required(VERSION 3.12)

project(EdoToolsTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(EdoTools STATIC ${EDOTOOLS_SOURCE_FILES} ${EDOTOOLS_INCLUDE_FILES})

file(GLOB_RECURSE UNITARY_TEST_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Unitary/*.cpp")
add_executable(UnitaryTest ${UNITARY_TEST_SOURCE_FILES})
target_include_directories(UnitaryTest PRIVATE ${CONAN_INCLUDE_DIRS_CATCH2})
target_link_libraries(UnitaryTest EdoTools)

