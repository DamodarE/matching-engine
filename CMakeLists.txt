cmake_minimum_required(VERSION 3.16)
project(matching_engine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(matching_engine_core
    src/OrderBook.cpp
)
target_include_directories(matching_engine_core PUBLIC include)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
# Match the project's runtime library setting so gtest links cleanly on Windows;
# a no-op elsewhere.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(matching_engine_tests
    tests/OrderBookTest.cpp
)
target_link_libraries(matching_engine_tests
    PRIVATE matching_engine_core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(matching_engine_tests)
