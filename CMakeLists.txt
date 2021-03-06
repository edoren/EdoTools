cmake_minimum_required(VERSION 3.12)

project(EdoTools)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

option(FMT_SUPPORT "Enable the support integration with fmt library" ON)
option(NLOHMANN_JSON_SUPPORT "Enable the support integration with nlohmann json library" ON)

file(GLOB_RECURSE EDOTOOLS_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE EDOTOOLS_INCLUDE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.inl")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} ${EDOTOOLS_SOURCE_FILES} ${EDOTOOLS_INCLUDE_FILES})

string(TOLOWER ${PROJECT_NAME} EDOTOOLS_LIB)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EDOTOOLS_LIB})

set_target_properties(${PROJECT_NAME} PROPERTIES DEFINE_SYMBOL "EDOTOOLS_EXPORTS")
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EDOTOOLS_STATIC)
endif()

if(FMT_SUPPORT)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EDOTOOLS_FMT_SUPPORT)
    target_link_libraries(${PROJECT_NAME} CONAN_PKG::fmt)
endif()

if(NLOHMANN_JSON_SUPPORT)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EDOTOOLS_NLOHMANN_JSON_SUPPORT)
    target_link_libraries(${PROJECT_NAME} CONAN_PKG::nlohmann_json)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
