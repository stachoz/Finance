cmake_minimum_required(VERSION 3.29)
project(TradeSim)

set(CMAKE_CXX_STANDARD 23)
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
        ${PROJECT_SOURCE_DIR}/src
)

add_executable(Optimizer apps/Optimizer/main.cpp)
target_link_libraries(Optimizer PRIVATE ${PROJECT_NAME}_lib)

add_executable(FinanceSim apps/FinanceSim/main.cpp)
target_link_libraries(FinanceSim PRIVATE ${PROJECT_NAME}_lib)

enable_testing()
add_subdirectory(tests)

target_compile_definitions(${PROJECT_NAME}_lib PUBLIC
        PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}"
)
target_compile_definitions(${PROJECT_NAME}_lib PUBLIC
        TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/test-data"
)
