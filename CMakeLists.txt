cmake_minimum_required(VERSION 3.23)

project(EdoTools CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(FMT_SUPPORT "Enable the support integration with fmt library" ON)
option(NLOHMANN_JSON_SUPPORT "Enable the support integration with nlohmann json library" ON)

file(GLOB_RECURSE EDOTOOLS_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE EDOTOOLS_INCLUDE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.inl")

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PUBLIC ${EDOTOOLS_SOURCE_FILES}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES ${EDOTOOLS_INCLUDE_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

string(TOLOWER ${PROJECT_NAME} EDOTOOLS_LIB)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EDOTOOLS_LIB})

set_target_properties(${PROJECT_NAME} PROPERTIES DEFINE_SYMBOL "EDOTOOLS_EXPORTS")
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EDOTOOLS_STATIC)
endif()

if(FMT_SUPPORT)
    find_package(fmt)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EDOTOOLS_FMT_SUPPORT)
    target_link_libraries(${PROJECT_NAME} fmt::fmt)
endif()

if(NLOHMANN_JSON_SUPPORT)
    find_package(nlohmann_json)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EDOTOOLS_NLOHMANN_JSON_SUPPORT)
    target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
endif()

install(TARGETS ${PROJECT_NAME} FILE_SET HEADERS)
