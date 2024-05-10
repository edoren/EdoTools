cmake_minimum_required(VERSION 3.15)

project(EdoToolsTest CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(edotools CONFIG REQUIRED)
find_package(Catch2 REQUIRED)

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

list(APPEND UNITARY_TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/FormattingTests.cpp)

list(APPEND UNITARY_TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/JSONTests.cpp)

add_executable(${PROJECT_NAME} ${UNITARY_TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Catch2::Catch2 edotools::edotools)
