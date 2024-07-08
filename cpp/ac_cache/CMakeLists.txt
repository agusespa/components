cmake_minimum_required(VERSION 3.14)
project(ac_cache CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

include(CTest)

add_library(ac_lru_cache ac_lru_cache.cpp ac_lru_cache.h)

add_subdirectory(test)

# Custom target to build and run tests
add_custom_target(t
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --extra-verbose
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

enable_testing()
