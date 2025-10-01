cmake_minimum_required(VERSION 3.29)
project(Finance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/*.hpp
)

list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main.cpp$")

add_library(${PROJECT_NAME}_lib ${LIB_SOURCES})
target_include_directories(${PROJECT_NAME}_lib PUBLIC
        ${PROJECT_SOURCE_DIR}/utils
)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Statisitcs.h)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

enable_testing()
add_subdirectory(tests)