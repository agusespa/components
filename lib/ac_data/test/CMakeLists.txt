cmake_minimum_required(VERSION 3.14)
project(test_ac_data CXX)

set(LIST test_ac_list)
add_executable(${LIST} ${LIST}.cpp)
target_link_libraries(${LIST} gtest_main ac_list)
add_test(NAME ${LIST} COMMAND ${LIST})

set(STRING test_ac_string)
add_executable(${STRING} ${STRING}.cpp)
target_link_libraries(${STRING} gtest_main ac_string)
add_test(NAME ${STRING} COMMAND ${STRING})
