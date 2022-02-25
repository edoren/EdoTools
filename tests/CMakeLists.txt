cmake_minimum_required(VERSION 3.12)

set(UNITARY_TEST_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/FunctionTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/StringTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/StringViewTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/UTFTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/VectorTests.cpp
)

if(FMT_SUPPORT)
    list(APPEND UNITARY_TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/FormattingTests.cpp)
endif()

if(NLOHMANN_JSON_SUPPORT)
    list(APPEND UNITARY_TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Unitary/JSONTests.cpp)
endif()

add_executable(UnitaryTest ${UNITARY_TEST_SOURCE_FILES})
target_include_directories(UnitaryTest PRIVATE ${CONAN_INCLUDE_DIRS_CATCH2})
target_link_libraries(UnitaryTest EdoTools)
add_test(NAME UnitaryTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND UnitaryTest)
