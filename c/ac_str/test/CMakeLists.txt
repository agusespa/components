cmake_minimum_required(VERSION 3.14)
project(test_ac_str CXX)

set(LIST test_ac_str_list)
add_executable(${LIST} ${LIST}.cpp)
target_link_libraries(${LIST} gtest_main ac_str_list)
add_test(NAME ${LIST} COMMAND ${LIST})
set_tests_properties(${LIST} PROPERTIES TIMEOUT 1000)

set(STR test_ac_str_builder)
add_executable(${STR} ${STR}.cpp)
target_link_libraries(${STR} gtest_main ac_str_builder)
add_test(NAME ${STR} COMMAND ${STR})
