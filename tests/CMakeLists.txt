cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# file(GLOB_RECURSE UNITARY_TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/*.cpp)

set(UNITARY_TEST_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/FunctionTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/StringTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/StringViewTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/UTFTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/FileSystemTests.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/container/VectorTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/container/ListTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/container/MapTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/container/SetTests.cpp
)

if(FMT_SUPPORT)
    list(APPEND UNITARY_TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/FormattingTests.cpp)
endif()

if(NLOHMANN_JSON_SUPPORT)
    list(APPEND UNITARY_TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/JSONTests.cpp)
endif()

add_executable(UnitaryTest ${UNITARY_TEST_SOURCE_FILES})
target_link_libraries(UnitaryTest CONAN_PKG::catch2 EdoTools)
add_test(NAME UnitaryTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND UnitaryTest)
