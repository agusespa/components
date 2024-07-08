cmake_minimum_required(VERSION 3.14)
project(test_ac_cache CXX)

find_package(GTest REQUIRED)

set(LIST test_ac_lru_cache)
add_executable(${LIST} ${LIST}.cpp)
target_link_libraries(${LIST} gtest_main ac_lru_cache)
add_test(NAME ${LIST} COMMAND ${LIST})
