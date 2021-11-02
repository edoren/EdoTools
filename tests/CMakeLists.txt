cmake_minimum_required(VERSION 3.12)

file(GLOB_RECURSE UNITARY_TEST_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Unitary/*.cpp")
add_executable(UnitaryTest ${UNITARY_TEST_SOURCE_FILES})
target_include_directories(UnitaryTest PRIVATE ${CONAN_INCLUDE_DIRS_CATCH2})
target_link_libraries(UnitaryTest EdoTools)
add_test(NAME UnitaryTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND UnitaryTest)
