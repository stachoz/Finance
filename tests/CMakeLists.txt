enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(tests
        StatisticsTests.cpp
)

target_link_libraries(tests PRIVATE
        Finance_lib
        GTest::gtest_main
)

target_include_directories(tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(tests)

if(ENABLE_COVERAGE)
    target_compile_options(tests PRIVATE -O0 -g --coverage)
    target_link_options(tests PRIVATE --coverage)
endif()
