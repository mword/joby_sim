cmake_minimum_required(VERSION 3.19)
project(joby_simulation)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES Vehicle.cpp Vehicle.h ChargingStation.cpp ChargingStation.h)
add_library(dummy_lib_for_gtest ${SOURCE_FILES})

add_executable(joby_simulation ${SOURCE_FILES} main.cpp)

enable_testing()

add_subdirectory(google_tests)