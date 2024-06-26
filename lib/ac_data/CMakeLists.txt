cmake_minimum_required(VERSION 3.14)
project(ac_data C)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

include(CTest)

add_library(ac_list ac_list.c ac_list.h)
add_library(ac_string ac_string.c ac_string.h)

add_subdirectory(test)

# Custom target to build and run tests
add_custom_target(check
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMAND ${CMAKE_CTEST_COMMAND}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
